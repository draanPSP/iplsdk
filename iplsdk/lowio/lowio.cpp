#include <lowio.h>

void sdkWait(std::uint32_t usec) {
	std::uint32_t dummy = 0;

	while (usec-- > 0) {
		for (std::uint32_t i = 0; i < 10; i++) {
			dummy ^= memoryK1(REG_GPIO_OUTPUT);
		}
	}
}