#include "assets.h"
#include "framework.h"
#include <bass.h>

// 
sSpriteAsset sprite[] =
{
	{ "assets/font.bmp", true , 0 },
	{ "assets/test.bmp", false, 0 }
};
sSoundAsset sound[] =
{
	{ "assets/null.wav"    },
	{ "assets/explode.wav" }
};

// 
const int cSpriteCount = sizeof( sprite ) / sizeof( sSpriteAsset );
const int cSoundCount  = sizeof( sound  ) / sizeof( sSoundAsset  );

// 
bool load_assets( void )
{
	// load sprites
	for ( int i=0; i<cSpriteCount; i++ )
	{
		// load the sprites we need
		sprite[i].data = SDL_LoadBMP( sprite[i].path );
		if ( sprite[i].data == 0 )
			return false;		
		// set the colour key
		if ( sprite[i].key )
			SDL_SetColorKey( sprite[i].data, SDL_SRCCOLORKEY, 0xFF00FF );
	}
	//
	SDL_AudioSpec audio =
	{
	};

	// load sounds
	for ( int i=0; i<cSoundCount; i++ )
	{
		sound[i].data =
			BASS_SampleLoad
			(
				FALSE,
				sound[i].path,
				0,
				0,
				1,
				0
			);
		if ( sound[i].data == 0 )
			return false;
		// since we have specified that the sound can only be played once
		// then can be garenteed that this will return an identical handle
		// to the sample handle. this function just needs to be called to
		// setup the sample for playback. this wont work if there can be
		// simaltaneous playbacks.
		BASS_SampleGetChannel( sound[i].data, FALSE );
	}
	//
	return true;
}

//
void free_assets( void )
{
	// free sprites
	for ( int i=0; i<cSpriteCount; i++ )
	{
		if ( sprite[i].data == 0 )
			continue;
		//
		SDL_FreeSurface( sprite[i].data );
		sprite[i].data = 0;
	}

	// free sounds
	for ( int i=0; i<cSoundCount; i++ )
	{
		if ( sound[i].data == 0 )
			continue;
		//
		BASS_SampleFree( sound[i].data );
		sound[i].data = 0;
	}
}

//
int getSpriteCount( void )
{
	return cSpriteCount;
}

//
int getSoundCount( void )
{
	return cSoundCount;
}