#include "Editor.hpp"
#include <stdio.h> 

#include <pthread.h>

class Core{

    public:
        Font EditorFont;
        const char* ConfPath = "/usr/local/bin/Config/";
        bool ShowFileDrop = false;
        char AsmPath[100];
};
static Core QIKCore;


void InitEditor(){
    QIKCore.EditorFont = LoadFont(FormatText("%srog.ttf", QIKCore.ConfPath));
    MainEditor.WipeLines();
}


void UpdateEditor(){
    DrawLine(0 , 30 , GetScreenWidth() / 1.5 , 31 , DarkProGray);
    DrawRectangle(0,0,80,30 , DarkProGray);
    if (GetCollision(0,0,80,30 , GetMouseX() , GetMouseY() , 1 , 1)){
        DrawRectangle(0,0,80,30 , MidProGray);

        if (IsMouseButtonPressed(0)){
            char FileName[1024] = "";
            FILE *f = popen("zenity --file-selection", "r");
            fgets(FileName, 1024, f);
            pclose(f);

            cout << FileName;
        }

    }
    DrawTextEx(QIKCore.EditorFont , "Load" , {MeasureTextEx(QIKCore.EditorFont , "Load" , 14 , 1).x / 2 + 80 / 5, 10 } , 14 , 1 , WHITE);

    DrawRectangle(80,0,80,30 , DarkProGray);
    if (GetCollision(80,0,80,30 , GetMouseX() , GetMouseY() , 1 , 1)){
        DrawRectangle(80,0,80,30 , MidProGray);

        if (IsMouseButtonPressed(0)){
            SaveFile(QIKCore.AsmPath);
        }

    }
    DrawTextEx(QIKCore.EditorFont , "Save" , {MeasureTextEx(QIKCore.EditorFont , "Save" , 14 , 1).x / 2 + 80 / 5 + 80, 10 } , 14 , 1 , WHITE);

    DrawRectangle(80 * 2,0,80,30 , DarkProGray);
    if (GetCollision(80 * 2,0,80,30 , GetMouseX() , GetMouseY() , 1 , 1)){
        DrawRectangle(80 * 2,0,80,30 , MidProGray);

        if (IsMouseButtonPressed(0)){
            InitEditor();
            LoadAsmFile(QIKCore.AsmPath);
        }

    }
    DrawTextEx(QIKCore.EditorFont , "Undo" , {MeasureTextEx(QIKCore.EditorFont , "Undo" , 14 , 1).x / 2 + 80 / 5 + 80 * 2, 10 } , 14 , 1 , WHITE);

    DrawTextEx(QIKCore.EditorFont , "QIK Asm Editor" , {GetScreenWidth() / 2 - MeasureTextEx(QIKCore.EditorFont , "QIK Asm Editor" , 14 , 1).x / 2 , 10 } , 14 , 1 , WHITE);

    DrawRectangle(30,50,GetScreenWidth(),GetScreenHeight() , MidProGray);
    DrawRectangleLines(30,50,GetScreenWidth(),GetScreenHeight() , DarkProGray);

    MainEditor.DisplayEditor(30 , 50 , QIKCore.EditorFont);

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)){
        SaveFile(QIKCore.AsmPath);
    }
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z)){
        InitEditor();
        LoadAsmFile(QIKCore.AsmPath);
    }
}