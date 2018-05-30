////////////////////////////////////////////////////////////////////
/////////////////////////// ASSIGNMENT 2 ///////////////////////////
////////////////////////////////////////////////////////////////////

/*
 
 COMP 371 - Assignment #2
 
 basic code taken & referenced from from Labs
 modified from http://learnopengl.com/
 
 */

// GLEW, GLFW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// CImg
#define cimg_display 0
#define cimg_use_png 1 //accepts .png type
#define cimg_use_jpeg 1 //accepts .jpeg/.jpg type
#include "CImg.h"

//GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

//OBJParser
#include "objloader.hpp"

// Other standard libraries
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace cimg_library;
using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

// Define some global variables

// Light space definition:
glm::vec3 light = glm::vec3(0.0f, 40.0f, 40.0f); // point light positioned here
glm::vec3 light_at = glm::vec3(0.0f); // for shadow, looks at origin
glm::vec3 light_up = glm::vec3(0.0f, 1.0f, 0.0f); // for shadow, y-axis as up
glm::mat4 light_view = glm::lookAt(light, light_at, light_up);
glm::mat4 proj_light; //defined once the aspect ratio is known


// Camera(View) transformation matrices control variables
glm::mat4 vtransf;
glm::mat4 s_vMat = glm::scale(vtransf, glm::vec3(1.0f)); //scale
glm::mat4 r_vMat = glm::rotate(vtransf, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)); //rotate
glm::mat4 t_vMat = glm::translate(vtransf, glm::vec3(0.0f, 0.0f, 0.0f)); //translate
glm::vec3 rot_axis = glm::vec3(0.0f, 1.0f, 0.0f);

// View transformation matrices control variables
glm::vec3 pos = glm::vec3(0.0f, 40.0f, 40.0f); //camera positioned here
glm::vec3 at = glm::vec3(0.0f, 0.0f, 0.0f); //looks at origin
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); //y-axis as up






////////////////////////////////////////////////////////////////////
/////////////////////////// KEY CALLBACK ///////////////////////////
////////////////////////////////////////////////////////////////////

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    //Moving the Camera
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        r_vMat = glm::rotate(0.0314f, glm::vec3(0.0f, -1.0f, 0.0f));
        pos = glm::vec3(r_vMat * glm::vec4(pos, 1.0f));
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        r_vMat = glm::rotate(0.0314f, glm::vec3(0.0f, 1.0f, 0.0f));
        pos = glm::vec3(r_vMat * glm::vec4(pos, 1.0f));
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        glm::vec3 to_origin = glm::vec3(0.0f) - pos;
        rot_axis = glm::normalize(glm::cross(to_origin, glm::vec3(0.0f, -1.0f, 0.0f)));
        r_vMat = glm::rotate(0.0314f, rot_axis);
        pos = glm::vec3(r_vMat * glm::vec4(pos, 1.0f));
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        glm::vec3 to_origin = glm::vec3(0.0f) - pos;
        rot_axis = glm::normalize(glm::cross(to_origin, glm::vec3(0.0f, 1.0f, 0.0f)));
        r_vMat = glm::rotate(0.0314f, rot_axis);
        pos = glm::vec3(r_vMat * glm::vec4(pos, 1.0f));
    }
    
    
    //Moving the Light
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        r_vMat = glm::rotate(0.0314f, glm::vec3(0.0f, -1.0f, 0.0f));
        light = glm::vec3(r_vMat * glm::vec4(light, 1.0f));
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        r_vMat = glm::rotate(0.0314f, glm::vec3(0.0f, 1.0f, 0.0f));
        light = glm::vec3(r_vMat * glm::vec4(light, 1.0f));
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        glm::vec3 to_origin = glm::vec3(0.0f) - light;
        rot_axis = glm::normalize(glm::cross(to_origin, glm::vec3(0.0f, -1.0f, 0.0f)));
        r_vMat = glm::rotate(0.0314f, rot_axis);
        light = glm::vec3(r_vMat * glm::vec4(light, 1.0f));
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        glm::vec3 to_origin = glm::vec3(0.0f) - light;
        rot_axis = glm::normalize(glm::cross(to_origin, glm::vec3(0.0f, 1.0f, 0.0f)));
        r_vMat = glm::rotate(0.0314f, rot_axis);
        light = glm::vec3(r_vMat * glm::vec4(light, 1.0f));
    }
}

// Is called whenever user scrolls via GLFW
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    std::cout << yoffset << std::endl;
    float change = 1.1f;
    if (yoffset < 0) change = 0.9f;
    else change = 1.1f;
    s_vMat = glm::scale(glm::vec3(change));
    pos = glm::vec3(s_vMat * glm::vec4(pos, 1.0f));
}







////////////////////////////////////////////////////////////////////
/////////////////////////// MAIN FUNCTION //////////////////////////
////////////////////////////////////////////////////////////////////

