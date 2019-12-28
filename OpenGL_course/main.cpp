//
//  main.cpp
//  OpenGL_course
//
//  Created by Anton Z on 26.12.2019.
//  Copyright © 2019 Anton Z. All rights reserved.
//

#include <iostream>
#include <cmath>

// init glm, force creation identity matrix
#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW - OpenGL Extenstion Wrangler, OpenGL interface
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW - window/context OpenGL API
#include <GLFW/glfw3.h>

// Window dimension
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader, uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

// Vertex Shader
static const char* vShader = "                      \n\
#version 330                                        \n\
                                                    \n\
layout (location = 0) in vec3 pos;                  \n\
                                                    \n\
uniform mat4 model;                                 \n\
                                                    \n\
void main() {                                       \n\
    gl_Position = model * vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);   \n\
}                                                  \n\
";

// Fragment Shader
static const char* fShader = "                      \n\
#version 330                                        \n\
                                                    \n\
out vec4 color;                                     \n\
                                                    \n\
void main() {                                        \n\
    color = vec4(1.0, 0.0, 0.0, 1.0);   \n\
}                                                \n\
";

void CreateTriangle() {
    GLfloat vertices[] = {
        -1.0, -1.0f, 0.0f,
        1.0, -1.0f, 0.0f,
        0.0, 1.0f, 0.0f,
    };
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
    GLint theShader = glCreateShader(shaderType);
    
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);
    
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);
    
    GLint result = 0;
    GLchar elog[1024] = {0};
    
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);
        printf("Error compiling the %d shader: %s\n", shaderType, elog);
        return;
    }
    
    glAttachShader(theProgram, theShader);
}

void CompileShaders() {
    shader = glCreateProgram();
    if (!shader) {
        printf("Error creating shader program\n");
        return;
    }
    
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);
    
    GLint result = 0;
    GLchar elog[1024] = {0};
    
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        printf("Error linking programm: %s\n", elog);
        return;
    }
    
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        printf("Error validating programm: %s\n", elog);
        return;
    }
    
    uniformModel = glGetUniformLocation(shader, "model");
}

int main(int argc, const char * argv[]) {
    // init GLFW
    if (!glfwInit()) {
        printf("GLFW init failed");
        glfwTerminate();
        return 1;
    }
    
    // setup GLFW window opts
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // core profle - no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
                   
    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test window", NULL, NULL);
    if (!mainWindow) {
        printf("GLFW window craation failed");
        glfwTerminate();
        return 1;
    }
    
    // Get buffer size info
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    
    // set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);
    
    // allow modern extension features
    glewExperimental = GL_TRUE;
    
    if (glewInit() != GLEW_OK) {
        printf("GLEW init failed");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    
    // setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    CreateTriangle();
    CompileShaders();
    
    while(!glfwWindowShouldClose(mainWindow)) {
        // handle user events
        glfwPollEvents();
        
        if (direction) {
            triOffset += triIncrement;
        } else {
            triOffset -= triIncrement;
        }
        
        if (abs(triOffset) >= triMaxOffset) {
            direction = !direction;
        }
        
        // clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shader);
        
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(triOffset, triOffset, 0.0f));
        
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        
        glUseProgram(0);
        
        glfwSwapBuffers(mainWindow);
    }
    
    return 0;
}
