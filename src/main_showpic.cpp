#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_video.h"
#include "Log.h"

void quit(SDL_Window *window){
    SDL_DestroyWindow(window);
    SDL_Quit();
}
/*
 * 使用过SDL展示bmp图片
 * */
int main() {
    int result = SDL_Init(SDL_INIT_VIDEO);
    if (result) {
        printError("SDL_Init");
        return 0;
    }
    SDL_Window* window = SDL_CreateWindow("title",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,800,600,SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
    if (!window) {
        printError("SDL_CreateWindow");
        return 0;
    }

    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    SDL_Surface* bitmapSurface = SDL_LoadBMP("/Users/momo/Documents/CLionProjects/sdltemp/test.bmp");
    if (!bitmapSurface) {
        printError("SDL_LoadBMP");
        quit(window);
        return 0;
    }
    SDL_Rect rectBmp;
    rectBmp.w = 100;
    rectBmp.h = 100;
    rectBmp.y = 100;
    rectBmp.x = 100;
    SDL_UpperBlit(bitmapSurface, &rectBmp,windowSurface, nullptr);
    SDL_UpdateWindowSurface(window);
    SDL_ShowWindow(window);
    SDL_Event e;

    bool isQuit = false;
    while (!isQuit) {
        SDL_PollEvent(&e);
        switch (e.type) {
            case SDL_QUIT:
                isQuit = true;
                break;
            case SDL_KEYDOWN:
                printf("on key down:%d\n",e.key.keysym.sym);
                break;
        }
    }
    quit(window);
    return 0;
}