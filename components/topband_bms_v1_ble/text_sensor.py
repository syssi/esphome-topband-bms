import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import ENTITY_CATEGORY_DIAGNOSTIC

from . import CONF_TOPBAND_BMS_V1_BLE_ID, TOPBAND_BMS_V1_BLE_COMPONENT_SCHEMA

CODEOWNERS = ["@syssi"]
DEPENDENCIES = ["topband_bms_v1_ble"]

CONF_ERRORS = "errors"

TEXT_SENSORS = [
    CONF_ERRORS,
]

CONFIG_SCHEMA = TOPBAND_BMS_V1_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_ERRORS): text_sensor.text_sensor_schema(
            icon="mdi:alert-circle-outline",
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_TOPBAND_BMS_V1_BLE_ID])
    for key in TEXT_SENSORS:
        if key in config:
            sens = await text_sensor.new_text_sensor(config[key])
            cg.add(getattr(hub, f"set_{key}_text_sensor")(sens))
