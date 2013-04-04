#ifndef __TSE_OS_H
#define __TSE_OS_H

#define TSE_SPACE ' '
#define TSE_TAB '\t'
#define TSE_NEWLINE '\n'
#define TSE_BEEP '\a'
#define TSE_VERT "\u2502"
#define TSE_HORIZ "\u2500"
#define TSE_VERT_BOLD "\u2503"
#define TSE_HORIZ_BOLD "\u2501"
#define TSE_TOPLEFT "\u250c"
#define TSE_TOPRIGHT "\u2510"
#define TSE_BOTTOMLEFT "\u2514"
#define TSE_BOTTOMRIGHT "\u2518"
#define TSE_TOPLEFT_BOLD "\u250f"
#define TSE_TOPRIGHT_BOLD "\u2513"
#define TSE_BOTTOMLEFT_BOLD "\u2517"
#define TSE_BOTTOMRIGHT_BOLD "\u251b"
#define TSE_T_TOP "\u252c"
#define TSE_T_BOTTOM "\u2534"
#define TSE_T_LEFT "\u251c"
#define TSE_T_RIGHT "\u2524"
#define TSE_T_TOP_BOLD "\u2533"
#define TSE_T_BOTTOM_BOLD "\u253b"
#define TSE_T_LEFT_BOLD "\u2523"
#define TSE_T_RIGHT_BOLD "\u252b"

#define TSE_COLOR_BLACK 0
#define TSE_COLOR_RED 1
#define TSE_COLOR_GREEN 2
#define TSE_COLOR_YELLOW 3
#define TSE_COLOR_BLUE 4
#define TSE_COLOR_MAGENTA 5
#define TSE_COLOR_CYAN 6
#define TSE_COLOR_WHITE 7

#define TSE_KEY_LEFT 200
#define TSE_KEY_RIGHT 201
#define TSE_KEY_UP 202
#define TSE_KEY_DOWN 203

int tse_raw_input_enable();
int tse_raw_input_disable();
int tse_getch();

int tse_putch(char c);
int tse_puts(char *s);
int tse_flush();

int tse_moverel_left(int n);
int tse_moverel_right(int n);
int tse_moverel_up(int n);
int tse_moverel_down(int n);
int tse_moveabs(int x, int y);

int tse_clear();
int tse_clear_line();
int tse_clear_scrollbuffer();

int tse_attr_reset();
int tse_attr_bold();
int tse_attr_dimmed();
int tse_attr_underline();
int tse_attr_blinking();
int tse_attr_inverted();
int tse_attr_fg(int col);
int tse_attr_bg(int col);

int tse_termsize(int *x, int *y);

#ifdef __linux__
	#include "linux/linux.h"
#else
	#error Unsupported operating system (supported: linux)
#endif

FILE *tse_memstream_new(char **p, size_t *s);
void tse_memstream_del(FILE *f);
char *tse_memstream_pull(FILE *f, char **p, size_t *s);

#define TSE_FORKED 0

int tse_fork();
int tse_merge(int pid);
int tse_stdout_to_stdin();
int tse_restore_stdin();

#endif
