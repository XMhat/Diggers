#ifdef __cplusplus
extern "C"
{
#endif
  typedef unsigned long pixel_t;

  extern pixel_t pixel_get(int x, int y, const unsigned char* pix, unsigned slice, unsigned pixel, unsigned dx, unsigned dy);
  extern void pixel_put(int x, int y, unsigned char* pix, unsigned slice, unsigned pixel, unsigned dx, unsigned dy, pixel_t v);
  extern void scale2x(unsigned char* dst_ptr, unsigned dst_slice, const unsigned char* src_ptr, unsigned src_slice, unsigned pixel, unsigned width, unsigned height);
  extern void scale3x(unsigned char* dst_ptr, unsigned dst_slice, const unsigned char* src_ptr, unsigned src_slice, unsigned pixel, unsigned width, unsigned height);
#ifdef __cplusplus
}
#endif
