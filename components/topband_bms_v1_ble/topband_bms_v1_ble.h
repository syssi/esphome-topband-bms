#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

#ifdef USE_ESP32
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include <esp_gattc_api.h>
namespace espbt = esphome::esp32_ble_tracker;
#endif

namespace esphome::topband_bms_v1_ble {

class TopbandBmsV1Ble :
#ifdef USE_ESP32
    public esphome::ble_client::BLEClientNode,
#endif
    public PollingComponent {
 public:
#ifdef USE_ESP32
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
#endif
  void dump_config() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_protocol_start_of_frame(uint8_t protocol_start_of_frame) {
    protocol_start_of_frame_ = protocol_start_of_frame;
  }

  void set_online_status_binary_sensor(binary_sensor::BinarySensor *s) { online_status_binary_sensor_ = s; }
  void set_charging_binary_sensor(binary_sensor::BinarySensor *s) { charging_binary_sensor_ = s; }
  void set_discharging_binary_sensor(binary_sensor::BinarySensor *s) { discharging_binary_sensor_ = s; }
  void set_charge_overtemperature_protection_binary_sensor(binary_sensor::BinarySensor *s) {
    charge_overtemperature_protection_binary_sensor_ = s;
  }
  void set_discharge_overtemperature_protection_binary_sensor(binary_sensor::BinarySensor *s) {
    discharge_overtemperature_protection_binary_sensor_ = s;
  }
  void set_charge_undertemperature_protection_binary_sensor(binary_sensor::BinarySensor *s) {
    charge_undertemperature_protection_binary_sensor_ = s;
  }
  void set_discharge_undertemperature_protection_binary_sensor(binary_sensor::BinarySensor *s) {
    discharge_undertemperature_protection_binary_sensor_ = s;
  }
  void set_discharge_overcurrent_protection_binary_sensor(binary_sensor::BinarySensor *s) {
    discharge_overcurrent_protection_binary_sensor_ = s;
  }
  void set_charge_overcurrent_protection_binary_sensor(binary_sensor::BinarySensor *s) {
    charge_overcurrent_protection_binary_sensor_ = s;
  }
  void set_undervoltage_protection_binary_sensor(binary_sensor::BinarySensor *s) {
    undervoltage_protection_binary_sensor_ = s;
  }
  void set_overvoltage_protection_binary_sensor(binary_sensor::BinarySensor *s) {
    overvoltage_protection_binary_sensor_ = s;
  }

  void set_total_voltage_sensor(sensor::Sensor *s) { total_voltage_sensor_ = s; }
  void set_current_sensor(sensor::Sensor *s) { current_sensor_ = s; }
  void set_power_sensor(sensor::Sensor *s) { power_sensor_ = s; }
  void set_charging_power_sensor(sensor::Sensor *s) { charging_power_sensor_ = s; }
  void set_discharging_power_sensor(sensor::Sensor *s) { discharging_power_sensor_ = s; }
  void set_state_of_charge_sensor(sensor::Sensor *s) { state_of_charge_sensor_ = s; }
  void set_charging_cycles_sensor(sensor::Sensor *s) { charging_cycles_sensor_ = s; }
  void set_temperature_sensor(sensor::Sensor *s) { temperature_sensor_ = s; }
  void set_capacity_sensor(sensor::Sensor *s) { capacity_sensor_ = s; }
  void set_error_bitmask_sensor(sensor::Sensor *s) { error_bitmask_sensor_ = s; }
  void set_afe_status_bitmask_sensor(sensor::Sensor *s) { afe_status_bitmask_sensor_ = s; }
  void set_min_cell_voltage_sensor(sensor::Sensor *s) { min_cell_voltage_sensor_ = s; }
  void set_max_cell_voltage_sensor(sensor::Sensor *s) { max_cell_voltage_sensor_ = s; }
  void set_min_voltage_cell_sensor(sensor::Sensor *s) { min_voltage_cell_sensor_ = s; }
  void set_max_voltage_cell_sensor(sensor::Sensor *s) { max_voltage_cell_sensor_ = s; }
  void set_delta_cell_voltage_sensor(sensor::Sensor *s) { delta_cell_voltage_sensor_ = s; }
  void set_average_cell_voltage_sensor(sensor::Sensor *s) { average_cell_voltage_sensor_ = s; }
  void set_cell_voltage_sensor(uint8_t cell, sensor::Sensor *s) { cells_[cell].cell_voltage_sensor_ = s; }

  void set_errors_text_sensor(text_sensor::TextSensor *s) { errors_text_sensor_ = s; }

  void on_topband_bms_v1_ble_data(const std::vector<uint8_t> &frame);

 protected:
  binary_sensor::BinarySensor *online_status_binary_sensor_{nullptr};
  binary_sensor::BinarySensor *charging_binary_sensor_{nullptr};
  binary_sensor::BinarySensor *discharging_binary_sensor_{nullptr};
  binary_sensor::BinarySensor *charge_overtemperature_protection_binary_sensor_{nullptr};
  binary_sensor::BinarySensor *discharge_overtemperature_protection_binary_sensor_{nullptr};
  binary_sensor::BinarySensor *charge_undertemperature_protection_binary_sensor_{nullptr};
  binary_sensor::BinarySensor *discharge_undertemperature_protection_binary_sensor_{nullptr};
  binary_sensor::BinarySensor *discharge_overcurrent_protection_binary_sensor_{nullptr};
  binary_sensor::BinarySensor *charge_overcurrent_protection_binary_sensor_{nullptr};
  binary_sensor::BinarySensor *undervoltage_protection_binary_sensor_{nullptr};
  binary_sensor::BinarySensor *overvoltage_protection_binary_sensor_{nullptr};

  sensor::Sensor *total_voltage_sensor_{nullptr};
  sensor::Sensor *current_sensor_{nullptr};
  sensor::Sensor *power_sensor_{nullptr};
  sensor::Sensor *charging_power_sensor_{nullptr};
  sensor::Sensor *discharging_power_sensor_{nullptr};
  sensor::Sensor *state_of_charge_sensor_{nullptr};
  sensor::Sensor *charging_cycles_sensor_{nullptr};
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *capacity_sensor_{nullptr};
  sensor::Sensor *error_bitmask_sensor_{nullptr};
  sensor::Sensor *afe_status_bitmask_sensor_{nullptr};
  sensor::Sensor *min_cell_voltage_sensor_{nullptr};
  sensor::Sensor *max_cell_voltage_sensor_{nullptr};
  sensor::Sensor *min_voltage_cell_sensor_{nullptr};
  sensor::Sensor *max_voltage_cell_sensor_{nullptr};
  sensor::Sensor *delta_cell_voltage_sensor_{nullptr};
  sensor::Sensor *average_cell_voltage_sensor_{nullptr};

  text_sensor::TextSensor *errors_text_sensor_{nullptr};

  struct Cell {
    sensor::Sensor *cell_voltage_sensor_{nullptr};
  } cells_[16];

  uint8_t protocol_start_of_frame_{0x5E};
  uint8_t no_response_count_{0};
  uint16_t char_notify_handle_{0};

  enum class State { SOI, INFO } state_{State::SOI};
  std::vector<uint8_t> frame_buffer_;

  static uint8_t ascii_hex_to_byte(uint8_t hi, uint8_t lo);
  void decode_(const std::vector<uint8_t> &frame);
  void update_cell_stats_(const float *voltages, uint8_t count);

  void publish_state_(binary_sensor::BinarySensor *s, bool state);
  void publish_state_(sensor::Sensor *s, float value);
  void publish_state_(text_sensor::TextSensor *s, const std::string &state);
  void publish_device_unavailable_();
  void reset_online_status_tracker_();
  void track_online_status_();

  std::string errors_to_string_(uint16_t status);

  bool check_bit_(uint8_t mask, uint8_t flag) { return (mask & flag) == flag; }

  static const uint8_t MAX_NO_RESPONSE_COUNT = 10;
  static const uint16_t SERVICE_UUID = 0xFFE0;
  static const uint16_t NOTIFY_UUID = 0xFFE4;
  static const uint8_t FRAME_SIZE = 113;
};

}  // namespace esphome::topband_bms_v1_ble
