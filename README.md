# Bullet Rain
Bullet Rain is a game made in C++ using the SFML library.

# Gameplay
You can steer the green player using WASD.

# Configuration
The game's fun comes from the fact that it is very easily and extensively configurable.
You can customize everything, from the amount of bullets on the field to your player's speed and size.
You can even customize the colors of the game, if the green and red is getting boring. I personally like purple bullets!

# compiling
Install SFML, cmake and a C++ compiler on your linux computer, clone the repository, create a build folder, run cmake, run make, and there should be a folder with the game binary.

    mkdir build
    cd build
    cmake ..
    make
    cd Bullet_Rain
    ./bullet-rain

# troubleshooting
If the game says it can't load an asset, it means that it can't find the font file. In order to make sure it can find it, make sure you run the game from the proper directory (which is the directory where the binary is located). Make sure that the asset folder exists and that there is a file named "font.ttf" inside.