#include "../core/stream.h"

int main()
{
	kti_clear();
	printf("Press enter to quit.\n");
	kti_region r = kti_region_new(10, 10, 40, 20);
	kti_stream s = kti_stream_new();
	kti_stream_clear(&s);
	FILE *f = s.desc;
	fprintf(f, "hello, world\nhi\naaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	kti_region_render(&r);
	kti_stream_to_region(&s, &r);
	sleep(1);
	for(int i = 0; i < 20; i++)
		fprintf(f, "\na");
	kti_stream_to_region(&s, &r);
	kti_getch();
	kti_clear();
	return 0;
}
