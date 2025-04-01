#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_events.h>
#include <iostream>
#include <vector>
#include <string>

// Structure pour les boutons
struct Button {
    SDL_FRect rect;
    SDL_Color color;
    std::string name;
    SDL_Texture* icon;  // Pour ajouter des icônes plus tard si nécessaire
};

// Dessiner un chiffre digital style LCD rouge
void drawDigit(SDL_Renderer* renderer, int digit, float x, float y, float w, float h) {
    // Segments: 0=top, 1=top-right, 2=bottom-right, 3=bottom, 4=bottom-left, 5=top-left, 6=middle
    bool segments[10][7] = {
            {1, 1, 1, 1, 1, 1, 0}, // 0
            {0, 1, 1, 0, 0, 0, 0}, // 1
            {1, 1, 0, 1, 1, 0, 1}, // 2
            {1, 1, 1, 1, 0, 0, 1}, // 3
            {0, 1, 1, 0, 0, 1, 1}, // 4
            {1, 0, 1, 1, 0, 1, 1}, // 5
            {1, 0, 1, 1, 1, 1, 1}, // 6
            {1, 1, 1, 0, 0, 0, 0}, // 7
            {1, 1, 1, 1, 1, 1, 1}, // 8
            {1, 1, 1, 1, 0, 1, 1}  // 9
    };

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    float thickness = h / 10.0f;

    // Dessiner les segments actifs
    if(segments[digit][0]) { // Top
        SDL_FRect seg = {x + thickness, y, w - 2*thickness, thickness};
        SDL_RenderFillRect(renderer, &seg);
    }
    if(segments[digit][1]) { // Top-right
        SDL_FRect seg = {x + w - thickness, y, thickness, h/2};
        SDL_RenderFillRect(renderer, &seg);
    }
    if(segments[digit][2]) { // Bottom-right
        SDL_FRect seg = {x + w - thickness, y + h/2, thickness, h/2};
        SDL_RenderFillRect(renderer, &seg);
    }
    if(segments[digit][3]) { // Bottom
        SDL_FRect seg = {x + thickness, y + h - thickness, w - 2*thickness, thickness};
        SDL_RenderFillRect(renderer, &seg);
    }
    if(segments[digit][4]) { // Bottom-left
        SDL_FRect seg = {x, y + h/2, thickness, h/2};
        SDL_RenderFillRect(renderer, &seg);
    }
    if(segments[digit][5]) { // Top-left
        SDL_FRect seg = {x, y, thickness, h/2};
        SDL_RenderFillRect(renderer, &seg);
    }
    if(segments[digit][6]) { // Middle
        SDL_FRect seg = {x + thickness, y + h/2 - thickness/2, w - 2*thickness, thickness};
        SDL_RenderFillRect(renderer, &seg);
    }
}

// Dessiner un piano
// Fonction corrigée pour dessiner un piano
void drawPiano(SDL_Renderer* renderer, float x, float y, float w, float h) {
    // Fond blanc pour le clavier complet
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect pianoBackground = {x, y, w, h};
    SDL_RenderFillRect(renderer, &pianoBackground);

    // Bordure du piano
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &pianoBackground);

    // Dessiner les touches blanches avec leurs bordures
    float whiteKeyWidth = w / 21.0f; // 21 touches blanches

    for(int i = 0; i < 21; i++) {
        // Bordures verticales entre les touches blanches
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_FRect keyBorder = {x + (i + 1) * whiteKeyWidth, y, 1.0f, h};
        SDL_RenderFillRect(renderer, &keyBorder);
    }

    // Dessiner les touches noires
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    float blackKeyWidth = whiteKeyWidth * 0.6f;
    float blackKeyHeight = h * 0.65f;

    for(int i = 0; i < 21; i++) {
        // Ne pas dessiner de touche noire après la dernière touche blanche
        if(i == 20) continue;

        // Motif pour les touches noires (après les touches 0, 1, 3, 4, 5...)
        // Pattern pour un clavier standard: pas de noir après E et B (positions 2 et 6 dans chaque octave)
        if((i % 7 != 2) && (i % 7 != 6)) {
            SDL_FRect key = {x + i * whiteKeyWidth + whiteKeyWidth - blackKeyWidth/2, y, blackKeyWidth, blackKeyHeight};
            SDL_RenderFillRect(renderer, &key);
        }
    }
}

