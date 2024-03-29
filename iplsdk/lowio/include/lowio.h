#ifndef IPLSDK_LOWIO
#define IPLSDK_LOWIO

#include <psptypes.h>
#include <hw.h>

enum class Reset : u8 {
	TOP = 0,
	SC,
	ME,
	AW,
	VME,
	AVC,
	USB,
	ATA,
	MSIF0,
	MSIF1,
	KIRK
};

enum class ResetMs : u8 {
	IF0 = static_cast<u8>(Reset::MSIF0),
	IF1
};

constexpr inline auto iplSysregMsifResetEnable = regEnableBit<REG_RESET_ENABLE, ResetMs>;
constexpr inline auto iplSysregMsifResetDisable = regDisableBit<REG_RESET_ENABLE, ResetMs>;

enum class BusClk : u8 {
	ME = 0,
	AWA,
	AWB,
	EDRAM,
	DMACPLUS,
	DMAC0,
	DMAC1,
	KIRK,
	ATA,
	USB,
	MSIF0,
	MSIF1,
	EMCDDR,
	EMCSM,
	APB,
	AUDIO0,
	AUDIO1
};

enum class BusClkMs : u8 {
	IF0 = static_cast<u8>(BusClk::MSIF0),
	IF1
};

constexpr inline auto iplSysregMsifBusClockEnable = regEnableBit<REG_BUS_CLK_ENABLE, BusClkMs>;
constexpr inline auto iplSysregMsifBusClockDisable = regDisableBit<REG_BUS_CLK_ENABLE, BusClkMs>;

inline void iplSysregApbBusClockEnable() {
	return regEnableBit<REG_BUS_CLK_ENABLE>(static_cast<u8>(BusClk::APB));
}

inline void iplSysregApbBusClockDisable() {
	return regDisableBit<REG_BUS_CLK_ENABLE>(static_cast<u8>(BusClk::APB));
}

inline void iplSysregEmcsmBusClockEnable() {
	return regEnableBit<REG_BUS_CLK_ENABLE>(static_cast<u8>(BusClk::EMCSM));
}

inline void iplSysregEmcsmBusClockDisable() {
	return regDisableBit<REG_BUS_CLK_ENABLE>(static_cast<u8>(BusClk::EMCSM));
}

inline void iplSysregKirkBusClockEnable() {
	return regEnableBit<REG_BUS_CLK_ENABLE>(static_cast<u8>(BusClk::KIRK));
}

inline void iplSysregKirkBusClockDisable() {
	return regDisableBit<REG_BUS_CLK_ENABLE>(static_cast<u8>(BusClk::KIRK));
}

enum class Clk1 : u8 {
	ATA = 0,
	USB = 4,
	MSIF0 = 8,
	MSIF1
};

enum class ClkMs : u8 {
	IF0 = static_cast<u8>(Clk1::MSIF0),
	IF1
};

constexpr inline auto iplSysregMsifClkEnable = regEnableBit<REG_CLK1_ENABLE, ClkMs>;
constexpr inline auto iplSysregMsifClkDisable = regDisableBit<REG_CLK1_ENABLE, ClkMs>;

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
	UART0 = static_cast<u8>(Clk2::UART0),
	UART1,
	UART2,
	DBG_UART4,	//available as pads on the motherboard
	HP_REMOTE,
	IRDA
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

enum class IoMs : u8 {
	IF0 = static_cast<u8>(Io::MSIF0),
	IF1
};

enum class IoUart : u8 {
	UART0 = static_cast<u8>(Io::UART0),
	UART1,
	UART2,
	DBG_UART4,
	HP_REMOTE,
	IRDA
};

enum class IoSpi : u8 {
	SYSCON = static_cast<u8>(Io::SPI0),
	SPI1,
	SPI2,
	SPI3,
	SPI4,
	SPI5
};

constexpr inline auto iplSysregSpiClkEnable = regEnableBit<REG_CLK2_ENABLE, ClkSpi>;
constexpr inline auto iplSysregSpiClkDisable = regDisableBit<REG_CLK2_ENABLE, ClkSpi>;

constexpr inline auto iplSysregUartClkEnable = regEnableBit<REG_CLK2_ENABLE, ClkUart>;
constexpr inline auto iplSysregUartClkDisable = regDisableBit<REG_CLK2_ENABLE, ClkUart>;

constexpr inline auto iplSysregMsifIoEnable = regEnableBit<REG_IO_ENABLE, IoMs>;
constexpr inline auto iplSysregMsifIoDisable = regDisableBit<REG_IO_ENABLE, IoMs>;

