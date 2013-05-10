#ifndef __KTI_OS_LINUX_H
#define __KTI_OS_LINUX_H

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <malloc.h>
#include <string.h>
#include <sys/wait.h>

extern struct termios _kti_old_termios;
extern struct termios _kti_new_termios;

#endif
