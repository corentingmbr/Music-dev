#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]){

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    int result = SDL_Init(SDL_INIT_VIDEO    | SDL_INIT_EVENTS);
    if(result < 0){
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    window = SDL_CreateWindow("SDL3 Window", 800, 600,0);
    if (window == NULL){
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -2;
    }
    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL){
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -3;
    }

    SDL_Log("SDL initialized successfully");


    SDL_Event event;
    int quit = 0;
    while(!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    SDL_Log("Quit event received");
                    quit = 1;
                    break;

            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0xff, 0xff);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Simulate a frame delay
    }

    SDL_Log("SDL3 SHUTDOWN");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}