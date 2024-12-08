#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main() {
    int glfwErr = glfwInit();
    if (glfwErr == GLFW_FALSE) {
        std::cerr << "glfwInit fail" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Triangle", NULL, NULL);

    if (window == NULL) {
        std::cerr << "glfwCreateWindow fail" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    int gladVersion = gladLoadGL(glfwGetProcAddress);
    if (gladVersion == 0) {
        std::cerr << "gladLoadGL fail" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    //srgb framebuffer
    glEnable(GL_FRAMEBUFFER_SRGB);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    const float vertices[] = {
        0.0f, 0.5f,     1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
        0.5f, -0.5f,    0.0f, 0.0f, 1.0f
    }; //  position           color

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 2));
    
    std::string vertexShaderSource =
        "#version 460 core\n"
        "in vec2 vxPosition;\n"
        "in vec3 vxColor;\n"

        "out vec4 fragmentColor;\n"

        "void main() {\n"
            "gl_Position = vec4(vxPosition, 0.0f, 1.0f);\n"
            "fragmentColor = vec4(vxColor, 1.0f);\n"
        "}\n";
    
    std::string fragmentShaderSource =
        "#version 460 core\n"
        "in vec4 fragmentColor;\n"

        "out vec4 FragColor;\n"

        "void main() {\n"
            "FragColor = fragmentColor;\n"
        "}\n";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertexShaderPtr = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderPtr, NULL);

    const char* fragmentShaderPtr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderPtr, NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    //draw loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    //cleanup
    glDeleteProgram(shaderProgram);

    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    glDeleteBuffers(1, &VBO);

    glBindVertexArray(NULL);
    glDeleteVertexArrays(1, &VAO);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}