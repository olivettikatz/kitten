#include "../core/region.h"

int main()
{
	tse_clear();
	printf("Press enter to exit.\n");
	tse_region r = tse_region_new(10, 10, 10, 10);
	tse_region_render(&r);
	tse_getch();
	tse_clear();
	return 0;
}

