#include <iostream>
#include <vector>
#include <fstream>
#include "raylib.h"
#include <cstdlib>

#define TILE 32, 32
#define WNDWWIDTH 1024
#define WNDWHEIGHT 576
#define MAP vector<vector<char>>

using namespace std;


struct memoryCell {                       // for the cell being overwritten by player
    int x = 0;
    int y = 0;
    char value = 0;
};

struct memoryCell2 {
    memoryCell m1;
    memoryCell m2;
};

struct player {
    int x;
    int y;
};

void DrawTree(MAP tree, int x, int y);

void DrawStar(MAP star, int X, int Y);

void DrawDownwardPortal(MAP portal_half, int X, int Y);

void DrawDownwardPortal(MAP portal_half, int X, int Y);

void DrawGrass(MAP grass, MAP grass_flowers, int X, int y, bool flowers);

void DrawGrassUnderTree(MAP grass, int X, int Y);

void DrawMap(MAP map, MAP tree, MAP tree_light, MAP tree_apple, MAP star, MAP sausage, MAP grass, MAP grass_flowers, MAP grass_tree, MAP portal_half, Texture2D tosya_tx);

void DrawMapBackground(MAP map_background, MAP grass, MAP grass_tree, MAP grass_flowers, MAP portal_half);


int main(){
    InitWindow(WNDWWIDTH, WNDWHEIGHT, "Tosya RUN");
    SetTargetFPS(60);
    string line;

    fstream file_map("assets/map.txt");
    MAP map;
    fstream file_map_background("assets/map_background.txt");
    MAP map_background;
    fstream file_tree("assets/tree.txt");
    MAP tree;
    fstream file_tree_light("assets/tree_light.txt");
    MAP tree_light;
    fstream file_tree_apple("assets/tree_apple.txt");
    MAP tree_apple;
    fstream file_star("assets/star.txt");
    MAP star;
    fstream file_sausage("assets/player.txt");
    MAP sausage;
    fstream file_grass("assets/grass.txt");
    MAP grass;
    fstream file_grass_flowers("assets/grass_flowers.txt");
    MAP grass_flowers;
    fstream file_grass_for_tree("assets/grass_for_tree.txt");
    MAP grass_tree;
    fstream file_portal_half("assets/portal_half.txt");
    MAP portal_half;

    Image tosya_img = LoadImage("assets/tosya.png");
    Texture2D tosya_tx = LoadTextureFromImage(tosya_img);
    UnloadImage(tosya_img);
    


    memoryCell mem1, mem2, mem3, mem4;
    memoryCell2 memGroup1 = {mem1, mem2}, memGroup2 = {mem3, mem4};
    int lastMem = 1;
    int lastMem2 = 3;

    //               y,  x
    player Player = {3,  1};
    player Tosya  = {13, 30};
    float moveDelay = 0.3f;
    float timer = 0;
    bool canMove = true;

 // error in reading 
    if (
        !file_tree.is_open() || !file_tree_apple.is_open() || !file_map.is_open() ||
        !file_star.is_open() || !file_tree_light.is_open() || !file_grass.is_open() ||
        !file_grass_flowers.is_open() || !file_grass_for_tree.is_open() || 
        !file_sausage.is_open() || !file_portal_half.is_open()
    ) {
        cout << "Unable to open file." << endl; 
        return -1;
    }

 // map reading
    while(getline(file_map, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        map.push_back(row);
    }

    while(getline(file_map_background, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        map_background.push_back(row);
    }

 // object reading
    while(getline(file_tree, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        tree.push_back(row);
    }

    while (getline(file_tree_light, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        tree_light.push_back(row);
    }

    while (getline(file_tree_apple, line)){
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        tree_apple.push_back(row);
    }

    while (getline(file_star, line)){
        vector<char> row;
        for (char c : line){
            row.push_back(c);
        }
        star.push_back(row);
    }

    while (getline(file_sausage, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        sausage.push_back(row);
    }

    while (getline(file_grass, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        grass.push_back(row);
    }

    while (getline(file_grass_flowers, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        grass_flowers.push_back(row);
    }

    while (getline(file_grass_for_tree, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        grass_tree.push_back(row);
    }
    
    while (getline(file_portal_half, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        portal_half.push_back(row);
    }

 // closing all open files
    {file_map.close();
    file_tree.close();
    file_tree_light.close();
    file_tree_apple.close();
    file_star.close();
    file_portal_half.close();
    file_grass.close();
    file_grass_flowers.close();
    file_grass_for_tree.close();
    file_sausage.close();}
    

    mem1.value = map[Player.x][Player.y];
    mem3.value = map[Tosya.x][Tosya.y];
    
 // game cycle
    while (!WindowShouldClose()) {
        BeginDrawing();

     // map drawing
        DrawMapBackground(map_background, grass, grass_tree, grass_flowers, portal_half);
        DrawMap(map, tree, tree_light, tree_apple, star, sausage, grass, grass_flowers, grass_tree, portal_half, tosya_tx);
        
     // spawning Player
        map[Player.x][Player.y] = 'p';
        map[Tosya.x][Tosya.y]   = 'T';
        

     // moving logic                checks for walls
        if (canMove) {
            if (IsKeyDown(KEY_W) && map[Player.x-1][Player.y] != '#' && map[Player.x-1][Player.y] != 'a') {

                if (lastMem == 1) {
                    mem2 = {Player.x-1, Player.y, map[Player.x-1][Player.y]};
                    map[Player.x][Player.y] = mem1.value;
                    Player.x -= 1;
                    lastMem = 2;
                }
                else {
                    mem1 = {Player.x-1, Player.y, map[Player.x-1][Player.y]};
                    map[Player.x][Player.y] = mem2.value;
                    Player.x -= 1;
                    lastMem = 1;
                }

            }
            if (IsKeyDown(KEY_S) && map[Player.x+1][Player.y] != '#' && map[Player.x+1][Player.y] != 'a') {

                if (lastMem == 1) {
                    mem2 = {Player.x+1, Player.y, map[Player.x+1][Player.y]};
                    map[Player.x][Player.y] = mem1.value;
                    Player.x += 1;
                    lastMem = 2;
                }
                else {
                    mem1 = {Player.x+1, Player.y, map[Player.x+1][Player.y]};
                    map[Player.x][Player.y] = mem2.value;
                    Player.x += 1;
                    lastMem = 1;
                }

            }
            if (IsKeyDown(KEY_A) && map[Player.x][Player.y-1] != '#' && map[Player.x][Player.y-1] != 'a') {

                if (lastMem == 1) {
                    mem2 = {Player.x, Player.y-1, map[Player.x][Player.y-1]};
                    map[Player.x][Player.y] = mem1.value;
                    Player.y -= 1;
                    lastMem = 2;
                }
                else {
                    mem1 = {Player.x, Player.y-1, map[Player.x][Player.y-1]};
                    map[Player.x][Player.y] = mem2.value;
                    Player.y -= 1;
                    
                    lastMem = 1;
                }

            }
            if (IsKeyDown(KEY_D) && map[Player.x][Player.y+1] != '#' && map[Player.x][Player.y+1] != 'a') {

                if (lastMem == 1) {
                    mem2 = {Player.x, Player.y+1, map[Player.x][Player.y+1]};
                    map[Player.x][Player.y] = mem1.value;
                    Player.y += 1;
                    lastMem = 2;
                }
                else {
                    mem1 = {Player.x, Player.y+1, map[Player.x][Player.y+1]};
                    map[Player.x][Player.y] = mem2.value;
                    Player.y += 1;
                    
                    lastMem = 1;
                }
            }

            timer = moveDelay;
            canMove = false;
        }
        else {
            timer -= GetFrameTime();
            if (timer <= 0) canMove = true;
        }
        EndDrawing();
    }
    UnloadTexture(tosya_tx);

    CloseWindow();

    
    return 0;
}

void DrawTree(MAP tree, int X, int Y){
    int x = X, y = Y;
    for (const auto& row : tree) {
        x = X;
        for (const auto& col : row) {
            if (col == ' ') {
                x += 1;
            }
            // base tree 
            else if (col == '1') {
                DrawRectangle(x, y, 1, 1, Color{62, 126, 14, 255});
                x += 1;
            }
            else if (col == '2') {
                DrawRectangle(x, y, 1, 1, Color{122, 190, 70, 255});  
                x += 1;
            }
            else if (col == '3') {
                DrawRectangle(x, y, 1, 1, Color{84, 171, 16, 255});   
                x += 1;
            }
            else if (col == '4') {
                DrawRectangle(x, y, 1, 1, Color{21, 80, 47, 255});    
                x += 1;
            }
            // base light tree
            else if (col == 'q') {
                DrawRectangle(x, y, 1, 1, Color{74, 145, 19, 255});
                x += 1;
            }
            else if (col == 'e') {
                DrawRectangle(x, y, 1, 1, Color{112, 169, 69, 255});
                x += 1;
            }
            else if (col == 'r') {
                DrawRectangle(x, y, 1, 1, Color{25, 96, 56, 255});
                x += 1;
            }
            // tree trunk
            else if (col == '5') {
                DrawRectangle(x, y, 1, 1, Color{71, 29, 43, 255});
                x += 1;
            }
            else if (col == '6') {
                DrawRectangle(x, y, 1, 1, Color{140, 67, 78, 255});
                x += 1;
            }
            else if (col == '7') {
                DrawRectangle(x, y, 1, 1, Color{113, 47, 54, 255});
                x += 1;
            }
            // apple
            else if (col == '8') {
                DrawRectangle(x, y, 1, 1, Color{183, 22, 37, 255});
                x += 1;
            }
            else if (col == '9') {
                DrawRectangle(x, y, 1, 1, Color{238, 121, 132, 255});
                x += 1;
            }
        }
        y += 1;
    }
}

void DrawStar(MAP star, int X, int Y){
    int x = X, y = Y;
    for (const auto& row : star) {
        x = X;
        for (const auto& col : row) {
            if (col == ' ') {
                x += 2;
            }
            else if (col == '4') {
                DrawRectangle(x, y, 2, 2, Color{254, 219, 72, 255});
                x += 2;
            }
            else if (col == '2') {
                DrawRectangle(x, y, 2, 2, Color{255, 176, 62, 255});
                x += 2;
            }
            else if (col == '5') {
                DrawRectangle(x, y, 2, 2, Color{254, 239, 76, 255});
                x += 2;
            }
            else if (col == '1') {
                DrawRectangle(x, y, 2, 2, Color{169, 116, 66, 255});
                x += 2;
            }
            else if (col == '3') {
                DrawRectangle(x, y, 2, 2, Color{254, 196, 68, 255});
                x += 2;
            }
        }
        y += 2;
    }
}

void DrawSausage(MAP sausage, int X, int Y) {
    int x = X, y = Y;
    for (const auto& row : sausage) {
        x = X;
        for (const auto& col : row) {
            if (col == ' ') {
                x += 1;
            }
            else if (col == '1') {
                DrawRectangle(x, y, 1, 1, Color{162, 55, 63, 255});
                x += 1;
            }
            else if (col == '2') {
                DrawRectangle(x, y, 1, 1, Color{228, 111, 121, 255});
                x += 1;
            }
            else if (col == '3') {
                DrawRectangle(x, y, 1, 1, Color{223, 88, 99, 255});
                x += 1;
            }
            else if (col == '4') {
                DrawRectangle(x, y, 1, 1, Color{181, 57, 67, 255});
                x += 1;
            }
            else if (col == '5') {
                DrawRectangle(x, y, 1, 1, Color{115, 24, 31, 255});
                x += 1;
            }
        }
        y += 1;
    }
}

void DrawUpwardPortal(MAP portal_half, int X, int Y) {
 // same, as Downward Portal, but flipped upside down
    int x = X+31, y = Y+32;
    for (const auto& row : portal_half) {
        x = X+31;
        for (const auto& col : row) {
            if (col == ' ') {
                x -= 1;
            }
            else if (col == '1') {
                DrawRectangle(x, y, 1, 1, Color{180, 28, 209, 255});
                x -= 1;
            }
            else if (col == '2') {
                DrawRectangle(x, y, 1, 1, Color{143, 25, 166, 255});
                x -= 1;
            }
            else if (col == '3') {
                DrawRectangle(x, y, 1, 1, Color{119, 26, 133, 255});
                x -= 1;
            }
            else if (col == '4') {
                DrawRectangle(x, y, 1, 1, Color{69, 13, 78, 255});
                x -= 1;
            }
            else if (col == '5') {
                DrawRectangle(x, y, 1, 1, Color{254, 178, 240, 255});
                x -= 1;
            }
            else if (col == '6') {
                DrawRectangle(x, y, 1, 1, Color{244, 82, 228, 255});
                x -= 1;
            }
        }
        y -= 1;
    }

}

void DrawDownwardPortal(MAP portal_half, int X, int Y){
    int x = X, y = Y;
    for (const auto& row : portal_half) {
        x = X;
        for (const auto& col : row) {
            if (col == ' ') {
                x += 1;
            }
            else if (col == '1') {
                DrawRectangle(x, y, 1, 1, Color{180, 28, 209, 255});
                x += 1;
            }
            else if (col == '2') {
                DrawRectangle(x, y, 1, 1, Color{143, 25, 166, 255});
                x += 1;
            }
            else if (col == '3') {
                DrawRectangle(x, y, 1, 1, Color{119, 26, 133, 255});
                x += 1;
            }
            else if (col == '4') {
                DrawRectangle(x, y, 1, 1, Color{69, 13, 78, 255});
                x += 1;
            }
            else if (col == '5') {
                DrawRectangle(x, y, 1, 1, Color{254, 178, 240, 255});
                x += 1;
            }
            else if (col == '6') {
                DrawRectangle(x, y, 1, 1, Color{244, 82, 228, 255});
                x += 1;
            }
        }
        y += 1;
    }
}

void DrawGrassUnderTree(MAP grass, int X, int Y) {
    int x = X, y = Y;
    for (const auto& row : grass) {
        x = X;
        for (const auto& col : row) {
            if (col == ' ') {
                x += 2;
            }
            else if (col == '1') {
                DrawRectangle(x, y, 2, 2, Color{62, 157, 53, 255});
                x += 2;
            }
            else if (col == '2') {
                DrawRectangle(x, y, 2, 2, Color{72, 183, 63, 255});  
                x += 2;
            }
            else if (col == '3') {
                DrawRectangle(x, y, 2, 2, Color{39, 101, 36, 255});   
                x += 2;
            }
            else if (col == '4') {
                DrawRectangle(x, y, 2, 2, Color{61, 139, 53, 255});    
                x += 2;
            }
            else if (col == '5') {
                DrawRectangle(x, y, 2, 2, Color{38, 84, 35, 255});    
                x += 2;
            }
            else if (col == '6') {
                DrawRectangle(x, y, 2, 2, Color{27, 56, 25, 255});    
                x += 2;
            }
        }
        y += 2;
    }
}

void DrawGrass(MAP grass, MAP grass_flowers, int X, int Y, bool flowers) {
    int x = X, y = Y;
    if (flowers) grass = grass_flowers;
    for (const auto& row : grass) {
        x = X;
        for (const auto& col : row) {
            if (col == ' ') {
                x += 2;
            }
            else if (col == '1') {
                DrawRectangle(x, y, 2, 2, Color{62, 157, 53, 255});
                x += 2;
            }
            else if (col == '2') {
                DrawRectangle(x, y, 2, 2, Color{72, 183, 63, 255});  
                x += 2;
            }
            else if (col == '3') {
                DrawRectangle(x, y, 2, 2, Color{61, 139, 53, 255});   
                x += 2;
            }
         // flowers
            else if (col == 'W') {
                DrawRectangle(x, y, 2, 2, WHITE);
                x += 2;
            }
            else if (col == 'Y') {
                DrawRectangle(x, y, 2, 2, Color{241, 218, 16, 255});
                x += 2;
            }
         // stones
            else if (col == '4') {
                DrawRectangle(x, y, 2, 2, Color{115, 115, 115, 255});
                x += 2;
            }
            else if (col == '5') {
                DrawRectangle(x, y, 2, 2, Color{131, 131, 131, 255});
                x += 2;
            }
            else if (col == '6') {
                DrawRectangle(x, y, 2, 2, Color{98, 98, 98, 255});
                x += 2;
            }
        }
        y += 2;
    }
}

void DrawMap(MAP map, MAP tree, MAP tree_light, MAP tree_apple, MAP star, MAP sausage, MAP grass, MAP grass_flowers, MAP grass_tree, MAP portal_half, Texture2D tosya_tx) {
    int x = 0, y = 0;

    for (int i = 0; i < map.size(); i++) {
        const auto& row = map[i];
        x = 0;

        for (int j = 0; j < row.size(); j++) {

            const auto& col = row[j];

            if (col == ' ') {
                x += 32;
            }
            
            else if (col == '#') {
                DrawTree(tree, x, y);
                x += 32;
            }
            else if (col == 'a') {
                DrawTree(tree_apple, x, y);
                x += 32;
            }
            else if (col == '%') {
                DrawTree(tree_light, x, y);
                x += 32;
            }
            else if (col == '*') {
                DrawStar(star, x, y);
                x += 32;
            }
            else if (col == 't') {
                x += 32;
            }
            else if (col == 'p') {
                DrawSausage(sausage, x, y);
                x+=32;
            }
            else if (col == 'T') {
                DrawTexture(tosya_tx, x, y, WHITE);
                x += 32;
            }
        }
        y += 32;
    }
}

void DrawMapBackground(MAP map_background, MAP grass, MAP grass_tree, MAP grass_flowers, MAP portal_half) {
    int x = 0, y = 0;
    for (int i = 0; i < map_background.size(); i++) {

        const auto& row = map_background[i];
        x = 0;

        for (int j = 0; j < row.size(); j++) {

            const auto& col = row[j];
            
            if (col == 'g') {
             // randomize grass rendering
                srand(y + x + y^y/4);
                bool variation = ((rand() % 4) + 1) < 4 ? false : true;

                DrawGrass(grass, grass_flowers, x, y, variation);
                x += 32;
            }
            else if (col == 't') {
                DrawGrassUnderTree(grass_tree, x, y);
                x += 32;
            }
            else if (col == 'u') {
                srand(y + x + y^y/4);
                bool variation = ((rand() % 4) + 1) < 4 ? false : true;

                DrawGrass(grass, grass_flowers, x, y, variation);
                DrawUpwardPortal(portal_half, x, y);
                x += 32;
            }
            else if (col == 'd') {
                srand(y + x + y^y/4);
                bool variation = ((rand() % 4) + 1) < 4 ? false : true;

                DrawGrass(grass, grass_flowers, x, y, variation);
                DrawDownwardPortal(portal_half, x, y);
                x += 32;
            }
        }
        y += 32;
    }
}