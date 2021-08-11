#pragma once
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>

class Bullet {
    public:
        sf::Vector2<float> position;
        float size_x = 8;
        float size_y = 30;

        Bullet(const int width, const int height, int bullet_width, int bullet_height, int start_frame_bound, int bullet_speed, sf::Color color) : size_x(bullet_width), size_y(bullet_height), bullet({size_x, size_y}), width(width), height(height), speed(bullet_speed), color(color) {
            bullet.setFillColor(color);

            position.x = rand() % (width - 5);
            position.y = -size_y;

            bullet.setPosition(position);

            start_frame = rand() % start_frame_bound;
        }

        void draw(sf::RenderWindow& window) {
            if (updates >= start_frame)
                window.draw(bullet);
        }

        void update() {
            if (updates >= start_frame) {
                position.y += speed;
                bullet.setPosition(position);
            }
            updates += 1;
        }

        void reset_position() {
            position.x = rand() % (width - 5);
            position.y = -size_y;
        }
    private:
        sf::RectangleShape bullet;
        int speed = 4;
        const int width, height;
        int start_frame;
        unsigned int updates = 0;
        sf::Color color;
};