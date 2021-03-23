#ifndef IPLSDK_PREIPL
#define IPLSDK_PREIPL

#include <psptypes.h>
#include <type_traits>

using v_v_function_t = std::add_pointer_t<void()>;
using i_v_function_t = std::add_pointer_t<s32()>;
using i_ivp_function_t = std::add_pointer_t<s32(s32,void*)>;
using i_ivpvp_function_t = std::add_pointer_t<s32(s32,void*,void*)>;
using i_vpvpi_function_t = std::add_pointer_t<s32(void*,void*,s32)>;

inline auto const preIplEntry = reinterpret_cast<i_v_function_t const>(0x80010000);

constexpr inline u32 TACHYON_0x140000_BOOTROM_TIMESTAMP = 0x20040420;
constexpr inline u32 TACHYON_0x400000_BOOTROM_TIMESTAMP = 0x20050104;
constexpr inline u32 TACHYON_0x600000_BOOTROM_TIMESTAMP = 0x20070910;

//Tachyon 0x00140000-0x00500000
inline auto const oldPreIplMsInit = reinterpret_cast<i_v_function_t const>(0x80010240);
inline auto const oldPreIplMsReadIplBlock = reinterpret_cast<i_ivp_function_t const>(0x80010248);
inline auto const oldPreIplMsReadSector = reinterpret_cast<i_ivp_function_t const>(0x80010418);

inline auto const oldPreIplNandInit = reinterpret_cast<i_v_function_t const>(0x80010130);
inline auto const oldPreIplNandReadIplBlock = reinterpret_cast<i_ivpvp_function_t const>(0x80010194);
inline auto const oldPreIplNandReadSector = reinterpret_cast<i_ivpvp_function_t const>(0x80010334);

inline auto const oldPreIplMemcpyChksum = reinterpret_cast<i_ivpvp_function_t const>(0x80010688);

inline auto const oldPreIplDcacheWritebackInvalidateAll = reinterpret_cast<v_v_function_t const>(0x800102D8);
inline auto const oldPreIplIcacheInvalidateAll = reinterpret_cast<v_v_function_t const>(0x800102A0);

//Tachyon 0x00600000-0x00900000
inline auto const newPreIplMsInit = reinterpret_cast<i_v_function_t const>(0x8001030C);
inline auto const newPreIplMsReadIplBlock = reinterpret_cast<i_ivp_function_t const>(0x80010314);
inline auto const newPreIplMsReadSector = reinterpret_cast<i_ivp_function_t const>(0x800104E8);

inline auto const newPreIplNandInit = reinterpret_cast<i_v_function_t const>(0x800101F8);
inline auto const newPreIplNandReadIplBlock = reinterpret_cast<i_ivpvp_function_t const>(0x8001025C);
inline auto const newPreIplNandReadSector = reinterpret_cast<i_ivpvp_function_t const>(0x80010404);

inline auto const newPreIplMemcpyChksum = reinterpret_cast<i_ivpvp_function_t const>(0x8001095C);

inline auto const newPreIplDcacheWritebackInvalidateAll = reinterpret_cast<v_v_function_t const>(0x800103A8);
inline auto const newPreIplIcacheInvalidateAll = reinterpret_cast<v_v_function_t const>(0x80010370);

#endif //IPLSDK_PREIPL
