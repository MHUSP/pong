#include <iostream>

#include "engine.h"

struct Pad {
    double y;
    double x;
    double height = 80;
    double width = 15;
    double vel;
};

struct Ball {
    double x;
    double y;
    double vel_x;
    double vel_y;
    double length = 10;
};

class Pong : public Engine {
private:
    Pad player, enemy;
    Ball ball;

public:
    Pong(std::string title, int width, int height) : Engine(title, width, height) {}

private:
    void start_up() override {
        player.y = height / 2 - player.height / 2;
        player.x = 0;
        player.vel = 0;

        enemy.vel = 0;
        enemy.y = height / 2 - enemy.height / 2;
        enemy.x = width - enemy.width;

        ball.x = width / 2;
        ball.y = height / 2;
        ball.vel_x = +200;
        ball.vel_y = -0;
    }

    void handle_key_down(SDL_Keycode key) {
        switch (key) {
            case SDLK_UP:
                player.vel = -200;
                break;
            case SDLK_DOWN:
                player.vel = 200;
                break;
        }
    }

    void handle_key_up(SDL_Keycode key) {
        switch (key) {
        case SDLK_UP: case SDLK_DOWN:
            player.vel = 0;
            break;
        }
    }

    void step(double dt) override {
        // Movimenta a bola
        ball.x += ball.vel_x * dt;
        ball.y += ball.vel_y * dt;

        // Movimenta player
        player.y += player.vel * dt;

        // Movimenta inimigo
        enemy.y += enemy.vel * dt;

        // Inteligência do inimigo
        double enemy_center = enemy.y + enemy.height / 2.0;
        if (ball.y < enemy_center) enemy.vel -= 300 * dt;
        if (ball.y > enemy_center) enemy.vel += 300 * dt;
        enemy.vel = .95 * enemy.vel;

        // Colisão da bola na parede superior
        if (ball.y <= 0) {
            ball.y = 0;
            ball.vel_y = - ball.vel_y;
        }

        // Colisão da bola na parede inferior
        if (ball.y >= height - ball.length) {
            ball.y = height - ball.length;
            ball.vel_y = -ball.vel_y;
        }

        // Colisão da bola no jogador
        double ball_bottom = ball.y + ball.length;
        double player_bottom = player.y + player.height;
        double player_right = player.x + player.width;
        double player_center = player.y + player.height / 2.0;
        if (ball_bottom >= player.y && ball.y <= player_bottom && ball.x <= player_right) {
            ball.x = player_right;
            ball.vel_x = - ball.vel_x;
            ball.vel_y += ball.y - player_center;
        }

        // Colisão da bola com o inimigo
        double enemy_bottom = enemy.y + enemy.height;
        double enemy_left = enemy.x - enemy.width;
        if (ball_bottom >= enemy.y && ball.y <= enemy_bottom && ball.x >= enemy_left) {
            ball.x = enemy_left;
            ball.vel_x = - ball.vel_x;
        }

        // Verifica se a bola saiu da arena
        double ball_right = ball.x + ball.length;
        if (ball_right >= width) ball.x = width / 2, ball.vel_x *= -1;
        if (ball.x <= 0) ball.x = width / 2, ball.vel_x *= -1;
    }

    void draw_game() override {
        // Desenha jogador
        SDL_SetRenderDrawColor(gpx.ren, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        SDL_Rect rect = {(int) player.x, (int) player.y, (int) player.width, (int) player.height};
        SDL_RenderDrawRect(gpx.ren, &rect);

        // Desenha inimigo
        SDL_SetRenderDrawColor(gpx.ren, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        rect = {(int) enemy.x, (int) enemy.y, (int) enemy.width, (int) enemy.height};
        SDL_RenderDrawRect(gpx.ren, &rect);

        // Desenha bola
        SDL_SetRenderDrawColor(gpx.ren, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        rect = {(int) ball.x, (int) ball.y, (int) ball.length, (int) ball.length};
        SDL_RenderDrawRect(gpx.ren, &rect);
    }
};

int main(int argc, char* argv[]) {
    Pong pong("Pong", 800, 450);
    pong.start();

    return EXIT_SUCCESS;
}
