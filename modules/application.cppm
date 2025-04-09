export module application;

#include <GLFW/glfw3.h>;
#include <iostream>;
import numbers_common;

export template<typename T = Ok<int>, typename W = GLFWwindow>
class Application
{
    private:
        W *window;
    
        T initialize_glfw() {
            if (!glfwInit())
                throw std::runtime_error("Failed to initialize GLFW.");
            return make_ok(0);

        }

    public:
}