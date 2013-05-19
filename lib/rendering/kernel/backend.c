#include "backend.h"

krd_backend krd_current_backend;
krd_callback krd_render = NULL, krd_resize = NULL, krd_keydown = NULL, krd_keyup = NULL, krd_mousemove = NULL, krd_mousedown = NULL, krd_mouseup = NULL;
krd_coord krd_size = {0, 0};
krd_coord krd_mouse = {0, 0};
unsigned int krd_fps = 30;

krd_coord krd_at(int x, int y)
{
	krd_coord tmp = {
		x, y
	};
	return tmp;
}

int krd_init(krd_backend b)
{
	krd_current_backend = b;
#ifndef KRD_DISABLE_X11
	if (b == x11)
		return krd_init_x11();
#endif
#ifndef KRD_DISABLE_OPENGL
	if (b == opengl)
		return krd_init_opengl();
#endif
}

int krd_halt()
{
#ifndef KRD_DISABLE_X11
	if (krd_current_backend == x11)
		return krd_halt_x11();
#endif
#ifndef KRD_DISABLE_OPENGL
	if (krd_current_backend == opengl)
		return krd_halt_opengl();
#endif
}

int krd_main(krd_render_mode rm)
{
#ifndef KRD_DISABLE_X11
	if (krd_current_backend == x11)
	{
		if (rm == lazy)
			return krd_main_x11_lazy();
		else if (rm == fps)
			return krd_main_x11_fps();
	}
#endif
#ifndef KRD_DISABLE_OPENGL
	if (krd_current_backend == opengl)
	{
		if (rm == lazy)
			return krd_main_opengl_lazy();
		else if (rm == fps)
			return krd_main_opengl_fps();
	}
#endif
}

int krd_title(char *t)
{
#ifndef KRD_DISABLE_X11
	if (krd_current_backend == x11)
		return krd_title_x11(t);
#endif
#ifndef KRD_DISABLE_OPENGL
	if (krd_current_backend == opengl)
		return krd_title_opengl(t);
#endif
}

int krd_request_render()
{
#ifndef KRD_DISABLE_X11
	if (krd_current_backend == x11)
		return krd_request_render_x11();
#endif
#ifndef KRD_DISABLE_OPENGL
	if (krd_current_backend == opengl)
		return krd_request_render_opengl();
#endif
}

int krd_color(char *color)
{
#ifndef KRD_DISABLE_X11
	if (krd_current_backend == x11)
		return krd_color_x11(color);
#endif
#ifndef KRD_DISABLE_OPENGL
	if (krd_current_backend == opengl)
		return krd_color_opengl(color);
#endif
}

int krd_render_pixel(krd_coord loc)
{
#ifndef KRD_DISABLE_X11
	if (krd_current_backend == x11)
		return krd_render_pixel_x11(loc);
#endif
#ifndef KRD_DISABLE_OPENGL
	if (krd_current_backend == opengl)
		return krd_render_pixel_opengl(loc);
#endif
}

int krd_render_line(krd_coord start, krd_coord end, int thickness, int rounded)
{
#ifndef KRD_DISABLE_X11
	if (krd_current_backend == x11)
		return krd_render_line_x11(start, end, thickness, rounded);
#endif
#ifndef KRD_DISABLE_OPENGL
	if (krd_current_backend == opengl)
		return krd_render_line_opengl(start, end, thickness, rounded);
#endif
}

int krd_render_rect(krd_coord topleft, krd_coord bottomright)
{
#ifndef KRD_DISABLE_X11
	if (krd_current_backend == x11)
		return krd_render_rect_x11(topleft, bottomright);
#endif
#ifndef KRD_DISABLE_OPENGL
	if (krd_current_backend == opengl)
		return krd_render_rect_opengl(topleft, bottomright);
#endif
}

int krd_render_arc(krd_coord center, int innerradius, int outerradius, double startangle, double endangle, int rounded)
{
#ifndef KRD_DISABLE_X11
	if (krd_current_backend == x11)
		return krd_render_arc_x11(center, innerradius, outerradius, startangle, endangle, rounded);
#endif
#ifndef KRD_DISABLE_OPENGL
	if (krd_current_backend == opengl)
		return krd_render_arc_opengl(center, innerradius, outerradius, startangle, endangle, rounded);
#endif
}

char **krd_fontlist()
{
#ifndef KRD_DISABLE_X11
	if (krd_current_backend == x11)
		return krd_fontlist_x11();
#endif
#ifndef KRD_DISABLE_OPENGL
	if (krd_current_backend == opengl)
		return krd_fontlist_opengl();
#endif
}

int krd_font(char *f, int s)
{
#ifndef KRD_DISABLE_X11
	if (krd_current_backend == x11)
		return krd_font_x11(f, s);
#endif
#ifndef KRD_DISABLE_OPENGL
	if (krd_current_backend == opengl)
		return krd_font_opengl(f, s);
#endif
}

int krd_render_text(krd_coord topleft, char *str)
{
#ifndef KRD_DISABLE_X11
	if (krd_current_backend == x11)
		return krd_render_text_x11(topleft, str);
#endif
#ifndef KRD_DISABLE_OPENGL
	if (krd_current_backend == opengl)
		return krd_render_text_opengl(topleft, str);
#endif
}

krd_bitmap krd_load_bitmap(char *path)
{
#ifndef KRD_DISABLE_X11
	if (krd_current_backend == x11)
		return krd_load_bitmap_x11(path);
#endif
#ifndef KRD_DISABLE_OPENGL
	if (krd_current_backend == opengl)
		return krd_load_bitmap_opengl(path);
#endif
}

int krd_render_bitmap(krd_coord topleft, krd_bitmap b)
{
#ifndef KRD_DISABLE_X11
	if (krd_current_backend == x11)
		return krd_render_bitmap_x11(topleft, b);
#endif
#ifndef KRD_DISABLE_OPENGL
	if (krd_current_backend == opengl)
		return krd_render_bitmap_opengl(topleft, b);
#endif
}

int krd_destroy_bitmap(krd_bitmap b)
{
#ifndef KRD_DISABLE_X11
	if (krd_current_backend == x11)
		return krd_destroy_bitmap_x11(b);
#endif
#ifndef KRD_DISABLE_OPENGL
	if (krd_current_backend == opengl)
		return krd_destroy_bitmap_opengl(b);
#endif
}

