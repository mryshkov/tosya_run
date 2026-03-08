#include <iostream>
#include <vector>
#include <fstream>
#include "raylib.h"
#include <cstdlib>
#include <cmath>
#include <queue>
#include <algorithm>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define TILE 32, 32
#define INF 1e9
#define WIDTH 1024
#define HEIGHT 576
#define MAP vector<vector<char>>

using namespace std;


struct memoryCell {
    int x = 0;
    int y = 0;
    char value = 0;
};

struct player {
    int x;
    int y;
};

struct retBfs {
    int dist;
    vector<player> finalMoves;
};


    // initializing sprites
    fstream file_map           ("assets/map.txt");
    MAP     map;
    fstream file_map_background("assets/map_background.txt");
    MAP     map_background;
    fstream file_tree          ("assets/tree.txt");
    MAP     tree;
    fstream file_tree_light    ("assets/tree_light.txt");
    MAP     tree_light;
    fstream file_tree_apple    ("assets/tree_apple.txt");
    MAP     tree_apple;
    fstream file_star          ("assets/star.txt");
    MAP     star;
    fstream file_sausage       ("assets/player.txt");
    MAP     sausage;
    fstream file_grass         ("assets/grass.txt");
    MAP     grass;
    fstream file_grass_flowers ("assets/grass_flowers.txt");
    MAP     grass_flowers;
    fstream file_grass_for_tree("assets/grass_for_tree.txt");
    MAP     grass_tree;
    fstream file_portal_half   ("assets/portal_half.txt");
    MAP     portal_half;


vector<player> FindStars();

player TosyaLookAround(float startX, float startY, float maxRange);

retBfs bfs(MAP &map, int startX, int startY, int finishX, int finishY);

void DrawTree(int x, int y, bool apples, bool isLight);

void DrawStar(int X, int Y);

void DrawDownwardPortal(int X, int Y);

void DrawDownwardPortal(int X, int Y);

void DrawGrass(int X, int y, bool flowers);

void DrawGrassUnderTree(int X, int Y);

void DrawMap(Texture2D tosya_tx);

void DrawMapBackground();

bool isGameLost(player Tosya, player Player);

bool isGameWon(char mem1, char mem2, int lastMem);

void DrawGameLost();

void DrawGameWon();


