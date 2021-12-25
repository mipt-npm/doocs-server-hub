#include <doocs/Server.h>
#include "EqDevice.h"

#include <doocs_zmq.h>

int main(int argc, char *argv[]) {
    // TODO connect to zmq
    std::shared_ptr<ZmqConnection> connection;

    // TODO ask for device properties via zmq
    DeviceDescriptor desc = {
        "EXAMPLE",
        {
            {"IR", "readonly property", DATA_INT, true, false},
            {"IW", "writeonly property", DATA_INT, false, true},
            {"IRW", "readwrite property", DATA_INT, true, true},
        }
    };

    doocs::Server server("hub_server");

    server.register_location_class(100, [&desc, &connection](){
      return std::make_unique<EqDevice>(desc.properties, connection, 100);
    });

    server.set_post_init_prolog([&desc](){
      add_location(100, desc.name);
    });

    server.run(argc, argv);

    return 0;
}
