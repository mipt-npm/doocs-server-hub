#ifndef D_CUSTOM_PROP_H_
#define D_CUSTOM_PROP_H_

#include "descriptors.h"
#include "EqDevice.h"
#include <d_fct.h>
#include <doocs_zmq.h>

class D_prop_wrapper : public D_fct {
private:
  EqData data_;
  bool readable_;
  bool writable_;
  bool defined_;
  const D_long &timeout_;
  const D_long &store_;
  std::shared_ptr<ZmqConnection> connection_;

public:
  D_prop_wrapper(EqFct* ef, const D_long &timeout, const D_long &store, const PropertyDescriptor& descriptor, std::shared_ptr<ZmqConnection> connection);
  D_prop_wrapper(D_prop_wrapper &other) = delete;
  D_prop_wrapper(D_prop_wrapper &&other) = default;

  ~D_prop_wrapper() override;

  void get(EqAdr*, EqData* data_in, EqData* result, EqFct*) override;
  void set(EqAdr*, EqData* data_in, EqData* result, EqFct*) override;

  void write(std::ostream&) override {};
  int data_type() const override { return data_.type(); };

  int d_error() const override { return data_.error(); };
  void d_error(int err) override { data_.error(err); };


};

#endif //D_CUSTOM_PROP_H_
