//
#include <sdl/SDL_syswm.h>
#include <bass.h>
#include "framework.h"
#include "assets.h"
#include "delta.h"
#include "scene.h"
#include "game.h"

//
const int c_ScreenWidth  = 320;
const int c_ScreenHeight = 240;

// duffs device (stollen from SDL a little bit ^^)
#define DUFFS_LOOP8( pixel_copy_increment, width )	\
{													\
	int n = ((width) + 7 ) >> 3;					\
	switch  ((width) & 7) {							\
	case 0: do {	pixel_copy_increment;			\
	case 7:		    pixel_copy_increment;			\
	case 6:		    pixel_copy_increment;			\
	case 5:		    pixel_copy_increment;			\
	case 4:		    pixel_copy_increment;			\
	case 3:		    pixel_copy_increment;			\
	case 2:		    pixel_copy_increment;			\
	case 1:		    pixel_copy_increment;			\
		} while ( --n > 0 );						\
	}												\
}

// 
static SDL_Surface *screen = NULL;
       SDL_Surface *target = NULL;
	   Uint8       *keys   = NULL;

// 
void render_target( void )
{
	//
	uint32 *s = (uint32*)screen->pixels;
	uint32 *t = (uint32*)target->pixels;
	//
	for ( int y = 0; y < target->h; y++ )
	{
		// 
		uint32 *sy1 = s;
		uint32 *sy2 = s + (screen->pitch >> 2);
		uint32 *t1  = t;
		// 
		DUFFS_LOOP8
		(
			{
				*sy1++ = (*t1);// & 0xF0F0F0;
				*sy1++ = (*t1);// & 0xF0F0F0;
				*sy2++ = (*t1);
				*sy2++ = (*t1);
				 t1++;
			},
			target->w
		)
		s += (screen->pitch >> 1);
		t += (target->pitch >> 2);
	}
}

// 
bool start_sdl( void )
{
	// why should you see the console?
#ifdef NDEBUG
	FreeConsole( );
#endif
	// 
	if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
		return false;
	// 
	screen = SDL_SetVideoMode
	(
		c_ScreenWidth*2,
		c_ScreenHeight*2, 32, 0
	);
	if ( screen == NULL )
		return false;
	// 
	target = SDL_CreateRGBSurface
	(
		SDL_SWSURFACE,
		c_ScreenWidth,
		c_ScreenHeight,
		32, 0, 0, 0, 0
	);
	if ( target == NULL )
		return false;
	//
	return true;
}

//
bool start_bass( void )
{
	// get hwnd
	SDL_SysWMinfo wmi;
	SDL_VERSION( &wmi.version );
	if(! SDL_GetWMInfo(&wmi) )
		return false;
	// init bass
	if ( BASS_Init( -1, 22050, BASS_DEVICE_MONO, (HWND)wmi.window, NULL ) == FALSE )
		return false;
	// start bass going already
	return (BASS_Start( ) == TRUE);
}

// 
bool tick_sdl( void )
{
	SDL_Event event;
	while ( SDL_PollEvent( &event ) )
	{
		switch ( event.type )
		{
		case ( SDL_QUIT ):
			return false;
		case ( SDL_KEYDOWN ):
			keyhit_game( event.key.keysym.sym );
			break;
		}
	}
	return true;
}

// 
int main( void )
{
	// 
	if (! start_sdl  ( ) )	return -1;
	if (! start_bass ( ) )	return -2;
	if (! load_assets( ) )	return -3;
	// get the current key state
	keys = SDL_GetKeyState( NULL );
	//
	if (! start_game ( ) )	return -4;
	// initalise the delta timer
	initDelta( );
	// 
	while ( tick_sdl( ) )
	{
		// advance the delta timer
		tickDelta( );
		//
		if (! tick_game( ) )
			break;
		//
		render_target( );
		SDL_Flip( screen );
	}
	//
	finish_game( );
	//
	BASS_Stop( );
	BASS_Free( );
	//
	return 0;
}