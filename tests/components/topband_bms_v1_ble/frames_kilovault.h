#pragma once

// Traffic from KiloVault (SOF=0xB0)
// Source: https://github.com/patman15/BMS_BLE-HA
// 113-byte frame: SOF(1) + 112 ASCII hex chars → 56 decoded bytes
// voltage=13.2V  current=-0.45A  capacity=200.5Ah
// SOC=94%  cycles=125  temp=23.0°C  error=0x0100 (bit 8 unknown)
// cells: 3.294V, 3.336V, 3.298V, 3.333V

namespace esphome::topband_bms_v1_ble::testing {

// clang-format off
static const std::vector<uint8_t> KILOVAULT_FRAME = {
    0xB0,                                              // SOF
    0x39, 0x30, 0x33, 0x33, 0x30, 0x30, 0x30, 0x30,  // voltage  "90330000" → 13200 mV
    0x33, 0x45, 0x46, 0x45, 0x46, 0x46, 0x46, 0x46,  // current  "3EFEFFFF" → -450 mA
    0x33, 0x34, 0x30, 0x46, 0x30, 0x33, 0x30, 0x30,  // capacity "340F0300" → 200500 mAh
    0x37, 0x44, 0x30, 0x30,                            // cycles   "7D00"     → 125
    0x35, 0x45, 0x30, 0x30,                            // SOC      "5E00"     → 94 %
    0x39, 0x31, 0x30, 0x42,                            // temp     "910B"     → 22.95 °C
    0x30, 0x30,                                        // error    "00"       → 0x00
    0x30, 0x31, 0x30, 0x30, 0x30, 0x30,               // unknown  "010000"   → dec[19:22]={0x01,0x00,0x00}
    0x44, 0x45, 0x30, 0x43,                            // cell 1   "DE0C"     → 3294 mV
    0x30, 0x38, 0x30, 0x44,                            // cell 2   "080D"     → 3336 mV
    0x45, 0x32, 0x30, 0x43,                            // cell 3   "E20C"     → 3298 mV
    0x30, 0x35, 0x30, 0x44,                            // cell 4   "050D"     → 3333 mV
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,  // cells 5-16: zero (48 chars)
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x37, 0x42, 0x41,                            // CRC BE   "07BA"     → 0x07BA
};
// clang-format on

static constexpr float KILOVAULT_VOLTAGE = 13.2f;
static constexpr float KILOVAULT_CURRENT = -0.45f;
static constexpr float KILOVAULT_CAPACITY = 200.5f;
static constexpr float KILOVAULT_SOC = 94.0f;
static constexpr float KILOVAULT_CYCLES = 125.0f;
static constexpr float KILOVAULT_TEMPERATURE = 23.0f;
static constexpr float KILOVAULT_ERROR_BITMASK = 256.0f;
static constexpr float KILOVAULT_CELL_1 = 3.294f;
static constexpr float KILOVAULT_CELL_2 = 3.336f;
static constexpr float KILOVAULT_CELL_3 = 3.298f;
static constexpr float KILOVAULT_CELL_4 = 3.333f;
static constexpr float KILOVAULT_MIN_CELL_VOLTAGE = 3.294f;
static constexpr float KILOVAULT_MAX_CELL_VOLTAGE = 3.336f;
static constexpr float KILOVAULT_DELTA_CELL_VOLTAGE = 0.042f;

}  // namespace esphome::topband_bms_v1_ble::testing
