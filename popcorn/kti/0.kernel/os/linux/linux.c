#include "../os.h"
#include "./linux.h"

struct termios _kti_old_termios;
struct termios _kti_new_termios;

int kti_raw_input_enable()
{
	if (tcgetattr(0, &_kti_old_termios) < 0)
		return 1;
	_kti_new_termios = _kti_old_termios;
	_kti_new_termios.c_lflag &= ~ICANON;
	_kti_new_termios.c_lflag &= ~ECHO;
	if (tcsetattr(0, TCSANOW, &_kti_new_termios) < 0)
		return 2;
	return 0;
}

int kti_raw_input_disable()
{
	if (tcsetattr(0, TCSANOW, &_kti_old_termios) < 0)
		return 1;
	return 0;
}

int kti_getch()
{
	int c = getc(stdin);
	if (c == 27)
	{
		c = getc(stdin);
		if (c == 91)
		{
			c = getc(stdin);
			if (c == 68)
				return KTI_KEY_LEFT;
			else if (c == 67)
				return KTI_KEY_RIGHT;
			else if (c == 65)
				return KTI_KEY_UP;
			else if (c == 66)
				return KTI_KEY_DOWN;
		}
	}
	else
		return c;
}

int kti_putch(char c)
{
	putc(c, stdout);
	kti_flush();
	return 0;
}

int kti_puts(char *s)
{
	printf("%s", s);
	kti_flush();
	return 0;
}

int kti_flush()
{
	fflush(stdout);
	return 0;
}

int kti_moverel_left(int n)
{
	printf("\033[%iD", n);
	kti_flush();
	return 0;
}

int kti_moverel_right(int n)
{
	printf("\033[%iC", n);
	kti_flush();
	return 0;
}

int kti_moverel_up(int n)
{
	printf("\033[%iA", n);
	kti_flush();
	return 0;
}

int kti_moverel_down(int n)
{
	printf("\033[%iB", n);
	kti_flush();
	return 0;
}

int kti_moveabs(int x, int y)
{
	printf("\033[%i;%iH", y, x);
	kti_flush();
	return 0;
}

int kti_clear()
{
	kti_puts("\033[2J");
	kti_flush();
	kti_moveabs(0, 0);
	return 0;
}

int kti_clear_line()
{
	kti_puts("\033[2K");
	kti_flush();
	return 0;
}

int kti_clear_scrollbuffer()
{
	kti_puts("\033[3J");
	kti_flush();
	kti_moveabs(0, 0);
	return 0;
}

int kti_attr_reset()
{
	kti_puts("\033[0m");
	kti_flush();
	return 0;
}

int kti_attr_bold()
{
	kti_puts("\033[1m");
	kti_flush();
	return 0;
}

int kti_attr_dimmed()
{
	kti_puts("\033[2m");
	kti_flush();
	return 0;
}

int kti_attr_underline()
{
	kti_puts("\033[4m");
	kti_flush();
	return 0;
}

int kti_attr_blinking()
{
	kti_puts("\033[5m");
	kti_flush();
	return 0;
}

int kti_attr_inverted()
{
	kti_puts("\033[7m");
	kti_flush();
	return 0;
}

int kti_attr_fg(int col)
{
	printf("\033[%im", 30+col);
	kti_flush();
	return 0;
}

int kti_attr_bg(int col)
{
	printf("\033[%im", 40+col);
	kti_flush();
	return 0;
}

int kti_termsize(int *x, int *y)
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

FILE *kti_memstream_new(char **p, size_t *s)
{
	return open_memstream(p, s);
}

void kti_memstream_del(FILE *f)
{
	fclose(f);
}

int kti_parent_pid = -1;

int kti_fork()
{
	kti_parent_pid = getpid();
	int pid = fork();
	if (pid == 0)
		return KTI_FORKED;
	else
		return pid;
}

int kti_kill_parent()
{
	if (kti_parent_pid == -1)
		_exit(0);
	else
		kill(kti_parent_pid, SIGTERM);
	return 0;
}

int kti_merge(int pid)
{
	int e = 0;
	waitpid(pid, &e, 0);
	return e;
}

int kti_old_stdin;

int kti_stdout_to_stdin()
{
	kti_old_stdin = dup(1);
	dup2(kti_old_stdin, STDIN_FILENO);
	dup2(STDIN_FILENO, STDOUT_FILENO);
	return 0;
}

int kti_restore_stdin()
{
	dup2(STDIN_FILENO, kti_old_stdin);
	close(kti_old_stdin);
	return 0;
}

