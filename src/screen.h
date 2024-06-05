#include "util.h"
#include <SDL2/SDL.h>
#include <Vector>
#include <cmath>

class Screen{
    SDL_Event e;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_Vertex> verts;
    std::vector<line> lines;

    public:
        
        const int WIDTH = 320;
        const int HEIGHT = 240;
        const int SCALE = 2;

        const float fNear = 0.1f;
        const float fFar = 1000.0f;
        const float fFov = 90.0f;
        const float fAspectRatio = ((float) HEIGHT)/((float)WIDTH);
        const float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
        
        mat4x4 matProj;

        Screen(){
            SDL_Init(SDL_INIT_VIDEO);
            SDL_CreateWindowAndRenderer(
                WIDTH*SCALE, HEIGHT*SCALE, 0, &window, &renderer
            );
            SDL_RenderSetScale(renderer, SCALE, SCALE);

            matProj.m[0][0] = fAspectRatio * fFovRad;
            matProj.m[1][1] = fFovRad;
            matProj.m[2][2] = fFar / (fFar - fNear);
            matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
            matProj.m[2][3] = 1.0f;
            matProj.m[3][3] = 0.0f;

        }

        void clear(){
            verts.clear();
            lines.clear();
        }


        void vertex(float x, float y){
            verts.push_back(SDL_Vertex{
                SDL_FPoint{ x, y }, SDL_Color{ 255, 255, 255, 255 }, SDL_FPoint{ 0 }
            });
        }
        
        void line(float x1, float y1, float x2, float y2){
            lines.emplace_back(x1, y1, x2, y2);
        }


        void show(){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            for(auto& line : lines){
                SDL_RenderDrawLineF(renderer, line.x1, line.y1, line.x2, line.y2);
            }

            SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);

            SDL_RenderPresent(renderer);
        }

        void input(){
            while(SDL_PollEvent(&e)){
                if(e.type == SDL_QUIT){
                    SDL_Quit();
                    exit(0);
                }
            }
        }
};