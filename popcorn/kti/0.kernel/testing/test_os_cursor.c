#include "../os/os.h"

int main()
{
	int w, h;
	kti_termsize(&w, &h);
	kti_clear();
	kti_moveabs(w/2, h/2);
	sleep(1);

	kti_moverel_left(1);
	sleep(1);

	kti_moverel_down(1);
	sleep(1);

	kti_moverel_right(1);
	sleep(1);

	kti_moverel_up(1);
	sleep(1);

	kti_clear();
	kti_moveabs(0, 0);

	return 0;
}
