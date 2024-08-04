#include <sys/types.h>
#include <uk/plat/common/fb.h>

/* Framebuffer metadata */
static __u32 *fb;
static __u64 fb_size;
static __u32 fb_real_width;
static __u32 fb_height;
static __u32 fb_width;

static inline __u32 color(__u8 r, __u8 g, __u8 b)
{
	/* TODO: use pixel format */
	return (r << 16) | (g << 8) | (b << 0);
}

/* Clear framebuffer */
static void clear_fb(void)
{
	for (size_t y = 0; y < fb_height; y++) {
		for (size_t x = 0; x < fb_width; x++) {
			const size_t index = x + y * fb_real_width;
			/* Clear with white color for demonstration */
			fb[index] = color(0xff, 0xff, 0xff);
		}
	}
}

void fb_init(__u32 *framebuffer, __u64 size, __u32 width, __u32 real_width,
             __u32 height)
{
	/* Initialize framebuffer metadata */
	fb = framebuffer;
	fb_size = size;
	fb_width = width;
	fb_real_width = real_width;
	fb_height = height;

	clear_fb();
}
