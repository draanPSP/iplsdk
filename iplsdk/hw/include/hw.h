#ifndef IPLSDK_HW
#define IPLSDK_HW

#include <psptypes.h>

enum class MemRegions : u32 {
	KU0 = 0x00000000,   //user cached
	KU1 = 0x40000000,   //user uncached
	K0 = 0x80000000,   //kernel cached
	K1 = 0xA0000000,   //kernel uncached
	K2 = 0xC0000000,   //kernel cached
	K3 = 0xE0000000    //kernel cached
};

constexpr inline u32 REG_RESET_ENABLE = 0x1C10004C;
constexpr inline u32 REG_BUS_CLK_ENABLE = 0x1C100050;
constexpr inline u32 REG_CLK1_ENABLE = 0x1C100054;
constexpr inline u32 REG_CLK2_ENABLE = 0x1C100058;

constexpr inline u32 REG_CLK_REF = 0x1C100064;

constexpr inline u32 REG_IO_ENABLE = 0x1C100078;
constexpr inline u32 REG_GPIO_ENABLE = 0x1C10007C;

constexpr inline u32 REG_MS_CMD = 0x1D200030;
constexpr inline u32 REG_MS_DATA = 0x1D200034;
constexpr inline u32 REG_MS_STATUS = 0x1D200038;
constexpr inline u32 REG_MS_SYS = 0x1D20003C;

constexpr inline u32 REG_GPIO_OUTPUT = 0x1E240000;
constexpr inline u32 REG_GPIO_VALUE = 0x1E240004;
constexpr inline u32 REG_GPIO_SET = 0x1E240008;
constexpr inline u32 REG_GPIO_CLEAR = 0x1E24000C;
constexpr inline u32 REG_GPIO_INTR_LEVEL_DETECT = 0x1E240010;
constexpr inline u32 REG_GPIO_INTR_LEVEL_HI = 0x1E240014;
constexpr inline u32 REG_GPIO_INTR_LEVEL_LO = 0x1E240018;
constexpr inline u32 REG_GPIO_INTR_VALUE = 0x1E240020;
constexpr inline u32 REG_GPIO_INTR_ACK = 0x1E240024;
constexpr inline u32 REG_GPIO_INPUT = 0x1E240040;

constexpr inline u32 UART_IO_BASE = 0x1E400000;

constexpr inline u32 DBG_UART4_BASE = UART_IO_BASE + 3*0x40000;
constexpr inline u32 HP_REMOTE_BASE = UART_IO_BASE + 4*0x40000;
constexpr inline u32 IRDA_BASE = UART_IO_BASE + 5*0x40000;


constexpr u32 physicalAddr(u32 const loc) {
	return loc & 0x1FFFFFFF;
}

template<u32 BA>
inline vu32 &memoryRef(u32 const loc) {
	return *reinterpret_cast<u32 *>(BA | physicalAddr(loc));
}

constexpr inline auto memoryK1 = memoryRef<static_cast<u32>(MemRegions::K1)>;

template<u32 R, typename T>
inline void regEnableBit(T const bit) {
	memoryK1(R) |= (1 << static_cast<u8>(bit));
}

template<u32 R, typename T>
inline void regDisableBit(T const bit) {
	memoryK1(R) &= ~(1 << static_cast<u8>(bit));
}

template<u32 R, typename T>
inline void regSetBit(T const bit) {
	memoryK1(R) = (1 << static_cast<u8>(bit));
}

#endif //IPLSDK_HW