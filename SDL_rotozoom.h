#ifndef _SDL_rotozoom_h
#define _SDL_rotozoom_h
#include <SDL/SDL.h>


/* ---- Defines */

#define SMOOTHING_OFF		0
#define SMOOTHING_ON		1

/* ---- Structures */

  typedef struct tColorRGBA
  {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
  }
  tColorRGBA;

  typedef struct tColorY
  {
    Uint8 y;
  }
  tColorY;


/* ---- Prototypes */

/* 
 
 zoomSurface()

 Zoomes a 32bit or 8bit 'src' surface to newly created 'dst' surface.
 'zoomx' and 'zoomy' are scaling factors for width and height. If 'smooth' is 1
 then the destination 32bit surface is anti-aliased. If the surface is not 8bit
 or 32bit RGBA/ABGR it will be converted into a 32bit RGBA format on the fly.

*/

  SDL_Surface *zoomSurface (SDL_Surface * src, double zoomx, double zoomy,
			    int smooth);


#endif /* _SDL_rotozoom_h */
