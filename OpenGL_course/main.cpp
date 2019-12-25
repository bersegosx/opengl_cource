//
//  main.cpp
//  OpenGL_course
//
//  Created by Anton Z on 26.12.2019.
//  Copyright © 2019 Anton Z. All rights reserved.
//

#include <iostream>

// GLEW - OpenGL Extenstion Wrangler, OpenGL interface
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW - window/context OpenGL API
#include <GLFW/glfw3.h>

// Window dimension
const GLint WIDTH = 800, HEIGHT = 600;

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
    
    while(!glfwWindowShouldClose(mainWindow)) {
        // handle user events
        glfwPollEvents();
        
        // clear window
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwSwapBuffers(mainWindow);
    }
    
    return 0;
}
