#include "../os/os.h"

int main()
{
	printf("Enter a character: ");
	tse_raw_input_enable();
	printf("keycode: %i\n", tse_getch());
	tse_raw_input_disable();

	return 0;
}
