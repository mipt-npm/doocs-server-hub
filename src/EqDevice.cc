#include <doocs/Server.h>

#include "EqDevice.h"

EqDevice::EqDevice(const std::vector<PropertyDescriptor>& props, const std::shared_ptr<ZmqConnection>& connection, int code)
    : EqFct("NAME location name")
    , code_(code) {
  props_.reserve(props.size());
  for (const auto& prop : props) {
    props_.emplace_back(this, prop, connection);
  }
}

void EqDevice::update() {
}
