#include <SDL2/SDL.h>
#include <Vector>

class Screen{
    SDL_Event e;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_FPoint> points;

    public:
        
        const int WIDTH = 640;
        const int HEIGHT = 480;
        const int SCALE = 1;
        Screen(){
            SDL_Init(SDL_INIT_VIDEO);
            SDL_CreateWindowAndRenderer(
                WIDTH*SCALE, HEIGHT*SCALE, 0, &window, &renderer
            );
            SDL_RenderSetScale(renderer, SCALE, SCALE);
        }

        void pixel(float x, float y){
            
            points.push_back(SDL_FPoint({x, y}));
        }

        void show(){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            for (auto& point: points){
                SDL_RenderDrawPointF(renderer, point.x, point.y);
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