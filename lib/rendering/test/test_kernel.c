#include "../kernel/backend.h"

int render(krd_coord c, unsigned int a0, unsigned int a1)
{
	krd_color("#555555");
	krd_render_rect(krd_at(0, 0), krd_at(100, 100));
	krd_color("#eeeeee");
	krd_render_pixel(krd_at(100, 90));
	krd_render_line(krd_at(0, 0), krd_at(100, 100), 1, 0);
	krd_render_rect(krd_at(10, 10), krd_at(20, 20));
	krd_font("/usr/share/fonts/TTF/DejaVuSans.ttf", 16);
	krd_render_text(krd_at(10, 50), "hi");
	return 0;
}

int main()
{
	if (krd_init(x11))
		return 1;

	krd_size.x = 200;
	krd_size.y = 200;

	krd_render = render;

	if (krd_main(lazy))
		return 1;

	if (krd_halt())
		return 1;

	return 0;
}

