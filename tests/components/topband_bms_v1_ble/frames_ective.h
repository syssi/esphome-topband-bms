#pragma once

// Traffic from Ective (SOF=0x5E)
// Source: https://github.com/patman15/BMS_BLE-HA
// 113-byte frame: SOF(1) + 112 ASCII hex chars → 56 decoded bytes
// voltage=13.7V  current=-12.808A  capacity=194.86Ah
// SOC=98%  cycles=407  temp=31.0°C  error=0x00
// cells: 3.422V, 3.441V, 3.429V, 3.422V

namespace esphome::topband_bms_v1_ble::testing {

// clang-format off
static const std::vector<uint8_t> ECTIVE_FRAME = {
    0x5E,                                              // SOF
    0x38, 0x34, 0x33, 0x35, 0x30, 0x30, 0x30, 0x30,  // voltage  "84350000" → 13700 mV
    0x46, 0x38, 0x43, 0x44, 0x46, 0x46, 0x46, 0x46,  // current  "F8CDFFFF" → -12808 mA
    0x32, 0x43, 0x46, 0x39, 0x30, 0x32, 0x30, 0x30,  // capacity "2CF90200" → 194860 mAh
    0x39, 0x37, 0x30, 0x31,                            // cycles   "9701"     → 407
    0x36, 0x32, 0x30, 0x30,                            // SOC      "6200"     → 98 %
    0x45, 0x31, 0x30, 0x42,                            // temp     "E10B"     → 30.95 °C
    0x30, 0x30,                                        // error    "00"       → 0x00
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30,               // unknown  (3 decoded bytes)
    0x35, 0x45, 0x30, 0x44,                            // cell 1   "5E0D"     → 3422 mV
    0x37, 0x31, 0x30, 0x44,                            // cell 2   "710D"     → 3441 mV
    0x36, 0x35, 0x30, 0x44,                            // cell 3   "650D"     → 3429 mV
    0x35, 0x45, 0x30, 0x44,                            // cell 4   "5E0D"     → 3422 mV
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,  // cells 5-16: zero (48 chars)
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x39, 0x34, 0x46,                            // CRC BE   "094F"     → 0x094F
};
// clang-format on

static constexpr float ECTIVE_VOLTAGE = 13.7f;
static constexpr float ECTIVE_CURRENT = -12.808f;
static constexpr float ECTIVE_CAPACITY = 194.86f;
static constexpr float ECTIVE_SOC = 98.0f;
static constexpr float ECTIVE_CYCLES = 407.0f;
static constexpr float ECTIVE_TEMPERATURE = 31.0f;
static constexpr float ECTIVE_ERROR_BITMASK = 0.0f;
static constexpr float ECTIVE_CELL_1 = 3.422f;
static constexpr float ECTIVE_CELL_2 = 3.441f;
static constexpr float ECTIVE_CELL_3 = 3.429f;
static constexpr float ECTIVE_CELL_4 = 3.422f;
static constexpr float ECTIVE_MIN_CELL_VOLTAGE = 3.422f;
static constexpr float ECTIVE_MAX_CELL_VOLTAGE = 3.441f;
static constexpr float ECTIVE_DELTA_CELL_VOLTAGE = 0.019f;

}  // namespace esphome::topband_bms_v1_ble::testing
