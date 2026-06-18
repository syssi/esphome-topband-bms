"""Schema structure tests for topband_bms_v1_ble ESPHome component modules."""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

import components.topband_bms_v1_ble as hub  # noqa: E402
from components.topband_bms_v1_ble import (  # noqa: E402
    binary_sensor,
    sensor,
    text_sensor,
)


class TestHubConstants:
    def test_conf_id_defined(self):
        assert hub.CONF_TOPBAND_BMS_V1_BLE_ID == "topband_bms_v1_ble_id"


class TestSensorLists:
    def test_cells_count(self):
        assert len(sensor.CELLS) == 16

    def test_cells_naming(self):
        assert sensor.CELLS[0] == "cell_voltage_1"
        assert sensor.CELLS[15] == "cell_voltage_16"
        for i, key in enumerate(sensor.CELLS, 1):
            assert key == f"cell_voltage_{i}"

    def test_sensor_defs_completeness(self):
        assert sensor.CONF_TOTAL_VOLTAGE in sensor.SENSOR_DEFS
        assert sensor.CONF_STATE_OF_CHARGE in sensor.SENSOR_DEFS
        assert sensor.CONF_ERROR_BITMASK in sensor.SENSOR_DEFS
        assert sensor.CONF_MIN_CELL_VOLTAGE in sensor.SENSOR_DEFS
        assert sensor.CONF_MAX_CELL_VOLTAGE in sensor.SENSOR_DEFS
        assert sensor.CONF_DELTA_CELL_VOLTAGE in sensor.SENSOR_DEFS
        assert sensor.CONF_AVERAGE_CELL_VOLTAGE in sensor.SENSOR_DEFS
        assert sensor.CONF_TEMPERATURE in sensor.SENSOR_DEFS
        assert sensor.CONF_CAPACITY in sensor.SENSOR_DEFS
        assert len(sensor.SENSOR_DEFS) == 17

    def test_no_cell_keys_in_sensor_defs(self):
        for key in sensor.SENSOR_DEFS:
            assert key not in sensor.CELLS


class TestBinarySensorConstants:
    def test_binary_sensor_defs_dict(self):
        assert binary_sensor.CONF_CHARGING in binary_sensor.BINARY_SENSOR_DEFS
        assert binary_sensor.CONF_DISCHARGING in binary_sensor.BINARY_SENSOR_DEFS
        assert binary_sensor.CONF_ONLINE_STATUS in binary_sensor.BINARY_SENSOR_DEFS
        assert (
            binary_sensor.CONF_CHARGE_OVERTEMPERATURE_PROTECTION
            in binary_sensor.BINARY_SENSOR_DEFS
        )
        assert (
            binary_sensor.CONF_DISCHARGE_OVERTEMPERATURE_PROTECTION
            in binary_sensor.BINARY_SENSOR_DEFS
        )
        assert (
            binary_sensor.CONF_CHARGE_UNDERTEMPERATURE_PROTECTION
            in binary_sensor.BINARY_SENSOR_DEFS
        )
        assert (
            binary_sensor.CONF_DISCHARGE_UNDERTEMPERATURE_PROTECTION
            in binary_sensor.BINARY_SENSOR_DEFS
        )
        assert (
            binary_sensor.CONF_DISCHARGE_OVERCURRENT_PROTECTION
            in binary_sensor.BINARY_SENSOR_DEFS
        )
        assert (
            binary_sensor.CONF_CHARGE_OVERCURRENT_PROTECTION
            in binary_sensor.BINARY_SENSOR_DEFS
        )
        assert (
            binary_sensor.CONF_UNDERVOLTAGE_PROTECTION
            in binary_sensor.BINARY_SENSOR_DEFS
        )
        assert (
            binary_sensor.CONF_OVERVOLTAGE_PROTECTION
            in binary_sensor.BINARY_SENSOR_DEFS
        )
        assert len(binary_sensor.BINARY_SENSOR_DEFS) == 11


class TestTextSensorConstants:
    def test_text_sensors_list(self):
        assert text_sensor.CONF_ERRORS in text_sensor.TEXT_SENSORS
        assert len(text_sensor.TEXT_SENSORS) == 1
