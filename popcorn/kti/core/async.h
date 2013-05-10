#ifndef __KTI_ASYNC_H
#define __KTI_ASYNC_H

#include "../os/os.h"

typedef int (*kti_callback_render)(int w, int h);
typedef int (*kti_callback_update)();
typedef int (*kti_callback_input)(int k);

int kti_async(kti_callback_render r, kti_callback_update u, kti_callback_input i);

#endif
