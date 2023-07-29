#pragma region includes
/*
GLEW is the OpenGL Extension Wrangler Library which is a simple tool that helps C/C++ developers
initialize extensions and write portable applications.
http://glew.sourceforge.net/
*/
#include <GL/glew.h>

/*
Dear ImGui is a bloat-free graphical user interface library for C++.
https://github.com/ocornut/imgui
*/
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

/*
GLFW is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan development on the desktop.
It provides a simple API for creating windows, contexts and surfaces, receiving input and events.
https://www.glfw.org/
*/
#include <GLFW/glfw3.h>

/*
OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software based on the
OpenGL Shading Language (GLSL) specifications.
https://github.com/g-truc/glm
*/
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
//#include <string>
#include <time.h>

#include "helpers/GuiHelpers.h"
#include "helpers/RenderHelpers.h"

#include "Camera.h"
#include "World.h"

#pragma endregion includes

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(12.0f, 20.f, 40.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

GLFWwindow* window;
IRenderHelpers* renderHelper;
IGuiHelpers* guiHelper;

World world;

bool isPaused;
bool verletButtonClicked;
bool eulerButtonClicked;
bool rungeKuttaButtonClicked;


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, Camera& pCamera, float pDeltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        pCamera.ProcessKeyboard(FORWARD, pDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        pCamera.ProcessKeyboard(BACKWARD, pDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            pCamera.ProcessKeyboard(LEFT, pDeltaTime);
        else
            pCamera.ProcessKeyboard(ROTATE_LEFT, pDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            pCamera.ProcessKeyboard(RIGHT, pDeltaTime);
        else
            pCamera.ProcessKeyboard(ROTATE_RIGHT, pDeltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

GLFWwindow* setupWindow()
{
    GLFWwindow* window;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    /* Initialize the library */
    if (!glfwInit())
        return 0;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello, ACW!", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return 0;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return window;
}

void renderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Hello, IMGUI!");
    ImGui::Text("Movement Method");
    
    if (ImGui::Button("Euler"))
    {
        eulerButtonClicked = true;
    }
    if (ImGui::Button("Verlet"))
    {
        verletButtonClicked = true;
    }
    if (ImGui::Button("RungeKutta"))
    {
        rungeKuttaButtonClicked = false;
    }
    if (isPaused)
    {
        if (ImGui::Button("Start"))
        {
            isPaused = false;
        }
    }
    else
    {
        if (ImGui::Button("Stop"))
        {
            isPaused = true;
        }
    }

    static glm::vec3 ball1Position;
    static glm::vec3 ball1Velocity;
    static float ball1Radius;

    camera.RenderGui(*guiHelper);

    if (!isPaused)
    {
        world.RenderGui(*guiHelper);
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static void renderOpenGL()
{
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.Render(*renderHelper);
    world.Render(*renderHelper);
}

int main(void)
{
    window = setupWindow();
    renderHelper = new OpenGLRenderHelpers();
    guiHelper = new ImGuiHelpers();
    renderHelper->Init();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    renderHelper->SetSize(display_w, display_h);

    double lastTime = glfwGetTime();
    double timeNow = lastTime;
    double renderTime = 1.0 / 60.0;
    double updateTime = 1.0 / 100.0f;

    isPaused = false;

    while (!glfwWindowShouldClose(window))
    {
        timeNow = glfwGetTime();
        double elaspedTime = timeNow - lastTime;
        timeNow = lastTime;

        renderTime -= elaspedTime;

        // render 60 times a second
        if (renderTime < 0)
        {
            renderTime = 1.0 / 60.0;
            renderOpenGL();
            renderImGui();
            glfwSwapBuffers(window);
        }

        glm::vec3 Y(0.0f, 1.0f, 0.0f);
        float rotationSpeed = 2 * glm::pi<float>() / 5.f;

        glm::vec3 X(1.0f, 0.0f, 0.0f);
        if (verletButtonClicked)
        {
            world.ChangeIntegrationMethod(IntegrationMethod::Verlet);
            verletButtonClicked = false;
        }
        if (eulerButtonClicked)
        {
            world.ChangeIntegrationMethod(IntegrationMethod::Euler);
            eulerButtonClicked = false;
        }
        if (rungeKuttaButtonClicked)
        {
            world.ChangeIntegrationMethod(IntegrationMethod::RungeKutta);
            rungeKuttaButtonClicked = false;
        }

        if (!isPaused)
        {
            updateTime -= elaspedTime;
            // update physics 100 times a second
            if (updateTime < 0)
            {
                updateTime = 1.0 / 100.0f;
                world.Update(updateTime);
            }
            processInput(window, camera, updateTime);
        }
        else
        {
            processInput(window, camera, updateTime);
        }

        /* Poll for and process events */
        glfwPollEvents();
    }

    delete renderHelper;
    delete guiHelper;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

	return 0;
}
