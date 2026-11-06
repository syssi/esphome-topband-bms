#include <gtest/gtest.h>
#include "common.h"

namespace esphome::topband_bms_v1_ble::testing {

TEST(TopbandBmsV1BlePublishDeviceUnavailableTest, InvalidatesBinarySensors) {
  TestableTopbandBmsV1Ble bms;
  binary_sensor::BinarySensor online, charging, discharging, charge_overtemperature_protection;
  binary_sensor::BinarySensor discharge_overtemperature_protection, charge_undertemperature_protection;
  binary_sensor::BinarySensor discharge_undertemperature_protection, discharge_overcurrent_protection;
  binary_sensor::BinarySensor charge_overcurrent_protection, undervoltage_protection, overvoltage_protection;
  bms.set_online_status_binary_sensor(&online);
  bms.set_charging_binary_sensor(&charging);
  bms.set_discharging_binary_sensor(&discharging);
  bms.set_charge_overtemperature_protection_binary_sensor(&charge_overtemperature_protection);
  bms.set_discharge_overtemperature_protection_binary_sensor(&discharge_overtemperature_protection);
  bms.set_charge_undertemperature_protection_binary_sensor(&charge_undertemperature_protection);
  bms.set_discharge_undertemperature_protection_binary_sensor(&discharge_undertemperature_protection);
  bms.set_discharge_overcurrent_protection_binary_sensor(&discharge_overcurrent_protection);
  bms.set_charge_overcurrent_protection_binary_sensor(&charge_overcurrent_protection);
  bms.set_undervoltage_protection_binary_sensor(&undervoltage_protection);
  bms.set_overvoltage_protection_binary_sensor(&overvoltage_protection);
  const std::vector<binary_sensor::BinarySensor *> binary_sensors = {&charging,
                                                                     &discharging,
                                                                     &charge_overtemperature_protection,
                                                                     &discharge_overtemperature_protection,
                                                                     &charge_undertemperature_protection,
                                                                     &discharge_undertemperature_protection,
                                                                     &discharge_overcurrent_protection,
                                                                     &charge_overcurrent_protection,
                                                                     &undervoltage_protection,
                                                                     &overvoltage_protection};

  online.publish_state(true);
  for (auto *binary_sensor : binary_sensors) {
    binary_sensor->publish_state(true);
  }

  bms.publish_device_unavailable_();

  EXPECT_TRUE(online.has_state());
  EXPECT_FALSE(online.state);
  for (size_t i = 0; i < binary_sensors.size(); i++) {
    EXPECT_FALSE(binary_sensors[i]->has_state()) << "binary sensor index " << i;
  }
}

}  // namespace esphome::topband_bms_v1_ble::testing
