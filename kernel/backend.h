#ifndef __INCANDESCENCE_KERNEL_BACKEND_H
#define __INCANDESCENCE_KERNEL_BACKEND_H

#include <stdio.h>
#include <unistd.h>

typedef struct
{
	unsigned int x;
	unsigned int y;
} ids_coord;

typedef struct
{
	ids_coord size;
	void *data;
} ids_bitmap;

ids_coord ids_at(int x, int y);

typedef int (*ids_callback)(ids_coord, unsigned int, unsigned int);

typedef enum
{
#ifndef IDS_DISABLE_X11
	x11,
#endif
#ifndef IDS_DISABLE_OPENGL
	opengl
#endif
} ids_backend;

typedef enum
{
	lazy,
	fps
} ids_render_mode;

#ifndef IDS_DISABLE_X11
int ids_init_x11();
int ids_halt_x11();
int ids_main_x11_lazy();
int ids_main_x11_fps();
int ids_title_x11(char *t);
int ids_request_render_x11();
int ids_color_x11(char *color);
int ids_render_pixel_x11(ids_coord loc);
int ids_render_line_x11(ids_coord start, ids_coord end);
int ids_render_rect_x11(ids_coord topleft, ids_coord bottomright);
char **ids_fontlist_x11();
int ids_font_x11(char *f, int s);
int ids_render_text_x11(ids_coord topleft, char *str);
ids_bitmap ids_load_bitmap_x11(char *path);
int ids_render_bitmap_x11(ids_coord topleft, ids_bitmap b);
int ids_destroy_bitmap_x11(ids_bitmap b);
#endif
#ifndef IDS_DISABLE_OPENGL
int ids_init_opengl();
int ids_halt_opengl();
int ids_main_opengl_lazy();
int ids_main_opengl_fps();
int ids_title_opengl(char *t);
int ids_request_render_opengl();
int ids_color_opengl(char *color);
int ids_render_pixel_opengl(ids_coord loc);
int ids_render_line_opengl(ids_coord start, ids_coord end);
int ids_render_rect_opengl(ids_coord topleft, ids_coord bottomright);
char **ids_fontlist_opengl();
int ids_font_opengl(char *f, int s);
int ids_render_text_opengl(ids_coord topleft, char *str);
ids_bitmap ids_load_bitmap_opengl(char *path);
int ids_render_bitmap_opengl(ids_coord topleft, ids_bitmap b);
int ids_destroy_bitmap_opengl(ids_bitmap b);
#endif
int ids_init(ids_backend b);
int ids_halt();
int ids_main(ids_render_mode rm);
int ids_title(char *t);
int ids_request_render();
int ids_color(char *color);
int ids_render_pixel(ids_coord loc);
int ids_render_line(ids_coord start, ids_coord end);
int ids_render_rect(ids_coord topleft, ids_coord bottomright);
char **ids_fontlist();
int ids_font(char *f, int s);
int ids_render_text(ids_coord topleft, char *str);
ids_bitmap ids_load_bitmap(char *path);
int ids_render_bitmap(ids_coord topleft, ids_bitmap b);
int ids_destroy_bitmap(ids_bitmap b);

extern ids_backend ids_current_backend;

extern unsigned int ids_fps;
extern ids_callback ids_render;
extern ids_callback ids_resize;
extern ids_callback ids_keydown;
extern ids_callback ids_keyup;
extern ids_callback ids_mousemove;
extern ids_callback ids_mousedown;
extern ids_callback ids_mouseup;

extern ids_coord ids_size;
extern ids_coord ids_mouse;

#define IDS_KEY_LOWER 128
#define IDS_KEY_LEFT IDS_KEY_LOWER
#define IDS_KEY_RIGHT (IDS_KEY_LOWER+1)
#define IDS_KEY_UP (IDS_KEY_LOWER+2)
#define IDS_KEY_DOWN (IDS_KEY_LOWER+3)
#define IDS_KEY_HOME (IDS_KEY_LOWER+4)
#define IDS_KEY_END (IDS_KEY_LOWER+5)
#define IDS_KEY_INSERT (IDS_KEY_LOWER+6)
#define IDS_KEY_ESCAPE (IDS_KEY_LOWER+7)
#define IDS_KEY_PAGEUP (IDS_KEY_LOWER+8)
#define IDS_KEY_PAGEDOWN (IDS_KEY_LOWER+9)
#define IDS_KEY_F(n) (IDS_KEY_LOWER+7+n)
#define IDS_MOD_CONTROL 0x1
#define IDS_MOD_SHIFT 0x2
#define IDS_MOD_ALT 0x4
#define IDS_MOD_META 0x8

#define IDS_ERROR_INIT(msg...) printf("[incandescence-init] " # msg "\n", ##msg)

#endif
