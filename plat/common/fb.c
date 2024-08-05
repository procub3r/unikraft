#include <string.h>
#include <sys/types.h>
#include <uk/plat/common/fb.h>

/* Framebuffer metadata */
static __u32 *fb;
static __u64 fb_size;
static __u32 fb_real_width;
static __u32 fb_height;
static __u32 fb_width;

/* Terminal metadata */
static __u32 row_count;
static __u32 col_count;
static __u32 cursor_row;
static __u32 cursor_col;

/* Font metadata */
static __u8 char_width = 8;
static __u8 char_height = 16;
extern __u8 fb_font8x16[256][16]; /* Default font */

#define WHITE (color(0xff, 0xff, 0xff))
#define BLACK (color(0x00, 0x00, 0x00))

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
			fb[index] = BLACK;
		}
	}
}

/* Draw character at given row and col */
static void fb_putc_at(char c, __u32 fg_color, __u32 bg_color, __u32 col,
		       __u32 row)
{
	for (size_t y = 0; y < char_height; y++) {
		const __u8 slice = fb_font8x16[(__u8)c][y];
		for (size_t x = 0; x < char_width; x++) {
			const size_t index = ((x + col * char_width)
				+ (y + row * char_height) * fb_real_width);
			if ((slice >> (char_width - x - 1)) & 1) {
				fb[index] = fg_color;
			} else {
				fb[index] = bg_color;
			}
		}
	}
}

/* Enter a new line, scroll if necessary */
static void fb_newline() {
	if (cursor_row == row_count - 1) {
		/* scroll content up one line */
		memmove(fb, &fb[char_height * fb_real_width],
			fb_size - char_height * fb_real_width);
		/* clean last row */
		for (__u32 col = 0; col < col_count - 2; col++)
			fb_putc_at(' ', WHITE, BLACK, col, row_count - 1);
	} else {
		cursor_row++;
	}
}

/* Draw a single character at the cursor position */
static void fb_putc(char c)
{
	switch (c) {
	case '\n':
		fb_newline();
		break;
	case '\r':
		cursor_col = 0;
		break;
	default:
		fb_putc_at(c, WHITE, BLACK, cursor_col, cursor_row);
		if (++cursor_col == col_count) {
			cursor_col = 0;
			fb_newline();
		}
	}
}

/* Print a null terminated string */
static void fb_print(const char *string)
{
	char c;
	while ((c = *string++))
		fb_putc(c);
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

	/* Initialize terminal metadata */
	row_count = fb_height / char_height;
	col_count = fb_width / char_width;
	cursor_row = 0;
	cursor_col = 0;

	clear_fb();

	fb_print("o.   .o       _ _               __ _\r\n");
	fb_print("Oo   Oo  ___ (_) | __ __  __ _ ' _) :_\r\n");
	fb_print("oO   oO ' _ `| | |/ /  _)' _` | |_|  _)\r\n");
	fb_print("oOo oOO| | | | |   (| | | (_) |  _) :_\r\n");
	fb_print(" OoOoO ._, ._:_:_,\\_._,  .__,_:_, \\___)\r\n");
	fb_print("\r\n");
	fb_print("    -\r\n");
	fb_print("  c'o'o  .--.\r\n");
	fb_print("  (| |)_/\r\n");
}
