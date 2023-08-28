#include <cstdint>
#include <cache.h>

namespace {
	inline std::uint32_t _IcacheSize() {
		std::uint32_t ret;

		asm("mfc0 %0, $16\n ext %0, 9, 3\n" : "=r" (ret));

		return ret;
	}

	inline std::uint32_t _DcacheSize() {
		std::uint32_t ret;

		asm("mfc0 %0, $16\n ext %0, 6, 3\n" : "=r" (ret));

		return ret;
	}

	inline void _storeTag(std::uint32_t hi, std::uint32_t lo) {
		asm("mtc0 %0, $28\n mtc0 %1, $29\n" :: "r" (lo), "r" (hi));
	}
}

void iplKernelDcacheWritebackInvalidateAll() {
	std::uint32_t cache_size = 0x800 << _DcacheSize();

	for (std::uint32_t i = 0; i != cache_size; i += 0x40) {
		__builtin_allegrex_cache(0x14, i);
		__builtin_allegrex_cache(0x14, i);
	}

	sdkSync();
}

void iplKernelIcacheInvalidateAll() {
	std::uint32_t cache_size = 0x1000 << _IcacheSize();

	_storeTag(0, 0);

	for (std::uint32_t i = 0; i != cache_size; i += 0x40) {
		__builtin_allegrex_cache(0x1, i);
		__builtin_allegrex_cache(0x3, i);
	}
}