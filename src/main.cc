#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include "Bullet.hh"
#include <iostream>
#include "Config.hh"

static const int width = 800;
static const int height = 800;

static int player_size = 15;
static int player_speed = 5;

static int bullet_count = 100;

enum class GameState {
    MENU, PLAYING, OVER
};

sf::Color get_color(const std::vector<int>& rgb) {
    sf::Color result;
    result.r = rgb[0];
    result.g = rgb[1];
    result.b = rgb[2];
    return result;
}

std::vector<Bullet> generate_bullets(Config& config) {
    std::vector<Bullet> bullets;

    bullets.reserve(bullet_count);
    for (int i = 0; i < bullet_count; i++)
        bullets.push_back(Bullet(width, height, config["advanced_configuration"]["bullet_width"].get<int>(), config["advanced_configuration"]["bullet_height"].get<int>(), config["advanced_configuration"]["bullet_fire_frame_bound"].get<int>(), config["basic_configuration"]["bullet_speed"].get<int>(), get_color(config["colors"]["bullet"].get<std::vector<int>>())));

    return bullets;
}

sf::Vector2<float> get_player_start_pos(Config& config) {
    return {width / 2.0f - player_size / 2.0f, height - player_size * config["advanced_configuration"]["player_start_height_multiplier"].get<float>()};
}

int main() {
    srand(time(NULL));

    Config config = read_from_file("config.json");
    player_size = config["basic_configuration"]["player_size"].get<int>();
    player_speed = config["basic_configuration"]["player_speed"].get<int>();
    bullet_count = config["basic_configuration"]["bullet_amount"].get<int>();

    sf::RenderWindow window(sf::VideoMode(width, height), config["language"]["title"].get<std::string>());

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition({(int)(desktop.width/2 - window.getSize().x/2), (int)(desktop.height/2 - window.getSize().y/2)});

    window.setVerticalSyncEnabled(true);

    std::vector<Bullet> bullets = generate_bullets(config);

    sf::RectangleShape player({(float)player_size, (float)player_size});
    player.setFillColor(get_color(config["colors"]["player"].get<std::vector<int>>()));
    sf::Vector2<float> player_pos = get_player_start_pos(config);
    player.setPosition(player_pos);

    sf::Font font;
    if (!font.loadFromFile(config["advanced_configuration"]["asset_directory"].get<std::string>() + "/font.ttf")) {
        std::cerr << "Failed to load asset! (check if you're running the game from the proper directory!)" << std::endl;
        window.close();
    }
    sf::Text game_over_text(config["language"]["game_over"].get<std::string>(), font);
    sf::FloatRect text_rect = game_over_text.getLocalBounds();
    game_over_text.setOrigin(text_rect.left + text_rect.width / 2, text_rect.top + text_rect.height / 2);
    game_over_text.setPosition((int)(width / 2), (int)(height / 2));
    game_over_text.setFillColor(get_color(config["colors"]["game_over_text"].get<std::vector<int>>()));

    sf::Text score_text;
    score_text.setFont(font);
    score_text.setFillColor(get_color(config["colors"]["score_text"].get<std::vector<int>>()));
    score_text.setCharacterSize(20);
    score_text.setPosition(0, height - 20);
    

    sf::Text start_text;
    start_text.setString(config["language"]["menu"].get<std::string>());
    start_text.setFont(font);
    start_text.setFillColor(get_color(config["colors"]["start_text"].get<std::vector<int>>()));
    start_text.setCharacterSize(30);
    sf::FloatRect start_text_rect = start_text.getLocalBounds();
    start_text.setOrigin(start_text_rect.left + start_text_rect.width / 2, start_text_rect.top + start_text_rect.height / 2);
    start_text.setPosition(width / 2.0f, height / 2.0f);

    GameState state = (config["basic_configuration"]["skip_menu"] ? GameState::PLAYING : GameState::MENU);
    int score = config["basic_configuration"]["starting_score"].get<int>();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    if (state == GameState::MENU) {
                        state = GameState::PLAYING;
                    } else if (state == GameState::OVER) {
                        bullets = generate_bullets(config);
                        player_pos = get_player_start_pos(config);
                        player.setPosition(player_pos);
                        score = config["basic_configuration"]["starting_score"].get<int>();
                        score_text.setCharacterSize(20);
                        score_text.setOrigin(0, 0);
                        score_text.setPosition(0, height - 20);
                        state = GameState::PLAYING;
                    }
                }
            }
        }
        if (state == GameState::MENU) {
            window.clear();
            window.draw(start_text);
            window.display();
        } else if (state == GameState::PLAYING) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                if (player_pos.y > 0)
                    player_pos.y -= player_speed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                if (player_pos.y < height - player_size)
                    player_pos.y += player_speed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                if (player_pos.x > 0)
                    player_pos.x -= player_speed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                if (player_pos.x < width - player_size)
                    player_pos.x += player_speed;
            }

            player.setPosition(player_pos);

            window.clear();
            
            window.draw(player);
            for (Bullet& bullet : bullets) {
                if (player_pos.x < bullet.position.x + bullet.size_x &&
                    player_pos.x + player_size > bullet.position.x &&
                    player_pos.y < bullet.position.y + bullet.size_y &&
                    player_pos.y + player_size > bullet.position.y) {
                        state = GameState::OVER;
                        score_text.setCharacterSize(40);
                        sf::FloatRect s_text_rect = score_text.getLocalBounds();
                        score_text.setString(config["language"]["score_display"].get<std::string>() + std::to_string(score));
                        score_text.setOrigin(s_text_rect.left + s_text_rect.width / 2, s_text_rect.top + s_text_rect.height / 2);
                        score_text.setPosition(width / 2.0f, height / 2.0f + 50);
                        break;
                    }

                bullet.update();
                if (bullet.position.y > height) {
                    bullet.reset_position();
                    score += 1;
                }
                bullet.draw(window);
            }

            score_text.setString(config["language"]["score_display"].get<std::string>() + std::to_string(score));
            if (state != GameState::OVER) window.draw(score_text);
            
            window.display();
        } else if (state == GameState::OVER) {
            window.clear();
            window.draw(game_over_text);
            window.draw(score_text);
            window.display();
        }
    }
}