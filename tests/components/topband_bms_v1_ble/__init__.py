import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

from tests.testing_helpers import ComponentManifestOverride

topband_bms_v1_ble_ns = cg.esphome_ns.namespace("topband_bms_v1_ble")
TopbandBmsV1Ble = topband_bms_v1_ble_ns.class_("TopbandBmsV1Ble", cg.PollingComponent)


def override_manifest(manifest: ComponentManifestOverride) -> None:
    manifest.dependencies = []

    manifest.config_schema = cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(TopbandBmsV1Ble),
        }
    ).extend(cv.polling_component_schema("10s"))

    async def to_code_stub(config):
        var = cg.new_Pvariable(config[CONF_ID])
        await cg.register_component(var, config)

    manifest.to_code = to_code_stub
