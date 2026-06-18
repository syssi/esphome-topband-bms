#include "topband_bms_v1_ble.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/version.h"

#if ESPHOME_VERSION_CODE >= VERSION_CODE(2025, 12, 0)
#define ADDR_STR(x) x
#else
#define ADDR_STR(x) (x).c_str()
#endif

namespace esphome::topband_bms_v1_ble {

static const char *const TAG = "topband_bms_v1_ble";

static const uint8_t ERRORS_SIZE = 16;
static constexpr const char *const ERRORS[ERRORS_SIZE] = {
    "Charging overtemperature",      // bit 0
    "Discharging overtemperature",   // bit 1
    "Charging undertemperature",     // bit 2
    "Discharging undertemperature",  // bit 3
    "Discharging overcurrent",       // bit 4
    "Charging overcurrent",          // bit 5
    "Undervoltage",                  // bit 6
    "Overvoltage",                   // bit 7
    "Unknown (0x08)",                // bit 8
    "Unknown (0x09)",                // bit 9
    "Unknown (0x0A)",                // bit 10
    "Unknown (0x0B)",                // bit 11
    "Unknown (0x0C)",                // bit 12
    "Unknown (0x0D)",                // bit 13
    "Unknown (0x0E)",                // bit 14
    "Unknown (0x0F)",                // bit 15
};

static bool is_valid_sof(uint8_t byte) { return byte == 0x5E || byte == 0x83 || byte == 0xB0; }

static bool is_hex_digit(uint8_t c) {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

uint8_t TopbandBmsV1Ble::ascii_hex_to_byte(uint8_t hi, uint8_t lo) {
  hi = (hi <= '9') ? hi - '0' : (hi & 0x7) + 9;
  lo = (lo <= '9') ? lo - '0' : (lo & 0x7) + 9;
  return (hi << 4) + lo;
}

#ifdef USE_ESP32

void TopbandBmsV1Ble::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                          esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_OPEN_EVT:
      break;

    case ESP_GATTC_DISCONNECT_EVT:
      this->node_state = espbt::ClientState::IDLE;
      if (this->char_notify_handle_ != 0) {
        esp_ble_gattc_unregister_for_notify(this->parent()->get_gattc_if(), this->parent()->get_remote_bda(),
                                            this->char_notify_handle_);
      }
      this->char_notify_handle_ = 0;
      this->frame_buffer_.clear();
      this->state_ = State::SOI;
      break;

    case ESP_GATTC_SEARCH_CMPL_EVT: {
      auto *chr = this->parent_->get_characteristic(SERVICE_UUID, NOTIFY_UUID);
      if (chr == nullptr) {
        ESP_LOGE(TAG, "[%s] No notify characteristic found - not a Topband v1 BMS?",
                 ADDR_STR(this->parent_->address_str()));
        break;
      }
      this->char_notify_handle_ = chr->handle;
      auto status = esp_ble_gattc_register_for_notify(this->parent()->get_gattc_if(), this->parent()->get_remote_bda(),
                                                      chr->handle);
      if (status)
        ESP_LOGW(TAG, "esp_ble_gattc_register_for_notify failed, status=%d", status);
      break;
    }

    case ESP_GATTC_REG_FOR_NOTIFY_EVT:
      this->node_state = espbt::ClientState::ESTABLISHED;
      break;

    case ESP_GATTC_NOTIFY_EVT:
      if (param->notify.handle != this->char_notify_handle_)
        break;
      for (uint16_t i = 0; i < param->notify.value_len; i++) {
        uint8_t byte = param->notify.value[i];
        switch (this->state_) {
          case State::SOI:
            if (is_valid_sof(byte)) {
              this->frame_buffer_.clear();
              this->frame_buffer_.push_back(byte);
              this->state_ = State::INFO;
            }
            break;
          case State::INFO:
            this->frame_buffer_.push_back(byte);
            if (this->frame_buffer_.size() == FRAME_SIZE) {
              this->on_topband_bms_v1_ble_data(this->frame_buffer_);
              this->frame_buffer_.clear();
              this->state_ = State::SOI;
            } else if (this->frame_buffer_.size() > FRAME_SIZE) {
              ESP_LOGW(TAG, "Frame buffer overflow, resetting");
              this->frame_buffer_.clear();
              this->state_ = State::SOI;
            }
            break;
        }
      }
      break;

    default:
      break;
  }
}

