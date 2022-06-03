#ifndef EQDEVICE_H_
#define EQDEVICE_H_

#include <eq_fct.h>
#include "D_prop_wrapper.h"
#include "descriptors.h"
#include "doocs_zmq.h"

class EqDevice : public EqFct {
public:

  explicit EqDevice(const std::vector<PropertyDescriptor>& props, const std::shared_ptr<ZmqConnection>& connection, int code_);

  int fct_code() override { return code_; }
  void init() override;

private:
  std::vector<D_prop_wrapper> props_;
  D_long timeout_;
  D_long store_;
  int code_;
};

#endif // EQDEVICE_H_
