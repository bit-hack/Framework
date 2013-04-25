#include <bass.h>
#include "sound.h"
#include "assets.h"

#include <stdio.h>

//
void triggerSound( int num )
{
	// is it a valid sound
	if ( num < 0                 ) return;
	if ( num >= getSoundCount( ) ) return;
	// play the audio already
	if ( BASS_ChannelPlay( sound[ num ].data, TRUE ) != TRUE )
	{
		int err = BASS_ErrorGetCode( );
		printf( "\nbass error %d\n", err );
	}
}
