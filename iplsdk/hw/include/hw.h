#ifndef IPLSDK_HW
#define IPLSDK_HW

#include <cstdint>

enum class MemRegions : std::uint32_t {
	KU0 = 0x00000000,   //user cached
	KU1 = 0x40000000,   //user uncached
	K0 = 0x80000000,   //kernel cached
	K1 = 0xA0000000,   //kernel uncached
	K2 = 0xC0000000,   //kernel cached
	K3 = 0xE0000000    //kernel cached
};

constexpr inline std::uint32_t REG_CHIP_VERSION = 0x1C100040;

constexpr inline std::uint32_t REG_RESET_ENABLE = 0x1C10004C;
constexpr inline std::uint32_t REG_BUS_CLK_ENABLE = 0x1C100050;
constexpr inline std::uint32_t REG_CLK1_ENABLE = 0x1C100054;
constexpr inline std::uint32_t REG_CLK2_ENABLE = 0x1C100058;

constexpr inline std::uint32_t REG_CLK_REF = 0x1C100064;

constexpr inline std::uint32_t REG_IO_ENABLE = 0x1C100078;
constexpr inline std::uint32_t REG_GPIO_ENABLE = 0x1C10007C;

constexpr inline std::uint32_t REG_EMC_CTRL = 0x1D101000;
constexpr inline std::uint32_t REG_EMC_STATUS = 0x1D101004;
constexpr inline std::uint32_t REG_EMC_CMD = 0x1D101008;
constexpr inline std::uint32_t REG_EMC_ADDR = 0x1D10100C;
constexpr inline std::uint32_t REG_EMC_RST = 0x1D101014;
constexpr inline std::uint32_t REG_EMC_DMA_ADDR = 0x1D101020;
constexpr inline std::uint32_t REG_EMC_DMA_CTRL = 0x1D101024;
constexpr inline std::uint32_t REG_EMC_DMA_STATUS = 0x1D101028;
constexpr inline std::uint32_t REG_EMC_RESUME = 0x1D101200;
constexpr inline std::uint32_t REG_EMC_DATA = 0x1D101300;
constexpr inline std::uint32_t EMC_USER_DATA_BUF = 0x1FF00000;
constexpr inline std::uint32_t REG_EMC_USER_ECC_BUF = 0x1FF00800;
constexpr inline std::uint32_t EMC_SPARE_DATA_BUF = 0x1FF00900;

constexpr inline std::uint32_t REG_MS_CMD = 0x1D200030;
constexpr inline std::uint32_t REG_MS_DATA = 0x1D200034;
constexpr inline std::uint32_t REG_MS_STATUS = 0x1D200038;
constexpr inline std::uint32_t REG_MS_SYS = 0x1D20003C;

constexpr inline std::uint32_t REG_KIRK_SIG = 0x1DE00000;
constexpr inline std::uint32_t REG_KIRK_VERSION = 0x1DE00004;
constexpr inline std::uint32_t REG_KIRK_ERROR = 0x1DE00008;
constexpr inline std::uint32_t REG_KIRK_PROC_PHASE = 0x1DE0000C;
constexpr inline std::uint32_t REG_KIRK_CMD = 0x1DE00010;
constexpr inline std::uint32_t REG_KIRK_RESULT = 0x1DE00014;
constexpr inline std::uint32_t REG_KIRK_PATTERN = 0x1DE0001C;
constexpr inline std::uint32_t REG_KIRK_PATTERN_END = 0x1DE00028;
constexpr inline std::uint32_t REG_KIRK_SRC_ADDR = 0x1DE0002C;
constexpr inline std::uint32_t REG_KIRK_DEST_ADDR = 0x1DE00030;

constexpr inline std::uint32_t REG_GPIO_OUTPUT = 0x1E240000;
constexpr inline std::uint32_t REG_GPIO_VALUE = 0x1E240004;
constexpr inline std::uint32_t REG_GPIO_SET = 0x1E240008;
constexpr inline std::uint32_t REG_GPIO_CLEAR = 0x1E24000C;
constexpr inline std::uint32_t REG_GPIO_INTR_LEVEL_DETECT = 0x1E240010;
constexpr inline std::uint32_t REG_GPIO_INTR_LEVEL_HI = 0x1E240014;
constexpr inline std::uint32_t REG_GPIO_INTR_LEVEL_LO = 0x1E240018;
constexpr inline std::uint32_t REG_GPIO_INTR_VALUE = 0x1E240020;
constexpr inline std::uint32_t REG_GPIO_INTR_ACK = 0x1E240024;
constexpr inline std::uint32_t REG_GPIO_INPUT = 0x1E240040;

constexpr inline std::uint32_t UART_IO_BASE = 0x1E400000;

constexpr inline std::uint32_t DBG_UART4_BASE = UART_IO_BASE + 3*0x40000;
constexpr inline std::uint32_t HP_REMOTE_BASE = UART_IO_BASE + 4*0x40000;
constexpr inline std::uint32_t IRDA_BASE = UART_IO_BASE + 5*0x40000;


constexpr std::uint32_t physicalAddr(std::uint32_t const loc) {
	return loc & 0x1FFFFFFF;
}

template<std::uint32_t BA>
inline std::uint32_t volatile &memoryRef(std::uint32_t const loc) {
	return *reinterpret_cast<std::uint32_t *>(BA | physicalAddr(loc));
}

constexpr inline auto memoryK0 = memoryRef<static_cast<std::uint32_t>(MemRegions::K0)>;
constexpr inline auto memoryK1 = memoryRef<static_cast<std::uint32_t>(MemRegions::K1)>;

template<std::uint32_t R, typename T>
inline void regEnableBit(T const bit) {
	memoryK1(R) |= (1 << static_cast<std::uint8_t>(bit));
}

template<std::uint32_t R, typename T>
inline void regDisableBit(T const bit) {
	memoryK1(R) &= ~(1 << static_cast<std::uint8_t>(bit));
}

template<std::uint32_t R, typename T>
inline void regSetBit(T const bit) {
	memoryK1(R) = (1 << static_cast<std::uint8_t>(bit));
}

#endif //IPLSDK_HW