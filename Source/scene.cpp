//
#include "scene.h"

//
static sScene scene[] =
{
	{ 0, 0, 0, 0 }				// dummy scene
};

//
static int activeScene = 0;

//
sScene *getActiveScene( void )
{
	return &(scene[ activeScene ]);
}

void setScene( int in )
{
	activeScene = in;
}