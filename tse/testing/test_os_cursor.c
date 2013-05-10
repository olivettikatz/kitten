#include "../os/os.h"

int main()
{
	int w, h;
	tse_termsize(&w, &h);
	tse_clear();
	tse_moveabs(w/2, h/2);
	sleep(1);

	tse_moverel_left(1);
	sleep(1);

	tse_moverel_down(1);
	sleep(1);

	tse_moverel_right(1);
	sleep(1);

	tse_moverel_up(1);
	sleep(1);

	tse_clear();
	tse_moveabs(0, 0);

	return 0;
}