#endif  // USE_ESP32

void TopbandBmsV1Ble::update() { this->track_online_status_(); }

void TopbandBmsV1Ble::on_topband_bms_v1_ble_data(const std::vector<uint8_t> &frame) {
  if (frame.size() != FRAME_SIZE) {
    ESP_LOGW(TAG, "Unexpected frame size: %zu (expected %d)", frame.size(), FRAME_SIZE);
    return;
  }
  if (!is_valid_sof(frame[0])) {
    ESP_LOGW(TAG, "Invalid SOF: 0x%02X", frame[0]);
    return;
  }
  for (uint8_t i = 1; i < FRAME_SIZE; i++) {
    if (!is_hex_digit(frame[i])) {
      ESP_LOGW(TAG, "Non-hex byte at index %d: 0x%02X", i, frame[i]);
      return;
    }
  }
  this->decode_(frame);
}

void TopbandBmsV1Ble::decode_(const std::vector<uint8_t> &frame) {
  uint8_t d[56];
  for (uint8_t i = 0; i < 56; i++)
    d[i] = ascii_hex_to_byte(frame[1 + 2 * i], frame[2 + 2 * i]);

  uint16_t crc = 0;
  for (uint8_t i = 0; i < 54; i++)
    crc += d[i];
  uint16_t stored_crc = ((uint16_t) d[54] << 8) | d[55];
  if (crc != stored_crc) {
    ESP_LOGW(TAG, "CRC mismatch: computed=0x%04X stored=0x%04X", crc, stored_crc);
    return;
  }

  this->reset_online_status_tracker_();

  auto get_16bit = [&](uint8_t o) -> uint16_t { return (uint16_t) d[o] | ((uint16_t) d[o + 1] << 8); };
  auto get_32bit = [&](uint8_t o) -> uint32_t {
    return (uint32_t) d[o] | ((uint32_t) d[o + 1] << 8) | ((uint32_t) d[o + 2] << 16) | ((uint32_t) d[o + 3] << 24);
  };

  // Offset  Len  Type       Content                         Scale   Unit
  // [0]      4   uint32     voltage                         0.001   V
  float voltage = get_32bit(0) * 0.001f;
  this->publish_state_(this->total_voltage_sensor_, voltage);

  // [4]      4   int32      current (negative = discharge)  0.001   A
  float current = (int32_t) get_32bit(4) * 0.001f;
  this->publish_state_(this->current_sensor_, current);
  this->publish_state_(this->charging_binary_sensor_, current > 0);
  this->publish_state_(this->discharging_binary_sensor_, current < 0);

  float power = voltage * current;
  this->publish_state_(this->power_sensor_, power);
  this->publish_state_(this->charging_power_sensor_, current > 0 ? power : 0.0f);
  this->publish_state_(this->discharging_power_sensor_, current < 0 ? std::abs(power) : 0.0f);

  // [8]      4   uint32     capacity                        0.001   Ah
  this->publish_state_(this->capacity_sensor_, get_32bit(8) * 0.001f);

  // [12]     2   uint16     charging cycles
  this->publish_state_(this->charging_cycles_sensor_, (float) get_16bit(12));

  // [14]     2   uint16     state of charge                 1       %
  this->publish_state_(this->state_of_charge_sensor_, (float) get_16bit(14));

  // [16]     2   uint16     temperature                     0.1     K  (-273.1 -> °C)
  this->publish_state_(this->temperature_sensor_, get_16bit(16) * 0.1f - 273.1f);

  // [18]     2   uint16     error bitmask
  uint16_t error_bitmask = get_16bit(18);
  this->publish_state_(this->error_bitmask_sensor_, (float) error_bitmask);
  this->publish_state_(this->errors_text_sensor_, this->errors_to_string_(error_bitmask));
  this->publish_state_(this->charge_overtemperature_protection_binary_sensor_, (error_bitmask >> 0) & 1);
  this->publish_state_(this->discharge_overtemperature_protection_binary_sensor_, (error_bitmask >> 1) & 1);
  this->publish_state_(this->charge_undertemperature_protection_binary_sensor_, (error_bitmask >> 2) & 1);
  this->publish_state_(this->discharge_undertemperature_protection_binary_sensor_, (error_bitmask >> 3) & 1);
  this->publish_state_(this->discharge_overcurrent_protection_binary_sensor_, (error_bitmask >> 4) & 1);
  this->publish_state_(this->charge_overcurrent_protection_binary_sensor_, (error_bitmask >> 5) & 1);
  this->publish_state_(this->undervoltage_protection_binary_sensor_, (error_bitmask >> 6) & 1);
  this->publish_state_(this->overvoltage_protection_binary_sensor_, (error_bitmask >> 7) & 1);

  // [20]     2   uint16     AFE status  (known: bit 5 = SC short circuit)
  this->publish_state_(this->afe_status_bitmask_sensor_, (float) get_16bit(20));

  // [22]    32   uint16x16  cell voltages                   0.001   V
  float cell_voltages[16] = {};
  for (uint8_t i = 0; i < 16; i++)
    cell_voltages[i] = get_16bit(22 + i * 2) * 0.001f;

  uint8_t cell_count = 0;
  for (uint8_t i = 0; i < 16; i++) {
    if (cell_voltages[i] > 0.0f)
      cell_count = i + 1;
  }

  for (uint8_t i = 0; i < cell_count; i++) {
    this->publish_state_(this->cells_[i].cell_voltage_sensor_, cell_voltages[i]);
  }

  if (cell_count > 0) {
    this->update_cell_stats_(cell_voltages, cell_count);
  }

  // [54]     2   uint16     CRC (big-endian, already validated)
}

