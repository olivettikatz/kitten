#include "../core/stream.h"

int main()
{
	tse_clear();
	printf("Press enter to quit.\n");
	tse_region r = tse_region_new(10, 10, 40, 20);
	tse_stream s = tse_stream_new();
	tse_stream_clear(&s);
	FILE *f = s.desc;
	fprintf(f, "hello, world\nhi\naaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	tse_region_render(&r);
	tse_stream_to_region(&s, &r);
	sleep(1);
	for(int i = 0; i < 20; i++)
		fprintf(f, "\na");
	tse_stream_to_region(&s, &r);
	tse_getch();
	tse_clear();
	return 0;
}
