#include <stdlib.h>
#include "SDL_rotozoom.h"
/* 
 
 32bit Zoomer with optional anti-aliasing by bilinear interpolation.

 Zoomes 32bit RGBA/ABGR 'src' surface to 'dst' surface.
 
*/

int
zoomSurfaceRGBA (SDL_Surface * src, SDL_Surface * dst, int smooth)
{
  int x, y, sx, sy, *sax, *say, *csax, *csay, csx, csy, ex, ey, t1, t2, sstep;
  tColorRGBA *c00, *c01, *c10, *c11;
  tColorRGBA *sp, *csp, *dp;
  int sgap, dgap, orderRGBA;

  /* Variable setup */
  if (smooth)
    {
      /* For interpolation: assume source dimension is one pixel */
      /* smaller to avoid overflow on right and bottom edge.     */
      sx = (int) (65536.0 * (float) (src->w - 1) / (float) dst->w);
      sy = (int) (65536.0 * (float) (src->h - 1) / (float) dst->h);
    }
  else
    {
      sx = (int) (65536.0 * (float) src->w / (float) dst->w);
      sy = (int) (65536.0 * (float) src->h / (float) dst->h);
    }

  /* Allocate memory for row increments */
  if ((sax = (int *) malloc ((dst->w + 1) * sizeof (Uint32))) == NULL)
    {
      return (-1);
    }
  if ((say = (int *) malloc ((dst->h + 1) * sizeof (Uint32))) == NULL)
    {
      free (sax);
      return (-1);
    }

  /* Precalculate row increments */
  csx = 0;
  csax = sax;
  for (x = 0; x <= dst->w; x++)
    {
      *csax = csx;
      csax++;
      csx &= 0xffff;
      csx += sx;
    }
  csy = 0;
  csay = say;
  for (y = 0; y <= dst->h; y++)
    {
      *csay = csy;
      csay++;
      csy &= 0xffff;
      csy += sy;
    }

  /* Pointer setup */
  sp = csp = (tColorRGBA *) src->pixels;
  dp = (tColorRGBA *) dst->pixels;
  sgap = src->pitch - src->w * 4;
  dgap = dst->pitch - dst->w * 4;
  orderRGBA = (src->format->Rmask == 0x000000ff);

  /* Switch between interpolating and non-interpolating code */
  if (smooth)
    {

      /* Interpolating Zoom */

      /* Scan destination */
      csay = say;
      for (y = 0; y < dst->h; y++)
	{
	  /* Setup color source pointers */
	  c00 = csp;
	  c01 = csp;
	  c01++;
	  c10 = (tColorRGBA *) ((Uint8 *) csp + src->pitch);
	  c11 = c10;
	  c11++;
	  csax = sax;
	  for (x = 0; x < dst->w; x++)
	    {
	      /* ABGR ordering */
	      /* Interpolate colors */
	      ex = (*csax & 0xffff);
	      ey = (*csay & 0xffff);
	      t1 = ((((c01->r - c00->r) * ex) >> 16) + c00->r) & 0xff;
	      t2 = ((((c11->r - c10->r) * ex) >> 16) + c10->r) & 0xff;
	      dp->r = (((t2 - t1) * ey) >> 16) + t1;
	      t1 = ((((c01->g - c00->g) * ex) >> 16) + c00->g) & 0xff;
	      t2 = ((((c11->g - c10->g) * ex) >> 16) + c10->g) & 0xff;
	      dp->g = (((t2 - t1) * ey) >> 16) + t1;
	      t1 = ((((c01->b - c00->b) * ex) >> 16) + c00->b) & 0xff;
	      t2 = ((((c11->b - c10->b) * ex) >> 16) + c10->b) & 0xff;
	      dp->b = (((t2 - t1) * ey) >> 16) + t1;
	      t1 = ((((c01->a - c00->a) * ex) >> 16) + c00->a) & 0xff;
	      t2 = ((((c11->a - c10->a) * ex) >> 16) + c10->a) & 0xff;
	      dp->a = (((t2 - t1) * ey) >> 16) + t1;
	      /* Advance source pointers */
	      csax++;
	      sstep = (*csax >> 16);
	      c00 += sstep;
	      c01 += sstep;
	      c10 += sstep;
	      c11 += sstep;
	      /* Advance destination pointer */
	      dp++;
	    }
	  /* Advance source pointer */
	  csay++;
	  csp = (tColorRGBA *) ((Uint8 *) csp + (*csay >> 16) * src->pitch);
	  /* Advance destination pointers */
	  dp = (tColorRGBA *) ((Uint8 *) dp + dgap);
	}

    }
  else
    {

      /* Non-Interpolating Zoom */

      csay = say;
      for (y = 0; y < dst->h; y++)
	{
	  sp = csp;
	  csax = sax;
	  for (x = 0; x < dst->w; x++)
	    {
	      /* Draw */
	      *dp = *sp;
	      /* Advance source pointers */
	      csax++;
	      sp += (*csax >> 16);
	      /* Advance destination pointer */
	      dp++;
	    }
	  /* Advance source pointer */
	  csay++;
	  csp = (tColorRGBA *) ((Uint8 *) csp + (*csay >> 16) * src->pitch);
	  /* Advance destination pointers */
	  dp = (tColorRGBA *) ((Uint8 *) dp + dgap);
	}

    }

  /* Remove temp arrays */
  free (sax);
  free (say);

  return (0);
}


