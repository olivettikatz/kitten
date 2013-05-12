#include "async.h"

int tse_async(tse_callback_render r, tse_callback_update u, tse_callback_input i)
{
	tse_raw_input_enable();
	if (r == NULL || u == NULL || i == NULL)
	{
		printf("error: null callback for tse_async\n");
		return 1;
	}

	if (tse_fork() == TSE_FORKED)
	{
		while(1)
		{
			int c = tse_getch();
			if (i(c) != 0)
			{
				tse_raw_input_disable();
				tse_clear();
				tse_kill_parent();
				_exit(0);
			}
			else
			{
				int w, h;
				tse_termsize(&w, &h);
				if (r(w, h) != 0)
				{
					printf("error: failed to render\n");
					tse_kill_parent();
					return 3;
				}
			}
		}
	}

	int delay = 0;
	int w, h;
	tse_termsize(&w, &h);
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
		tse_termsize(&w, &h);
		if (r(w, h) != 0)
		{
			printf("error: failed to render\n");
			return 3;
		}
		usleep(delay);
	}

	return 0;
}

