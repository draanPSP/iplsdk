#ifndef IPLSDK_KIRK
#define IPLSDK_KIRK

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

	enum KirkCmd {
		CMD1 = 1
	};

    std::int32_t sdkKirkCmd(void *dst, std::uint32_t dsize, void *src, std::uint32_t ssize, KirkCmd cmd);

#ifdef __cplusplus
}
#endif



#endif //IPLSDK_KIRK