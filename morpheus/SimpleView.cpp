#include <morpheus/SimpleView.h>

#define GLEW_STATIC
#include <GL/glew.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <rabbit/camera.h>
#include <rabbit/opengl/drawer.h>
#include <rabbit/opengl/projection.h>
#include <rabbit/opengl/util.h>
#include <rabbit/util.h>

#include <chrono>
#include <thread>

SimpleView::SimpleView(ModelServer &server) : _server(server) {}

SimpleView::~SimpleView() {}

void SimpleView::init()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window =
        glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, WIDTH, HEIGHT);

    drawer.init_opengl_context();

    _window = window;
}

void SimpleView::draw() {}

void SimpleView::loop()
{

    float aspect = (float)WIDTH / (float)HEIGHT;
    rabbit::mat4f projection =
        rabbit::opengl_perspective(rabbit::deg(100) / aspect, aspect, 0.1, 200);

    while (!glfwWindowShouldClose(_window))
    {
        camera.set_camera(rabbit::vec3f{100.f * float(cos(glfwGetTime())),
                                        100.f * float(sin(glfwGetTime())),
                                        10.f},
                          {0, 0, 0});

        glfwPollEvents();

        drawer.clean(0.2f, 0.3f, 0.3f, 1.0f);

        for (auto &[id, model] : _server.models())
        {
            drawer.draw_mesh(model.mesh(),
                             model.pose_matrix(),
                             camera.view_matrix(),
                             projection,
                             rabbit::vec4f{1.0f, 0.5f, 0.2f, 1.0f});
        }

        glfwSwapBuffers(_window);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    glfwTerminate();
}