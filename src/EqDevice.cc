#include <doocs/Server.h>

#include "EqDevice.h"

EqDevice::EqDevice(const std::vector<PropertyDescriptor>& props, const std::shared_ptr<ZmqConnection>& connection, int code)
    : EqFct("NAME location name")
    , timeout_("TIMEOUT", this)
    , store_("STORE", this)
    , code_(code) {
  props_.reserve(props.size());
  for (const auto& prop : props) {
    props_.emplace_back(this, timeout_, store_, prop, connection);
  }
}

void EqDevice::init() {
    timeout_.set_value(100);
}
