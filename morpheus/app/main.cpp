#include <morpheus/ModelServer.h>
#include <morpheus/SimpleView.h>
#include <rabbit/mesh/primitives.h>

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <thread>

int main(int argc, char **argv)
{
    rabbit::mesh<float> mesh = rabbit::box_mesh(10.f, 10.f, 10.f);
    Body model(mesh);

    ModelServer server;
    server.add_model(BodyId("box"), model);

    SimpleView view(server);
    view.init();
    view.loop();
    return 0;
}