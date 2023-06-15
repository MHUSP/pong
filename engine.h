#ifndef PONG_ENGINE_H
#define PONG_ENGINE_H

#include <string>
#include <chrono>

#include <SDL2/SDL.h>

struct Graphics {
    SDL_Window* win;
    SDL_Renderer* ren;
};

class Engine {
protected:
    Graphics gpx;

    std::string title = "Engine";
    int width = 800;
    int height = 600;

    bool quit = false;

public:
    Engine(std::string title, int width, int height): title(title), width(width), height(height) {
        init_video();
    }

    ~Engine() {
        end_video();
    }

    void start() {
        start_up();
        game_loop();
    }

private:
    virtual void start_up() = 0;
    virtual void step(double dt) = 0;
    virtual void draw_game() = 0;

    virtual void handle_key_down(SDL_Keycode key) {};
    virtual void handle_key_up(SDL_Keycode key) {};

    void game_loop() {
        SDL_Event event;

        auto before = std::chrono::steady_clock::now();
        while (!quit) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        quit = true;
                        break;
                    case SDL_KEYDOWN:
                        handle_key_down(event.key.keysym.sym);
                        break;
                    case SDL_KEYUP:
                        handle_key_up(event.key.keysym.sym);
                        break;
                }
            }

            auto now = std::chrono::steady_clock::now();
            double dt = (std::chrono::duration_cast<std::chrono::microseconds>(now - before)).count() * 1e-6;
            before = now;

            step(dt);
            draw();
        }
    }

    void draw() {
        clear_screen();
        draw_game();
        SDL_RenderPresent(gpx.ren);
    }

    void clear_screen() {
        SDL_SetRenderDrawColor(gpx.ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(gpx.ren);
    }

    void init_video() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) sdl_error("Impossível iniciarlizar SDL");

        gpx.win = SDL_CreateWindow(
                title.c_str(),
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                width,
                height,
                SDL_WINDOW_SHOWN
        );
        if (gpx.win == nullptr) sdl_error("Impossível criar janela");

        gpx.ren = SDL_CreateRenderer(gpx.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (gpx.ren == nullptr) sdl_error("Impossível criar o renderer");
    }

    void end_video() {
        SDL_DestroyRenderer(gpx.ren);
        SDL_DestroyWindow(gpx.win);
        SDL_Quit();
    }

    void sdl_error(std::string msg) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s: %s", msg.c_str(), SDL_GetError());
        exit(EXIT_FAILURE);
    }
};

#endif //PONG_ENGINE_H