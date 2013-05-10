#include "../os/os.h"

int main()
{
	kti_attr_reset();
	kti_attr_bold();
	printf("This is bold\n");
	kti_attr_reset();
	kti_attr_bold();
	printf("This is dim\n");
	kti_attr_reset();
	kti_attr_underline();
	printf("This is underlined\n");
	kti_attr_reset();
	kti_attr_blinking();
	printf("This is blinking\n");
	kti_attr_reset();
	kti_attr_inverted();
	printf("This is inverted\n");
	kti_attr_reset();
	kti_attr_fg(KTI_COLOR_CYAN);
	kti_attr_bg(KTI_COLOR_RED);
	printf("This is colorful");
	kti_attr_reset();
	kti_putch(KTI_NEWLINE);
	return 0;
}

