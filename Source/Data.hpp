#include <iostream>
#include <string>
#include <sstream>

#include <fstream>
#include <raylib.h>
#include <stdlib.h>
#include <cstring>

using namespace std;

static int LineSpacing = 14;


Color ProGray = {33,33,33,255};
Color MidProGray = {26,26,26,255};
Color DarkProGray = {22,22,22,255};

Color GhostProGray = {44,44,44,155};

auto LoadFile(const char* Path){ 
    std::ifstream Input(Path);
    std::stringstream sstr;

    while(Input >> sstr.rdbuf());

    return sstr.str();
}


auto ReadValue(string Data , int Start , int End){ // Reads Value from (Start to End) 
    string Out;
    for (int i = Start ; i <= End ; i ++){
        Out += Data[i]; 
    }
    return Out;
}

auto SplitValue(string Data , int Place ){ 
    string Out;

    int PlaceCounter = 0; 

    int Start = 0;
    int End = 0;
    
    if (Place == 0){
        for (int i = 0 ; i <= Data.size() ; i ++){
            if (Data[i] == ' ' || i == Data.size()){
                End = i - 1;
                break;
            }
        }
    }
    else {
        Place = Place;
        PlaceCounter = 1;
        for (int i = 0 ; i <= Data.size() ; i ++){
            if (PlaceCounter == Place){
                Start = i + 1;
                for (int x = i+1 ; x <= Data.size(); x++){
                    if (Data[x] == ' ' || x == Data.size()){
                        End = x - 1;
                        break;
                    }
                }
            }
            if (Data[i] == ' '){
                PlaceCounter ++;
            }
        }
    }

    Out = ReadValue(Data , Start , End);

    return Out;
}

bool GetCollision(int x, int y , int w, int h, int x2 , int y2 , int w2 , int h2){
    if( y+h <= y2 )
    {
        return false;
    }

    if( y >= y2+h2 )
    {
        return false;
    }

    if( x+w <= x2 )
    {
        return false;
    }

    if( x >= x2+w2 )
    {
        return false;
    }

    return true;
}

Color RunSyntaxHighlight(string Line){
    bool FoundColor = false;

    int Values = 0;

    for (int i = 0 ; i <= Line.size(); i ++)if (Line[i] == ' ')Values++;

    for (int i = 0 ; i <= Values; i ++){
        if (SplitValue(Line , i) == "ld"){
            FoundColor = true;
            return GREEN;
        }
        if (SplitValue(Line , i) == "dw"){
            FoundColor = true;
            return YELLOW;
        }
        if (SplitValue(Line , i) == "INCLUDE"){
            FoundColor = true;
            return PINK;
        }
        if (SplitValue(Line , i) == "jp"){
            FoundColor = true;
            return DARKPURPLE;
        }        
        if (SplitValue(Line , i) == "call"){
            FoundColor = true;
            return GOLD;
        }
        if (SplitValue(Line , i) == "ret"){
            FoundColor = true;
            return BLUE;
        }
    }
    if (!FoundColor){
        return WHITE;
    }
}

class Editor{
    public:
        string Lines[10000];
        int ActiveLines = 1;
        bool ShowEditor = true;
        int SelectedLine = 1;
        int BackspaceDelay = 5;
        int CharacterSelector = 0;
        int Scroll = 0;

        void WipeLines(){
            for (int i = 0; i <= 9999; i ++){
                Lines[i] = "";
            }

            ActiveLines = 1;
        }

        void DrawStatus(Font EFont){
            if (ActiveLines < 1){
                DrawTextEx(EFont , "File Status: Empty", {float(50), float(GetScreenHeight() - 30 )} , 14 , 1 , WHITE);
            }
            else {
                DrawTextEx(EFont , FormatText("Lines: %i", ActiveLines), {float(50), float(GetScreenHeight() - 30 )} , 14 , 1 , WHITE);
            }
        }