int main(int argc, char* argv[]) {
    // Initialiser SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Dimensions de la fenêtre
    int windowWidth = 1440;
    int windowHeight = 1024;

    SDL_Window* window = SDL_CreateWindow("Piano Interface", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    if(!window) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if(!renderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Créer les boutons de la barre d'outils
    std::vector<Button> buttons;
    float buttonWidth = 121.0f;  // Taille exacte selon Figma
    float buttonHeight = 90.0f;  // Taille exacte selon Figma
    float buttonSpacing = 50.0f; // Espacement de 50px entre les boutons
    float toolbarY = 20.0f;      // Position Y à 20px
    float startX = 63.0f;        // Position X du premier bouton à 63px

    SDL_Color darkGray = {46, 46, 46, 255}; // Couleur des boutons (plus foncée)

    // Tableau des noms de boutons
    std::vector<std::string> buttonNames = {
            "Select",
            "Remove Octave",
            "Add Octave",
            "Import File",
            "Play Song",
            "Start Recording",
            "Export",
            "Finish Recording"
    };

    // Créer les 8 boutons avec les bonnes dimensions et espacements
    for(int i = 0; i < 8; i++) {
        buttons.push_back({
                                  {startX + i * (buttonWidth + buttonSpacing), toolbarY, buttonWidth, buttonHeight},
                                  darkGray,
                                  buttonNames[i],
                                  nullptr // Pas d'icône pour l'instant
                          });
    }

    // Calculer les dimensions de l'interface principale
    float topBarHeight = 129.0f;  // Hauteur de la barre du haut selon Figma
    float mainAreaX = 50.0f;
    float mainAreaY = toolbarY + buttonHeight + 30.0f;
    float mainAreaWidth = windowWidth - 100.0f;
    float mainAreaHeight = windowHeight - mainAreaY - 50.0f;

    // Dimensions de l'en-tête
    float headerHeight = 50.0f;

    // Dimensions du panneau de piano
    float pianoPanelHeight = 150.0f;
    float pianoHeight = 250.0f;

    // Couleur marron pour la barre de menu (selon le Figma)
    SDL_Color brownColor = {90, 60, 55, 255}; // Ajuster si nécessaire pour correspondre exactement

    // Boucle principale
    bool quit = false;
    SDL_Event event;

    while(!quit) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            else if(event.type == SDL_EVENT_KEY_DOWN) {
                if(event.key.key == SDLK_ESCAPE) {
                    quit = true;
                }
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // Dessiner la surface de travail (background)
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
        SDL_FRect workspace = {0, 0, (float)windowWidth, (float)windowHeight};
        SDL_RenderFillRect(renderer, &workspace);

        // Dessiner la barre de menu marron en haut
        SDL_SetRenderDrawColor(renderer, brownColor.r, brownColor.g, brownColor.b, brownColor.a);
        SDL_FRect topBar = {0, 0, (float)windowWidth, topBarHeight};
        SDL_RenderFillRect(renderer, &topBar);

        // Dessiner les boutons dans la barre marron
        for(const auto& button : buttons) {
            // Fond du bouton
            SDL_SetRenderDrawColor(renderer, button.color.r, button.color.g, button.color.b, button.color.a);
            SDL_RenderFillRect(renderer, &button.rect);

            // On pourrait ajouter le rendu du texte des boutons ici
            // avec SDL_ttf si nécessaire
        }

        // Dessiner l'en-tête instrument
        SDL_SetRenderDrawColor(renderer, 185, 211, 230, 255);
        SDL_FRect instrumentHeader = {mainAreaX, mainAreaY, mainAreaWidth, headerHeight};
        SDL_RenderFillRect(renderer, &instrumentHeader);

        // Panneau du piano
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
        SDL_FRect pianoPanel = {mainAreaX, mainAreaY + headerHeight + 10, mainAreaWidth, pianoPanelHeight};
        SDL_RenderFillRect(renderer, &pianoPanel);

        // Zone d'affichage pour le nom de fichier
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_FRect fileNameDisplay = {pianoPanel.x + pianoPanel.w * 0.25f, pianoPanel.y + 25, pianoPanel.w * 0.5f, pianoPanel.h - 50};
        SDL_RenderFillRect(renderer, &fileNameDisplay);

        // Zones d'affichage pour les chiffres
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        float digitSize = pianoPanel.h - 50;
        SDL_FRect leftDigit = {pianoPanel.x + 30, pianoPanel.y + 25, digitSize, digitSize};
        SDL_FRect rightDigit = {pianoPanel.x + pianoPanel.w - digitSize - 30, pianoPanel.y + 25, digitSize, digitSize};
        SDL_RenderFillRect(renderer, &leftDigit);
        SDL_RenderFillRect(renderer, &rightDigit);

        // Dessiner les chiffres
        drawDigit(renderer, 3, leftDigit.x + 10, leftDigit.y + 10, digitSize - 20, digitSize - 20);
        drawDigit(renderer, 7, leftDigit.x + (digitSize * 0.65f), leftDigit.y + 10, digitSize * 0.25f, digitSize - 20);
        drawDigit(renderer, 5, rightDigit.x + 10, rightDigit.y + 10, digitSize - 20, digitSize - 20);

        // Dessiner le piano
        drawPiano(renderer, mainAreaX, mainAreaY + headerHeight + pianoPanelHeight + 20, mainAreaWidth, pianoHeight);

        // Présenter le rendu
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Nettoyer et quitter
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}