void TopbandBmsV1Ble::update_cell_stats_(const float *voltages, uint8_t count) {
  float min_v = voltages[0], max_v = voltages[0], sum = 0.0f;
  uint8_t min_cell = 0, max_cell = 0;

  for (uint8_t i = 0; i < count; i++) {
    sum += voltages[i];
    if (voltages[i] < min_v) {
      min_v = voltages[i];
      min_cell = i;
    }
    if (voltages[i] > max_v) {
      max_v = voltages[i];
      max_cell = i;
    }
  }

  this->publish_state_(this->min_cell_voltage_sensor_, min_v);
  this->publish_state_(this->max_cell_voltage_sensor_, max_v);
  this->publish_state_(this->delta_cell_voltage_sensor_, max_v - min_v);
  this->publish_state_(this->average_cell_voltage_sensor_, sum / count);
  this->publish_state_(this->min_voltage_cell_sensor_, (float) (min_cell + 1));
  this->publish_state_(this->max_voltage_cell_sensor_, (float) (max_cell + 1));
}

std::string TopbandBmsV1Ble::errors_to_string_(uint16_t status) {
  if (status == 0)
    return "OK";
  std::string result;
  for (uint8_t i = 0; i < ERRORS_SIZE; i++) {
    if (status & (1 << i)) {
      if (!result.empty())
        result += ", ";
      result += ERRORS[i];
    }
  }
  return result;
}

