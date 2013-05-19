#ifndef __TSE_OS_LINUX_H
#define __TSE_OS_LINUX_H

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <malloc.h>
#include <string.h>
#include <sys/wait.h>

extern struct termios _tse_old_termios;
extern struct termios _tse_new_termios;

#endif
