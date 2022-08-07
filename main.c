#include <log.h>
#include <util.h>

#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main(void) {
    logger_init("test.log", LOG_TRACE);

    log_trace("initializing SDL");
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        die_fancy("failed to initialize SDL: %s", SDL_GetError());

    log_trace("creating window");
    SDL_Window *win = SDL_CreateWindow(
        "Raytracer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280, 720,
        0
    );

    if (!win)
        die_fancy("failed to create window: %s", SDL_GetError());

    log_trace("creating renderer");
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, 0);
    if (!renderer)
        die_fancy("failed to create renderer: %s", SDL_GetError());
        
    SDL_Event ev;
    bool running = true;

    uint8_t colour = 0;

    while (running) {
        while (SDL_PollEvent(&ev) > 0) {
            switch (ev.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                
                case SDL_KEYDOWN:
                    colour = ~colour;
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, colour, colour, colour, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
    
    log_trace("destroying renderer");
    SDL_DestroyRenderer(renderer);
    log_trace("destroying window");
    SDL_DestroyWindow(win);

    logger_free();
    return 0;
}