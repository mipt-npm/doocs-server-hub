#include <doocs/Server.h>
#include "EqDevice.h"

#include <doocs_zmq.h>

std::vector<PropertyDescriptor> properties_from_json(const std::vector<nlohmann::json>& properties) {
    std::vector<PropertyDescriptor> result;
    result.reserve(properties.size());

    for (const auto& prop : properties) {
        result.push_back({
            prop["name"].get<std::string>(),
            prop["info"].get<std::string>(),
            prop["type"].get<int>(),
            prop["readable"].get<bool>(),
            prop["writable"].get<bool>()
        });
    }

    return result;
}

DeviceDescriptor acquire_device_description(const std::shared_ptr<ZmqConnection>& connection) {
    nlohmann::json acq = {
        {"id", std::time(nullptr)},
        {"origin", "doocs"},
        {"format", "json"},
        {"payload", { {"action", "names"} } }
    };

    connection->push_socket.send(acq.dump());
    zmqpp::poller poller;
    poller.add(connection->sub_socket);

    if (!poller.poll(1000)) {
        throw std::runtime_error("could not acquire device description\n");
    }

    std::string response_string;
    connection->push_socket.receive(response_string);

    nlohmann::json response = nlohmann::json::parse(response_string)["payload"];

    DeviceDescriptor desc = {
        response["name"].get<std::string>(),
        properties_from_json(response["properties"].get<std::vector<nlohmann::json>>())
    };

    return desc;
}

int main(int argc, char *argv[]) {
    zmqpp::context zmq_ctx;
    ZmqConnection zmq_connection = connect_to_zmq(zmq_ctx, argv[1]);
    auto connection = std::shared_ptr<ZmqConnection>(&zmq_connection);

    DeviceDescriptor desc;

    try {
        desc = acquire_device_description(connection);
    } catch (std::runtime_error &e) {
        printftostderr("main", "%s\n", e.what());
        return -1;
    }

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
