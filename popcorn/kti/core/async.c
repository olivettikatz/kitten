#include "async.h"

int kti_async(kti_callback_render r, kti_callback_update u, kti_callback_input i)
{
	kti_raw_input_enable();
	if (r == NULL || u == NULL || i == NULL)
	{
		printf("error: null callback for kti_async\n");
		return 1;
	}

	if (kti_fork() == KTI_FORKED)
	{
		while(1)
		{
			int c = kti_getch();
			if (i(c) != 0)
			{
				kti_raw_input_disable();
				kti_clear();
				kti_kill_parent();
				_exit(0);
			}
			else
			{
				int w, h;
				kti_termsize(&w, &h);
				if (r(w, h) != 0)
				{
					printf("error: failed to render\n");
					kti_kill_parent();
					return 3;
				}
			}
		}
	}

	int delay = 0;
	int w, h;
	kti_termsize(&w, &h);
	if (r(w, h) != 0)
	{
		printf("error: failed to render\n");
		return 3;
	}

	while(1)
	{
		delay = u();
		if (delay < 0)
			return 2;
		kti_termsize(&w, &h);
		if (r(w, h) != 0)
		{
			printf("error: failed to render\n");
			return 3;
		}
		usleep(delay);
	}

	return 0;
}

