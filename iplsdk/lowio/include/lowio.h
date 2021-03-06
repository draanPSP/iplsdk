#ifndef IPLSDK_LOWIO
#define IPLSDK_LOWIO

#include <psptypes.h>
#include <hw.h>

enum class Clk2 : u8 {
	SPI0 = 0,
	SPI1,
	SPI2,
	SPI3,
	SPI4,
	SPI5,
	UART0,
	UART1,
	UART2,
	UART3,
	UART4,
	UART5,
	APB0,
	APB1,
	APB2,
	APB3,
	AUDIO0,
	AUDIO1,
	LCDC,
	PWM,
	KEY,
	IIC,
	SIRCS,
	GPIO,
	UNK
};

enum class ClkSpi : u8 {
	SYSCON = static_cast<u8>(Clk2::SPI0),
	SPI1,
	SPI2,
	SPI3,
	SPI4,
	SPI5
};

enum class ClkUart : u8 {
	UARTO = static_cast<u8>(Clk2::UART0),
	UART1,
	UART2,
	UART3,
	HP_REMOTE,
	UART5
};

enum class Io : u8 {
	EMCSM = 1,
	USB,
	ATA,
	MSIF0,
	MSIF1,
	LCDC,
	AUDIO0,
	AUDIO1,
	IIC,
	SIRCS,
	UNK,
	KEY,
	PWM,
	UART0 = 16,
	UART1,
	UART2,
	UART3,
	UART4,
	UART5,
	SPI0 = 24,
	SPI1,
	SPI2,
	SPI3,
	SPI4,
	SPI5
};

enum class IoUart : u8 {
	UART0 = static_cast<u8>(Io::UART0),
	UART1,
	UART2,
	UART3,
	HP_REMOTE,
	UART5
};

enum class IoSpi : u8 {
	SYSCON = static_cast<u8>(Io::SPI0),
	SPI1,
	SPI2,
	SPI3,
	SPI4,
	SPI5
};

const auto iplSysregSpiClkEnable = regEnableBit<REG_CLK_ENABLE, ClkSpi>;
const auto iplSysregSpiClkDisable = regDisableBit<REG_CLK_ENABLE, ClkSpi>;

const auto iplSysregUartClkEnable = regEnableBit<REG_CLK_ENABLE, ClkUart>;
const auto iplSysregUartClkDisable = regDisableBit<REG_CLK_ENABLE, ClkUart>;

const auto iplSysregSpiIoEnable = regEnableBit<REG_IO_ENABLE, IoSpi>;
const auto iplSysregSpiIoDisable = regDisableBit<REG_IO_ENABLE, IoSpi>;

const auto iplSysregUartIoEnable = regEnableBit<REG_IO_ENABLE, IoUart>;
const auto iplSysregUartIoDisable = regDisableBit<REG_IO_ENABLE, IoUart>;

enum class ClkRef : u8 {
	REF0,
	REF1,
	REF2,
	REF3,
	REF4,
	REF5,
	REF6,
	REF7,
};

inline void iplSysregSpiClkSelect(ClkSpi const clk, ClkRef const ref) {
	const auto refMask = static_cast<u8>(ClkRef::REF7);
	const auto clkBits = static_cast<u8>(clk) << 2;

	memoryK1(REG_CLK_REF) = memoryK1(REG_CLK_REF) & ~(refMask << clkBits) | (static_cast<u8>(ref) << clkBits);
}

inline void iplSysregGpioEnable() {
	return regEnableBit<REG_CLK_ENABLE>(static_cast<u8>(Clk2::GPIO));
}

inline void iplSysregGpioDisable() {
	return regDisableBit<REG_CLK_ENABLE>(static_cast<u8>(Clk2::GPIO));
}

enum class GpioPort : u8 {
	LCD_RESET = 0,
	DEBUG_1,
	DEBUG_2,
	SYSCON_REQUEST,
	TACHYON_SPI_CS,
	HEADPHONE_EN,
	MS_LED,
	WLAN_LED,
	DEBUG_8,
	LEPTON_STATUS,
	LEPTON_WAKE,
	UMD_STATUS
};

enum class GpioPortMode : u8 {
	OUTPUT = 0,
	INPUT,
	NONE        //high impedance?
};

enum class GpioIntrMode : u8 {
	HI,
	LO,
	RISING_EDGE,
	FALLING_EDGE,
	EDGE,
};

const auto iplSysregGpioIoEnable = regEnableBit<REG_GPIO_ENABLE, GpioPort>;
const auto iplSysregGpioIoDisable = regDisableBit<REG_GPIO_ENABLE, GpioPort>;

inline void iplGpioSetPortMode(GpioPort const port, GpioPortMode const mode) {
	switch (mode) {
		case GpioPortMode::OUTPUT:
			regEnableBit<REG_GPIO_OUTPUT>(port);
			regDisableBit<REG_GPIO_INPUT>(port);
			break;
		case GpioPortMode::INPUT:
			regDisableBit<REG_GPIO_OUTPUT>(port);
			regEnableBit<REG_GPIO_INPUT>(port);
			break;
		case GpioPortMode::NONE:
			regDisableBit<REG_GPIO_OUTPUT>(port);
			regDisableBit<REG_GPIO_INPUT>(port);
			break;
	}

	if (mode == GpioPortMode::NONE)
		iplSysregGpioIoDisable(port);
	else
		iplSysregGpioIoEnable(port);
}

const auto iplGpioPortSet = regSetBit<REG_GPIO_SET, GpioPort>;
const auto iplGpioPortClear = regSetBit<REG_GPIO_CLEAR, GpioPort>;
const auto iplGpioAcquireIntr = regSetBit<REG_GPIO_INTR_ACK, GpioPort>;

inline bool iplGpioQueryIntr(GpioPort const port) {
	const u32 mask = (1 << static_cast<u8>(port));

	return (memoryK1(REG_GPIO_INTR_VALUE) & mask) == mask;
}

inline u32 iplGpioPortRead() {
	return memoryK1(REG_GPIO_VALUE);
}

inline void iplGpioSetIntrMode(GpioPort const port, GpioIntrMode const mode) {
	bool detectLevel, levelHi, levelLo;

	switch (mode) {
		case GpioIntrMode::HI:
			detectLevel = true;
			levelHi = true;
			levelLo = false;
			break;
		case GpioIntrMode::LO:
			detectLevel = true;
			levelHi = false;
			levelLo = true;
			break;
		case GpioIntrMode::RISING_EDGE:
			detectLevel = false;
			levelHi = true;
			levelLo = false;
			break;
		case GpioIntrMode::FALLING_EDGE:
			detectLevel = false;
			levelHi = false;
			levelLo = true;
			break;
		case GpioIntrMode::EDGE:
			detectLevel = false;
			levelHi = true;
			levelLo = true;
			break;
	}

	if (detectLevel)
		regEnableBit<REG_GPIO_INTR_LEVEL_DETECT>(port);
	else
		regDisableBit<REG_GPIO_INTR_LEVEL_DETECT>(port);

	if (levelHi)
		regEnableBit<REG_GPIO_INTR_LEVEL_HI>(port);
	else
		regDisableBit<REG_GPIO_INTR_LEVEL_HI>(port);

	if (levelLo)
		regEnableBit<REG_GPIO_INTR_LEVEL_LO>(port);
	else
		regDisableBit<REG_GPIO_INTR_LEVEL_LO>(port);

	iplGpioAcquireIntr(port);
}

void sdkWait(u32 usec);

#endif //IPLSDK_LOWIO
