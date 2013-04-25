#pragma once
#include <sdl/SDL.h>
#include "types.h"

// the target surface for all video work
extern SDL_Surface *target;

// the current sdl key state
extern uint8       *keys;