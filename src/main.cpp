#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    // Initialiser SDL avant de créer des objets SDL
    int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if(result < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Créer la fenêtre
    SDL_Window* window = SDL_CreateWindow("SDL3 Window", 1440, 1024, 0);
    if (window == NULL) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -2;
    }

    // Créer le renderer principal
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) {
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -3;
    }

    SDL_Log("SDL initialized successfully");

    // Définir le rectangle
    SDL_FRect rect;  // Utiliser SDL_FRect pour SDL3
    rect.x = 0;
    rect.y = 0;
    rect.w = 1440;
    rect.h = 130;

    SDL_Event event;
    int quit = 0;
    while(!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    SDL_Log("Quit event received");
                    quit = 1;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.key == SDLK_ESCAPE) {
                        SDL_Log("Escape key quit");
                        quit = 1;
                    }
                    break;
            }
        }

        // Effacer l'écran avec une couleur de fond
        SDL_SetRenderDrawColor(renderer, 238, 232, 232, 255);
        SDL_RenderClear(renderer);

        // Dessiner le rectangle rouge
        SDL_SetRenderDrawColor(renderer, 50, 19, 19, 20);
        SDL_RenderFillRect(renderer, &rect);  // Pour un rectangle plein
        // ou SDL_RenderRect(renderer, &rect); // Pour un rectangle vide (contour)

        // Afficher le rendu
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    SDL_Log("SDL3 SHUTDOWN");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}