//
// Created by fanqiang on 2019/10/10.
//
#include "Log.h"
#include "SDL2/SDL_thread.h"

#define PIXELS_W 176
#define PIXELS_H 144
#define WIN_WIDTH  (PIXELS_W * 2)
#define WIN_HEIGHT (PIXELS_H * 2)
#define BPP 12
#define EVENT_REFRESH (SDL_USEREVENT+1);
#define BUFFER_SIZE (PIXELS_W * PIXELS_H * BPP/8)

unsigned char buffer[BUFFER_SIZE];
SDL_Rect targetRect;
bool refresh = true;

int refreshVideo(void *) {
    while (refresh) {
        SDL_Event event;
        event.type = EVENT_REFRESH;
        SDL_PushEvent(&event);
        SDL_Delay(30);
    }
}

/*使用SDL播放Yuv视频*/
int main() {
    int result = SDL_Init(SDL_INIT_VIDEO);
    if (result) {
        printError("SDL_Init");
        return 0;
    }
    SDL_Window *window = SDL_CreateWindow("video", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH,
                                          WIN_HEIGHT, SDL_WINDOW_OPENGL);
    if (!window) {
        SDL_DestroyWindow(window);
        printError("SDL_CreateWindow");
        return 0;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        printError("SDL_CreateRenderer");
        return 0;
    }

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, PIXELS_W,
                                             PIXELS_H);
    if (!texture) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(texture);
        printError("SDL_CreateTexture");
        return 0;
    }

    FILE *file = fopen("/Users/momo/Documents/CLionProjects/sdltemp/akiyo_qcif.yuv", "rb");
    if (!file) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(texture);
        printf("fopen error");
        return 0;
    }

    SDL_CreateThread(refreshVideo, nullptr, nullptr);

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                refresh = false;
                break;
            case (SDL_USEREVENT + 1):
                size_t result = fread(buffer, 1, BUFFER_SIZE, file);
                printf("read size:%d\n", result);
                if (result == 0) {
                    fseek(file, 0, SEEK_SET);
                    fread(buffer, 1, BUFFER_SIZE, file);
                }
                SDL_UpdateTexture(texture, nullptr, buffer, PIXELS_W);
                SDL_RenderClear(renderer);
                targetRect.x = 0;
                targetRect.y = 0;
                targetRect.w = WIN_WIDTH;
                targetRect.h = WIN_HEIGHT;
                SDL_RenderCopy(renderer, texture, nullptr, &targetRect);
                SDL_RenderPresent(renderer);
                break;
        }
    }
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}