#include "../headers/Shader.h"
#include "../headers/ResourceManager.h"
#include "../headers/Game.h"
#include "../headers/Texture2D.h"
#include "../headers/GameObject.h"

#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 800;
const unsigned int FPS_LIMIT = 120;

Game TicTacToe(SCREEN_WIDTH, SCREEN_HEIGHT);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TicTacToe", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialize game
    // ---------------
    TicTacToe.Init();

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    double prevTime = 0.0f;
    double crntTime = 0.0f;
    double timeDiff;
    unsigned int counter = 0;

    while (!glfwWindowShouldClose(window))
    {
        //fps time
        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        counter++;
        if (timeDiff >= 1.0 / 10.0) {
            std::string FPS = std::to_string((1.0 / timeDiff) * counter);
            std::string ms = std::to_string((timeDiff / counter) * 1000);
            std::string title = "TicTacToe - fps: " + FPS + " ms: " + ms;
            glfwSetWindowTitle(window, title.c_str());
            prevTime = crntTime;
            counter = 0;
        }

        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // manage user input
        // -----------------
        TicTacToe.ProcessInput(deltaTime);

        // update game state
        // -----------------
        TicTacToe.Update(deltaTime);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        TicTacToe.Render();

        glfwSwapBuffers(window);

        // Frame rate limiter
        // ------------------
        float frameTime = glfwGetTime() - currentFrame;
        if (frameTime < 1.0f / FPS_LIMIT) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((1.0f / FPS_LIMIT - frameTime) * 1000.0f)));
        }
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            TicTacToe.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            TicTacToe.Keys[key] = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// Function to check if the mouse click is within the bounds of a game object
bool IsMouseOver(GameObject& object, double mouseX, double mouseY) {
    return mouseX >= object.Position.x && mouseX <= object.Position.x + object.Size.x &&
           mouseY >= object.Position.y && mouseY <= object.Position.y + object.Size.y;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        // Convert y coordinate to match OpenGL's coordinate system

        // Check if the mouse click is on any game object
        for (int i = 0; i < TicTacToe.Squares.size(); i++) {
            GameObject& object = TicTacToe.Squares[i];
            if (IsMouseOver(object, x, y)) {
                if (TicTacToe.turn % 2 == 0 && object.State == Empty) {
                    object.State = Cross;
                    TicTacToe.turn++;
                }
                else if (object.State == Empty){
                    object.State = Knot;
                    TicTacToe.turn++;
                }
                // Perform any additional actions on the object here
            }
        }
    }
}
