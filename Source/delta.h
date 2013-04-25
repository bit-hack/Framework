#pragma once
#include "types.h"

//
extern void  setDeltaRate( float tps );
extern float getDelta    ( void );
extern float getFPS      ( void );
extern void  initDelta   ( void );
extern void  tickDelta   ( void );