import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_CAPACITY,
    CONF_CURRENT,
    CONF_POWER,
    CONF_TEMPERATURE,
    DEVICE_CLASS_BATTERY,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_EMPTY,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_EMPTY,
    UNIT_PERCENT,
    UNIT_VOLT,
    UNIT_WATT,
)

from . import CONF_TOPBAND_BMS_V1_BLE_ID, TOPBAND_BMS_V1_BLE_COMPONENT_SCHEMA

CODEOWNERS = ["@syssi"]
DEPENDENCIES = ["topband_bms_v1_ble"]

CONF_TOTAL_VOLTAGE = "total_voltage"
CONF_CHARGING_POWER = "charging_power"
CONF_DISCHARGING_POWER = "discharging_power"
CONF_STATE_OF_CHARGE = "state_of_charge"
CONF_CHARGING_CYCLES = "charging_cycles"
CONF_ERROR_BITMASK = "error_bitmask"
CONF_AFE_STATUS_BITMASK = "afe_status_bitmask"
CONF_MIN_CELL_VOLTAGE = "min_cell_voltage"
CONF_MAX_CELL_VOLTAGE = "max_cell_voltage"
CONF_MIN_VOLTAGE_CELL = "min_voltage_cell"
CONF_MAX_VOLTAGE_CELL = "max_voltage_cell"
CONF_DELTA_CELL_VOLTAGE = "delta_cell_voltage"
CONF_AVERAGE_CELL_VOLTAGE = "average_cell_voltage"

ICON_CURRENT_DC = "mdi:current-dc"
ICON_CAPACITY = "mdi:battery-50"
ICON_CHARGING_CYCLES = "mdi:battery-sync"
ICON_MIN_CELL_VOLTAGE = "mdi:battery-minus-outline"
ICON_MAX_CELL_VOLTAGE = "mdi:battery-plus-outline"
ICON_ERROR_BITMASK = "mdi:alert-circle-outline"

UNIT_AMPERE_HOURS = "Ah"

CELLS = [f"cell_voltage_{i}" for i in range(1, 17)]

SENSOR_DEFS = {
    CONF_TOTAL_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_CURRENT: {
        "unit_of_measurement": UNIT_AMPERE,
        "icon": ICON_CURRENT_DC,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_CURRENT,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_CHARGING_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_DISCHARGING_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_STATE_OF_CHARGE: {
        "unit_of_measurement": UNIT_PERCENT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_BATTERY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_CHARGING_CYCLES: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_CHARGING_CYCLES,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
    CONF_TEMPERATURE: {
        "unit_of_measurement": UNIT_CELSIUS,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_CAPACITY: {
        "unit_of_measurement": UNIT_AMPERE_HOURS,
        "icon": ICON_CAPACITY,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_ERROR_BITMASK: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_ERROR_BITMASK,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_AFE_STATUS_BITMASK: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_ERROR_BITMASK,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_MIN_CELL_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_MIN_CELL_VOLTAGE,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_MAX_CELL_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_MAX_CELL_VOLTAGE,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_MIN_VOLTAGE_CELL: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_MIN_CELL_VOLTAGE,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_MAX_VOLTAGE_CELL: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_MAX_CELL_VOLTAGE,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_DELTA_CELL_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_AVERAGE_CELL_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 4,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
}

_CELL_VOLTAGE_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_VOLT,
    icon=ICON_EMPTY,
    accuracy_decimals=3,
    device_class=DEVICE_CLASS_VOLTAGE,
    state_class=STATE_CLASS_MEASUREMENT,
)

CONFIG_SCHEMA = TOPBAND_BMS_V1_BLE_COMPONENT_SCHEMA.extend(
    {
        **{
            cv.Optional(key): sensor.sensor_schema(**kwargs)
            for key, kwargs in SENSOR_DEFS.items()
        },
        **{cv.Optional(key): _CELL_VOLTAGE_SCHEMA for key in CELLS},
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_TOPBAND_BMS_V1_BLE_ID])
    for i, key in enumerate(CELLS):
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(hub.set_cell_voltage_sensor(i, sens))
    for key in SENSOR_DEFS:
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(hub, f"set_{key}_sensor")(sens))
