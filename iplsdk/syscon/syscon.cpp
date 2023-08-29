#include <syscon.h>
#include <lowio.h>
#include <cstring>

std::uint32_t g_baryonVersion = 0;

namespace {
	std::int32_t _iplSysconPacketStart(std::uint8_t const *tx) {
		std::uint32_t volatile dummy;

		dummy = iplGpioPortRead();
		iplGpioPortClear(GpioPort::SYSCON_REQUEST);

		while ((memoryK1(0x1E58000C) & 0x4) != 0) {
			dummy = memoryK1(0x1E580008);
		}

		dummy = memoryK1(0x1E58000C);
		memoryK1(0x1E580020) = 3;

		for (std::uint32_t i = 0; i < tx[TX_LEN] + 1; i += 2) {
			dummy = memoryK1(0x1E58000C);
			memoryK1(0x1E580008) = (tx[i] << 8) | tx[i + 1];
		}

		memoryK1(0x1E580004) = 6;
		iplGpioPortSet(GpioPort::SYSCON_REQUEST);

		return 0;
	}

	void _sdkSysconWaitForResponse() {
		while (!iplGpioQueryIntr(GpioPort::TACHYON_SPI_CS));

		iplGpioAcquireIntr(GpioPort::TACHYON_SPI_CS);
	}

	std::int32_t _iplSysconPacketEnd(std::uint8_t *rx) {
		std::int32_t ret = 0;

		if ((memoryK1(0x1E58000C) & 4) == 0) {
			rx[RX_STATUS] = -1;
			rx[RX_LEN] = 0;

			for (std::uint32_t i = 0; i < 16; i++) {
				asm("\n");
			}

			ret = -1;
		}

		if ((memoryK1(0x1E58000C) & 1) == 0) {
			ret = -1;
		}

		if ((memoryK1(0x1E580018) & 1) != 0) {
			memoryK1(0x1E580020) = 1;
		}

		for (std::uint32_t i = 0; i < 16; i += 2) {
			if ((memoryK1(0x1E58000C) & 4) == 0) {
				break;
			}

			std::uint16_t v = memoryK1(0x1E580008) & 0xFFFF;
			if (i == 0) {
				ret = v >> 8;
			}

			rx[i + 0] = v >> 8;
			rx[i + 1] = v;

		}

		memoryK1(0x1E580004) = 4;
		iplGpioPortClear(GpioPort::SYSCON_REQUEST);

		if (ret >= 0) {
			std::uint32_t hash = 0;
			if (rx[RX_LEN] < 3) { /* Received data is too short */
				ret = -2;
			} else if (rx[RX_LEN] < 16) {
				for (std::uint32_t i = 0; i < rx[RX_LEN]; i++) {
					hash = (hash + rx[i]) & 0xFF;
				}
				if ((rx[rx[RX_LEN]] ^ (~hash & 0xFF)) != 0) { /* Wrong hash */
					ret = -2;
				}
			} else {
				ret = -2;
			}
		}

		if (ret >= 0) {
			switch (rx[RX_RESPONSE]) {
				case 0x80:
				case 0x81:
					return rx[RX_RESPONSE];
			}
		}

		return ret;
	}

	void _iplSysconGetBaryonVersion(std::uint32_t *baryonVersionPtr) {
		_iplSysconCommonRead(baryonVersionPtr, SysconCmd::GET_BARYON);
	}
}

std::uint32_t iplSysconGetBaryonVersion() {
	return g_baryonVersion;
}

std::int32_t _iplSysconCommonRead(std::uint32_t *ptr, SysconCmd const cmd) {
	std::uint8_t tx[0x10], rx[0x10];

	std::int32_t buf[4];

	tx[TX_LEN] = 2;
	tx[TX_CMD] = static_cast<std::uint8_t>(cmd);

	sdkSysconTransmitReceive(tx, rx);

	buf[0] = 0;

	memcpy(buf, &rx[RX_DATA(0)], rx[RX_LEN] - 3);

	*ptr = buf[0];

	return 0;
}

std::int32_t _iplSysconCommonWrite(std::uint32_t const val, SysconCmd const cmd, std::uint32_t const size) {
	std::uint8_t tx_buf[0x10], rx_buf[0x10];

	tx_buf[TX_CMD] = static_cast<std::uint8_t>(cmd);
	tx_buf[TX_LEN] = size;

	tx_buf[TX_DATA(1)] = (val >> 8);
	tx_buf[TX_DATA(2)] = (val >> 16);
	tx_buf[TX_DATA(3)] = (val >> 24);
	tx_buf[TX_DATA(0)] = val;

	return sdkSysconTransmitReceive(tx_buf, rx_buf);
}

std::int32_t sdkSysconTransmitReceive(std::uint8_t *tx, std::uint8_t *rx) {
	std::int32_t ret;

	do {
		std::uint8_t hash = 0;
		std::uint32_t i;

		for (i = 0; i < tx[TX_LEN]; i++) {
			hash += tx[i];
		}

		tx[i] = ~hash;

		for (i++; i < 16; i++) {
			tx[i] = 0xFF;
		}

		for (i = 0; i < 16; i++) {
			rx[i] = 0xFF;
		}

		_iplSysconPacketStart(tx);

		_sdkSysconWaitForResponse();

		ret = _iplSysconPacketEnd(rx);
	} while (ret == 0x80 || ret == 0x81); //syscon busy?

	return ret;
}

void iplSysconInit() {
	iplSysregSpiClkSelect(ClkSpi::SYSCON, ClkRef::REF1);
	iplSysregSpiClkEnable(ClkSpi::SYSCON);
	iplSysregSpiIoEnable(IoSpi::SYSCON);

	memoryK1(0x1E580000) = 0xCF;
	memoryK1(0x1E580004) = 0x04;
	memoryK1(0x1E580014) = 0x0;
	memoryK1(0x1E580024) = 0x0;

	iplGpioPortClear(GpioPort::SYSCON_REQUEST);

	iplGpioSetPortMode(GpioPort::SYSCON_REQUEST, GpioPortMode::OUTPUT);
	iplGpioSetPortMode(GpioPort::TACHYON_SPI_CS, GpioPortMode::INPUT);

	iplGpioSetIntrMode(GpioPort::TACHYON_SPI_CS, GpioIntrMode::FALLING_EDGE);

	_iplSysconGetBaryonVersion(&g_baryonVersion);
}

std::int32_t iplSysconCtrlLED(SysconLed const led, bool const enable) {
	std::uint8_t ledMask, setMask;
	switch (led) {
		case SysconLed::WLAN:
			ledMask = 0x80;
			break;
		case SysconLed::MS:
			ledMask = 0x40;
			break;
		case SysconLed::POWER:
			ledMask = 0x20;
			break;
		case SysconLed::BT:
			ledMask = 0x10;
			return -1; //TODO: Add pommel check
	}

	setMask = 0;
	if (enable) {
		std::uint32_t ver = (g_baryonVersion >> 16) & 0xF0;
		setMask = 0x10;
		if (ver != 0 && ver != 0x10) {
			setMask = 1;
		}
	}

	return _iplSysconCommonWrite(ledMask | setMask, SysconCmd::CTRL_LED, 3);
}

std::int32_t _iplSysconCmdNoParam(SysconCmd const cmd) {
	return _iplSysconCommonWrite(0, cmd, 2);
}
