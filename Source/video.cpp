#include "video.h"
#include "assets.h"
#include "framework.h"

//
#define _WIDTH	320
#define _HEIGHT	240

#define _XMIN	0
#define _XMAX	1
#define _YMIN	2
#define _YMAX	3

#define _FALSE	0
#define _TRUE	1

//
static sint32 g_window[ 4 ] =
{
	0, _WIDTH-1, 0, _HEIGHT-1
};

//
static int colour = 0;

//
void setColour( int c )
{
	colour = c;
}

//
static inline void plot_( int x, int y )
{
	uint32 *p  = (uint32*)target->pixels;
	        p += x + y * (target->pitch >> 2);
		   *p  = colour;
}

//
void plot( int x, int y )
{
	if ( x <  0       ) return;
	if ( y <  0       ) return;
	if ( x >= _WIDTH  ) return;
	if ( y >= _HEIGHT ) return;
	plot_( x, y );
}

//
static inline uint32 _clipt( const sint64 d, const sint64 q, sint64* tL, sint64* tU )
{
	if ( 0 < d )
	{
		const sint64 t  =  (q << 16) / d;
		if (*tU < t) return _FALSE;
		if (*tL < t) *tL = t;
	}
	else
		if (d < 0)
		{
			const sint64 t = (q << 16) / d;
			if (t < *tL) return _FALSE;
			if (t < *tU) *tU = t;
		}
		else
			if (0 < q) return _FALSE;
	return _TRUE;
}

//
static inline sint32 lb_clip( sint32* x0, sint32* y0, sint32* x1, sint32* y1 )
{
	sint64 dx = *x1 - *x0;
	sint64 dy = *y1 - *y0;
	sint64 tL  = 0x00000;
	sint64 tU  = 0x10000;
	if (_clipt( dx, g_window[_XMIN] - *x0, &tL, &tU) &&
		_clipt(-dx, *x0 - g_window[_XMAX], &tL, &tU) &&
		_clipt( dy, g_window[_YMIN] - *y0, &tL, &tU) &&
		_clipt(-dy, *y0 - g_window[_YMAX], &tL, &tU))
	{
		if ( tU < 0x10000 )
		{
			*x1 = (sint32)(*x0 + ((tU * dx + 0x8000) >> 16));
			*y1 = (sint32)(*y0 + ((tU * dy + 0x8000) >> 16));
		}
		if ( 0 < tL )
		{
			*x0 = (sint32)(*x0 + ((tL * dx + 0x8000) >> 16));
			*y0 = (sint32)(*y0 + ((tL * dy + 0x8000) >> 16));
		}
		return _TRUE;
	}
	return _FALSE;
}

//
void drawLine( int _X0, int _Y0, int _X1, int _Y1 )
{
	/* clipping */
	if ( lb_clip( &_X0, &_Y0, &_X1, &_Y1 ) == _FALSE )
		return;
	/*  */
	sint32 decInc, j=0, incrementVal, endVal;
	sint32 shortLen = _Y1-_Y0, longLen = _X1-_X0;
	bool yLonger = false;
	/* find the longest length */
	if ( abs( shortLen ) > abs( longLen ) )
	{
		int swap = shortLen;
		shortLen = longLen;
		longLen  = swap;
		yLonger  = true;
	}
	endVal=longLen;
	/* find the heading of the line */
	if ( longLen < 0 )
	{
		incrementVal = -1;
		longLen      = -longLen;
	}
	else
		incrementVal = 1;
	if ( longLen == 0 )
		decInc = 0;
	else
		decInc = (shortLen << 16) / longLen;
	/* plot the line */
	if ( yLonger )
		for ( sint32 i=0; i!=endVal; i+=incrementVal )
		{ 
			plot_( _X0 + (j >> 16), _Y0 + i );
 			j   += decInc;
		}
	else
		for ( sint32 i=0; i!=endVal; i+=incrementVal )
		{
			plot_( _X0 + i, _Y0 + (j >> 16) );
			j   += decInc;
		}
}

//
void drawRect( int x, int y, int w, int h )
{
	SDL_Rect rect = { x, y, w, h };
	SDL_FillRect( target, &rect, colour );
}

