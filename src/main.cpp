/*
"Galactic Rap " Kevin MacLeod (incompetech.com)
Licensed under Creative Commons: By Attribution 4.0 License
http://creativecommons.org/licenses/by/4.0/
*/
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
using namespace sf;

int main() {
    const int width = 1200;
    const int height = 800;
    const double speed = 0.1;
    const double max_speed = 10;
    const double turn = 1;
    const float friction = 0.01;

    struct Player {
        float velocity = 0;
        float angle = 0;
    };
    struct Torpedo {
        float velocity = 0;
        float angle = 0;
    };
    // std::vector<Torpedo> torpedos;
    Player player;    

    RenderWindow window = RenderWindow(VideoMode({width, height}), "Game");
    window.setFramerateLimit(60);

    Music music("C:\\Users\\henry\\Desktop\\SFML-Game\\assets\\galactic_rap.mp3");
    music.play();
    music.setLooping(true);

    Texture texture("C:\\Users\\henry\\Desktop\\SFML-Game\\assets\\submarine.png");
    Sprite sprite(texture);
    sprite.scale({.15f, 0.1f});
    sprite.setOrigin((Vector2f)texture.getSize() / 2.f);
    sprite.setPosition({10, 400});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::W)) {
            player.velocity += speed;
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::S)) {
            player.velocity -= speed;
        }
        std::cout << player.velocity << std::endl;
        if (Keyboard::isKeyPressed(Keyboard::Key::A)) {
            sprite.rotate(degrees(-turn));
            player.angle -= turn;
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::D)) {
            sprite.rotate(degrees(turn));
            player.angle += turn;
        }
        if (player.velocity > max_speed) {
            player.velocity = max_speed;
        }
        if (player.velocity < -max_speed) {
            player.velocity = -max_speed;
        }
        if (sprite.getPosition().x < 0) {
            sprite.setPosition({width, sprite.getPosition().y});
        }
        if (sprite.getPosition().x > width) {
            sprite.setPosition({0, sprite.getPosition().y});
        }
        if (sprite.getPosition().y < 0) {
            sprite.setPosition({sprite.getPosition().x, height});
        }
        if (sprite.getPosition().y > height) {
            sprite.setPosition({sprite.getPosition().x, 0});
        }

        window.clear();

        sprite.move({player.velocity * std::cos(player.angle * M_PI / 180.f), player.velocity * std::sin(player.angle * M_PI / 180.f)});
        window.draw(sprite);
        player.velocity *= (1 - friction);

        window.display();
    }
}