        auto DisplayEditor(int X , int Y , Font EFont){

            if (Scroll < 0){
                Scroll = 0;
            }
            if (Scroll > 8000){
                Scroll = 8000;
            }
            for (int i = Scroll ; i <= Scroll + GetScreenHeight() / 16 ; i++){

                Color TextColor = RunSyntaxHighlight(Lines[i]);

                for (int x = 0 ; x <= Lines[i].size(); x++){
                    if(Lines[i][x] == ':'){
                        TextColor = RED;
                    }
                }


                DrawTextEx(EFont , FormatText("%i", i), {float(X), (float(Y)+ i * LineSpacing) - Scroll* LineSpacing} , 14 , 1 , TextColor);

                if (i == SelectedLine){
                    DrawRectangle(X + 20, ((Y  + 1)+ i * LineSpacing )- Scroll* LineSpacing, GetScreenWidth() , 10 , GhostProGray);
                }

                DrawTextEx(EFont , FormatText("%s", Lines[i].c_str()), {float(X) + 22, (float(Y)+ i * LineSpacing ) - Scroll* LineSpacing} , 14 , 1 , TextColor);

            }

            DrawTextEx(EFont , FormatText("|"), {float(X) + 22 + (CharacterSelector * MeasureTextEx(EFont , "T" , 14 , 1).x), (float(Y) + SelectedLine * LineSpacing )- Scroll* LineSpacing} , 14 , 1 , WHITE);

            int Key = GetCharPressed();

            while (Key > 0)
            {
                if ((Key >= 32) && (Key <= 125))
                {
                    cout << (char)Key;
                    Lines[SelectedLine] += (char)Key;
                    CharacterSelector++;
                }
                Key = GetCharPressed();
            }
            

            if (IsKeyPressed(KEY_ENTER)){
                int i;
            
                // increase the size by 1
                ActiveLines++;

                SelectedLine ++ ;

                // shift elements forward
                for (i = ActiveLines; i >= SelectedLine; i--)
                    Lines[i] = Lines[i - 1];
            
                // insert x at pos
                Lines[SelectedLine - 1] = "";

                CharacterSelector = 0;
            }

            if (IsKeyPressed(KEY_BACKSPACE) && ActiveLines > 0){
               
                if (Lines[SelectedLine].empty()){

                    for (int i = SelectedLine; i<= ActiveLines;i++){
                        Lines[i] = Lines[i +1];
                    }
                    ActiveLines --;

                    SelectedLine = SelectedLine - 1;
                    int Size = Lines[SelectedLine].size();
                    if (Size > 1){
                        CharacterSelector = Size;
                    }
                    else {
                        CharacterSelector = 0;
                    }

                }
                else {
                    int Size = Lines[SelectedLine].size();
                    Lines[SelectedLine].resize(Size-1);
                    BackspaceDelay = 2;
                    CharacterSelector --;
                }

            }

            if (IsMouseButtonDown(0)){
                if (GetMouseY() > 30){
                    int MY = GetMouseY() - Y + Scroll* LineSpacing;

                    if (MY / LineSpacing <= ActiveLines){
                        SelectedLine = MY / LineSpacing;
                    }
                    CharacterSelector = Lines[SelectedLine].size();
                }
            }

            if (IsKeyPressed(KEY_UP)){
                SelectedLine --;
            }
            if (IsKeyPressed(KEY_DOWN)){
                SelectedLine ++;
            }

            Scroll -= (GetMouseWheelMove()*3);

            if (Scroll < 0){
                Scroll = 0;
            }
            if (Scroll > 8000){
                Scroll = 8000;
            }


            
            DrawStatus(EFont);
        }
};

static Editor MainEditor;

void LoadAsmFile(const char *ScriptPath){ // Loads Script
    istringstream ProgramData(LoadFile(ScriptPath));
    string CurrentLine;
    int LineCounter = 0;

    while (getline(ProgramData, CurrentLine)) {
        MainEditor.Lines[LineCounter] = CurrentLine;
        LineCounter ++;
    } 
    MainEditor.ActiveLines = LineCounter;
} 
auto SaveFile(const char* Path){
    std::ofstream out(Path);

    for (int i = 0 ; i <= MainEditor.ActiveLines ; i++){
        out << MainEditor.Lines[i] << "\n";
    }

    out.close();

    cout << "Saved\n";
}