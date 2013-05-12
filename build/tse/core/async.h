#ifndef __TSE_ASYNC_H
#define __TSE_ASYNC_H

#include "../os/os.h"

typedef int (*tse_callback_render)(int w, int h);
typedef int (*tse_callback_update)();
typedef int (*tse_callback_input)(int k);

int tse_async(tse_callback_render r, tse_callback_update u, tse_callback_input i);

#endif
