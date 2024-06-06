#include "util.h"
#include <SDL2/SDL.h>
#include <Vector>
#include <algorithm>
#include <cmath>

class Screen{
    SDL_Event e;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_Vertex> verts;
    std::vector<line> lines;
    std::vector<triangle> triangles;

    public:
        
        const int WIDTH = 320;
        const int HEIGHT = 240;
        const int SCALE = 2;

        vec3 camera;

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
            triangles.clear();
        }

        void vertex(float x, float y, float light){

            Uint8 r, g, b;

            r = static_cast<Uint8>(std::round((100 * light)));
            g = static_cast<Uint8>(std::round((100 * light)));
            b = static_cast<Uint8>(std::round((255 * light)));

            verts.push_back(SDL_Vertex{
                SDL_FPoint{ x, y }, SDL_Color{ r, g, b, 255 }, SDL_FPoint{ 0 }
            });
        }
        
        void line(float x1, float y1, float x2, float y2){
            lines.emplace_back(x1, y1, x2, y2);
        }

        void addTriangle(vec3 points[], float light = 1){
            triangles.emplace_back(points, light);
        }

        void show(bool showWireframe){

            //sort the triangles first
            std::sort(triangles.begin(), triangles.end(), [](triangle &t1, triangle &t2)
            {
                float z1 = (t1.points[0].z + t1.points[1].z + t1.points[2].z) / 3.0f;
                float z2 = (t2.points[0].z + t2.points[1].z + t2.points[2].z) / 3.0f;
                return z1 > z2;
            });

            //populate the lines and verts
            for (auto &tri : triangles){
                vertex(tri.points[0].x, tri.points[0].y, tri.light);
                vertex(tri.points[1].x, tri.points[1].y, tri.light);
                vertex(tri.points[2].x, tri.points[2].y, tri.light);


                if(showWireframe){
                    line(tri.points[0].x, tri.points[0].y,
                         tri.points[1].x, tri.points[1].y);
                    line(tri.points[1].x, tri.points[1].y,
                         tri.points[2].x, tri.points[2].y);
                    line(tri.points[2].x, tri.points[2].y,
                         tri.points[0].x, tri.points[0].y);
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);


            SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            for(auto& line : lines){
                SDL_RenderDrawLineF(renderer, line.x1, line.y1, line.x2, line.y2);
            }

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