#include <uk/plat/common/fb.h>

static __u32 *fb;
static __u64 fb_size;
static __u32 fb_width;
static __u32 fb_height;
static __u32 fb_pixels_per_scanline;

void fb_init(__u32 *framebuffer, __u64 size, __u32 width, __u32 height,
	     __u32 pixels_per_scanline)
{
	fb = framebuffer;
	fb_size = size;
	fb_width = width;
	fb_height = height;
	fb_pixels_per_scanline = pixels_per_scanline;

	/* Draw a graphic before jumping to the kernel to demonstrate GOP */
	for (__u32 i = 0; i < fb_width; i++) {
		for (__u32 j = 0; j < fb_height; j++) {
			__u32 b = i * 256 / fb_width / 2 + j * 256 / fb_height / 2;
			fb[i + j * fb_pixels_per_scanline] = (b << 8) | (b << 0);
		}
	}
}
