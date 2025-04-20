#pragma once

#include <iostream>

#include <GLFW/glfw3.h>

template <typename W = GLFWwindow>
class Application
{
    public:
        Application(const char* title, int width, int height);
        W* get_window();
        ~Application();
    private:
        GlfwInitResult initialize_glfw();
        GladInitResult initialize_glad();
        GlfwWindowCreateResult initialize_window(const char* title, int width, int height);
        W* window;
};