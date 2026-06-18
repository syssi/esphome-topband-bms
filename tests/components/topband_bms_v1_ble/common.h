#pragma once
#include <cstdint>
#include <vector>
#include "esphome/components/topband_bms_v1_ble/topband_bms_v1_ble.h"

namespace esphome::topband_bms_v1_ble::testing {

class TestableTopbandBmsV1Ble : public TopbandBmsV1Ble {
 public:
  using TopbandBmsV1Ble::on_topband_bms_v1_ble_data;
  using TopbandBmsV1Ble::track_online_status_;
  using TopbandBmsV1Ble::reset_online_status_tracker_;
  using TopbandBmsV1Ble::publish_device_unavailable_;

  uint8_t get_no_response_count() const { return no_response_count_; }
};

}  // namespace esphome::topband_bms_v1_ble::testing
