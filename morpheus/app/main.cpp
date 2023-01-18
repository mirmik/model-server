#include <morpheus/ModelServer.h>
#include <morpheus/SimpleView.h>
#include <nos/trent/json_print.h>
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
    ModelServer server;
    server.start_server();

    SimpleView view(server);
    view.set_finalizer([&server]() { server.stop_server(); });
    view.init();
    view.loop();
    return 0;
}