/* 
 
 zoomSurface()

 Zoomes a 32bit or 8bit 'src' surface to newly created 'dst' surface.
 'zoomx' and 'zoomy' are scaling factors for width and height. If 'smooth' is 1
 then the destination 32bit surface is anti-aliased. If the surface is not 8bit
 or 32bit RGBA/ABGR it will be converted into a 32bit RGBA format on the fly.

*/

#define VALUE_LIMIT	0.001

SDL_Surface *lcd::zoomSurface (SDL_Surface * src, double zoomx, double zoomy, int smooth)
{
  SDL_Surface *rz_src;
  SDL_Surface *rz_dst;
  int dstwidth, dstheight;
  int is32bit;
  int i, src_converted;

  /* Sanity check */
  if (src == NULL)
    return (NULL);

  /* Determine if source surface is 32bit or 8bit */
  is32bit = (src->format->BitsPerPixel == 32);
  if ((is32bit) || (src->format->BitsPerPixel == 8))
    {
      /* Use source surface 'as is' */
      rz_src = src;
      src_converted = 0;
    }

  /* Sanity check zoom factors */
  if (zoomx < VALUE_LIMIT)
    {
      zoomx = VALUE_LIMIT;
    }
  if (zoomy < VALUE_LIMIT)
    {
      zoomy = VALUE_LIMIT;
    }

  /* Calculate target size and set rect */
  dstwidth = (int) ((double) rz_src->w * zoomx);
  dstheight = (int) ((double) rz_src->h * zoomy);
  if (dstwidth < 1)
    {
      dstwidth = 1;
    }
  if (dstheight < 1)
    {
      dstheight = 1;
    }

  /* Alloc space to completely contain the zoomed surface */
  rz_dst = NULL;
  if (is32bit)
    {
      /* Target surface is 32bit with source RGBA/ABGR ordering */
      rz_dst =
	SDL_CreateRGBSurface (SDL_SWSURFACE, dstwidth, dstheight, 32,
			      rz_src->format->Rmask, rz_src->format->Gmask,
			      rz_src->format->Bmask, rz_src->format->Amask);
    }

  /* Lock source surface */
  SDL_LockSurface (rz_src);
  /* Check which kind of surface we have */
  if (is32bit)
    {
      /* Call the 32bit transformation routine to do the zooming (using alpha) */
      zoomSurfaceRGBA (rz_src, rz_dst, smooth);
      /* Turn on source-alpha support */
      SDL_SetAlpha (rz_dst, SDL_SRCALPHA, 255);
    }
  /* Unlock source surface */
  SDL_UnlockSurface (rz_src);

  /* Cleanup temp surface */
  if (src_converted)
    {
      SDL_FreeSurface (rz_src);
    }

  /* Return destination surface */
  return (rz_dst);
}