void TopbandBmsV1Ble::dump_config() {
  ESP_LOGCONFIG(TAG, "Topband BMS v1 BLE:");
  ESP_LOGCONFIG(TAG, "  Protocol start of frame: 0x%02X", this->protocol_start_of_frame_);
  LOG_BINARY_SENSOR("", "Online Status", this->online_status_binary_sensor_);
  LOG_BINARY_SENSOR("", "Charging", this->charging_binary_sensor_);
  LOG_BINARY_SENSOR("", "Discharging", this->discharging_binary_sensor_);
  LOG_SENSOR("", "Total Voltage", this->total_voltage_sensor_);
  LOG_SENSOR("", "Current", this->current_sensor_);
  LOG_SENSOR("", "Power", this->power_sensor_);
  LOG_SENSOR("", "Charging Power", this->charging_power_sensor_);
  LOG_SENSOR("", "Discharging Power", this->discharging_power_sensor_);
  LOG_SENSOR("", "State of Charge", this->state_of_charge_sensor_);
  LOG_SENSOR("", "Charging Cycles", this->charging_cycles_sensor_);
  LOG_SENSOR("", "Temperature", this->temperature_sensor_);
  LOG_SENSOR("", "Capacity", this->capacity_sensor_);
  LOG_SENSOR("", "Error Bitmask", this->error_bitmask_sensor_);
  LOG_SENSOR("", "AFE Status Bitmask", this->afe_status_bitmask_sensor_);
  LOG_SENSOR("", "Min Cell Voltage", this->min_cell_voltage_sensor_);
  LOG_SENSOR("", "Max Cell Voltage", this->max_cell_voltage_sensor_);
  LOG_SENSOR("", "Min Voltage Cell", this->min_voltage_cell_sensor_);
  LOG_SENSOR("", "Max Voltage Cell", this->max_voltage_cell_sensor_);
  LOG_SENSOR("", "Delta Cell Voltage", this->delta_cell_voltage_sensor_);
  LOG_SENSOR("", "Average Cell Voltage", this->average_cell_voltage_sensor_);
  for (uint8_t i = 0; i < 16; i++) {
    if (this->cells_[i].cell_voltage_sensor_ != nullptr) {
      char label[20];
      snprintf(label, sizeof(label), "Cell Voltage %u", i + 1);
      LOG_SENSOR("", label, this->cells_[i].cell_voltage_sensor_);
    }
  }
  LOG_TEXT_SENSOR("", "Errors", this->errors_text_sensor_);
}

void TopbandBmsV1Ble::track_online_status_() {
  if (this->no_response_count_ < MAX_NO_RESPONSE_COUNT) {
    this->no_response_count_++;
  }
  if (this->no_response_count_ == MAX_NO_RESPONSE_COUNT) {
    this->publish_device_unavailable_();
    this->no_response_count_++;
  }
}

void TopbandBmsV1Ble::reset_online_status_tracker_() {
  this->no_response_count_ = 0;
  this->publish_state_(this->online_status_binary_sensor_, true);
}

void TopbandBmsV1Ble::publish_device_unavailable_() {
  this->publish_state_(this->online_status_binary_sensor_, false);
  this->publish_state_(this->total_voltage_sensor_, NAN);
  this->publish_state_(this->current_sensor_, NAN);
  this->publish_state_(this->power_sensor_, NAN);
  this->publish_state_(this->charging_power_sensor_, NAN);
  this->publish_state_(this->discharging_power_sensor_, NAN);
  this->publish_state_(this->state_of_charge_sensor_, NAN);
  this->publish_state_(this->charging_cycles_sensor_, NAN);
  this->publish_state_(this->temperature_sensor_, NAN);
  this->publish_state_(this->capacity_sensor_, NAN);
  this->publish_state_(this->error_bitmask_sensor_, NAN);
  this->publish_state_(this->afe_status_bitmask_sensor_, NAN);
  this->publish_state_(this->min_cell_voltage_sensor_, NAN);
  this->publish_state_(this->max_cell_voltage_sensor_, NAN);
  this->publish_state_(this->min_voltage_cell_sensor_, NAN);
  this->publish_state_(this->max_voltage_cell_sensor_, NAN);
  this->publish_state_(this->delta_cell_voltage_sensor_, NAN);
  this->publish_state_(this->average_cell_voltage_sensor_, NAN);
  for (auto &cell : this->cells_)
    this->publish_state_(cell.cell_voltage_sensor_, NAN);
}

void TopbandBmsV1Ble::publish_state_(binary_sensor::BinarySensor *s, bool state) {
  if (s != nullptr)
    s->publish_state(state);
}

void TopbandBmsV1Ble::publish_state_(sensor::Sensor *s, float value) {
  if (s != nullptr)
    s->publish_state(value);
}

void TopbandBmsV1Ble::publish_state_(text_sensor::TextSensor *s, const std::string &state) {
  if (s != nullptr)
    s->publish_state(state);
}

}  // namespace esphome::topband_bms_v1_ble
