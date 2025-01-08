#include <raylib.h>
#include "parser.hpp"


int main(void) {
    // Initialisierung
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Fullscreen-Button Demo");
    SetTargetFPS(60);

    bool isFullscreen = false;         //x      y    length heigth
    Rectangle fullscreenButton = { 700, 10, 80, 30 };  // Position und Größe des Buttons

    // Hauptschleife
    while (!WindowShouldClose()) {
        // Eingaben verarbeiten
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, fullscreenButton)) {
                isFullscreen = !isFullscreen;
                ToggleFullscreen();  // Vollbildmodus umschalten
            }
        }

        // Zeichnen
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Drücke den Button, um Vollbild umzuschalten!", 10, 50, 20, DARKGRAY);

        // Button zeichnen
        DrawRectangleRec(fullscreenButton, LIGHTGRAY);
        DrawText("Fullscreen", fullscreenButton.x + 5, fullscreenButton.y + 5, 10, BLACK);

        EndDrawing();
    }

    // Aufräumen
    CloseWindow();
    return 0;
}
