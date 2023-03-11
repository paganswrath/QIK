#include "Core.hpp"


int main(int argc, char **argv){
    InitWindow(800 , 600, "QIK GB Assembly Editor");
    SetTargetFPS(60);

    SetWindowState(FLAG_WINDOW_RESIZABLE);

    InitEditor();

    if (argc > 1){
        LoadAsmFile(argv[1]);
        strcpy(QIKCore.AsmPath , argv[1]);
    }
    else {
        strcpy(QIKCore.AsmPath , "Main.asm");
    }

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(ProGray);

        UpdateEditor();

        if (GetScreenWidth() < 800){
            SetWindowSize(800 , 600);
        }

        EndDrawing();
    }
    UnloadFont(QIKCore.EditorFont);
    CloseWindow();
}