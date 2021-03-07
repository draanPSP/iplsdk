#ifndef IPLSDK_LIBC
#define IPLSDK_LIBC

#include <psptypes.h>

inline void *sdk_memcpy(void *dest, void const *src, u32 len) {
	auto d = reinterpret_cast<u8 *>(dest);
	auto s = reinterpret_cast<const u8 *>(src);
	while (len-- > 0) {
		*d++ = *s++;
	}
	return dest;
}

#endif //IPLSDK_LIBC