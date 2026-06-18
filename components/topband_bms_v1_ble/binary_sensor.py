import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import (
    DEVICE_CLASS_CONNECTIVITY,
    DEVICE_CLASS_PROBLEM,
    ENTITY_CATEGORY_DIAGNOSTIC,
)

from . import CONF_TOPBAND_BMS_V1_BLE_ID, TOPBAND_BMS_V1_BLE_COMPONENT_SCHEMA

CODEOWNERS = ["@syssi"]
DEPENDENCIES = ["topband_bms_v1_ble"]

CONF_ONLINE_STATUS = "online_status"
CONF_CHARGING = "charging"
CONF_DISCHARGING = "discharging"
CONF_CHARGE_OVERTEMPERATURE_PROTECTION = "charge_overtemperature_protection"
CONF_DISCHARGE_OVERTEMPERATURE_PROTECTION = "discharge_overtemperature_protection"
CONF_CHARGE_UNDERTEMPERATURE_PROTECTION = "charge_undertemperature_protection"
CONF_DISCHARGE_UNDERTEMPERATURE_PROTECTION = "discharge_undertemperature_protection"
CONF_DISCHARGE_OVERCURRENT_PROTECTION = "discharge_overcurrent_protection"
CONF_CHARGE_OVERCURRENT_PROTECTION = "charge_overcurrent_protection"
CONF_UNDERVOLTAGE_PROTECTION = "undervoltage_protection"
CONF_OVERVOLTAGE_PROTECTION = "overvoltage_protection"

BINARY_SENSOR_DEFS = {
    CONF_ONLINE_STATUS: {
        "device_class": DEVICE_CLASS_CONNECTIVITY,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_CHARGING: {"icon": "mdi:battery-charging"},
    CONF_DISCHARGING: {"icon": "mdi:power-plug"},
    CONF_CHARGE_OVERTEMPERATURE_PROTECTION: {
        "device_class": DEVICE_CLASS_PROBLEM,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_DISCHARGE_OVERTEMPERATURE_PROTECTION: {
        "device_class": DEVICE_CLASS_PROBLEM,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_CHARGE_UNDERTEMPERATURE_PROTECTION: {
        "device_class": DEVICE_CLASS_PROBLEM,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_DISCHARGE_UNDERTEMPERATURE_PROTECTION: {
        "device_class": DEVICE_CLASS_PROBLEM,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_DISCHARGE_OVERCURRENT_PROTECTION: {
        "device_class": DEVICE_CLASS_PROBLEM,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_CHARGE_OVERCURRENT_PROTECTION: {
        "device_class": DEVICE_CLASS_PROBLEM,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_UNDERVOLTAGE_PROTECTION: {
        "device_class": DEVICE_CLASS_PROBLEM,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_OVERVOLTAGE_PROTECTION: {
        "device_class": DEVICE_CLASS_PROBLEM,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
}

CONFIG_SCHEMA = TOPBAND_BMS_V1_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(key): binary_sensor.binary_sensor_schema(**kwargs)
        for key, kwargs in BINARY_SENSOR_DEFS.items()
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_TOPBAND_BMS_V1_BLE_ID])
    for key in BINARY_SENSOR_DEFS:
        if key in config:
            sens = await binary_sensor.new_binary_sensor(config[key])
            cg.add(getattr(hub, f"set_{key}_binary_sensor")(sens))
