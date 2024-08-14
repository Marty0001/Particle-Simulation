#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "ParticleSystem.h"

const int PARTICLE_COUNT = 100000;
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;


// Resizes the window when changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    // Setup an orthographic projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(void) {
    // Initialize GLFW
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Particle Simulation", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set up viewport and callback
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Setup orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);

    // Initialize scene
    glm::vec2 windowSize(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT));
    ParticleSystem particleSystem(PARTICLE_COUNT, windowSize);

    // Timing variables
    double lastTime = glfwGetTime();
    double currentTime;
    float deltaTime;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate deltaTime
        currentTime = glfwGetTime();
        deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        // Process input
        glm::vec2 mousePos;
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        mousePos.x = static_cast<float>(x);
        mousePos.y = windowSize.y - static_cast<float>(y); // Invert y-axis

        bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

        // Update scene
        particleSystem.Update(deltaTime, mousePos, mousePressed);

        // Render
        glClear(GL_COLOR_BUFFER_BIT);
        particleSystem.Render();

        // Swap buffers
        glfwSwapBuffers(window);

        // Poll events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
