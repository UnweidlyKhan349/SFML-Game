#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <algorithm>
using namespace sf;

int main() {
    const int width = 1200;
    const int height = 800;
    const float speed = 0.03;
    const float max_speed = 0.05;
    const float max_reverse_speed = -0.03;
    const float turn = 90;
    const float friction = 0.02;
    const float torpedo_speed = 100;
    const float torpedo_lifetime = 5;
    const float torpedo_reload = 3;

    struct Entity {
        float velocity = 0;
        float angle = 0;
    };
    struct Torpedo {
        Angle angle;
        Sprite sprite;
        float time = 0;
        Torpedo(Sprite &s, Angle ang): sprite(s), angle(ang) {};
    };
    std::vector<Torpedo> torpedos;
    Entity player;    

    RenderWindow window = RenderWindow(VideoMode({width, height}), "Game");

    sf::Clock clock;

    Music music("C:\\Users\\henry\\Desktop\\SFML-Game\\assets\\galactic_rap.mp3");
    music.play();
    music.setLooping(true);

    Texture texture("C:\\Users\\henry\\Desktop\\SFML-Game\\assets\\submarine.png");
    Sprite sprite(texture);
    sprite.scale({.15f, 0.1f});
    sprite.setOrigin((Vector2f)texture.getSize() / 2.f);
    sprite.setPosition({50, 400});

    Texture texture_torp("C:\\Users\\henry\\Desktop\\SFML-Game\\assets\\torpedo.png");

    while (window.isOpen()) {
        Time dt = clock.restart();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::W)) {
            player.velocity += speed * dt.asSeconds();
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::S)) {
            player.velocity -= speed * dt.asSeconds();
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::A)) {
            sprite.rotate(degrees(-turn * dt.asSeconds()));
            player.angle -= turn * dt.asSeconds();
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::D)) {
            sprite.rotate(degrees(turn * dt.asSeconds()));
            player.angle += turn * dt.asSeconds();
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Space)) {
            Sprite torpedo_s(texture_torp);
            torpedo_s.scale({.15f, 0.1f});
            torpedo_s.setOrigin((Vector2f)texture_torp.getSize() / 2.f);
            torpedo_s.setPosition(sprite.getPosition());
            torpedo_s.setRotation(sprite.getRotation());
            Torpedo torpedo(torpedo_s, sprite.getRotation());
            torpedos.push_back(torpedo);
        }
        // Max speed
        if (player.velocity > max_speed) {
            player.velocity = max_speed;
        }
        if (player.velocity < max_reverse_speed) {
            player.velocity = max_reverse_speed;
        }
        // Window wrap
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
        sprite.move({static_cast<float>(player.velocity * std::cos(player.angle * M_PI / 180.f)), static_cast<float>(player.velocity * std::sin(player.angle * M_PI / 180.f))});
        for (Torpedo &torpedo : torpedos) {
            torpedo.sprite.move({torpedo_speed * std::cos(torpedo.angle.asRadians()) * dt.asSeconds(), torpedo_speed * std::sin(torpedo.angle.asRadians()) * dt.asSeconds()});
            window.draw(torpedo.sprite);
            torpedo.time += dt.asSeconds();
            if (torpedo.time > torpedo_lifetime) {
                torpedos.erase(std::remove(torpedos.begin(), torpedos.end(), torpedo), torpedos.end());
            }
        }
        window.draw(sprite);
        if (player.velocity != 0)
            player.velocity -= player.velocity > 0 ? friction * dt.asSeconds() : -friction * dt.asSeconds();

        window.display();
    }
}
