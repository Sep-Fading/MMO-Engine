#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


// Shader source codes are written in GLSL, You can find the stand-alone files in the project.
// Source code for vertex shader:
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

// Source code for the fragment shader:
const char *fragmentShaderSource = "#version 420 core\n"
                                   "out vec4 FragColor;\n"
                                   "\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";

// Shader Program
unsigned int shaderProgram;

void ClearWithColor(float red, float green, float blue, float alpha);

void CreateTriangle();

void HandleShaders();

void CheckShaderCompilation(unsigned int shader);

void CheckShaderProgramHealth(unsigned int program);

void DrawTriangle(unsigned int program, unsigned int vao);

using namespace std;

// Error call backs for GLFW
void error_callback(int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
}

// Input handling for the GLFW Window
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // IF ESCAPE IS PRESSED EXIT WINDOW.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// Viewport resizer based on window adjustments.
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0, width, height);
}

int main() {
    // GLFW Library Init
    if (!glfwInit()) {
        // GLFW: Init failed
        cout << "GLFW: Initialization failed.";
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    // Declaring Window Specifications
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "MMO-Engine-TestBuild v.Init",
                                          nullptr, nullptr);

    if (!window) {
        // Problem with creating the window.
        cout << "GLFW: Window creation failed.";
        glfwTerminate();
        return -1;
    }

    // Makes the window the relevant one?
    glfwMakeContextCurrent(window);

    // GLAD Library Init
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        cout << "GLAD: Initialization failed.";
        return -1;
    }

    // Setting the viewport for the renderer.
    glViewport(0,0,1920,1080);
    // Letting GLFW know we want to call this function on every window resize by registering it.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Sets the key_callback function to apply to this window.
    glfwSetKeyCallback(window, key_callback);

    // Method to handle making the required shaders. (Will also activate the shader program!)
    HandleShaders();

    // While window is not supposed to close.
    while (!glfwWindowShouldClose(window)){
        // Render here
        ClearWithColor(0.2f, 0.3f, 0.3f, 1.0f);
        CreateTriangle();
        // --------------------------------------------------------------------

        glfwSwapBuffers(window); // This one swaps back and front buffer, back one is the one you
                                 // render to and front is what's being displayed.

        glfwPollEvents(); // Checks if events are triggered, updates window state and calls the corresponding functions.
    }

    // Closing the window.
    glfwTerminate();
    return 0;
}

// Clears the screen with specified color.
void ClearWithColor(float red, float green, float blue, float alpha) {
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Creates a triangle on the screen.
void CreateTriangle(){
    // Giving OpenGL vertices that we want the triangle to be made on.
    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
    };


    // Initialization code (done once unless object frequently changes.)
    // Creating a VAO (Vertex Array Object) which stores the vertex attribute config and which VBO to use.
    // When used to draw objects, take the corresponding VAO, bind it, draw and unbind once done.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // Bind the VAO
    glBindVertexArray(VAO);

    // Creates this memory space to store vertices in the GPU.
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Bind the created buffer to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send the vertices defined for the triangle to the GPU memory.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Telling OpenGL how to interpret the vertex data using glVertexAttribPointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Draw the triangle
    DrawTriangle(shaderProgram, VAO);
}

// Method that draws a Triangle on the screen.
void DrawTriangle(unsigned int program, unsigned int vao) {
    glUseProgram(program);
    glBindVertexArray(vao);

    // Arguments - 1. Primitive type we would like to draw.
    // 2. Starting index of the vertex array we'd like to draw.
    // 3. How many vertices we want to draw, which is 3, it's a triangle!
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

// Handles the shader creation.
void HandleShaders() {
    // Create vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach the shader source code to the shader object and compile it.
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Method to check if the shader compiled successfully.
    CheckShaderCompilation(vertexShader);


    // Create fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Attach the shader source code to the shader object and compile it.
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Check fragment shader compilation status.
    CheckShaderCompilation(fragmentShader);

    // Create the shader program from our created shaders.
    shaderProgram = glCreateProgram();

    // Attach the compiled shaders to the object and link them with glLinkProgram.
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check the shader program for any linking errors.
    CheckShaderProgramHealth(shaderProgram);

    // Activate the shader program.
    glUseProgram(shaderProgram);

    // We no longer need the individual shader objects, we dispose of them.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Checks to see if a given shader compiles fine.
void CheckShaderCompilation(unsigned int shader) {
    int success;
    char infoLog[512];
    // Get compile status of the given shader and give the code to the success variable as likely boolean (0,1)?
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    // Result check.
    if (!success){
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        cout << "GLSL: Vertex Shader compilation failed ! \n" << infoLog;
    }
}

// Checks to see if the shaders are now correctly linked to a shader program.
void CheckShaderProgramHealth(unsigned int program) {
    int success;
    char infoLog[512];

    // Get program status
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    // Result check.
    if(!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        cout << "Shader Program: Linking failed ! \n" << infoLog;
    }
}
