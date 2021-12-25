#ifndef DESCRIPTORS_H_
#define DESCRIPTORS_H_

struct PropertyDescriptor {
  std::string name;
  std::string info;
  int type;
  bool readable;
  bool writable;
};

struct DeviceDescriptor {
  std::string name;
  std::vector<PropertyDescriptor> properties;
};

#endif //DESCRIPTORS_H_
