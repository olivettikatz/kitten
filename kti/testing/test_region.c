#include "../core/region.h"

int main()
{
	kti_clear();
	printf("Press enter to exit.\n");
	kti_region r = kti_region_new(10, 10, 10, 10);
	kti_region_render(&r);
	kti_getch();
	kti_clear();
	return 0;
}

