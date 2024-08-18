#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

static const char* vertexShaderFile = "./vertex_shader.glsl";
static const char* fragmentShaderFile = "./fragment_shader.glsl";

static const std::string defaultVertexShader = R"glsl(
#version 330 core
layout(location = 0) in vec3 aPos;
out vec3 texCoord;
void main()
{
    gl_Position = vec4(aPos, 1.0);
    texCoord = vec3(aPos.xy, 0.0);
})glsl";

// Define screen width and height
static int screenWidth = 800;
static int screenHeight = 800;

const std::string usage = R"(
USAGE: shaderun [-w <width>] [-h <height>] [-v <vertex file>] <fragment-shader>
arguments:
    -w | --width     width of screen in pixels (default )"
+ std::to_string(screenWidth) + R"()
    -h | --height    height of screen in pixels (default )"
+ std::to_string(screenHeight) + R"()
    -v | --vertex    file containing vertex shader
    fragment-shader  file containing fragment shader

By default, the fragment shader has access to the following variables:
in vec3 texCoord;    // coordinates (x, y, 0) x, y are in [-1, 1]
out vec4 fragColor;  // pixel color
uniform float uTime; // time since animation started in seconds
)";
void printUsage()
{
    std::cout << usage << "\n";
    std::cout << "\nThe default vertex shader is\n" << defaultVertexShader
        << "\n";
}

static unsigned int CompileShader(const std::string& source, unsigned int type)
{
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(shader, length, NULL, message);
        std::cerr << "failed to compile " << 
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << 
            " shader: " << message << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

std::string readFile(const std::string& fileName)
{
    std::ifstream file(fileName);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[])
{
    // ----------- Process arguments ------------------------------------------
    int argNum = 1;
    bool fragmentSet = false;
    bool useDefaultVertexShader = true;

    if (argc <= 1) {
        printUsage();
        return 0;
    }
    while (argNum < argc) {

        std::string arg = argv[argNum];

        if (arg == "-v" || arg == "--vertex") {
            if (++argNum >= argc) {
                std::cout << "error: -v|--vertex followed by no argument\n";
                printUsage();
                return 1;
            }
            vertexShaderFile = argv[argNum];
            useDefaultVertexShader = false;
        } else if (arg == "-w" || arg == "--width") {
            if (++argNum >= argc) {
                std::cout << "error: -w|--width followed by no argument\n";
                printUsage();
                return 1;
            }
            const int ret = std::sscanf(argv[argNum], "%d", &screenWidth);
            if (!ret) {
                std::cout << "error: -w|--width followed by invalid argument: "
                    << argv[argNum] << "\n";
                printUsage();
                return 1;
            }
        } else if (arg == "-h" || arg == "--height") {
            if (++argNum >= argc) {
                std::cout << "error: -h|--height followed by no argument\n";
                printUsage();
                return 1;
            }
            const int ret = std::sscanf(argv[argNum], "%d", &screenHeight);
            if (!ret) {
                std::cout << "error: -h|--height followed by invalid argument:"
                    " " << argv[argNum] << "\n";
                printUsage();
                return 1;
            }
        } else {
            if (fragmentSet) {
                std::cout << "errror: invalid argument: " << arg << "\n";
                printUsage();
                return 1;
            }
            fragmentShaderFile = argv[argNum];
            fragmentSet = true;
        }
        argNum++;
    }

    if (!fragmentSet) {
        std::cout << "error: no fragment shader specified\n" << usage;
        return 1;
    }
    
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
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight,
            "Simple Shader Example", NULL, NULL);
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

    glfwSwapInterval(1); // 1 to enable VSync, 0 to disable

    // ----------- Compile Shader Program -------------------------------------
    std::string vertexShader;
    if (useDefaultVertexShader) {
        vertexShader = defaultVertexShader;
    } else {
        vertexShader = readFile(vertexShaderFile);
    }
    std::string fragmentShader = readFile(fragmentShaderFile);
    unsigned int shaderProgram = CreateShader(vertexShader, fragmentShader);

    float vertices[] = {
        // Positions        // Colors
        -1.0f, -1.0f, 0.0f, // Bottom-left
         1.0f, -1.0f, 0.0f, // Bottom-right
         1.0f,  1.0f, 0.0f, // Top-right

        -1.0f, -1.0f, 0.0f, // Bottom-left
         1.0f,  1.0f, 0.0f, // Top-right
        -1.0f,  1.0f, 0.0f, // Top-left
    };

    // Vertex Array Object and Vertex Buffer Object
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // index of attribute, size in size(float) of attribute, type, noarmalized,
    // stride (size of each vertex)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
            (const void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Render loop
    int timeLocation = glGetUniformLocation(shaderProgram, "uTime");
    while (!glfwWindowShouldClose(window)) {
        // Input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Render
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program
        glUseProgram(shaderProgram);

        // Pass the time value to the shader
        float timeValue = glfwGetTime(); // Get the current time
        glUniform1f(timeLocation, timeValue);
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
