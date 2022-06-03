#include <eq_fct.h>

#include <utility>
#include "D_prop_wrapper.h"

D_prop_wrapper::D_prop_wrapper(EqFct* ef, const D_long &timeout, const D_long &store, const PropertyDescriptor& descriptor, std::shared_ptr<ZmqConnection> connection)
  : D_fct(descriptor.name + " " + descriptor.info, ef)
  , data_(nullptr)
  , readable_(descriptor.readable)
  , writable_(descriptor.writable)
  , defined_(false)
  , timeout_(timeout)
  , store_(store)
  , connection_(std::move(connection)) {
  ef->add_property(this);
  data_.set_type(descriptor.type);
}

D_prop_wrapper::~D_prop_wrapper() {
  if (efp_ != nullptr) {
    efp_->rem_property(this);
  }
}

void D_prop_wrapper::get(EqAdr* adr, EqData* data_in, EqData* result, EqFct*) {
  if (!readable_) {
    result->error(eq_errors::read_error, "property is not readable");
  } else {
    auto request = request_magix(adr, data_in);
    std::string request_string = request.dump();
    connection_->push_socket.send(request_string);
    if (connection_->poller.poll(timeout_.get_value())) {
        std::string response_string;
        connection_->sub_socket.receive(request_string);
        nlohmann::json response = nlohmann::json::parse(response_string);
        result->copy_from(eq_data_from_json(response).get());
        defined_ = true;
    } else if (defined_ && get_timestamp() < data_.get_timestamp()) {
      result->copy_from(&data_);
    } else {
        result->error(eq_errors::read_error, "could not get property");
        defined_ = false;
    }
  }
}

void D_prop_wrapper::set(EqAdr* adr, EqData* data_in, EqData* result, EqFct*) {
  if (!writable_) {
    result->error(eq_errors::write_error, "property is not writable");
  } else {
      auto request = request_magix(adr, data_in);
      std::string request_string = request.dump();
      connection_->push_socket.send(request_string);
      if (connection_->poller.poll(timeout_.get_value())) {
          std::string response_string;
          connection_->sub_socket.receive(request_string);
          nlohmann::json response = nlohmann::json::parse(response_string);
          result->copy_from(eq_data_from_json(response).get());
          defined_ = true;
      } else if (defined_ && get_timestamp() < data_.get_timestamp()) {
          result->copy_from(&data_);
      } else {
          result->error(eq_errors::read_error, "could not set property");
          defined_ = false;
      }
  }
}
