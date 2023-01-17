#include <morpheus/ModelServer.h>
#include <nos/input.h>
#include <nos/trent/json.h>
#include <nos/trent/json_print.h>
#include <thread>

ModelServer::ModelServer() {}

void ModelServer::add_model(BodyId id, Body &model)
{
    _models[id] = model;
}

Body::Body(rabbit::mesh<float> mesh) : _mesh(mesh) {}

BodyId::BodyId(std::string id) : _id(id) {}

void ModelServer::start_server()
{
    _tcp_server_thread = std::thread([this]() { tcp_server_thread_func(); });
}

void ModelServer::client_thread_func(nos::inet::tcp_client client)
{
    char buffer[1024];
    while (1)
    {
        auto ret = nos::read_paired_from(client, buffer, 1024, '{', '}', true);
        if (!ret)
        {
            std::cout << "Client disconnected" << std::endl;
            return;
        }

        auto msg = ret.value();
        auto json = nos::json::parse(buffer);
        std::cout << "Received: " << nos::json::to_string(json) << std::endl;

        std::string answer = "OK";
        client.write(answer.data(), answer.size());
    }
}

void ModelServer::tcp_server_thread_func()
{
    _tcp_server.init();
    _tcp_server.bind("0.0.0.0", 18923);
    _tcp_server.listen();

    while (1)
    {
        auto client = _tcp_server.accept();
        std::cout << "Client connected" << std::endl;

        new std::thread(client_thread_func, client);
    }
}