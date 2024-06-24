#ifndef __PLAT_CMN_FB_H__
#define __PLAT_CMN_FB_H__

#include <uk/arch/types.h>

void fb_init(__u32 *framebuffer, __u64 size, __u32 width, __u32 height,
	     __u32 pixels_per_scanline);

#endif /* __PLAT_CMN_FB_H__ */
