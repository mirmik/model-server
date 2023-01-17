#ifndef MORHEUS_MODEL_SERVER_H
#define MORHEUS_MODEL_SERVER_H

#include <map>
#include <rabbit/mesh/mesh.h>
#include <string>

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
    rabbit::mat4f _pose_matrix = linalg::identity;

public:
    Body() = default;
    Body(rabbit::mesh<float> mesh);
    ~Body() = default;

    Body(const Body &other) = default;
    Body(Body &&other) = default;
    Body &operator=(const Body &other) = default;
    Body &operator=(Body &&other) = default;

    rabbit::mesh<float> &mesh()
    {
        return _mesh;
    }

    rabbit::mat4f &pose_matrix()
    {
        return _pose_matrix;
    }
};

class ModelServer
{
    std::map<BodyId, Body> _models;

public:
    ModelServer();
    ~ModelServer() = default;

    void add_model(BodyId id, Body &model);
    std::map<BodyId, Body> &models()
    {
        return _models;
    }
};

#endif