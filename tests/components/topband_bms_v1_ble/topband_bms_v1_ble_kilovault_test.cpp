#include <gtest/gtest.h>
#include <cmath>
#include "common.h"
#include "frames_kilovault.h"

namespace esphome::topband_bms_v1_ble::testing {

TEST(TopbandBmsV1BleKiloVaultTest, Voltage) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor v;
  bms.set_total_voltage_sensor(&v);
  bms.on_topband_bms_v1_ble_data(KILOVAULT_FRAME);
  EXPECT_NEAR(v.state, KILOVAULT_VOLTAGE, 0.001f);
}

TEST(TopbandBmsV1BleKiloVaultTest, Current) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor c;
  bms.set_current_sensor(&c);
  bms.on_topband_bms_v1_ble_data(KILOVAULT_FRAME);
  EXPECT_NEAR(c.state, KILOVAULT_CURRENT, 0.001f);
}

TEST(TopbandBmsV1BleKiloVaultTest, Power) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor p, cp, dp;
  bms.set_power_sensor(&p);
  bms.set_charging_power_sensor(&cp);
  bms.set_discharging_power_sensor(&dp);
  bms.on_topband_bms_v1_ble_data(KILOVAULT_FRAME);
  EXPECT_NEAR(p.state, KILOVAULT_VOLTAGE * KILOVAULT_CURRENT, 0.01f);
  EXPECT_FLOAT_EQ(cp.state, 0.0f);
  EXPECT_NEAR(dp.state, std::abs(KILOVAULT_VOLTAGE * KILOVAULT_CURRENT), 0.01f);
}

TEST(TopbandBmsV1BleKiloVaultTest, StateOfCharge) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor soc;
  bms.set_state_of_charge_sensor(&soc);
  bms.on_topband_bms_v1_ble_data(KILOVAULT_FRAME);
  EXPECT_FLOAT_EQ(soc.state, KILOVAULT_SOC);
}

TEST(TopbandBmsV1BleKiloVaultTest, ChargingCycles) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor cycles;
  bms.set_charging_cycles_sensor(&cycles);
  bms.on_topband_bms_v1_ble_data(KILOVAULT_FRAME);
  EXPECT_FLOAT_EQ(cycles.state, KILOVAULT_CYCLES);
}

TEST(TopbandBmsV1BleKiloVaultTest, Temperature) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor temp;
  bms.set_temperature_sensor(&temp);
  bms.on_topband_bms_v1_ble_data(KILOVAULT_FRAME);
  EXPECT_NEAR(temp.state, KILOVAULT_TEMPERATURE, 0.01f);
}

TEST(TopbandBmsV1BleKiloVaultTest, Capacity) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor cap;
  bms.set_capacity_sensor(&cap);
  bms.on_topband_bms_v1_ble_data(KILOVAULT_FRAME);
  EXPECT_NEAR(cap.state, KILOVAULT_CAPACITY, 0.001f);
}

TEST(TopbandBmsV1BleKiloVaultTest, ErrorBitmask) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor err;
  bms.set_error_bitmask_sensor(&err);
  bms.on_topband_bms_v1_ble_data(KILOVAULT_FRAME);
  EXPECT_FLOAT_EQ(err.state, KILOVAULT_ERROR_BITMASK);
}

TEST(TopbandBmsV1BleKiloVaultTest, ChargingDischarging) {
  TestableTopbandBmsV1Ble bms;
  binary_sensor::BinarySensor charging, discharging;
  bms.set_charging_binary_sensor(&charging);
  bms.set_discharging_binary_sensor(&discharging);
  bms.on_topband_bms_v1_ble_data(KILOVAULT_FRAME);
  EXPECT_FALSE(charging.state);
  EXPECT_TRUE(discharging.state);
}

TEST(TopbandBmsV1BleKiloVaultTest, CellVoltages) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor c1, c2, c3, c4;
  bms.set_cell_voltage_sensor(0, &c1);
  bms.set_cell_voltage_sensor(1, &c2);
  bms.set_cell_voltage_sensor(2, &c3);
  bms.set_cell_voltage_sensor(3, &c4);
  bms.on_topband_bms_v1_ble_data(KILOVAULT_FRAME);
  EXPECT_NEAR(c1.state, KILOVAULT_CELL_1, 0.001f);
  EXPECT_NEAR(c2.state, KILOVAULT_CELL_2, 0.001f);
  EXPECT_NEAR(c3.state, KILOVAULT_CELL_3, 0.001f);
  EXPECT_NEAR(c4.state, KILOVAULT_CELL_4, 0.001f);
}

TEST(TopbandBmsV1BleKiloVaultTest, CellStats) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor min_v, max_v, delta;
  bms.set_min_cell_voltage_sensor(&min_v);
  bms.set_max_cell_voltage_sensor(&max_v);
  bms.set_delta_cell_voltage_sensor(&delta);
  bms.on_topband_bms_v1_ble_data(KILOVAULT_FRAME);
  EXPECT_NEAR(min_v.state, KILOVAULT_MIN_CELL_VOLTAGE, 0.001f);
  EXPECT_NEAR(max_v.state, KILOVAULT_MAX_CELL_VOLTAGE, 0.001f);
  EXPECT_NEAR(delta.state, KILOVAULT_DELTA_CELL_VOLTAGE, 0.001f);
}

TEST(TopbandBmsV1BleKiloVaultTest, OnlineStatusSetAfterValidFrame) {
  TestableTopbandBmsV1Ble bms;
  binary_sensor::BinarySensor online;
  bms.set_online_status_binary_sensor(&online);
  bms.on_topband_bms_v1_ble_data(KILOVAULT_FRAME);
  EXPECT_TRUE(online.state);
  EXPECT_EQ(bms.get_no_response_count(), 0);
}

}  // namespace esphome::topband_bms_v1_ble::testing
