#include "../backend.h"
#include <string.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <Imlib2.h>

Display *dsp;
Window win;
GC gc;
XftFont *font;
XftDraw *draw;
XftColor xftcolor;

int krd_init_x11()
{
	dsp = XOpenDisplay(NULL);
	if (dsp == NULL)
	{
		KRD_ERROR_INIT("cannot open X11 display");
		return 1;
	}
	return 0;
}

int krd_halt_x11()
{
	XCloseDisplay(dsp);
	return 0;
}

int krd_main_x11_lazy()
{
	win = XCreateSimpleWindow(dsp, RootWindow(dsp, DefaultScreen(dsp)), 1, 1, krd_size.x, krd_size.y, 0, BlackPixel(dsp, DefaultScreen(dsp)), BlackPixel(dsp, DefaultScreen(dsp)));
	XSelectInput(dsp, win, ExposureMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|PointerMotionMask|StructureNotifyMask);
	XMapWindow(dsp, win);
	gc = XCreateGC(dsp, win, 0, 0);
	XSetBackground(dsp, gc, BlackPixel(dsp, DefaultScreen(dsp)));
	XSetForeground(dsp, gc, BlackPixel(dsp, DefaultScreen(dsp)));
	draw = XftDrawCreate(dsp, win, DefaultVisual(dsp, DefaultScreen(dsp)), DefaultColormap(dsp, DefaultScreen(dsp)));

	XEvent e;
	while(1)
	{
		XNextEvent(dsp, &e);
		if (e.type == Expose)
		{
			krd_size.x = ((XExposeEvent *)&e)->width;
			krd_size.y = ((XExposeEvent *)&e)->height;
			if (krd_render)
				krd_render(krd_size, 0, 0);
		}
		else if (e.type == DestroyNotify)
		{
			break;
		}
		else if (e.type == KeyPress || e.type == KeyRelease)
		{
			char buf[32];
			KeySym sym;
			XComposeStatus status;
			int l = XLookupString(&e.xkey, buf, 32, &sym, &status);
			unsigned int k = 0;
			if (l == 1)
				k = (unsigned int)buf[0];
			else if (sym == XK_Left)
				k = KRD_KEY_LEFT;
			else if (sym == XK_Right)
				k = KRD_KEY_RIGHT;
			else if (sym == XK_Up)
				k = KRD_KEY_UP;
			else if (sym == XK_Down)
				k = KRD_KEY_DOWN;
			else if (sym == XK_Home)
				k = KRD_KEY_HOME;
			else if (sym == XK_End)
				k = KRD_KEY_END;
			else if (sym == XK_Insert)
				k = KRD_KEY_INSERT;
			else if (sym == XK_Escape)
				k = KRD_KEY_ESCAPE;
			else if (sym == XK_F1)
				k = KRD_KEY_F(1);
			else if (sym == XK_F2)
				k = KRD_KEY_F(2);
			else if (sym == XK_F3)
				k = KRD_KEY_F(3);
			else if (sym == XK_F4)
				k = KRD_KEY_F(4);
			else if (sym == XK_F5)
				k = KRD_KEY_F(5);
			else if (sym == XK_F6)
				k = KRD_KEY_F(6);
			else if (sym == XK_F7)
				k = KRD_KEY_F(7);
			else if (sym == XK_F8)
				k = KRD_KEY_F(8);
			else if (sym == XK_F9)
				k = KRD_KEY_F(9);
			else if (sym == XK_F10)
				k = KRD_KEY_F(10);
			else if (sym == XK_F11)
				k = KRD_KEY_F(11);
			else if (sym == XK_F12)
				k = KRD_KEY_F(12);
			else if (sym == XK_BackSpace)
				k = '\b';
			else if (sym == XK_Tab)
				k = '\t';
			else if (sym == XK_Return)
				k = '\n';
			else if (sym == XK_Page_Up)
				k = KRD_KEY_PAGEUP;
			else if (sym == XK_Page_Down)
				k = KRD_KEY_PAGEDOWN;
			else
				k = 0;

			unsigned int m = 0;
			if (e.xkey.state & ControlMask)
				m |= KRD_MOD_CONTROL;
			if (e.xkey.state & ShiftMask)
				m |= KRD_MOD_SHIFT;
			if (e.xkey.state & Mod1Mask)
				m |= KRD_MOD_ALT;
			if (e.xkey.state & Mod4Mask)
				m |= KRD_MOD_META;

			if (krd_keydown && e.type == KeyPress)
				krd_keydown(krd_at(0, 0), k, m);
			if (krd_keyup && e.type == KeyRelease)
				krd_keyup(krd_at(0, 0), k, m);
		}
		else if (e.type == MotionNotify)
		{
			krd_mouse.x = e.xmotion.x;
			krd_mouse.y = e.xmotion.y;
			if (krd_mousemove)
				krd_mousemove(krd_mouse, 0, 0);
		}
		else if (e.type == ButtonPress || e.type == ButtonRelease)
		{
			krd_coord c = krd_at(e.xbutton.x, e.xbutton.y);
			if (krd_mousedown && e.type == ButtonPress)
				krd_mousedown(c, e.xbutton.button, 0);
			if (krd_mouseup && e.type == ButtonRelease)
				krd_mouseup(c, e.xbutton.button, 0);
		}
	}

	return 0;
}

