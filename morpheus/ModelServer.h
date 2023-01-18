#ifndef MORHEUS_MODEL_SERVER_H
#define MORHEUS_MODEL_SERVER_H

#include <map>
#include <morpheus/defs.h>
#include <mutex>
#include <nos/inet/tcp_server.h>
#include <nos/shell/weaked_function.h>
#include <rabbit/mesh/mesh.h>
#include <string>
#include <thread>

class BodyId
{
    std::string _id;

public:
    BodyId(std::string id);
    ~BodyId() = default;

    bool operator<(const BodyId &other) const
    {
        // lexicographical comparison
        return _id < other._id;
    }
};

class Body
{
    rabbit::mesh<float> _mesh;
    std::vector<linalg::vec<unsigned int, 2>> _edges;
    rabbit::mat4f _pose_matrix = linalg::identity;

public:
    Body() = default;
    Body(rabbit::mesh<float> mesh);
    ~Body() = default;

    Body(const Body &other) = default;
    Body(Body &&other) = default;
    Body &operator=(const Body &other) = default;
    Body &operator=(Body &&other) = default;

    void set_pose(rabbit::mat4f pose_matrix)
    {
        _pose_matrix = pose_matrix;
    }

    void set_pose(ralgo::pose3<float> pose)
    {
        _pose_matrix = pose.to_mat4();
    }

    rabbit::mesh<float> &mesh()
    {
        return _mesh;
    }

    std::vector<linalg::vec<unsigned int, 2>> &edges()
    {
        return _edges;
    }

    rabbit::mat4f &pose_matrix()
    {
        return _pose_matrix;
    }
};

class ModelServer
{
    std::map<BodyId, Body> _models;
    nos::inet::tcp_server _tcp_server;
    std::thread _tcp_server_thread;
    nos::wf_collection _wf_collection;
    bool _cancel = false;
    uint16_t _port = MORPHEUS_DEFAULT_PORT;
    std::mutex _mutex;

public:
    ModelServer();
    ~ModelServer() = default;

    void add_model(BodyId id, Body &model);
    void set_model_pose(BodyId id, ralgo::pose3<float> pose);
    std::map<BodyId, Body> &models()
    {
        return _models;
    }

    void tcp_server_thread_func();
    static void client_thread_func(nos::inet::tcp_client client,
                                   ModelServer *server);

    void start_server();
    void stop_server();

    void command_add_model(std::string id, nos::trent mesh, nos::trent pose);
    void command_set_model_pose(std::string id, nos::trent pose);

    void execute_json(std::string json);
    void execute_trent(nos::trent trent);

    void lock()
    {
        _mutex.lock();
    }

    void unlock()
    {
        _mutex.unlock();
    }
};

#endif