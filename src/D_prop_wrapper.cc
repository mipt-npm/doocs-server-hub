#include <eq_fct.h>

#include <utility>
#include "D_prop_wrapper.h"

D_prop_wrapper::D_prop_wrapper(EqFct* ef, const PropertyDescriptor& descriptor, std::shared_ptr<ZmqConnection> connection)
  : D_fct(descriptor.name + " " + descriptor.info, ef)
  , readable_(descriptor.readable)
  , writable_(descriptor.writable)
  , connection_(std::move(connection)) {
  ef->add_property(this);
  data_.set_type(descriptor.type);
  data_.set(42); // TODO remove this set
}

D_prop_wrapper::~D_prop_wrapper() {
  if (efp_ != nullptr) {
    efp_->rem_property(this);
  }
}

void D_prop_wrapper::get(EqAdr* adr, EqData*, EqData* result, EqFct*) {
  if (!readable_) {
    result->error(eq_errors::read_error, "property is not readable");
  } else {
    result->copy_from(&data_); // TODO query zmq instead
  }
}

void D_prop_wrapper::set(EqAdr* adr, EqData* data_in, EqData* result, EqFct*) {
  if (!writable_) {
    result->error(eq_errors::write_error, "property is not writable");
  } else {
    data_.copy_from(data_in); // TODO query zmq instead
  }
}
