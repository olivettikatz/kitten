#include "region.h"

tse_region tse_region_new(int x, int y, int w, int h)
{
	tse_region tmp = {
		x,
		y,
		w,
		h,
		TSE_COLOR_BLACK,
		TSE_REGION_VERT,
		TSE_REGION_HORIZ,
		TSE_REGION_TOPLEFT,
		TSE_REGION_TOPRIGHT,
		TSE_REGION_BOTTOMLEFT,
		TSE_REGION_BOTTOMRIGHT,
		TSE_REGION_EMPTY
	};

	return tmp;
}

int tse_region_render(tse_region *r)
{
	if (r == NULL)
		return 1;

	if (r->x < 0)
		return 2;

	if (r->y < 0)
		return 3;

	int sw = 0;
	int sh = 0;
	tse_termsize(&sw, &sh);

	if (r->x+r->w >= sw)
		return 4;

	if (r->y+r->h >= sh)
		return 5;

	tse_moveabs(r->x, r->y);
	tse_attr_reset();
	tse_attr_bg(r->bg);
	
	tse_puts(r->topleft);
	for (int i = 0; i < r->w-2; i++)
	{
		tse_puts(r->horiz);
	}
	tse_puts(r->topright);
	tse_putch(TSE_NEWLINE);

	for (int i = 0; i < r->h-2; i++)
	{
		tse_moverel_right(r->x-1);
		tse_puts(r->vert);
		for (int j = 0; j < r->w-2; j++)
		{
			tse_puts(r->empty);
		}
		tse_puts(r->vert);
		tse_putch(TSE_NEWLINE);
	}

	tse_moverel_right(r->x-1);
	tse_puts(r->bottomleft);
	for (int i = 0; i < r->w-2; i++)
	{
		tse_puts(r->horiz);
	}
	tse_puts(r->bottomright);
	tse_moveabs(r->x+1, r->y+1);
	tse_attr_reset();

	return 0;
}

