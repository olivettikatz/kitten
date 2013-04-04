#include "../os.h"
#include "./linux.h"

struct termios _tse_old_termios;
struct termios _tse_new_termios;

int tse_raw_input_enable()
{
	if (tcgetattr(0, &_tse_old_termios) < 0)
		return 1;
	_tse_new_termios = _tse_old_termios;
	_tse_new_termios.c_lflag &= ~ICANON;
	_tse_new_termios.c_lflag &= ~ECHO;
	if (tcsetattr(0, TCSANOW, &_tse_new_termios) < 0)
		return 2;
	return 0;
}

int tse_raw_input_disable()
{
	if (tcsetattr(0, TCSANOW, &_tse_old_termios) < 0)
		return 1;
	return 0;
}

int tse_getch()
{
	int c = getc(stdin);
	if (c == 27)
	{
		c = getc(stdin);
		if (c == 91)
		{
			c = getc(stdin);
			if (c == 68)
				return TSE_KEY_LEFT;
			else if (c == 67)
				return TSE_KEY_RIGHT;
			else if (c == 65)
				return TSE_KEY_UP;
			else if (c == 66)
				return TSE_KEY_DOWN;
		}
	}
	else
		return c;
}

int tse_putch(char c)
{
	putc(c, stdout);
	tse_flush();
	return 0;
}

int tse_puts(char *s)
{
	printf("%s", s);
	tse_flush();
	return 0;
}

int tse_flush()
{
	fflush(stdout);
	return 0;
}

int tse_moverel_left(int n)
{
	printf("\033[%iD", n);
	tse_flush();
	return 0;
}

int tse_moverel_right(int n)
{
	printf("\033[%iC", n);
	tse_flush();
	return 0;
}

int tse_moverel_up(int n)
{
	printf("\033[%iA", n);
	tse_flush();
	return 0;
}

int tse_moverel_down(int n)
{
	printf("\033[%iB", n);
	tse_flush();
	return 0;
}

int tse_moveabs(int x, int y)
{
	printf("\033[%i;%iH", y, x);
	tse_flush();
	return 0;
}

int tse_clear()
{
	tse_puts("\033[2J");
	tse_flush();
	tse_moveabs(0, 0);
	return 0;
}

int tse_clear_line()
{
	tse_puts("\033[2K");
	tse_flush();
	return 0;
}

int tse_clear_scrollbuffer()
{
	tse_puts("\033[3J");
	tse_flush();
	tse_moveabs(0, 0);
	return 0;
}

int tse_attr_reset()
{
	tse_puts("\033[0m");
	tse_flush();
	return 0;
}

int tse_attr_bold()
{
	tse_puts("\033[1m");
	tse_flush();
	return 0;
}

int tse_attr_dimmed()
{
	tse_puts("\033[2m");
	tse_flush();
	return 0;
}

int tse_attr_underline()
{
	tse_puts("\033[4m");
	tse_flush();
	return 0;
}

int tse_attr_blinking()
{
	tse_puts("\033[5m");
	tse_flush();
	return 0;
}

int tse_attr_inverted()
{
	tse_puts("\033[7m");
	tse_flush();
	return 0;
}

int tse_attr_fg(int col)
{
	printf("\033[%im", 30+col);
	tse_flush();
	return 0;
}

int tse_attr_bg(int col)
{
	printf("\033[%im", 40+col);
	tse_flush();
	return 0;
}

int tse_termsize(int *x, int *y)
{
	if (x == NULL && y == NULL)
		return 1;

	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	if (x != NULL)
		*x = (int)w.ws_col;
	if (y != NULL)
		*y = (int)w.ws_row;
	return 0;
}

FILE *open_memstream(char **ptr, size_t *sizeloc);

FILE *tse_memstream_new(char **p, size_t *s)
{
	return open_memstream(p, s);
}

void tse_memstream_del(FILE *f)
{
	fclose(f);
}

char *tse_memstream_pull(FILE *f, char **p, size_t *s)
{
	if (f == NULL)
		return NULL;

	char *buf = (char *)malloc(ftell(f));
	memset(buf, 0, ftell(f));
	fread(buf, 1, ftell(f), f);

	if (*p != NULL)
	{
		tse_memstream_del(f);
		f = tse_memstream_new(p, s);
	}

	return buf;
}

int tse_fork()
{
	int pid = fork();
	if (pid == 0)
		return TSE_FORKED;
	else
		return pid;
}

int tse_merge(int pid)
{
	int e = 0;
	waitpid(pid, &e, 0);
	return e;
}

int tse_old_stdin;

int tse_stdout_to_stdin()
{
	tse_old_stdin = dup(1);
	dup2(tse_old_stdin, STDIN_FILENO);
	dup2(STDIN_FILENO, STDOUT_FILENO);
	return 0;
}

int tse_restore_stdin()
{
	dup2(STDIN_FILENO, tse_old_stdin);
	close(tse_old_stdin);
	return 0;
}

