#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>

// Structure des boutons
struct Button {
    SDL_FRect rect;
    std::string text;
    SDL_Color color;
    SDL_Color textColor;
    SDL_Texture *iconTexture;
    bool isIcon;
};

// Couleurs
const SDL_Color DARK_BROWN = {60, 40, 40, 255};
const SDL_Color LIGHT_BLUE = {173, 216, 230, 255};
const SDL_Color DARK_GRAY = {40, 40, 40, 255};
const SDL_Color WHITE = {255, 255, 255, 255};
const SDL_Color RED = {255, 50, 50, 255};

// Fonction pour créer une texture texte
SDL_Texture *createTextTexture(SDL_Renderer *renderer, TTF_Font *font, const std::string &text, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "TTF_RenderText_Blended Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return texture;
}

// Fonction principale
int main(int argc, char *argv[]) {
    // Initialisation SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialisation SDL_ttf
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Création de la fenêtre
    SDL_Window *window = SDL_CreateWindow("Application Musicale", 1440, 1024, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Création du renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Chargement de la police
    TTF_Font *font = TTF_OpenFont("arial.ttf", 16);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Boucle principale
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // Nettoyage et rendu de l'écran
        SDL_SetRenderDrawColor(renderer, DARK_BROWN.r, DARK_BROWN.g, DARK_BROWN.b, DARK_BROWN.a);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    // Libération des ressources
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
