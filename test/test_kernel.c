#include "../kernel/backend.h"

int render(ids_coord c, unsigned int a0, unsigned int a1)
{
	ids_color("#555555");
	ids_render_rect(ids_at(0, 0), c);
	ids_color("#eeeeee");
	ids_render_pixel(ids_at(10, 10));
	ids_font("/usr/share/fonts/TTF/DejaVu.ttf");
	ids_render_text(ids_at(10, 50), "hi");
	return 0;
}

int main()
{
	if (ids_init(opengl))
		return 1;

	ids_size.x = 200;
	ids_size.y = 200;

	ids_render = render;

	if (ids_main(lazy))
		return 1;

	if (ids_halt())
		return 1;

	return 0;
}

