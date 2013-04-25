#pragma once

//
#include <sdl/SDL.h>
#include <bass.h>
#include "types.h"

//
namespace eSprite
{
	enum _eSprite
	{
		font = 0,
		test ,
	};
};

//
namespace eSound
{
	enum _eSound
	{
		null    = 0,
		explode ,
	};
};

//
struct sSpriteAsset
{
	char        *path;
	bool		 key ;
	SDL_Surface *data;
};

//
struct sSoundAsset
{
	char    *path;
	HSAMPLE  data;
};

// 
extern sSpriteAsset sprite[];
extern sSoundAsset  sound [];

// 
extern bool load_assets( void );
extern void free_assets( void );

// 
extern int getSpriteCount( void );
extern int getSoundCount ( void );

//
inline sSpriteAsset *getSprite( int num )
{
	return &sprite[ num ];
}

// 
inline sSoundAsset *getSound( int num )
{
	return &sound[ num ];
}