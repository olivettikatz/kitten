#include "../os/os.h"

int main()
{
	tse_attr_reset();
	tse_attr_bold();
	printf("This is bold\n");
	tse_attr_reset();
	tse_attr_bold();
	printf("This is dim\n");
	tse_attr_reset();
	tse_attr_underline();
	printf("This is underlined\n");
	tse_attr_reset();
	tse_attr_blinking();
	printf("This is blinking\n");
	tse_attr_reset();
	tse_attr_inverted();
	printf("This is inverted\n");
	tse_attr_reset();
	tse_attr_fg(TSE_COLOR_CYAN);
	tse_attr_bg(TSE_COLOR_RED);
	printf("This is colorful");
	tse_attr_reset();
	tse_putch(TSE_NEWLINE);
	return 0;
}

