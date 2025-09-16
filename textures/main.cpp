#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <stb_image.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main() {
    glfwInit(); // intialize GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // get GLFW v3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Shaders!", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all opengl function pointers
    if ( !gladLoadGL(glfwGetProcAddress) ) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader ourShader("basic.vert", "basic.frag");

    //define vertex points
    float firstTriangle[] = {
        // position                          
         0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,    // top right
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    // define indices for EBO
    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3  // second triangle
    };


    glViewport(0, 0, 800, 600);

    // declare type for VBO, VAO
    unsigned int VBO, VAO, EBO;
    
    // generate an ID for the objects and store that ID
    // at their memory address  
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); 
    glGenBuffers(1, &EBO);
    
    // note that we're in fact passing pointers, since they're
    // arrays, which get passed as pointers by default

    // bind the first VAO
    // we're binding it to opengl's internal state, not to a specific object
    // like selecting this VAO as the active one
    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind GL_ARRAY_BUFFER and VBO

    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    // copy user-defined data into the currently bound buffer
    // glBufferData(type of buffer we want to copy into, size of data in bytes, data to send,
    // instructions for how the GPU should manage the data)

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // attribute 0: position
    // define an array of generic vertex attribute data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    // enable that array in position 0
    glEnableVertexAttribArray(0);

    //attribute 1: colour
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    //attribute 2: texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // load & create texture
    unsigned int texture;
    glGenTextures(1, &texture); // (how many textures we want, where to store them)

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

    // we're initializing *data as a pointer because stbi_load returns a memory address
    // we're saying "the object 'data' contains whatever is at the 
    // memory address provided by stbi_load"
    
    if (data)
    {
        // generate the texture using our image stored in "data"
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    else
    {
        std::cout << "Container data not found" << std::endl;
    }

    stbi_image_free(data);

    // render loop-
    while(!glfwWindowShouldClose(window)) {

        //process input
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

            // define the transformation
        float pi_over_two = M_PI / 2.0f;
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, pi_over_two, glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

        //draw triangle
        ourShader.use();

        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        // has to be inside the loop because a uniform is set in the state space

        glBindVertexArray(VAO);            // VAO has to be bound every frame
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //  because here we're drawing whatever
                                         //   object is bound in the state-space

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    // terminate the process, which clears the previously allocated GLFW resources
    glfwTerminate();
    return 0;

}

void processInput(GLFWwindow *window) { //here *window is a type declaration
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}