import esphome.codegen as cg
from esphome.components import ble_client
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@syssi"]

DEPENDENCIES = ["ble_client"]
AUTO_LOAD = ["binary_sensor", "sensor", "text_sensor"]
MULTI_CONF = True

CONF_TOPBAND_BMS_V1_BLE_ID = "topband_bms_v1_ble_id"

CONF_PROTOCOL_START_OF_FRAME = "protocol_start_of_frame"

topband_bms_v1_ble_ns = cg.esphome_ns.namespace("topband_bms_v1_ble")
TopbandBmsV1Ble = topband_bms_v1_ble_ns.class_(
    "TopbandBmsV1Ble", ble_client.BLEClientNode, cg.PollingComponent
)

TOPBAND_BMS_V1_BLE_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_TOPBAND_BMS_V1_BLE_ID): cv.use_id(TopbandBmsV1Ble),
    }
)

CONFIG_SCHEMA = cv.All(
    cv.require_esphome_version(2024, 12, 0),
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(TopbandBmsV1Ble),
            cv.Optional(CONF_PROTOCOL_START_OF_FRAME, default=0x5E): cv.hex_uint8_t,
        }
    )
    .extend(ble_client.BLE_CLIENT_SCHEMA)
    .extend(cv.polling_component_schema("10s")),
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await ble_client.register_ble_node(var, config)
    cg.add(var.set_protocol_start_of_frame(config[CONF_PROTOCOL_START_OF_FRAME]))
