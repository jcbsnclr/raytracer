#include <log.h>
#include <util.h>
#include <colour.h>
#include <scene.h>

#include <SDL2/SDL.h>

#include <cglm/cglm.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main(void) {
    logger_init("test.log", LOG_TRACE);

    log_debug("initializing SDL");
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        die_fancy("failed to initialize SDL: %s", SDL_GetError());

    log_debug("creating window");
    SDL_Window *win = SDL_CreateWindow(
        "Raytracer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640, 400,
        0
    );

    if (!win)
        die_fancy("failed to create window: %s", SDL_GetError());

    log_debug("acquiring window surface");
    SDL_Surface *surf = SDL_GetWindowSurface(win);
    if (!surf)
        die_fancy("failed to acquire window surface: %s", SDL_GetError());

    if (surf->format->format != SDL_PIXELFORMAT_RGB888)
        die_fancy("unexpected pixel format (expected SDL_PIXELFORMAT_RGB888, found %s)", SDL_GetPixelFormatName(surf->format->format));

    if (surf->format->BytesPerPixel != 4)
        die_fancy("unexpected bpp (expected 4, found %u)", (uint32_t)surf->format->BytesPerPixel);

    SDL_Event ev;
    bool running = true;
    uint32_t colour = 0;

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

        SDL_LockSurface(surf);

        struct scene sc;
        scene_render(&sc, surf);

        SDL_UnlockSurface(surf);

        SDL_UpdateWindowSurface(win);
    }
    
    log_debug("freeing surface");
    SDL_FreeSurface(surf);
    log_debug("destroying window");
    SDL_DestroyWindow(win);

    logger_free();
    return 0;
}