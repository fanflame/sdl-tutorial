//
// Created by fanqiang on 2019/10/10.
//

#ifndef SDLTEMP_LOG_H
#define SDLTEMP_LOG_H

#include <SDL2/SDL.h>

void printError(const char *error) {
    printf("%s:%s\n", error, SDL_GetError());
}

#endif //SDLTEMP_LOG_H
