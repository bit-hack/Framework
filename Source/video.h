#pragma once
#include "types.h"

//
void setColour( int c );

// [x1,y1]	// point 1
// [x2,y2]	// point 2
void drawLine( int x1, int y1, int x2, int y2 );

// [x,y]	// start location
//  w		// length before wrap
//  c		// true, center text on line
// text		// raw text data
void drawText( int x, int y, int w, bool c, char *text );

// [x,y]	// plot a single pixel
void plot( int x, int y );

// draw a single sprite frame a sprite frame
void drawSprite( int spr, int x, int y, int w, int h, int f );

// 
void drawCircle( int px, int py, uint32 radius );

//
void drawRect( int x, int y, int w, int h );