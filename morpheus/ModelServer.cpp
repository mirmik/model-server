#include <morpheus/ModelServer.h>
#include <nos/input.h>
#include <nos/trent/json.h>
#include <nos/trent/json_print.h>
#include <rabbit/types.h>
#include <ralgo/space/pose3.h>
#include <thread>

ModelServer::ModelServer()
{
    _wf_collection.add("hello",
                       std::function<std::string(std::string)>(
                           [](std::string s) { return "Hello " + s; }));
    _wf_collection.add(
        "add_model",
        std::function<void(std::string, nos::trent, nos::trent)>(
            [this](std::string id, nos::trent mesh_trent, nos::trent pose_trent)
            { command_add_model(id, mesh_trent, pose_trent); }));
    _wf_collection.add("set_model_pose",
                       std::function<void(std::string, nos::trent)>(
                           [this](std::string id, nos::trent pose_trent)
                           { command_set_model_pose(id, pose_trent); }));
}

void ModelServer::add_model(BodyId id, Body &model)
{
    _models[id] = model;
}

void ModelServer::command_add_model(std::string id,
                                    nos::trent mesh_trent,
                                    nos::trent pose_trent)
{
    nos::fprintln("Command: add_model: id:{} mesh:{}",
                  id,
                  nos::json::to_string(mesh_trent));
    auto mesh = rabbit::mesh<float>::from_trent(mesh_trent);
    Body body(mesh);
    body.set_pose(ralgo::pose3<float>::from_trent(pose_trent));
    add_model(BodyId(id), body);
}

void ModelServer::set_model_pose(BodyId id, ralgo::pose3<float> pose)
{
    _models[id].set_pose(pose);
}

void ModelServer::command_set_model_pose(std::string id, nos::trent pose_trent)
{
    nos::fprintln("Command: set_model_pose: id:{} pose:{}",
                  id,
                  nos::json::to_string(pose_trent));
    auto pose = ralgo::pose3<float>::from_trent(pose_trent);
    set_model_pose(BodyId(id), pose);
}

Body::Body(rabbit::mesh<float> mesh) : _mesh(mesh)
{
    _edges = rabbit::edges_from_triangles(_mesh.triangles());
}

BodyId::BodyId(std::string id) : _id(id) {}

void ModelServer::start_server()
{
    _tcp_server_thread = std::thread([this]() { tcp_server_thread_func(); });
}

void ModelServer::client_thread_func(nos::inet::tcp_client client,
                                     ModelServer *server)
{
    char buffer[10240];
    while (1)
    {
        memset(buffer, 0, 1024);
        auto ret = nos::read_paired_from(client, buffer, 10240, '{', '}', true);
        if (!ret)
        {
            std::cout << "Client disconnected" << std::endl;
            return;
        }

        auto len = ret.value();
        if (len == 0)
        {
            std::cout << "Client disconnected" << std::endl;
            return;
        }

        auto tr = nos::json::parse(buffer);
        std::cout << "Received: " << nos::json::to_string(tr) << std::endl;
        // to trent

        // execute
        server->execute_trent(tr);

        // std::string answer = "OK";
        // client.write(answer.data(), answer.size());
    }
}

void ModelServer::tcp_server_thread_func()
{
    _tcp_server.init();
    _tcp_server.bind("0.0.0.0", _port);
    _tcp_server.listen();

    while (1)
    {
        auto client = _tcp_server.accept();
        if (_cancel)
        {
            return;
        }
        std::cout << "Client connected" << std::endl;
        new std::thread(client_thread_func, client, this);
    }
}

void ModelServer::stop_server()
{
    _cancel = true;
    // connect to the server to make it stop
    auto client = nos::inet::tcp_client::dial("127.0.0.1", _port);
    _tcp_server_thread.join();
}

void ModelServer::execute_json(std::string json)
{
    auto answer_json = _wf_collection.execute_json(json);
    auto answer = nos::json::to_string(answer_json);
    std::cout << "Answer: " << answer << std::endl;
}

void ModelServer::execute_trent(nos::trent trent)
{
    std::lock_guard<std::mutex> lock(_mutex);
    auto answer_trent = _wf_collection.execute_trent(trent);
    auto answer = nos::json::to_string(answer_trent);
    std::cout << "Answer: " << answer << std::endl;
}