#include "region.h"

kti_region kti_region_new(int x, int y, int w, int h)
{
	kti_region tmp = {
		x,
		y,
		w,
		h,
		KTI_COLOR_BLACK,
		1,
		KTI_REGION_VERT,
		KTI_REGION_HORIZ,
		KTI_REGION_TOPLEFT,
		KTI_REGION_TOPRIGHT,
		KTI_REGION_BOTTOMLEFT,
		KTI_REGION_BOTTOMRIGHT,
		KTI_REGION_EMPTY
	};

	return tmp;
}

int kti_region_render(kti_region *r)
{
	if (r == NULL)
		return 1;

	if (r->x < 0)
		return 2;

	if (r->y < 0)
		return 3;

	int sw = 0;
	int sh = 0;
	kti_termsize(&sw, &sh);

	if (r->x+r->w >= sw)
		return 4;

	if (r->y+r->h >= sh)
		return 5;

	kti_moveabs(r->x, r->y);
	kti_attr_reset();
	kti_attr_bg(r->bg);
	
	kti_puts(r->topleft);
	for (int i = 0; i < r->w-r->border*2; i++)
	{
		kti_puts(r->horiz);
	}
	kti_puts(r->topright);
	kti_putch(KTI_NEWLINE);

	for (int i = 0; i < r->h-r->border*2; i++)
	{
		kti_moverel_right(r->x-1);
		kti_puts(r->vert);
		for (int j = 0; j < r->w-r->border*2; j++)
		{
			kti_puts(r->empty);
		}
		kti_puts(r->vert);
		kti_putch(KTI_NEWLINE);
	}

	kti_moverel_right(r->x-1);
	kti_puts(r->bottomleft);
	for (int i = 0; i < r->w-r->border*2; i++)
	{
		kti_puts(r->horiz);
	}
	kti_puts(r->bottomright);
	kti_moveabs(r->x+r->border, r->y+r->border);
	kti_attr_reset();

	return 0;
}

