#include <cache.h>

namespace {
	inline u32 _IcacheSize() {
		s32 ret;

		asm("mfc0 %0, $16\n ext %0, 9, 3\n" : "=r" (ret));

		return ret;
	}

	inline u32 _DcacheSize() {
		s32 ret;

		asm("mfc0 %0, $16\n ext %0, 6, 3\n" : "=r" (ret));

		return ret;
	}

	inline void _storeTag(u32 hi, u32 lo) {
		asm("mtc0 %0, $28\n mtc0 %1, $29\n" ::"r" (lo), "r" (hi));
	}

	inline void sync() {
		asm("sync\n");
	}
}

void iplKernelDcacheWritebackInvalidateAll() {
	u32 cache_size = 0x800 << _DcacheSize();

	for (u32 i = 0; i != cache_size; i += 0x40) {
		__builtin_allegrex_cache(0x14, i);
		__builtin_allegrex_cache(0x14, i);
	}

	sync();
}

void iplKernelIcacheInvalidateAll() {
	u32 cache_size = 0x1000 << _IcacheSize();

	_storeTag(0, 0);

	for (u32 i = 0; i != cache_size; i += 0x40) {
		__builtin_allegrex_cache(0x1, i);
		__builtin_allegrex_cache(0x3, i);
	}
}