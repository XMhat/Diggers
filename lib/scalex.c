#include "scalex.h"

void pixel_put(int x, int y, unsigned char* pix, unsigned slice, unsigned pixel, unsigned dx, unsigned dy, pixel_t v)
{
  register unsigned char* p;
  register unsigned i;

  p = pix + (y * slice) + (x * pixel);

  for(i=0;i<pixel;++i) {
    p[i] = v >> (i*8);
  }
}

pixel_t pixel_get(int x, int y, const unsigned char* pix, unsigned slice, unsigned pixel, unsigned dx, unsigned dy)
{
  const unsigned char* p;
  register unsigned i;
  pixel_t v;

  if (x < 0)
    x = 0;
  if (x >= dx)
    x = dx - 1;
  if (y < 0)
    y = 0;
  if (y >= dy)
    y = dy - 1;

  p = pix + (y * slice) + (x * pixel);

  v = 0;

  for(i=0;i<pixel;++i)
    v |= ((pixel_t)p[i]) << (i*8);

  return v;
}

void scale2x(unsigned char* dst_ptr, unsigned dst_slice, const unsigned char* src_ptr, unsigned src_slice, unsigned pixel, unsigned width, unsigned height)
{
  register int x,y;
  pixel_t E0, E1, E2, E3;
  pixel_t A, B, C, D, E, F, G, H, I;

  for(y=0;y<height;++y) {
    for(x=0;x<width;++x) {

      A = pixel_get(x-1, y-1, src_ptr, src_slice, pixel, width, height);
      B = pixel_get(x, y-1, src_ptr, src_slice, pixel, width, height);
      C = pixel_get(x+1, y-1, src_ptr, src_slice, pixel, width, height);
      D = pixel_get(x-1, y, src_ptr, src_slice, pixel, width, height);
      E = pixel_get(x, y, src_ptr, src_slice, pixel, width, height);
      F = pixel_get(x+1, y, src_ptr, src_slice, pixel, width, height);
      G = pixel_get(x-1, y+1, src_ptr, src_slice, pixel, width, height);
      H = pixel_get(x, y+1, src_ptr, src_slice, pixel, width, height);
      I = pixel_get(x+1, y+1, src_ptr, src_slice, pixel, width, height);

      // E0 = E1 = E2 = E3 = E;

      E0 = D == B && B != F && D != H ? D : E;
      E1 = B == F && B != D && F != H ? F : E;
      E2 = D == H && D != B && H != F ? D : E;
      E3 = H == F && D != H && B != F ? F : E;

      pixel_put(x*2, y*2, dst_ptr, dst_slice, pixel, width*2, height*2, E0);
      pixel_put(x*2+1, y*2, dst_ptr, dst_slice, pixel, width*2, height*2, E1);
      pixel_put(x*2, y*2+1, dst_ptr, dst_slice, pixel, width*2, height*2, E2);
      pixel_put(x*2+1, y*2+1, dst_ptr, dst_slice, pixel, width*2, height*2, E3);
    }
  }
}

void scale3x(unsigned char* dst_ptr, unsigned dst_slice, const unsigned char* src_ptr, unsigned src_slice, unsigned pixel, unsigned width, unsigned height)
{
  register int x,y;
  pixel_t E0, E1, E2, E3, E4, E5, E6, E7, E8;
  pixel_t A, B, C, D, E, F, G, H, I;
  register int k0, k1, k2, k3;
  register int c0, c1, c2, c3;

  for(y=0;y<height;++y) {
    for(x=0;x<width;++x) {
      A = pixel_get(x-1, y-1, src_ptr, src_slice, pixel, width, height);
      B = pixel_get(x, y-1, src_ptr, src_slice, pixel, width, height);
      C = pixel_get(x+1, y-1, src_ptr, src_slice, pixel, width, height);
      D = pixel_get(x-1, y, src_ptr, src_slice, pixel, width, height);
      E = pixel_get(x, y, src_ptr, src_slice, pixel, width, height);
      F = pixel_get(x+1, y, src_ptr, src_slice, pixel, width, height);
      G = pixel_get(x-1, y+1, src_ptr, src_slice, pixel, width, height);
      H = pixel_get(x, y+1, src_ptr, src_slice, pixel, width, height);
      I = pixel_get(x+1, y+1, src_ptr, src_slice, pixel, width, height);

      k0 = D == B && B != F && D != H;
      k1 = B == F && B != D && F != H;
      k2 = D == H && D != B && H != F;
      k3 = H == F && D != H && B != F;
      E0 = k0 ? D : E;
      E1 = (k0 && E != C) || (k1 && E != A) ? B : E;
      E2 = k1 ? F : E;
      E3 = (k0 && E != G) || (k2 && E != A) ? D : E;
      E4 = E;
      E5 = (k1 && E != I) || (k3 && E != C) ? F : E;
      E6 = k2 ? D : E;
      E7 = (k2 && E != I) || (k3 && E != G) ? H : E;
      E8 = k3 ? F : E;

      pixel_put(x*3, y*3, dst_ptr, dst_slice, pixel, width*3, height*3, E0);
      pixel_put(x*3+1, y*3, dst_ptr, dst_slice, pixel, width*3, height*3, E1);
      pixel_put(x*3+2, y*3, dst_ptr, dst_slice, pixel, width*3, height*3, E2);
      pixel_put(x*3, y*3+1, dst_ptr, dst_slice, pixel, width*3, height*3, E3);
      pixel_put(x*3+1, y*3+1, dst_ptr, dst_slice, pixel, width*3, height*3, E4);
      pixel_put(x*3+2, y*3+1, dst_ptr, dst_slice, pixel, width*3, height*3, E5);
      pixel_put(x*3, y*3+2, dst_ptr, dst_slice, pixel, width*3, height*3, E6);
      pixel_put(x*3+1, y*3+2, dst_ptr, dst_slice, pixel, width*3, height*3, E7);
      pixel_put(x*3+2, y*3+2, dst_ptr, dst_slice, pixel, width*3, height*3, E8);
    }
  }
}