int main(){
    InitWindow(WIDTH, HEIGHT, "Tosya RUN");
    SetTargetFPS(60);
    string line;

    Image tosya_img = LoadImage("assets/tosya.png");
    Texture2D tosya_tx = LoadTextureFromImage(tosya_img);
    UnloadImage(tosya_img);

    memoryCell mem1, mem2, mem3, mem4;
    int lastMem = 1;
    int lastMem2 = 3;

    //               y,  x
    player Player = {3,  1};
    player Tosya  = {13, 30};
    float logicDelay = 0.3f;
    float timer = 0;
    bool canMove = true;
    vector<player> stars;
    vector<player> starsFound;
    int collectedStarsCount = 0;
    player lastSeenPlayer = {0, 0};
    player currentVision = {-1, -1};
    int dist;
    vector<player> finalMoves(1, player{0, 0});


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

    stars = FindStars();
    
    // game cycle
    while (!WindowShouldClose()) {
        // spawning Player
        map[Player.x][Player.y] = 'p';
        map[Tosya.x][Tosya.y]   = 'T';
        
        
        if (canMove) {
            // ============================================================================================
            //          moving logic for player           checks for walls                                 
            // ============================================================================================
            if (!isGameWon(mem1.value, mem2.value, lastMem) && !isGameLost(Tosya, Player) && IsKeyDown(KEY_W) && map[Player.x-1][Player.y] != '#' && map[Player.x-1][Player.y] != 'a') {

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
            if (!isGameWon(mem1.value, mem2.value, lastMem) && !isGameLost(Tosya, Player) && IsKeyDown(KEY_S) && map[Player.x+1][Player.y] != '#' && map[Player.x+1][Player.y] != 'a') {

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
            if (!isGameWon(mem1.value, mem2.value, lastMem) && !isGameLost(Tosya, Player) && IsKeyDown(KEY_A) && map[Player.x][Player.y-1] != '#' && map[Player.x][Player.y-1] != 'a') {

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
            if (!isGameWon(mem1.value, mem2.value, lastMem) && !isGameLost(Tosya, Player) && IsKeyDown(KEY_D) && map[Player.x][Player.y+1] != '#' && map[Player.x][Player.y+1] != 'a') {

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

            map[Player.x][Player.y] = 'p';


            // check if star is collected
            if (starsFound.size() < stars.size()) {
                for (const auto& star : stars) {
                    if (Player.x == star.x && Player.y == star.y) {
                        starsFound.push_back(star);
                        collectedStarsCount++;
                    }
                }
                for (int i = 0; i < starsFound.size(); i++) {
                    const auto& starFound = starsFound[i];

                    if (map[starFound.x][starFound.y] == '*') {
                        map[0][31-i] = '*';
                        map[starFound.x][starFound.y] = ' ';
                    }
                }
            }

            // ============================================================================================
            //                                    moving logic for tosya                                   
            // ============================================================================================
            currentVision = TosyaLookAround(Tosya.y, Tosya.x, 20);

            if (currentVision.x != -1 && currentVision.y != -1) {
                lastSeenPlayer = currentVision;
            }
            
            if (lastSeenPlayer.x != 0 && lastSeenPlayer.y != 0) {
                retBfs result = bfs(map, Tosya.y, Tosya.x, lastSeenPlayer.y, lastSeenPlayer.x);
                dist = result.dist;
                finalMoves = result.finalMoves;

                if (!finalMoves.empty()){
                    if (lastMem2 == 3) {
                        mem4.value = map[finalMoves[0].x][finalMoves[0].y];
                        map[Tosya.x][Tosya.y] = mem3.value;

                        Tosya.x = finalMoves[0].x;
                        Tosya.y = finalMoves[0].y;

                        lastMem2 = 4;
                    }
                    else {
                        mem3.value = map[finalMoves[0].x][finalMoves[0].y];
                        map[Tosya.x][Tosya.y] = mem4.value;

                        Tosya.x = finalMoves[0].x;
                        Tosya.y = finalMoves[0].y;

                        lastMem2 = 3;
                    }
                } 
                else {
                    lastSeenPlayer = {0, 0};
                }
            }

            map[Tosya.x][Tosya.y] = 'T';
            
            timer += logicDelay;
            canMove = false;
        }
        else {
            timer -= GetFrameTime();
            if (timer <= 0) canMove = true;
        }

        // ============================================================================================
        //                                        GAME END                                             
        // ============================================================================================
        if (isGameLost(Tosya, Player)) {
            DrawGameLost();
        }
        if (isGameWon(mem1.value, mem2.value, lastMem)) {
            DrawGameWon();
        }


        // ============================================================================================
        //                                         DRAWING                                  
        // ============================================================================================
        BeginDrawing();
            DrawMapBackground();
            DrawMap(tosya_tx);
            if (isGameLost(Tosya, Player)) {
                DrawGameLost();
            }
            if (isGameWon(mem1.value, mem2.value, lastMem)) {
                DrawGameWon();
            }
        EndDrawing();
    }
    UnloadTexture(tosya_tx);

    CloseWindow();
    
    return 0;
}

vector<player> FindStars() {
    vector<player> stars;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == '*'){
                stars.push_back(player{i, j});
            }
        }
    }

    return stars;
}

player TosyaLookAround(float startX, float startY, float maxRange) {
    float startAngle = 0.0f;
    float endAngle = 360.0f;

    for (float angle = startAngle; angle < endAngle; angle += 1.0f) {
        float angleRad = angle * (M_PI / 180.0f);

        float dx = cos(angleRad);
        float dy = sin(angleRad);

        float currX = startX + 0.5f;
        float currY = startY + 0.5f;

        for (float distance = 0.0f; distance < maxRange; distance += 0.5f) {
            currX += dx * 0.5f;
            currY += dy * 0.5f;

            int mapX = static_cast<int>(currX + 0.001f);
            int mapY = static_cast<int>(currY + 0.001f);

            //checking map borders
            if (mapY <= 0 || mapY >= map.size() - 1 || mapX <= 0 || mapX >= map[0].size() - 1 ) {
                break;
            }
            
            // checking diagonals
            if (
                    (map[mapY+1][mapX] == '#' && map[mapY][mapX+1] == '#') || 
                    (map[mapY+1][mapX] == '#' && map[mapY][mapX-1] == '#') || 
                    (map[mapY-1][mapX] == '#' && map[mapY][mapX+1] == '#') || 
                    (map[mapY-1][mapX] == '#' && map[mapY][mapX-1] == '#')
                ) {
                break;
            }

            // wall check
            if (map[mapY][mapX] == '#') {
                break;
            }
            else if (map[mapY][mapX] == 'p') {
                return { mapY, mapX };
            }

            currX += dx * 0.5f;
            currY += dy * 0.5f;
        }
    }
    return {-1, -1};
}

retBfs bfs(MAP &map, int startX, int startY, int finishX, int finishY){
    vector<player> finalMoves;
    vector<vector<int>> dist(map.size(), vector<int>(map[0].size(), INF));
    vector<vector<player>> from(map.size(), vector<player>(map[0].size(), {-1, -1}));
    queue<player> q;

    dist[startY][startX] = 0;
    q.push({ startY, startX });

    vector<int> dx = { 0, 1, -1, 0 };
    vector<int> dy = { -1, 0, 0, 1 };

    while(!q.empty()) {
        auto [y, x] = q.front();
        q.pop();

        for (int d = 0; d < dy.size(); d++) {
            int ty = y + dy[d];
            int tx = x + dx[d];

            if (ty >= 0 && ty < map.size() && tx >= 0 && tx < map[0].size() && 
                map[ty][tx] != '#' && dist[ty][tx] > dist[y][x] + 1) {
                
                dist[ty][tx] = dist[y][x] + 1;
                from[ty][tx] = {y, x};
                q.push({ ty, tx });
            }
        }
    }

    if (dist[finishY][finishX] != INF) {
        int y = finishY;
        int x = finishX;
        while(x != startX || y != startY) {
            finalMoves.push_back({ y, x });
            auto [py, px] = from[y][x];
            y = py;
            x = px;
        }

        reverse(finalMoves.begin(), finalMoves.end());
    }

    return {dist[finishY][finishX], finalMoves};
}

void DrawTree(int X, int Y, bool apples, bool isLight){
    int x = X, y = Y;
    MAP tree_tmp = tree;
    if (apples) tree_tmp = tree_apple;
    else if (isLight) tree_tmp = tree_light;
    
    for (const auto& row : tree_tmp) {
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

void DrawStar(int X, int Y){
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

void DrawSausage(int X, int Y) {
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

void DrawUpwardPortal(int X, int Y) {
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

void DrawDownwardPortal(int X, int Y){
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

void DrawGrassUnderTree(int X, int Y) {
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

void DrawGrass(int X, int Y, bool flowers) {
    int x = X, y = Y;
    MAP grass_tmp = grass;
    if (flowers) grass_tmp = grass_flowers;
    for (const auto& row : grass_tmp) {
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

void DrawMap(Texture2D tosya_tx) {
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
                srand(y - x - (y^(y/4)) * 771);
                bool variation = ((rand() % 3) + 1) < 3 ? false : true;

                DrawTree(x, y, variation, false);
                x += 32;
            }
            else if (col == 'a') {
                DrawTree(x, y, true, false);
                x += 32;
            }
            else if (col == '%') {
                DrawTree(x, y, false, true);
                x += 32;
            }
            else if (col == '*') {
                DrawStar(x, y);
                x += 32;
            }
            else if (col == 't') {
                x += 32;
            }
            else if (col == 'p') {
                DrawSausage(x, y);
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

void DrawMapBackground() {
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

                DrawGrass(x, y, variation);
                x += 32;
            }
            else if (col == 't') {
                DrawGrassUnderTree(x, y);
                x += 32;
            }
            else if (col == 'u') {
                DrawGrass(x, y, false);
                DrawUpwardPortal(x, y);
                x += 32;
            }
            else if (col == 'd') {
                srand(y + x + y^y/4);
                bool variation = ((rand() % 4) + 1) < 4 ? false : true;

                DrawGrass(x, y, variation);
                DrawDownwardPortal(x, y);
                x += 32;
            }
        }
        y += 32;
    }
}

bool isGameLost(player Tosya, player Player) {
    if (Tosya.x == Player.x && Tosya.y == Player.y) {
        return true;
    }
    return false;
}

bool isGameWon(char mem1, char mem2, int lastMem) {
    if (lastMem == 1 && mem1 == 't') {
        return true;
    } 
    else if (lastMem == 2 && mem2 == 't') {
        return true;
    }
    return false;
}

void DrawGameLost() {
    DrawRectangle(10*32, 6*32, 12*32, 7*32, Color{181, 57, 67, 255});
    DrawText("get fucked", 13*32, 9*32, 32, DARKGRAY);
}

void DrawGameWon() {
    DrawRectangle(10*32, 6*32, 12*32, 7*32, Color{122, 190, 70, 255});
    DrawText("you won!", 14*32, 9*32, 32, DARKGRAY);
}