// The MAIN function, from here we start the application and run the game loop
int main()
{
    
    
    
    //added to figure out which directory to include the image files and shader files
    //ref. https://stackoverflow.com/questions/7279100/c-ifstream-on-xcode-where-is-the-default-directory
    std::cout << "Project Detail:" << std::endl;
    char * dir = getcwd(NULL, 0);
    printf("Current dir: %s\n\n", dir);
    delete [] dir;
    dir = nullptr;
    
    
    
    
    
    ////////////////////////////////////////////////////////////////////
    /////////////////////////// READING OBJ ////////////////////////////
    ////////////////////////////////////////////////////////////////////
    
    
    // READING OBJ FILE
    std::vector<glm::vec3> obj_vertices;
    std::vector<glm::vec3> obj_normals;
    std::vector<glm::vec2> obj_uvs;
    loadOBJ("scene.obj", obj_vertices, obj_normals, obj_uvs);
    
    
    ////////////////////////////////////////////////////////////////////
    ///////////////////////// GLFW WINDOW SETUP ////////////////////////
    ////////////////////////////////////////////////////////////////////
    
    // SET UP GLFW WINDOW
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //required for Mac. Ignored by Windows.
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Light & Shadow", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    
    
    ////////////////////////////////////////////////////////////////////
    //////////////////////// SETTING UP SHADERS ////////////////////////
    ////////////////////////////////////////////////////////////////////
    
    // Build and compile our shader program
    
    // Read the Vertex Shader code from the file
    string vertex_shader_path = "vertex.shader";
    string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_shader_path, ios::in);
    
    if (VertexShaderStream.is_open())
    {
        string Line = "";
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    else
    {
        printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
        getchar();
        exit(-1);
    }
    
    // Read the Fragment Shader code from the file
    string fragment_shader_path = "fragment.shader";
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);
    
    if (FragmentShaderStream.is_open())
    {
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    else
    {
        printf("Impossible to open %s. Are you in the right directory?\n", fragment_shader_path.c_str());
        getchar();
        exit(-1);
    }
    
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader); //free up memory
    glDeleteShader(fragmentShader);
    
    glUseProgram(shaderProgram);
    
    
    
    
    
    ////////////////////////////////////////////////////////////////////
    ////////////////////////// VAO, VBO, EBOs //////////////////////////
    ////////////////////////////////////////////////////////////////////
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    GLuint VAO, VBO, VBO_normals, VBO_uvs, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*obj_vertices.size(), &obj_vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glGenBuffers(1, &VBO_normals);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*obj_normals.size(), &obj_normals.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    
    glGenBuffers(1, &VBO_uvs);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_uvs);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*obj_uvs.size(), &obj_uvs.front(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(2);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    
    
    ////////////////////////////////////////////////////////////////////
    ///////////////////////// UNIFORM LOCATION /////////////////////////
    ////////////////////////////////////////////////////////////////////
    // Obtain uniform location for the mvp matrices from shader
    GLuint light_Loc = glGetUniformLocation(shaderProgram, "light_pos");
    GLuint cam_Loc = glGetUniformLocation(shaderProgram, "camera_pos");
    
    
    ////////////////////////////////////////////////////////////////////
    ///////////////////// MODEL, VIEW, PROJECTION //////////////////////
    ////////////////////////////////////////////////////////////////////
    // Obtain uniform location for the mvp matrices from shader
    GLuint m_Loc = glGetUniformLocation(shaderProgram, "m_mat");
    GLuint v_Loc = glGetUniformLocation(shaderProgram, "v_mat");
    GLuint p_Loc = glGetUniformLocation(shaderProgram, "p_mat");
    
    //apply initial transformation and declare
    glm::mat4 m_mat, v_mat, p_mat;
    
    //enable depth buffer to hide overlap
    glEnable(GL_DEPTH_TEST);
    
    
    
    ////////////////////////////////////////////////////////////////////
    //////////////////////////// GAME LOOP /////////////////////////////
    ////////////////////////////////////////////////////////////////////
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been  activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        // Apply necessary Transformations
        v_mat = glm::lookAt(pos, at, up);
        p_mat = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
        
        // Send MVP Info to Shaders
        glUniformMatrix4fv(m_Loc, 1, GL_FALSE, glm::value_ptr(m_mat));
        glUniformMatrix4fv(v_Loc, 1, GL_FALSE, glm::value_ptr(v_mat));
        glUniformMatrix4fv(p_Loc, 1, GL_FALSE, glm::value_ptr(p_mat));
        
        // Send light & cam position to Shaders
        glUniform3fv(light_Loc, 1, glm::value_ptr(light));
        glUniform3fv(cam_Loc, 1, glm::value_ptr(pos));
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, obj_vertices.size());
        glBindVertexArray(0);
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}
