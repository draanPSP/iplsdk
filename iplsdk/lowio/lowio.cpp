#include <lowio.h>

void sdkWait(u32 usec) {
	vu32 dummy = 0;

	while (usec-- > 0) {
		for (u32 i = 0; i < 10; i++) {
			dummy ^= memoryK1(REG_GPIO_OUTPUT);
		}
	}
}