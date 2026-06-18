#pragma once

// Traffic from StartCraft (SOF=0x83)
// Source: https://github.com/patman15/BMS_BLE-HA
// 113-byte frame: SOF(1) + 112 ASCII hex chars → 56 decoded bytes
// voltage=13.41V  current=-0.394A  capacity=101.0Ah
// SOC=98%  cycles=22  temp=16.8°C  error=0x8000 (bit 15 unknown)
// cells: 3.350V, 3.351V, 3.353V, 3.356V

namespace esphome::topband_bms_v1_ble::testing {

// clang-format off
static const std::vector<uint8_t> STARTCRAFT_FRAME = {
    0x83,                                              // SOF
    0x36, 0x32, 0x33, 0x34, 0x30, 0x30, 0x30, 0x30,  // voltage  "62340000" → 13410 mV
    0x37, 0x36, 0x46, 0x45, 0x46, 0x46, 0x46, 0x46,  // current  "76FEFFFF" → -394 mA
    0x38, 0x38, 0x38, 0x41, 0x30, 0x31, 0x30, 0x30,  // capacity "888A0100" → 101000 mAh
    0x31, 0x36, 0x30, 0x30,                            // cycles   "1600"     → 22
    0x36, 0x32, 0x30, 0x30,                            // SOC      "6200"     → 98 %
    0x35, 0x33, 0x30, 0x42,                            // temp     "530B"     → 16.75 °C
    0x30, 0x30,                                        // error    "00"       → 0x00
    0x38, 0x30, 0x30, 0x37, 0x42, 0x34,               // unknown  "8007B4"   → dec[19:22]={0x80,0x07,0xB4}
    0x31, 0x36, 0x30, 0x44,                            // cell 1   "160D"     → 3350 mV
    0x31, 0x37, 0x30, 0x44,                            // cell 2   "170D"     → 3351 mV
    0x31, 0x39, 0x30, 0x44,                            // cell 3   "190D"     → 3353 mV
    0x31, 0x43, 0x30, 0x44,                            // cell 4   "1C0D"     → 3356 mV
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,  // cells 5-16: zero (48 chars)
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x37, 0x43, 0x32,                            // CRC BE   "07C2"     → 0x07C2
};
// clang-format on

static constexpr float STARTCRAFT_VOLTAGE = 13.41f;
static constexpr float STARTCRAFT_CURRENT = -0.394f;
static constexpr float STARTCRAFT_CAPACITY = 101.0f;
static constexpr float STARTCRAFT_SOC = 98.0f;
static constexpr float STARTCRAFT_CYCLES = 22.0f;
static constexpr float STARTCRAFT_TEMPERATURE = 16.8f;
static constexpr float STARTCRAFT_ERROR_BITMASK = 32768.0f;
static constexpr float STARTCRAFT_CELL_1 = 3.350f;
static constexpr float STARTCRAFT_CELL_2 = 3.351f;
static constexpr float STARTCRAFT_CELL_3 = 3.353f;
static constexpr float STARTCRAFT_CELL_4 = 3.356f;
static constexpr float STARTCRAFT_MIN_CELL_VOLTAGE = 3.350f;
static constexpr float STARTCRAFT_MAX_CELL_VOLTAGE = 3.356f;
static constexpr float STARTCRAFT_DELTA_CELL_VOLTAGE = 0.006f;

}  // namespace esphome::topband_bms_v1_ble::testing
