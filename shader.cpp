#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

const char* vertexShaderFile = "./vertex_shader.glsl";
const char* fragmentShaderFile = "./fragment_shader.glsl";

// Define screen width and height
const int screenWidth = 800;
const int screenHeight = 800;

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL version to 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Simple Shader Example", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }


    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Vertex data with positions and colors
    float vertices[] = {
        // Positions       // Colors
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-right
         1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-right

        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
         1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-right
        -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-left
    };

    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    std::ifstream vertexFile(vertexShaderFile);
    std::stringstream buffer;
    buffer << vertexFile.rdbuf();
    std::string vertexShaderSourceStr = buffer.str();
    const char* vertexShaderSource = vertexShaderSourceStr.c_str();

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::ifstream fragmentFile(fragmentShaderFile);
    std::stringstream fragmentBuffer;
    fragmentBuffer << fragmentFile.rdbuf();
    std::string fragmentShaderSourceStr = fragmentBuffer.str();
    const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Clean up shaders as we don't need them anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex Array Object and Vertex Buffer Object
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind and set vertex buffer and array object
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Render
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program
        glUseProgram(shaderProgram);

        // Draw the triangles
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