int krd_main_x11_fps()
{
	return 1;
}

int krd_title_x11(char *t)
{
	if (dsp == NULL)
		return 1;

	XStoreName(dsp, win, t);
	return 0;
}

int krd_request_render_x11()
{
	XEvent e;
	e.type = Expose;
	e.xexpose.window = win;
	XSendEvent(dsp, win, False, ExposureMask, &e);
	XFlush(dsp);
	return 0;
}

int krd_color_x11(char *color)
{
	XColor xc;
	XParseColor(dsp, DefaultColormap(dsp, DefaultScreen(dsp)), color, &xc);
	XAllocColor(dsp, DefaultColormap(dsp, DefaultScreen(dsp)), &xc);
	XSetForeground(dsp, gc, xc.pixel);
	XRenderColor xrc;
	xrc.red = xc.red;
	xrc.green = xc.green;
	xrc.blue = xc.blue;
	XftColorAllocValue(dsp, DefaultVisual(dsp, DefaultScreen(dsp)), DefaultColormap(dsp, DefaultScreen(dsp)), &xrc, &xftcolor);
	return 0;
}

int krd_render_pixel_x11(krd_coord loc)
{
	XDrawPoint(dsp, win, gc, loc.x, loc.y);
	return 0;
}

int krd_render_line_x11(krd_coord start, krd_coord end, int thickness, int rounded)
{
	XSetLineAttributes(dsp, gc, thickness, LineSolid, (rounded ? CapRound : CapNotLast), JoinBevel);
	XDrawLine(dsp, win, gc, start.x, start.y, end.x, end.y);
	return 0;
}

int krd_render_rect_x11(krd_coord topleft, krd_coord bottomright)
{
	XFillRectangle(dsp, win, gc, topleft.x, topleft.y, bottomright.x-topleft.x, bottomright.y-topleft.y);
	return 0;
}

#define KRD_X11_ANGLE(a) (int)(a*11520/M_PI)

int krd_render_arc_x11(krd_coord center, int innerradius, int outerradius, double startangle, double endangle, int rounded)
{
	XSetLineAttributes(dsp, gc, outerradius-innerradius, LineSolid, (rounded ? CapRound : CapNotLast), JoinBevel);
	XDrawArc(dsp, win, gc, center.x-(innerradius+outerradius)*0.5, center.y-(innerradius+outerradius)*0.5, innerradius+outerradius, innerradius+outerradius, KRD_X11_ANGLE(startangle), KRD_X11_ANGLE(endangle));
	return 0;
}

char **krd_fontlist_x11()
{
	int n = 0;
	char **tmp = XListFonts(dsp, "*", 8092, &n);
	tmp[n] = NULL;
	return tmp;
}

int krd_font_x11(char *f, int s)
{
	if (f == NULL)
		return 1;
	font = XftFontOpenName(dsp, DefaultScreen(dsp), f);
	if (font == NULL)
		return 2;
	return 0;
}

int krd_render_text_x11(krd_coord topleft, char *str)
{
	if (draw == NULL)
		return 1;
	if (font == NULL)
		return 2;
	if (str == NULL)
		return 3;
	if (str[0] == 0)
		return 4;

	XftDrawString8(draw, &xftcolor, font, topleft.x, topleft.y, (XftChar8 *)str, strlen(str));
	return 0;
}

krd_bitmap krd_load_bitmap_x11(char *path)
{
	int x, y;
	unsigned int w, h;
	Pixmap bitmap;

	krd_bitmap rtn;
	rtn.size = krd_at(0, 0);
	rtn.data = NULL;

	Imlib_Image img = imlib_load_image(path);
	if (!img)
		return rtn;

	imlib_context_set_image(img);

	rtn.size.x = imlib_image_get_width();
	rtn.size.y = imlib_image_get_height();
	rtn.data = malloc(sizeof(Imlib_Image));
	memcpy(rtn.data, &img, sizeof(Imlib_Image));
	return rtn;
}

int krd_render_bitmap_x11(krd_coord topleft, krd_bitmap b)
{
	imlib_context_set_image(*(Imlib_Image *)b.data);
	imlib_blend_image_onto_image(*(Imlib_Image *)b.data, 0, 0, 0, b.size.x, b.size.y, topleft.x, topleft.y, b.size.x, b.size.y);
	return 0;
}

int krd_destroy_bitmap_x11(krd_bitmap b)
{
	imlib_context_set_image(*(Imlib_Image *)b.data);
	imlib_free_image();
	free(b.data);
	return 0;
}

