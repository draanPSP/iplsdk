#include <lowio.h>
#include <type_traits>

namespace {
	using putcharFuncType = std::add_pointer_t<void(u8)>;

	putcharFuncType g_putcharFunc = nullptr;

	enum class Status : u8 {
		UNK = 0x8,
		RX = 0x10,
		TX = 0x20,
		BUSY = 0x80
	};
}

constexpr inline u32 RX_TX_OFFSET = 0;
constexpr inline u32 STATUS_OFFSET = 0x20;
constexpr inline u32 BPS_HIGH_OFFSET = 0x24;
constexpr inline u32 BPS_LOW_OFFSET = 0x28;
constexpr inline u32 UNK_2C_OFFSET = 0x2C;
constexpr inline u32 UNK_30_OFFSET = 0x30;
constexpr inline u32 FIFO_OFFSET = 0x34;
constexpr inline u32 UNK_44_OFFSET = 0x4;

constexpr inline u32 SPEED = 115200;

template<u32 UB>
inline void uartSetBps(u32 const bps) {
	u32 const val = 96000000 / bps;

	memoryK1(UB + BPS_HIGH_OFFSET) = val >> 6;
	memoryK1(UB + BPS_LOW_OFFSET) = val & 0x3F;
}

constexpr inline auto iplKernelUart4SetBps = uartSetBps<DBG_UART4_BASE>;
constexpr inline auto sdkUartHpRemoteSetBps = uartSetBps<HP_REMOTE_BASE>;
constexpr inline auto sdkUartIrdaSetBps = uartSetBps<IRDA_BASE>;

template<u32 UB>
inline void setFIFO(u32 const fifo) {
	if (fifo != 0) {
		memoryK1(UB + UNK_2C_OFFSET) |= 0x10;
		switch (fifo) {
			case 8:
				memoryK1(UB + FIFO_OFFSET) = 9;
				break;
			case 16:
				memoryK1(UB + FIFO_OFFSET) = 18;
				break;
			case 24:
				memoryK1(UB + FIFO_OFFSET) = 27;
				break;
			case 28:
				memoryK1(UB + FIFO_OFFSET) = 36;
				break;
			default:
				memoryK1(UB + FIFO_OFFSET) = 0;
				break;
		}
	} else {
		memoryK1(UB + FIFO_OFFSET) &= 0xFFFFFFEF;
	}
}

constexpr inline auto uart4SetFIFO = setFIFO<DBG_UART4_BASE>;
constexpr inline auto uartHpRemoteSetFIFO = setFIFO<HP_REMOTE_BASE>;
constexpr inline auto uartIrdaSetFIFO = setFIFO<IRDA_BASE>;

template<u32 UB>
inline void uartSendChar(u8 const c) {
	while ((memoryK1(UB + STATUS_OFFSET) & static_cast<u8>(Status::TX)) != 0);

	memoryK1(UB + RX_TX_OFFSET) = c;
}

constexpr inline auto iplKernelUart4SendChar = uartSendChar<DBG_UART4_BASE>;
constexpr inline auto sdkUartHpRemoteSendChar = uartSendChar<HP_REMOTE_BASE>;
constexpr inline auto sdkUartIrdaSendChar = uartSendChar<IRDA_BASE>;

void iplKernelUart4Init() {
	sceSysregApbBusClockEnable();
	iplSysregUartClkEnable(ClkUart::DBG_UART4);
	iplSysregUartIoEnable(IoUart::DBG_UART4);

	iplKernelUart4SetBps(SPEED);

	memoryK1(DBG_UART4_BASE + UNK_2C_OFFSET) = 0x60;
	memoryK1(DBG_UART4_BASE + UNK_30_OFFSET) |= 0x301;
	uart4SetFIFO(4);
	memoryK1(DBG_UART4_BASE + UNK_44_OFFSET) |= 0x7FF;

	g_putcharFunc = iplKernelUart4SendChar;
}

void sdkUartHpRemoteInit() {
	sceSysregApbBusClockEnable();
	iplSysregUartClkEnable(ClkUart::HP_REMOTE);
	iplSysregUartIoEnable(IoUart::HP_REMOTE);

	memoryK1(HP_REMOTE_BASE + UNK_30_OFFSET) = 0x300;

	sdkUartHpRemoteSetBps(SPEED);
	memoryK1(HP_REMOTE_BASE + UNK_2C_OFFSET) = 0x60;
	uartHpRemoteSetFIFO(4);
	memoryK1(HP_REMOTE_BASE + UNK_30_OFFSET) = 0x301;

	g_putcharFunc = sdkUartHpRemoteSendChar;
}

void sdkUartIrdaInit() {
	sceSysregApbBusClockEnable();
	iplSysregUartClkEnable(ClkUart::IRDA);
	iplSysregUartIoEnable(IoUart::IRDA);

	sdkUartIrdaSetBps(SPEED);

	memoryK1(IRDA_BASE + UNK_2C_OFFSET) = 0x60;
	memoryK1(IRDA_BASE + UNK_30_OFFSET) |= 0x301;
	uartIrdaSetFIFO(4);

	g_putcharFunc = sdkUartIrdaSendChar;
}

template<u32 UB>
inline void uartRecvBytes(u8 *buff, u32 const count) {
	for (u32 i = 0; i < count; i++) {
		while ((memoryK1(UB + STATUS_OFFSET) & static_cast<u8>(Status::RX)) != 0);

		buff[i] = memoryK1(UB + RX_TX_OFFSET);
	}
}

template<u32 UB>
inline u32 uartWaitBusy() {
	do {
		while ((memoryK1(UB + STATUS_OFFSET) & static_cast<u8>(Status::BUSY)) != 0);
	} while ((memoryK1(UB + STATUS_OFFSET) & static_cast<u8>(Status::UNK)) != 0);

	u32 const result = memoryK1(UB + FIFO_OFFSET);

	return result;
}

u32 iplKernelUart4Suspend() {
	u32 const result = uartWaitBusy<DBG_UART4_BASE>();

	iplSysregUartIoDisable(IoUart::DBG_UART4);

	return result;
}

u32 sdkKernelUartHpRemoteSuspend() {
	u32 const result = uartWaitBusy<HP_REMOTE_BASE>();

	iplSysregUartIoDisable(IoUart::HP_REMOTE);

	return result;
}

u32 sdkKernelUartIrdaSuspend() {
	u32 const result = uartWaitBusy<IRDA_BASE>();

	iplSysregUartIoDisable(IoUart::IRDA);

	return result;
}

void iplKernelUart4End() {
	iplSysregUartIoDisable(IoUart::DBG_UART4);
	iplSysregUartClkDisable(ClkUart::DBG_UART4);

	g_putcharFunc = nullptr;
}

void sdkKernelUartHpRemoteEnd() {
	iplSysregUartIoDisable(IoUart::HP_REMOTE);
	iplSysregUartClkDisable(ClkUart::HP_REMOTE);

	g_putcharFunc = nullptr;
}

void sdkKernelUartIrdaEnd() {
	iplSysregUartIoDisable(IoUart::IRDA);
	iplSysregUartClkDisable(ClkUart::IRDA);

	g_putcharFunc = nullptr;
}

extern "C" {
void _putchar(char c) {
	if (g_putcharFunc) {
		g_putcharFunc(c);
	}
}
}