//
void drawTextRow( int x, int y, uint8 *c, uint8* e=NULL )
{
	// font is 16x16 items and 8x8 for each item making 128x128 pixels all over
	sSpriteAsset *font =getSprite( eSprite::font );
	if ( font->data == 0 )
		return;
	//
	while ( (*c != '\0') && (c != e) ) 
	{
		// index the source font surface
		SDL_Rect src =
		{
			((*c) % 16 ) * 8,
			((*c) / 16 ) * 8,
			8, 8
		};
		// find the location of its output
		SDL_Rect dst =
		{
			x, y, 8, 8
		};
		//
		SDL_BlitSurface( font->data, &src, target, &dst );
		x += 8;
		c++;
	}
}

void drawText_c( int x, int y, int w, char *text )
{
	uint8 *t = (uint8*)text;
	uint8 *c = (uint8*)text;
	//
	while ( *c != '\0' )
	{
		// are we asked for a new line
		if ( *c == '\n' )
		{
			if ( c != t )
			{
				int s = (int)(c-t)*8;
				int tx = (x+w/2)-(s/2);
				drawTextRow( tx, y, t, c );
			}
			y += 8;
			t = c+1;
		}
		// is this a space character
		else if ( *c == ' ' )
		{
			// size in chars to this point
			int s = (int)(c-t)*8;
			//
			if ( (s > w) )
			{
				int tx = (x+w/2)-(s/2);
				drawTextRow( tx, y, t, c );
				y += 8;
				t  = c+1;
			}
		}
		// move on to the next character
		c++;
	}
	//
	if ( t != c )
	{
		int s = (int)(c-t)*8;
		int tx = (x+w/2)-(s/2);
		drawTextRow( tx, y, t, c );
	}
}

//
void drawText_uc( int x, int y, int w, char *text )
{
	// font is 16x16 items and 8x8 for each item making 128x128 pixels all over
	sSpriteAsset *font =getSprite( eSprite::font );
	if ( font->data == 0 )
		return;
	//
	unsigned char *utext = (unsigned char*)text;
	int tx = x;
	// 
	while ( *utext != '\0' )
	{
		// index the source font surface
		SDL_Rect src =
		{
			((*utext) % 16 ) * 8,
			((*utext) / 16 ) * 8,
			8, 8
		};
		// find the location of its output
		SDL_Rect dst =
		{
			tx, y, 8, 8
		};
		//
		SDL_BlitSurface( font->data, &src, target, &dst );
		// skip over to next character
		utext++;
		tx += 8;
		// do we want to wrap
		if ( (w > 0) && (tx >= (x+w)) )
		{
			tx  = x;
			y  += 8;
		}
	}
}

// [x,y]	// start location
//  w		// length before wrap
//  c		// true, center text on line
// text		// raw text data
void drawText( int x, int y, int w, bool c, char *text )
{
	if ( c ) drawText_c ( x, y, w, text );
	else     drawText_uc( x, y, w, text );
}

// draw a single sprite frame a sprite frame
// image index
// image [x,y] draw location
// sprite size
// sprite frame
void drawSprite( int spr, int x, int y, int w, int h, int f )
{
	SDL_Surface *src = sprite[ spr ].data;
	// sprites per width of sprite sheet
	if ( w==0 ) return;
	int spw = (src->w / w);
	if ( spw == 0 ) return;
	// 
	SDL_Rect srcRect =
	{
		( f % spw ) * w,
		( f / spw ) * h,
		w, h
	};
	//
	SDL_Rect dstRect = 
	{
		x, y, w, h
	};
	//
	SDL_BlitSurface( src, &srcRect, target, &dstRect );
}

// breshman circle drawing
void drawCircle( int px, int py, uint32 radius )
{
	//
	int f	  = 1 - radius;
	int ddF_x = 1;
	int ddF_y =-2 * radius;
	int x	  = 0;
	int y	  = radius;
	// plot the initial points
	plot( px, py + radius ); plot( px, py - radius );
	plot( px + radius, py ); plot( px - radius, py );
	// loop over arc
	while( x < y )
	{
		if( f >= 0 ) 
		{
			y     -= 1;
			ddF_y += 2;
			f     += ddF_y;
		}
		x     += 1;
		ddF_x += 2;
		f     += ddF_x;
		// plot in all quadrants
		plot( px + x, py + y ); plot( px - x, py + y );
		plot( px + x, py - y ); plot( px - x, py - y );
		plot( px + y, py + x ); plot( px - y, py + x );
		plot( px + y, py - x ); plot( px - y, py - x );
	}
}