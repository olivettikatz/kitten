#ifndef __KTI_OS_H
#define __KTI_OS_H

#define KTI_SPACE ' '
#define KTI_TAB '\t'
#define KTI_NEWLINE '\n'
#define KTI_BEEP '\a'
#define KTI_VERT "\u2502"
#define KTI_HORIZ "\u2500"
#define KTI_VERT_BOLD "\u2503"
#define KTI_HORIZ_BOLD "\u2501"
#define KTI_TOPLEFT "\u250c"
#define KTI_TOPRIGHT "\u2510"
#define KTI_BOTTOMLEFT "\u2514"
#define KTI_BOTTOMRIGHT "\u2518"
#define KTI_TOPLEFT_BOLD "\u250f"
#define KTI_TOPRIGHT_BOLD "\u2513"
#define KTI_BOTTOMLEFT_BOLD "\u2517"
#define KTI_BOTTOMRIGHT_BOLD "\u251b"
#define KTI_T_TOP "\u252c"
#define KTI_T_BOTTOM "\u2534"
#define KTI_T_LEFT "\u251c"
#define KTI_T_RIGHT "\u2524"
#define KTI_T_TOP_BOLD "\u2533"
#define KTI_T_BOTTOM_BOLD "\u253b"
#define KTI_T_LEFT_BOLD "\u2523"
#define KTI_T_RIGHT_BOLD "\u252b"

#define KTI_COLOR_BLACK 0
#define KTI_COLOR_RED 1
#define KTI_COLOR_GREEN 2
#define KTI_COLOR_YELLOW 3
#define KTI_COLOR_BLUE 4
#define KTI_COLOR_MAGENTA 5
#define KTI_COLOR_CYAN 6
#define KTI_COLOR_WHITE 7
#define KTI_N_COLORS 8

#define KTI_KEY_LEFT 200
#define KTI_KEY_RIGHT 201
#define KTI_KEY_UP 202
#define KTI_KEY_DOWN 203

int kti_raw_input_enable();
int kti_raw_input_disable();
int kti_getch();

int kti_putch(char c);
int kti_puts(char *s);
int kti_flush();

int kti_moverel_left(int n);
int kti_moverel_right(int n);
int kti_moverel_up(int n);
int kti_moverel_down(int n);
int kti_moveabs(int x, int y);

int kti_clear();
int kti_clear_line();
int kti_clear_scrollbuffer();

int kti_attr_reset();
int kti_attr_bold();
int kti_attr_dimmed();
int kti_attr_underline();
int kti_attr_blinking();
int kti_attr_inverted();
int kti_attr_fg(int col);
int kti_attr_bg(int col);

int kti_termsize(int *x, int *y);

#ifdef __linux__
	#include "linux/linux.h"
#else
	#error Unsupported operating system (supported: linux)
#endif

FILE *kti_memstream_new(char **p, size_t *s);
void kti_memstream_del(FILE *f);

#define KTI_FORKED 0

int kti_fork();
int kti_kill_parent();
int kti_merge(int pid);
int kti_stdout_to_stdin();
int kti_restore_stdin();

#endif