constexpr inline auto iplSysregSpiIoEnable = regEnableBit<REG_IO_ENABLE, IoSpi>;
constexpr inline auto iplSysregSpiIoDisable = regDisableBit<REG_IO_ENABLE, IoSpi>;

constexpr inline auto iplSysregUartIoEnable = regEnableBit<REG_IO_ENABLE, IoUart>;
constexpr inline auto iplSysregUartIoDisable = regDisableBit<REG_IO_ENABLE, IoUart>;

inline void iplSysregEmcsmIoEnable() {
	return regEnableBit<REG_IO_ENABLE>(static_cast<u8>(Io::EMCSM));
}
inline void iplSysregEmcsmIoDisable() {
	return regDisableBit<REG_IO_ENABLE>(static_cast<u8>(Io::EMCSM));
}

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
	auto const refMask = static_cast<u8>(ClkRef::REF7);
	auto const clkBits = static_cast<u8>(clk) << 2;

	memoryK1(REG_CLK_REF) = memoryK1(REG_CLK_REF) & ~(refMask << clkBits) | (static_cast<u8>(ref) << clkBits);
}

inline void iplSysregGpioEnable() {
	return regEnableBit<REG_CLK2_ENABLE>(static_cast<u8>(Clk2::GPIO));
}

inline void iplSysregGpioDisable() {
	return regDisableBit<REG_CLK2_ENABLE>(static_cast<u8>(Clk2::GPIO));
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

constexpr inline auto iplSysregGpioIoEnable = regEnableBit<REG_GPIO_ENABLE, GpioPort>;
constexpr inline auto iplSysregGpioIoDisable = regDisableBit<REG_GPIO_ENABLE, GpioPort>;

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

	if (mode == GpioPortMode::NONE) {
		iplSysregGpioIoDisable(port);
	} else {
		iplSysregGpioIoEnable(port);
	}
}

constexpr inline auto iplGpioPortSet = regSetBit<REG_GPIO_SET, GpioPort>;
constexpr inline auto iplGpioPortClear = regSetBit<REG_GPIO_CLEAR, GpioPort>;
constexpr inline auto iplGpioAcquireIntr = regSetBit<REG_GPIO_INTR_ACK, GpioPort>;

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

	if (detectLevel) {
		regEnableBit<REG_GPIO_INTR_LEVEL_DETECT>(port);
	} else {
		regDisableBit<REG_GPIO_INTR_LEVEL_DETECT>(port);
	}

	if (levelHi) {
		regEnableBit<REG_GPIO_INTR_LEVEL_HI>(port);
	} else {
		regDisableBit<REG_GPIO_INTR_LEVEL_HI>(port);
	}

	if (levelLo) {
		regEnableBit<REG_GPIO_INTR_LEVEL_LO>(port);
	} else {
		regDisableBit<REG_GPIO_INTR_LEVEL_LO>(port);
	}

	iplGpioAcquireIntr(port);
}

inline u32 iplSysregGetTachyonVersion() {
	u32 const ver = memoryK1(REG_CHIP_VERSION);
	
	if (ver & 0xFF000000) {
		return (ver >> 8);
	} else {
		return 0x100000;
	}
}

//When the ROM is still mapped, the timestamp is available 
//directly from the end of the binary
inline u32 sdkGetBootromTimestampFromRom() {
	return memoryK1(0x1FC00FFC);
}

//Official IPL saves the timestamp to the COP0 register,
//from where it can be retrieved after ROM access is lost
inline u32 sdkGetBootromTimestampFromReg() {
	u32 tstamp;
	
	asm ("cfc0 %0, $17\n" : "=r" (tstamp));
	
	return tstamp;
}

void sdkWait(u32 usec);

constexpr inline u32 sdkKernelGetModel(u32 const tachyonVer, u32 const baryonVer) {
	if (tachyonVer <= 0x400000) {
		return 0; //PSP Fat
	} else if (tachyonVer == 0x500000) {
		return 1; //PSP Slim
	} else if (tachyonVer == 0x600000 && baryonVer == 0x243000) {
		return 1; //PSP Slim
	} else if (tachyonVer <= 0x600000) {
		return 2; //PSP Brite
	} else if (tachyonVer == 0x810000 && baryonVer == 0x2C4000) {
		return 3; //PSP Brite
	} else if (tachyonVer <= 0x800000) {
		return 4; //PSP Go
	} else if (tachyonVer == 0x810000 && baryonVer == 0x2E4000) {
		return 6; //PSP Brite
	} else if (tachyonVer == 0x820000 && baryonVer == 0x2E4000) {
		return 8; //PSP Brite
	} else {
		return 10; //PSP Street
	}
}

#endif //IPLSDK_LOWIO
