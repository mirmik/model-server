#include <morpheus/ModelServer.h>

ModelServer::ModelServer() {}

void ModelServer::add_model(BodyId id, Body &model)
{
    _models[id] = model;
}

Body::Body(rabbit::mesh<float> mesh) : _mesh(mesh) {}

BodyId::BodyId(std::string id) : _id(id) {}