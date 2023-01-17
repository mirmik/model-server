#ifndef MORPHEUS_SIMPLE_VIEW_H
#define MORPHEUS_SIMPLE_VIEW_H

#include <morpheus/ModelServer.h>
#include <rabbit/camera.h>
#include <rabbit/opengl/drawer.h>
#include <rabbit/opengl/util.h>

#include <GLFW/glfw3.h>

class SimpleView
{
    rabbit::camera camera;
    ModelServer &_server;
    GLFWwindow *_window;
    const GLuint WIDTH = 1600, HEIGHT = 800;

public:
    rabbit::opengl_drawer drawer;

public:
    SimpleView(ModelServer &server);
    ~SimpleView();

    void init();
    void draw();
    void loop();

    GLFWwindow *window()
    {
        return _window;
    }
};

#endif