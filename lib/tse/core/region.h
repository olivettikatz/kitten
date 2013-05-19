#ifndef __TSE_REGION_H
#define __TSE_REGION_H

#include "../os/os.h"

#define TSE_REGION_BG TSE_COLOR_BLACK
#define TSE_REGION_VERT TSE_VERT
#define TSE_REGION_HORIZ TSE_HORIZ
#define TSE_REGION_TOPLEFT TSE_TOPLEFT
#define TSE_REGION_TOPRIGHT TSE_TOPRIGHT
#define TSE_REGION_BOTTOMLEFT TSE_BOTTOMLEFT
#define TSE_REGION_BOTTOMRIGHT TSE_BOTTOMRIGHT
#define TSE_REGION_EMPTY " "

typedef struct
{
	int x;
	int y;
	int w;
	int h;
	int bg;
	int border;
	char *vert;
	char *horiz;
	char *topleft;
	char *topright;
	char *bottomleft;
	char *bottomright;
	char *empty;
} tse_region;

tse_region tse_region_new(int x, int y, int w, int h);
int tse_region_render(tse_region *r);

#endif
