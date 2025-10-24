#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

// Ball state
float y = 0.0f;
float vy = 0.0f;
const float g = -9.8f; // gravity
const float dt = 0.01f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    float aspect = float(width) / float(height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (aspect >= 1.0f) {
        glOrtho(-10.0f * aspect, 10.0f * aspect, -10.0f, 10.0f, -1.0f, 1.0f);
    } else {
        glOrtho(-10.0f, 10.0f, -10.0f / aspect, 10.0f / aspect, -1.0f, 1.0f);
    }
    glMatrixMode(GL_MODELVIEW);
}

void drawBall(float radius)
{
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 100; ++i) {
        float angle = i / 100.0f * 2.0f * M_PI;
        glVertex2f(radius * cos(angle), radius * sin(angle));
    }
    glEnd();
}

int main()
{
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Bouncing Ball", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    // Set initial viewport and projection
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window)) {
        // Physics
        vy += g * dt;
        y += vy * dt;
        if (y < -9.0f) { y = -9.0f; vy = -vy * 0.9f; }

        glClear(GL_COLOR_BUFFER_BIT);

        glPushMatrix();
        glTranslatef(0.0f, y, 0.0f);
        drawBall(1.0f);
        glPopMatrix();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
