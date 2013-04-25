//
#include "delta.h"
#include <sdl/SDL.h>

//
float  gDeltaRate = 25.0f;
float  gLastDelta = 1.0f;
uint32 gLastTicks = 0;
float  gFPS       = 0.0f;
float  gMADelta   = 1.0f;

//
static float limitf( float min, float in, float max )
{
	if ( in < min ) return min;
	if ( in > max ) return max;
	return in;
}

// tps = ticks per second
void setDeltaRate( float tps )
{
	// condition
	if ( tps < 1.0f ) tps = 1.0f;
	//
	gDeltaRate = tps;
}

//
float getDelta( void )
{
	return gMADelta;
}

//
float getFPS( void )
{
	return gFPS;
}

//
void initDelta( void )
{
	gLastTicks = SDL_GetTicks( );
}

//
void tickDelta( void )
{
	uint32 ticks = SDL_GetTicks( );
	uint32 dt    = ticks - gLastTicks;
	if ( dt == 0 )
	{
	}
	else
	{
		// calculate the instantaneous FPS
		gFPS = 1000.0f / (float)dt;
		// work out the delta scaler
		// limit to [+/-]10% of the last delta time to minimise explosions
		float newDelta = limitf
		(
			gLastDelta * 0.9f,
			gDeltaRate / gFPS,
			gLastDelta * 1.1f
		);
		// save this delta time
		gLastDelta = newDelta;
		// filter the delta time to reduce fluctuations in the delta timer
		// this is dont with an exponential time constant filter
		gMADelta = (gMADelta + (newDelta-gMADelta) * 0.1f);
	}
	//
	gLastTicks = ticks;
}