#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <cmath>

// Ball state
float y = 0.0f;
float vy = 0.0f;
const float g = -9.8f;
const float dt = 0.01f;
const int NUM_SEGMENTS = 100;
const float BALL_RADIUS = 1.0f;

// OpenGL handles
GLuint vao = 0, vbo = 0;

// Vertex shader
const char* vertexShaderSrc = R"glsl(
#version 330 core
layout(location = 0) in vec2 aPos;
uniform float yOffset;
uniform mat4 proj;
void main() {
    gl_Position = proj * vec4(aPos.x, aPos.y + yOffset, 0.0, 1.0);
}
)glsl";

// Fragment shader
const char* fragmentShaderSrc = R"glsl(
#version 330 core
out vec4 FragColor;
void main() { FragColor = vec4(1.0, 0.0, 0.0, 1.0); }
)glsl";

// Generate circle vertices (triangle fan)
std::vector<float> generateCircleVertices(float radius, int segments) {
    std::vector<float> verts;
    verts.push_back(0.0f);
    verts.push_back(0.0f);
    for (int i = 0; i <= segments; ++i) {
        float angle = i / float(segments) * 2.0f * M_PI;
        verts.push_back(radius * cos(angle));
        verts.push_back(radius * sin(angle));
    }
    return verts;
}

// Compile shader
GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, nullptr, info);
        std::cerr << "Shader compile error: " << info << std::endl;
        return 0;
    }
    return shader;
}

// Link program
GLuint createShaderProgram() {
    GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    if (!vertex || !fragment) return 0;

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info[512];
        glGetProgramInfoLog(program, 512, nullptr, info);
        std::cerr << "Program link error: " << info << std::endl;
        return 0;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

void checkGLError(const char* place) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "GL error at " << place << ": " << err << std::endl;
    }
}

// Callback to adjust viewport on resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) return -1;

    // Core profile 3.3 on macOS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "Bouncing Ball", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // --- Prepare vertices ---
    std::vector<float> circleVerts = generateCircleVertices(BALL_RADIUS, NUM_SEGMENTS);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    if (!vao || !vbo) {
        std::cerr << "Failed to generate VAO/VBO" << std::endl;
        return -1;
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, circleVerts.size() * sizeof(float), circleVerts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint shaderProgram = createShaderProgram();
    if (!shaderProgram) {
        std::cerr << "Shader program creation failed" << std::endl;
        return -1;
    }

    GLint yOffsetLoc = glGetUniformLocation(shaderProgram, "yOffset");
    if (yOffsetLoc == -1) {
        std::cerr << "Uniform 'yOffset' not found" << std::endl;
        return -1;
    }

    GLint projLoc = glGetUniformLocation(shaderProgram, "proj");
    if (projLoc == -1) {
        std::cerr << "Uniform 'proj' not found" << std::endl;
        return -1;
    }

    // --- Projection (orthographic) ---
    float worldWidth = 20.0f;
    float worldHeight = 20.0f;
    float aspect = float(width)/float(height);

    float left = -worldWidth/2.0f, right = worldWidth/2.0f;
    float bottom = -worldHeight/2.0f, top = worldHeight/2.0f;
    if (aspect > 1.0f) { left *= aspect; right *= aspect; }
    else { bottom /= aspect; top /= aspect; }

    glm::mat4 proj = glm::ortho(left, right, bottom, top);
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);

	// --- Main loop ---
	while (!glfwWindowShouldClose(window)) {

		// -------------------------------
		// BALL PHYSICS UPDATE
		// -------------------------------
		vy += g * dt;        // Apply gravity
		y  += vy * dt;       // Update vertical position
		if (y < -9.0f) {     // Bounce on the floor
			y = -9.0f;
			vy = -vy * 0.9f; // dampen velocity
		}

		// -------------------------------
		// RENDER BALL
		// -------------------------------
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glUniform1f(yOffsetLoc, y); // Send current y position to shader

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_SEGMENTS + 2);
		glBindVertexArray(0);

		checkGLError("Draw");

		// -------------------------------
		// FRAME SWAP & EVENT POLL
		// -------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
