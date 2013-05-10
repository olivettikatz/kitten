#include "../os/os.h"

int main()
{
	printf("Enter a character: ");
	kti_raw_input_enable();
	printf("keycode: %i\n", kti_getch());
	kti_raw_input_disable();

	return 0;
}
