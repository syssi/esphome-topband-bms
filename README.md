# esphome-topband-bms

![GitHub actions](https://github.com/syssi/esphome-topband-bms/actions/workflows/ci.yaml/badge.svg)
![GitHub stars](https://img.shields.io/github/stars/syssi/esphome-topband-bms)
![GitHub forks](https://img.shields.io/github/forks/syssi/esphome-topband-bms)
![GitHub watchers](https://img.shields.io/github/watchers/syssi/esphome-topband-bms)
[!["Buy Me A Coffee"](https://img.shields.io/badge/buy%20me%20a%20coffee-donate-yellow.svg)](https://www.buymeacoffee.com/syssi)

ESPHome component to monitor a Topband Battery Management System via BLE

## Supported devices

Protocol version is detected automatically from the first notification byte (`0xAA` = v2, anything else = v1).

### topband_bms_v1_ble (ASCII push, older firmware)

Devices advertising via BLE with service UUID `FFE0`:

* Offgridtec LiFePO4 battery (older firmware)<br>
  Typically advertised as `SmartBattery-XXXXXX`

* Topband BMS with older firmware (pre-2022)<br>
  Typically advertised as `SmartBattery-XXXXXX`

### topband_bms_v2_ble (TSmartBle binary, newer firmware)

Not yet implemented.

## Requirements

* [ESPHome 2024.12.0 or higher](https://github.com/esphome/esphome/releases)
* Generic ESP32 board

## Installation

You can install this component with [ESPHome external components feature](https://esphome.io/components/external_components.html) like this:
```yaml
external_components:
  - source: github://syssi/esphome-topband-bms@main
```

or just use the `esp32-v1-ble-example.yaml` as proof of concept:

```bash
# Install esphome
pip3 install esphome

# Clone this external component
git clone https://github.com/syssi/esphome-topband-bms.git
cd esphome-topband-bms

# Create a secrets.yaml containing some setup specific secrets
cat > secrets.yaml <<EOF
bms0_mac_address: MY_BMS_MAC_ADDRESS

wifi_ssid: MY_WIFI_SSID
wifi_password: MY_WIFI_PASSWORD

mqtt_host: MY_MQTT_HOST
mqtt_username: MY_MQTT_USERNAME
mqtt_password: MY_MQTT_PASSWORD
EOF

# Validate the configuration, create a binary, upload it, and start logs
esphome run esp32-v1-ble-example.yaml
```

## Example response all sensors enabled

```
[D][topband_bms_v1_ble:076]: Received frame: 66 bytes, 4 cells
[D][topband_bms_v1_ble:110]: Voltage: 13.600 V  Current: -1.500 A  Power: -20.40 W
[D][topband_bms_v1_ble:111]: SOC: 70%  Cycles: 26  Temp: 23.4 degC
[D][topband_bms_v1_ble:112]: Status: 0x00  AFE: 0x00  Capacity: 100.000 Ah
[D][sensor:094]: 'topband-bms-v1-ble total voltage': Sending state 13.60000 V with 3 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble current': Sending state -1.50000 A with 3 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble power': Sending state -20.40000 W with 2 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble charging power': Sending state 0.00000 W with 2 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble discharging power': Sending state 20.40000 W with 2 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble state of charge': Sending state 70.00000 % with 0 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble charging cycles': Sending state 26.00000  with 0 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble temperature': Sending state 23.35001 degC with 1 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble capacity': Sending state 100.00000 Ah with 3 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble error bitmask': Sending state 0.00000  with 0 decimals of accuracy
[D][binary_sensor:036]: 'topband-bms-v1-ble charging': Sending state OFF
[D][binary_sensor:036]: 'topband-bms-v1-ble discharging': Sending state ON
[D][text_sensor:064]: 'topband-bms-v1-ble errors': Sending state 'OK'
[D][topband_bms_v1_ble:130]: Cell 1: 3.400 V
[D][topband_bms_v1_ble:130]: Cell 2: 3.400 V
[D][topband_bms_v1_ble:130]: Cell 3: 3.400 V
[D][topband_bms_v1_ble:130]: Cell 4: 3.400 V
[D][sensor:094]: 'topband-bms-v1-ble cell voltage 1': Sending state 3.40000 V with 3 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble cell voltage 2': Sending state 3.40000 V with 3 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble cell voltage 3': Sending state 3.40000 V with 3 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble cell voltage 4': Sending state 3.40000 V with 3 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble min cell voltage': Sending state 3.40000 V with 3 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble max cell voltage': Sending state 3.40000 V with 3 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble delta cell voltage': Sending state 0.00000 V with 3 decimals of accuracy
[D][sensor:094]: 'topband-bms-v1-ble average cell voltage': Sending state 3.40000 V with 4 decimals of accuracy
[D][binary_sensor:036]: 'topband-bms-v1-ble online status': Sending state ON
```

## Protocol

TBD.

## Known issues

None.

## Debugging

If this component doesn't work out of the box for your device please update your configuration to increase the log level to see details about the BLE communication and incoming traffic:

```yaml
logger:
  level: VERY_VERBOSE
  logs:
    esp32_ble: DEBUG
    esp32_ble_tracker: VERY_VERBOSE
    topband_bms_v1_ble: VERY_VERBOSE
    scheduler: DEBUG
    component: DEBUG
    sensor: DEBUG
    mqtt: INFO
    mqtt.idf: INFO
    mqtt.component: INFO
    mqtt.sensor: INFO
    mqtt.switch: INFO
    api.service: INFO
    api: INFO
```

## References

* [patman15/BMS_BLE-HA](https://github.com/patman15/BMS_BLE-HA)
