#include <ctrl.h>
#include <syscon.h>

//All button bits (see ctrl.prx in uOFW)
constexpr inline std::uint32_t CTRL_SYSCON_KERNEL_MODE_BUTTONS_NEGATIVE_LOGIC = 0x20F7F3F9;

std::int32_t iplReadBufferPositive(SceCtrlData *pad_data) {
	std::uint8_t tx[0x10], rx[0x10];

	tx[TX_CMD] = static_cast<std::uint8_t>(SysconCmd::GET_KERNEL_DIGITAL_KEY);
	tx[TX_LEN] = 2;

    auto const ret = sdkSysconTransmitReceive(tx, rx);

    pad_data->Buttons = (((rx[RX_DATA(3)] & 3) << 28) 
	    | ((rx[RX_DATA(2)] & 0xBF) << 20)
	    | ((rx[RX_DATA(1)] & 0xF0) << 12)
	    | ((rx[RX_DATA(0)] & 0xF0) << 8)
	    | ((rx[RX_DATA(1)] & 6) << 7)
	    | ((rx[RX_DATA(0)] & 0xF) << 4)
	    | (rx[RX_DATA(1)] & 9))
        ^ CTRL_SYSCON_KERNEL_MODE_BUTTONS_NEGATIVE_LOGIC;
    
    return ret;
}