#include <morpheus/ModelServer.h>
#include <morpheus/SimpleView.h>
#include <nos/trent/json_print.h>
#include <rabbit/mesh/primitives.h>
#include <igris/getopt/cliopts.h>

#include <rabbit/mesh/primitives.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <thread>

void add_scene0( ModelServer & server )
{
    nos::println("add_scene0");
    auto mesh = rabbit::box_mesh(1, 1, 1);
    auto body = Body(mesh);
    server.add_model(BodyId("box"), body);
}

int main(int argc, char **argv)
{
    igris::cliopts parser;
    parser.add_string("scene", 's', "none");
    parser.parse(argc, argv);

    std::string scene = parser.get_string("scene");

    ModelServer server;
    server.start_server();

    if (scene == "scene0")
    {
        add_scene0(server);
    }

    SimpleView view(server);
    view.set_finalizer([&server]() { server.stop_server(); });
    view.init();
    view.loop();
    return 0;
}