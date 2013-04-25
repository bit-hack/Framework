//#include <sdl/SDL_keysym.h>
#include <sdl/SDL.h>
#include "framework.h"
#include "assets.h"
#include "video.h"
#include "sound.h"
#include "delta.h"

//
#include <stdio.h>

//
uint32 g_noise = 0xDEADBABE;

// 
static
inline int myRand( void )
{
	const uint32 taps = 0x80306031;
    sint32 l = g_noise;
	if (l & 0x1)	l = (1 << 31) | ((l ^ taps) >> 1);
	else			l = (l >> 1);
	g_noise = l;
	return l;
}

// 
void test( void )
{
	uint32 *p = (uint32*)target->pixels;
	//
	for ( int i=0; i< ((target->pitch>>2) * target->h); i++ )
	{
		*p++ = myRand( );
	}
}

//
bool start_game( void )
{
	g_noise = SDL_GetTicks( );
	return true;
}


//
void keyhit_game( int sym )
{
	switch ( sym )
	{
	case ( SDLK_1 ):
		triggerSound( eSound::null );
		break;
	case ( SDLK_2 ):
		triggerSound( eSound::explode );
		break;
	}
	
}

//
float text_y = 240;
float spr_ix = 0.0f;

//
bool tick_game( void )
{
	// 
	if ( keys[ SDLK_SPACE ] )
		SDL_Delay( 100 );
	// 
	test( );
	drawText( 2, 2, 32, false, "Hello World!" );
	drawLine( 64 , 64, 64 , 198 );
	drawLine( 256, 64, 256, 198 );
	drawText( 64, (int)text_y, 256-64, true, "this...\n\nIs a realy freaking LONG sentence and it will surely cause a wrap around effect that is good for testing, things and such!" );
	plot( 64, 64 );
	// 
	printf( "\rfps : %f, dt : %f", getFPS( ), getDelta( ) );
	//
	text_y -= getDelta( );
	if ( text_y <-128 )
		text_y = 240;
	//
	spr_ix += getDelta( ) / 8.0f;
	drawSprite( eSprite::test, 128, 128, 24, 24, (int)spr_ix );
	//
	drawCircle( 128, 192, 16 );
	//
	return true;
}

//
void finish_game( void )
{
}