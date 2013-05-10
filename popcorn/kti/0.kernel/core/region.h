#ifndef __KTI_REGION_H
#define __KTI_REGION_H

#include "../os/os.h"

#define KTI_REGION_BG KTI_COLOR_BLACK
#define KTI_REGION_VERT KTI_VERT
#define KTI_REGION_HORIZ KTI_HORIZ
#define KTI_REGION_TOPLEFT KTI_TOPLEFT
#define KTI_REGION_TOPRIGHT KTI_TOPRIGHT
#define KTI_REGION_BOTTOMLEFT KTI_BOTTOMLEFT
#define KTI_REGION_BOTTOMRIGHT KTI_BOTTOMRIGHT
#define KTI_REGION_EMPTY " "

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
} kti_region;

kti_region kti_region_new(int x, int y, int w, int h);
int kti_region_render(kti_region *r);

#endif
