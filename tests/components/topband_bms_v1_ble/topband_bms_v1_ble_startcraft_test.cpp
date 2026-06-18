#include <gtest/gtest.h>
#include <cmath>
#include "common.h"
#include "frames_startcraft.h"

namespace esphome::topband_bms_v1_ble::testing {

TEST(TopbandBmsV1BleStartCraftTest, Voltage) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor v;
  bms.set_total_voltage_sensor(&v);
  bms.on_topband_bms_v1_ble_data(STARTCRAFT_FRAME);
  EXPECT_NEAR(v.state, STARTCRAFT_VOLTAGE, 0.001f);
}

TEST(TopbandBmsV1BleStartCraftTest, Current) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor c;
  bms.set_current_sensor(&c);
  bms.on_topband_bms_v1_ble_data(STARTCRAFT_FRAME);
  EXPECT_NEAR(c.state, STARTCRAFT_CURRENT, 0.001f);
}

TEST(TopbandBmsV1BleStartCraftTest, Power) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor p, cp, dp;
  bms.set_power_sensor(&p);
  bms.set_charging_power_sensor(&cp);
  bms.set_discharging_power_sensor(&dp);
  bms.on_topband_bms_v1_ble_data(STARTCRAFT_FRAME);
  EXPECT_NEAR(p.state, STARTCRAFT_VOLTAGE * STARTCRAFT_CURRENT, 0.01f);
  EXPECT_FLOAT_EQ(cp.state, 0.0f);
  EXPECT_NEAR(dp.state, std::abs(STARTCRAFT_VOLTAGE * STARTCRAFT_CURRENT), 0.01f);
}

TEST(TopbandBmsV1BleStartCraftTest, StateOfCharge) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor soc;
  bms.set_state_of_charge_sensor(&soc);
  bms.on_topband_bms_v1_ble_data(STARTCRAFT_FRAME);
  EXPECT_FLOAT_EQ(soc.state, STARTCRAFT_SOC);
}

TEST(TopbandBmsV1BleStartCraftTest, ChargingCycles) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor cycles;
  bms.set_charging_cycles_sensor(&cycles);
  bms.on_topband_bms_v1_ble_data(STARTCRAFT_FRAME);
  EXPECT_FLOAT_EQ(cycles.state, STARTCRAFT_CYCLES);
}

TEST(TopbandBmsV1BleStartCraftTest, Temperature) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor temp;
  bms.set_temperature_sensor(&temp);
  bms.on_topband_bms_v1_ble_data(STARTCRAFT_FRAME);
  EXPECT_NEAR(temp.state, STARTCRAFT_TEMPERATURE, 0.01f);
}

TEST(TopbandBmsV1BleStartCraftTest, Capacity) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor cap;
  bms.set_capacity_sensor(&cap);
  bms.on_topband_bms_v1_ble_data(STARTCRAFT_FRAME);
  EXPECT_NEAR(cap.state, STARTCRAFT_CAPACITY, 0.001f);
}

TEST(TopbandBmsV1BleStartCraftTest, ErrorBitmask) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor err;
  bms.set_error_bitmask_sensor(&err);
  bms.on_topband_bms_v1_ble_data(STARTCRAFT_FRAME);
  EXPECT_FLOAT_EQ(err.state, STARTCRAFT_ERROR_BITMASK);
}

TEST(TopbandBmsV1BleStartCraftTest, ChargingDischarging) {
  TestableTopbandBmsV1Ble bms;
  binary_sensor::BinarySensor charging, discharging;
  bms.set_charging_binary_sensor(&charging);
  bms.set_discharging_binary_sensor(&discharging);
  bms.on_topband_bms_v1_ble_data(STARTCRAFT_FRAME);
  EXPECT_FALSE(charging.state);
  EXPECT_TRUE(discharging.state);
}

TEST(TopbandBmsV1BleStartCraftTest, CellVoltages) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor c1, c2, c3, c4;
  bms.set_cell_voltage_sensor(0, &c1);
  bms.set_cell_voltage_sensor(1, &c2);
  bms.set_cell_voltage_sensor(2, &c3);
  bms.set_cell_voltage_sensor(3, &c4);
  bms.on_topband_bms_v1_ble_data(STARTCRAFT_FRAME);
  EXPECT_NEAR(c1.state, STARTCRAFT_CELL_1, 0.001f);
  EXPECT_NEAR(c2.state, STARTCRAFT_CELL_2, 0.001f);
  EXPECT_NEAR(c3.state, STARTCRAFT_CELL_3, 0.001f);
  EXPECT_NEAR(c4.state, STARTCRAFT_CELL_4, 0.001f);
}

TEST(TopbandBmsV1BleStartCraftTest, CellStats) {
  TestableTopbandBmsV1Ble bms;
  sensor::Sensor min_v, max_v, delta;
  bms.set_min_cell_voltage_sensor(&min_v);
  bms.set_max_cell_voltage_sensor(&max_v);
  bms.set_delta_cell_voltage_sensor(&delta);
  bms.on_topband_bms_v1_ble_data(STARTCRAFT_FRAME);
  EXPECT_NEAR(min_v.state, STARTCRAFT_MIN_CELL_VOLTAGE, 0.001f);
  EXPECT_NEAR(max_v.state, STARTCRAFT_MAX_CELL_VOLTAGE, 0.001f);
  EXPECT_NEAR(delta.state, STARTCRAFT_DELTA_CELL_VOLTAGE, 0.001f);
}

TEST(TopbandBmsV1BleStartCraftTest, OnlineStatusSetAfterValidFrame) {
  TestableTopbandBmsV1Ble bms;
  binary_sensor::BinarySensor online;
  bms.set_online_status_binary_sensor(&online);
  bms.on_topband_bms_v1_ble_data(STARTCRAFT_FRAME);
  EXPECT_TRUE(online.state);
  EXPECT_EQ(bms.get_no_response_count(), 0);
}

}  // namespace esphome::topband_bms_v1_ble::testing
