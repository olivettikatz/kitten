#ifndef __KRD_KERNEL_BACKEND_H
#define __KRD_KERNEL_BACKEND_H

#include <stdio.h>
#include <unistd.h>

typedef struct
{
	unsigned int x;
	unsigned int y;
} krd_coord;

typedef struct
{
	krd_coord size;
	void *data;
} krd_bitmap;

krd_coord krd_at(int x, int y);

typedef int (*krd_callback)(krd_coord, unsigned int, unsigned int);

typedef enum
{
#ifndef KRD_DISABLE_X11
	x11,
#endif
#ifndef KRD_DISABLE_OPENGL
	opengl
#endif
} krd_backend;

typedef enum
{
	lazy,
	fps
} krd_render_mode;

#ifndef KRD_DISABLE_X11
int krd_init_x11();
int krd_halt_x11();
int krd_main_x11_lazy();
int krd_main_x11_fps();
int krd_title_x11(char *t);
int krd_request_render_x11();
int krd_color_x11(char *color);
int krd_render_pixel_x11(krd_coord loc);
int krd_render_line_x11(krd_coord start, krd_coord end, int thickness, int rounded);
int krd_render_rect_x11(krd_coord topleft, krd_coord bottomright);
int krd_render_arc_x11(krd_coord center, int innerradius, int outerradius, double startangle, double endangle, int rounded);
char **krd_fontlist_x11();
int krd_font_x11(char *f, int s);
int krd_render_text_x11(krd_coord topleft, char *str);
krd_bitmap krd_load_bitmap_x11(char *path);
int krd_render_bitmap_x11(krd_coord topleft, krd_bitmap b);
int krd_destroy_bitmap_x11(krd_bitmap b);
#endif
#ifndef KRD_DISABLE_OPENGL
int krd_init_opengl();
int krd_halt_opengl();
int krd_main_opengl_lazy();
int krd_main_opengl_fps();
int krd_title_opengl(char *t);
int krd_request_render_opengl();
int krd_color_opengl(char *color);
int krd_render_pixel_opengl(krd_coord loc);
int krd_render_line_opengl(krd_coord start, krd_coord end, int thickness, int rounded);
int krd_render_rect_opengl(krd_coord topleft, krd_coord bottomright);
int krd_render_arc_opengl(krd_coord center, int innerradius, int outerradius, double startangle, double endangle, int rounded);
char **krd_fontlist_opengl();
int krd_font_opengl(char *f, int s);
int krd_render_text_opengl(krd_coord topleft, char *str);
krd_bitmap krd_load_bitmap_opengl(char *path);
int krd_render_bitmap_opengl(krd_coord topleft, krd_bitmap b);
int krd_destroy_bitmap_opengl(krd_bitmap b);
#endif
int krd_init(krd_backend b);
int krd_halt();
int krd_main(krd_render_mode rm);
int krd_title(char *t);
int krd_request_render();
int krd_color(char *color);
int krd_render_pixel(krd_coord loc);
int krd_render_line(krd_coord start, krd_coord end, int thickness, int rounded);
int krd_render_rect(krd_coord topleft, krd_coord bottomright);
int krd_render_arc(krd_coord center, int innerradius, int outerradius, double startangle, double endangle, int rounded);
char **krd_fontlist();
int krd_font(char *f, int s);
int krd_render_text(krd_coord topleft, char *str);
krd_bitmap krd_load_bitmap(char *path);
int krd_render_bitmap(krd_coord topleft, krd_bitmap b);
int krd_destroy_bitmap(krd_bitmap b);

extern krd_backend krd_current_backend;

extern unsigned int krd_fps;
extern krd_callback krd_render;
extern krd_callback krd_resize;
extern krd_callback krd_keydown;
extern krd_callback krd_keyup;
extern krd_callback krd_mousemove;
extern krd_callback krd_mousedown;
extern krd_callback krd_mouseup;

extern krd_coord krd_size;
extern krd_coord krd_mouse;

#define KRD_KEY_LOWER 128
#define KRD_KEY_LEFT KRD_KEY_LOWER
#define KRD_KEY_RIGHT (KRD_KEY_LOWER+1)
#define KRD_KEY_UP (KRD_KEY_LOWER+2)
#define KRD_KEY_DOWN (KRD_KEY_LOWER+3)
#define KRD_KEY_HOME (KRD_KEY_LOWER+4)
#define KRD_KEY_END (KRD_KEY_LOWER+5)
#define KRD_KEY_INSERT (KRD_KEY_LOWER+6)
#define KRD_KEY_ESCAPE (KRD_KEY_LOWER+7)
#define KRD_KEY_PAGEUP (KRD_KEY_LOWER+8)
#define KRD_KEY_PAGEDOWN (KRD_KEY_LOWER+9)
#define KRD_KEY_F(n) (KRD_KEY_LOWER+7+n)
#define KRD_MOD_CONTROL 0x1
#define KRD_MOD_SHIFT 0x2
#define KRD_MOD_ALT 0x4
#define KRD_MOD_META 0x8

#ifndef M_PI
#define M_PI 3.141592653589
#endif

#define KRD_ERROR_INIT(msg...) printf("[krd-init] " # msg "\n", ##msg)

#endif
