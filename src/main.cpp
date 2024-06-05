#include <SDL2/SDL.h>
#include"screen.h"

mesh cube;
Screen screen;
float fTheta;

void drawCube(float fElapsedTime){
    // Set up rotation matrices
    mat4x4 matRotZ, matRotX;
    fTheta += 1.0f * (fElapsedTime/1000);

    // Rotation Z
    matRotZ.m[0][0] = cosf(fTheta);
    matRotZ.m[0][1] = sinf(fTheta);
    matRotZ.m[1][0] = -sinf(fTheta);
    matRotZ.m[1][1] = cosf(fTheta);
    matRotZ.m[2][2] = 1;
    matRotZ.m[3][3] = 1;

    // Rotation X
    matRotX.m[0][0] = 1;
    matRotX.m[1][1] = cosf(fTheta * 0.5f);
    matRotX.m[1][2] = sinf(fTheta * 0.5f);
    matRotX.m[2][1] = -sinf(fTheta * 0.5f);
    matRotX.m[2][2] = cosf(fTheta * 0.5f);
    matRotX.m[3][3] = 1;

    for(auto face : cube.faces){
        vec3 triProjected[3], triRotatedZ[3], triRotatedX[3];

        MultiplyMatrixVector(cube.vertecis[face.a], triRotatedZ[0], matRotZ);
        MultiplyMatrixVector(cube.vertecis[face.b], triRotatedZ[1], matRotZ);
        MultiplyMatrixVector(cube.vertecis[face.c], triRotatedZ[2], matRotZ);

        MultiplyMatrixVector(triRotatedZ[0], triRotatedX[0], matRotX);
        MultiplyMatrixVector(triRotatedZ[1], triRotatedX[1], matRotX);
        MultiplyMatrixVector(triRotatedZ[2], triRotatedX[2], matRotX);

        triRotatedX[0].z += 3;
        triRotatedX[1].z += 3;
        triRotatedX[2].z += 3;

        MultiplyMatrixVector(triRotatedX[0], triProjected[0], screen.matProj);
        MultiplyMatrixVector(triRotatedX[1], triProjected[1], screen.matProj);
        MultiplyMatrixVector(triRotatedX[2], triProjected[2], screen.matProj);
        
        
        // Scale into view
        triProjected[0].x += 1.0f; triProjected[0].y += 1.0f;
        triProjected[1].x += 1.0f; triProjected[1].y += 1.0f;
        triProjected[2].x += 1.0f; triProjected[2].y += 1.0f;
        triProjected[0].x *= 0.5f * screen.WIDTH;
        triProjected[0].y *= 0.5f * screen.HEIGHT;
        triProjected[1].x *= 0.5f * screen.WIDTH;
        triProjected[1].y *= 0.5f * screen.HEIGHT;
        triProjected[2].x *= 0.5f * screen.WIDTH;
        triProjected[2].y *= 0.5f * screen.HEIGHT;

        screen.line(triProjected[0].x, triProjected[0].y,
                    triProjected[1].x, triProjected[1].y);
        screen.line(triProjected[1].x, triProjected[1].y,
                    triProjected[2].x, triProjected[2].y);
        screen.line(triProjected[2].x, triProjected[2].y,
                    triProjected[0].x, triProjected[0].y);
    }
}

int main(int argc, char *argv[]){

    cube.vertecis = {
        { 0.0f, 0.0f, 0.0f}, //0
        { 0.0f, 0.0f, 1.0f}, //1
        { 0.0f, 1.0f, 0.0f}, //2
        { 0.0f, 1.0f, 1.0f}, //3

        { 1.0f, 0.0f, 0.0f}, //4
        { 1.0f, 0.0f, 1.0f}, //5
        { 1.0f, 1.0f, 0.0f}, //6
        { 1.0f, 1.0f, 1.0f}, //7
    };

    cube.faces = {
        //front
        { 0, 2, 6 },
        { 0, 6, 4 },
        
        //back
        { 5, 7, 3 },
        { 5, 6, 1 },

        //bottom
        { 5, 1, 0 },
        { 5, 0, 4 },

        //top
        { 2, 3, 7 },
        { 2, 7, 6 },

        //right
        { 4, 6, 7 },
        { 4, 7, 5 },

        //left
        { 1, 3, 2 },
        { 1, 2, 0 },
    };

    float time1 = SDL_GetTicks64();

    while(true){
        float time2 = SDL_GetTicks64();
        drawCube(time2 - time1);
        time1 = time2;
        screen.show();
        screen.input();
        SDL_Delay(30);
        screen.clear();
    }

    return 0;
}