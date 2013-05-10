#include "backend.h"

ids_backend ids_current_backend;
ids_callback ids_render = NULL, ids_resize = NULL, ids_keydown = NULL, ids_keyup = NULL, ids_mousemove = NULL, ids_mousedown = NULL, ids_mouseup = NULL;
ids_coord ids_size = {0, 0};
ids_coord ids_mouse = {0, 0};
unsigned int ids_fps = 30;

ids_coord ids_at(int x, int y)
{
	ids_coord tmp = {
		x, y
	};
	return tmp;
}

int ids_init(ids_backend b)
{
	ids_current_backend = b;
#ifndef IDS_DISABLE_X11
	if (b == x11)
		return ids_init_x11();
#endif
#ifndef IDS_DISABLE_OPENGL
	if (b == opengl)
		return ids_init_opengl();
#endif
}

int ids_halt()
{
#ifndef IDS_DISABLE_X11
	if (ids_current_backend == x11)
		return ids_halt_x11();
#endif
#ifndef IDS_DISABLE_OPENGL
	if (ids_current_backend == opengl)
		return ids_halt_opengl();
#endif
}

int ids_main(ids_render_mode rm)
{
#ifndef IDS_DISABLE_X11
	if (ids_current_backend == x11)
	{
		if (rm == lazy)
			return ids_main_x11_lazy();
		else if (rm == fps)
			return ids_main_x11_fps();
	}
#endif
#ifndef IDS_DISABLE_OPENGL
	if (ids_current_backend == opengl)
	{
		if (rm == lazy)
			return ids_main_opengl_lazy();
		else if (rm == fps)
			return ids_main_opengl_fps();
	}
#endif
}

int ids_title(char *t)
{
#ifndef IDS_DISABLE_X11
	if (ids_current_backend == x11)
		return ids_title_x11(t);
#endif
#ifndef IDS_DISABLE_OPENGL
	if (ids_current_backend == opengl)
		return ids_title_opengl(t);
#endif
}

int ids_request_render()
{
#ifndef IDS_DISABLE_X11
	if (ids_current_backend == x11)
		return ids_request_render_x11();
#endif
#ifndef IDS_DISABLE_OPENGL
	if (ids_current_backend == opengl)
		return ids_request_render_opengl();
#endif
}

int ids_color(char *color)
{
#ifndef IDS_DISABLE_X11
	if (ids_current_backend == x11)
		return ids_color_x11(color);
#endif
#ifndef IDS_DISABLE_OPENGL
	if (ids_current_backend == opengl)
		return ids_color_opengl(color);
#endif
}

int ids_render_pixel(ids_coord loc)
{
#ifndef IDS_DISABLE_X11
	if (ids_current_backend == x11)
		return ids_render_pixel_x11(loc);
#endif
#ifndef IDS_DISABLE_OPENGL
	if (ids_current_backend == opengl)
		return ids_render_pixel_opengl(loc);
#endif
}

int ids_render_line(ids_coord start, ids_coord end, int thickness, int rounded)
{
#ifndef IDS_DISABLE_X11
	if (ids_current_backend == x11)
		return ids_render_line_x11(start, end, thickness, rounded);
#endif
#ifndef IDS_DISABLE_OPENGL
	if (ids_current_backend == opengl)
		return ids_render_line_opengl(start, end, thickness, rounded);
#endif
}

int ids_render_rect(ids_coord topleft, ids_coord bottomright)
{
#ifndef IDS_DISABLE_X11
	if (ids_current_backend == x11)
		return ids_render_rect_x11(topleft, bottomright);
#endif
#ifndef IDS_DISABLE_OPENGL
	if (ids_current_backend == opengl)
		return ids_render_rect_opengl(topleft, bottomright);
#endif
}

int ids_render_arc(ids_coord center, int innerradius, int outerradius, double startangle, double endangle, int rounded)
{
#ifndef IDS_DISABLE_X11
	if (ids_current_backend == x11)
		return ids_render_arc_x11(center, innerradius, outerradius, startangle, endangle, rounded);
#endif
#ifndef IDS_DISABLE_OPENGL
	if (ids_current_backend == opengl)
		return ids_render_arc_opengl(center, innerradius, outerradius, startangle, endangle, rounded);
#endif
}

char **ids_fontlist()
{
#ifndef IDS_DISABLE_X11
	if (ids_current_backend == x11)
		return ids_fontlist_x11();
#endif
#ifndef IDS_DISABLE_OPENGL
	if (ids_current_backend == opengl)
		return ids_fontlist_opengl();
#endif
}

int ids_font(char *f, int s)
{
#ifndef IDS_DISABLE_X11
	if (ids_current_backend == x11)
		return ids_font_x11(f, s);
#endif
#ifndef IDS_DISABLE_OPENGL
	if (ids_current_backend == opengl)
		return ids_font_opengl(f, s);
#endif
}

int ids_render_text(ids_coord topleft, char *str)
{
#ifndef IDS_DISABLE_X11
	if (ids_current_backend == x11)
		return ids_render_text_x11(topleft, str);
#endif
#ifndef IDS_DISABLE_OPENGL
	if (ids_current_backend == opengl)
		return ids_render_text_opengl(topleft, str);
#endif
}

ids_bitmap ids_load_bitmap(char *path)
{
#ifndef IDS_DISABLE_X11
	if (ids_current_backend == x11)
		return ids_load_bitmap_x11(path);
#endif
#ifndef IDS_DISABLE_OPENGL
	if (ids_current_backend == opengl)
		return ids_load_bitmap_opengl(path);
#endif
}

int ids_render_bitmap(ids_coord topleft, ids_bitmap b)
{
#ifndef IDS_DISABLE_X11
	if (ids_current_backend == x11)
		return ids_render_bitmap_x11(topleft, b);
#endif
#ifndef IDS_DISABLE_OPENGL
	if (ids_current_backend == opengl)
		return ids_render_bitmap_opengl(topleft, b);
#endif
}

int ids_destroy_bitmap(ids_bitmap b)
{
#ifndef IDS_DISABLE_X11
	if (ids_current_backend == x11)
		return ids_destroy_bitmap_x11(b);
#endif
#ifndef IDS_DISABLE_OPENGL
	if (ids_current_backend == opengl)
		return ids_destroy_bitmap_opengl(b);
#endif
}

