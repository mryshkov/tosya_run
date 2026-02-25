#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "raylib.h"

#define TILE 32, 32

using namespace std;


static void DrawTree(vector<vector<char>> tree, int x, int y);

static void DrawMap(vector<vector<char>> map, vector<vector<char>> tree, vector<vector<char>> tree_light, char memoryCell, int memX, int memY);


int main(){
    InitWindow(1024, 576, "Game");
    SetTargetFPS(60);
    
    string line;
    int memX, memY;
    char player = 'p', memoryCell;

    // map reading
    fstream file_map("assets/map.txt");
    vector<vector<char>> map;
    while(getline(file_map, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        map.push_back(row);
    }

    // object reading
    fstream file_tree("assets/tree.txt");
    vector<vector<char>> tree;
    while(getline(file_tree, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        tree.push_back(row);
    }

    fstream file_tree_light("assets/tree_light.txt");
    vector<vector<char>> tree_light;
    while (getline(file_tree_light, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        tree.push_back(row);
    }

    // error in reading 
    if (!file_map.is_open() || !file_tree.is_open() || !file_tree_light.is_open()) {
        cout << "Unable to open file." << endl; 
        return -1;
    } 
    else {
        file_map.close();
        file_tree.close();
    }

    // game cycle
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGREEN);

        // map drawing
        int x = 0, y = 0;
        DrawMap(map, tree, tree_light, memoryCell, memX, memY);
        

        // spawning tosya
        /*for (auto row = map.begin(); row < map.end(); row++){
            auto elem = find(row->begin(), row->end(), 'p');
            if(elem != row->end()) {
                cout << elem;
            }
        }*/

        EndDrawing();
    }

    
    CloseWindow();

    return 0;
}

void DrawTree(vector<vector<char>> tree, int X, int Y){
    int x = X, y = Y;
    for (const auto& row : tree) {
        x = X;
        for (const auto& col : row) {
            if (col == ' ') {
                x += 2;
            }
            else if (col == '1') {
                DrawRectangle(x, y, 2, 2, Color{11, 66, 14, 255});
                x += 2;
            }
            else if (col == '3') {
                DrawRectangle(x, y, 2, 2, Color{9, 107, 14, 255});
                x += 2;
            }
            else if (col == '5') {
                DrawRectangle(x, y, 2, 2, Color{113, 255, 110, 255});
                x += 2;
            }
            else if (col == '2') {
                DrawRectangle(x, y, 2, 2, Color{28, 124, 33, 255});
                x += 2;
            }
            else if (col == '4') {
                DrawRectangle(x, y, 2, 2, Color{58, 37, 0, 255});
                x += 2;
            }
        }
        y += 2;
    }
}

void DrawMap(vector<vector<char>> map, vector<vector<char>> tree, vector<vector<char>> tree_light, char memoryCell, int memX, int memY){
    int x = 0, y = 0;

    for (int i = 0; i < map.size(); i++) {

        const auto& row = map[i];
        x = 0;

        for (int j = 0; j < map[i].size(); j++) {

            const auto& col = map[i][j];
            if (col == '#') {
                DrawTree(tree, x, y);
                //DrawRectangle(x, y, TILE, BLUE);
                x += 32;
            }
            else if (col == ' ') {
                x += 32;
            }
            else if (col == '%'){
                DrawTree(tree_light, x, y);
                x += 32;
            }
            else if (col == '*'){
                DrawRectangle(x, y, TILE, GOLD);
                x += 32;
            }
            else if (col == 't') {
                DrawRectangle(x, y, TILE, BROWN);
                x += 32;
            }
            else if (col == 'p') {
                DrawRectangle(x, y, TILE, BROWN);
                x += 32;
                memoryCell = col;
                memX = i;
                memY = j;
            }
        }
        y += 32;
    }
}