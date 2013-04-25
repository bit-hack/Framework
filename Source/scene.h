#pragma once

//
struct sScene
{
	void (*start   )( void );
	bool (*tick    )( void );
	void (*end     )( void );
	void (*onKeyhit)( void );
};

//
extern sScene *getActiveScene( void      );
extern void    setScene      ( int scene );