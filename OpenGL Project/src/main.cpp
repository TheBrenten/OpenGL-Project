
// https://github.com/TheBrenten/OpenGL-Project

#define say(x) std::cout << x << '\n';
#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>

#include "Globals.h"
#include "Shader.h"
#include "Mesh.h"
//#include "Model.h"

float speed = 0.015f;

glm::vec3 position = glm::vec3(0, 0, 5);
glm::vec3 direction;
glm::vec3 right;
// Up vector : perpendicular to both direction and right
glm::vec3 up;

// horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// vertical angle : 0, look at the horizon
float verticalAngle = 0.0f;

float mouseSpeed = 0.005f;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        //activate_airship();
        position += up * speed;
    }
}

void getInputs(GLFWwindow* window) {
    glfwPollEvents();
    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += direction * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= direction * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= right * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += right * speed;
    }
    // key inputs
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position for next frame
    glfwSetCursorPos(window, globals.resx / 2, globals.resy / 2);

    // Compute new orientation
    horizontalAngle += mouseSpeed * float(globals.resx / 2 - xpos);
    verticalAngle += mouseSpeed * float(globals.resy / 2 - ypos);
}

double previousTime = glfwGetTime();
int frameCount = 0;

void printFPS() {
    double currentTime = glfwGetTime();
    frameCount++;
    // If a second has passed.
    if (currentTime - previousTime >= 1.0)
    {
        // Display the frame count here any way you want.
        say(frameCount);

        frameCount = 0;
        previousTime = currentTime;
    }
}

auto target_time = std::chrono::system_clock::now();

int main(void)
{
    // Init GLFW then GLEW
    GLFWwindow* window;
    {
        //glfwWindowHint(GLFW_DEPTH_BITS, 16);
        /* Initialize the library */
        if (!glfwInit())
            return -1;

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(globals.resx, globals.resy, "ass", NULL, NULL);
        if (!window) {
            glfwTerminate();
            return -1;
        }
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        /* Make the window's context current */
        glfwMakeContextCurrent(window);
        if (glewInit() != GLEW_OK)
            std::cout << "Error!" << std::endl;

        std::cout << glGetString(GL_VERSION) << std::endl;

        glfwSetKeyCallback(window, keyCallback);

        glfwSwapInterval(0);
        glfwSetCursorPos(window, globals.resx / 2, globals.resy / 2);
    }

    // triangle
    //float vertices[] = {
    //    -1.0f, -1.0f, 0.0f,
    //     1.0f, -1.0f, 0.0f,
    //     0.0f,  1.0f, 0.0f };
    //
    //unsigned int VBO;
    //glGenBuffers(1, &VBO);
    //
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    Shader shader("src/shader");

    //unsigned int vertexShader = loadShaderFromFile("src/shader.vs", GL_VERTEX_SHADER);
    //unsigned int fragmentShader = loadShaderFromFile("src/shader.fs", GL_FRAGMENT_SHADER);
    //GLuint programID = glCreateProgram();
    //glAttachShader(programID, vertexShader);
    //glAttachShader(programID, fragmentShader);
    //glLinkProgram(programID);
    //
    //// gay debugging
    //// Check the program
    //GLint Result;
    //GLint InfoLogLength;
    //glGetProgramiv(programID, GL_LINK_STATUS, &Result);
    //glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    //if (InfoLogLength > 0) {
    //    std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
    //    glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    //    printf("%s\n", &ProgramErrorMessage[0]);
    //}

    float fov = 90;

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // Enable z-buffer
    glEnable(GL_DEPTH_TEST);

    // model
    //Model ourModel("src/bomb.obj");
    //Vertex vertices[] = { Vertex(glm::vec3(-1,-1, 0)),
    //                       Vertex(glm::vec3(0, 1, 0)),
    //                       Vertex(glm::vec3(1,-1, 0)),
    //                       Vertex(glm::vec3(1,-1, 2))};
    //unsigned int indices[] = { 0,1,2, 0, 1, 3 };
    //Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices)/sizeof(indices[0]));
    Mesh mesh2("src/bomb.obj"); // try monkey.obj too

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        printFPS();
        
        // read mouse and keys
        getInputs(window);
        
        // set direction/right/up vectors
        // Direction : Spherical coordinates to Cartesian coordinates conversion
        direction = glm::vec3(cos(verticalAngle) * sin(horizontalAngle),
                              sin(verticalAngle),
                              cos(verticalAngle) * cos(horizontalAngle));
        right = glm::vec3(sin(horizontalAngle - 3.14f / 2.0f),
                          0,
                          cos(horizontalAngle - 3.14f / 2.0f));
        up = glm::cross(right, direction);

        //vertices[1] -= 0.01;
        glm::mat4 Projection = glm::perspective(glm::radians(fov), (float)globals.resx / (float)globals.resy, 0.1f, 100.0f);
        glm::mat4 View = glm::lookAt(position,
                                     position+direction,
                                     up); 

        //// Model matrix : an identity matrix (model will be at the origin)
        //glm::mat4 Model = glm::mat4(1.0f);
        //// Our ModelViewProjection : multiplication of our 3 matrices
        //glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
        //// set up normal matrix?
        //glm::vec3 pos;
        //glm::vec3 scale(.3,.3,.3);
        //glm::vec3 rot = glm::vec3(0,0,0);
        //
        //glm::mat4 posMat = glm::translate(pos);
        //glm::mat4 scaleMat = glm::scale(scale);
        //glm::mat4 rotX = glm::rotate(rot.x, glm::vec3(1.0, 0.0, 0.0));
        //glm::mat4 rotY = glm::rotate(rot.y, glm::vec3(0.0, 1.0, 0.0));
        //glm::mat4 rotZ = glm::rotate(rot.z, glm::vec3(0.0, 0.0, 1.0));
        //glm::mat4 rotMat = rotX * rotY * rotZ;
        //glm::mat4 Normal = posMat * rotMat * scaleMat;
                                                   // Get a handle for our "MVP" uniform
                                                   // Only during the initialisation
        //GLuint MatrixID = glGetUniformLocation(programID, "MVP");

        


        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glUseProgram(programID);

        // draw modern triangle
        // 1st attribute buffer : vertices
        /*glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);
        */
        //mesh.draw();
        shader.bind();

        //shader.update(transform, camera);
        //glUniformMatrix4fv(m_uniforms[0], 1, GL_FALSE, &MVP[0][0]);
        //glUniformMatrix4fv(m_uniforms[1], 1, GL_FALSE, &Normal[0][0]);
        //glUniform3f(m_uniforms[2], 0.0f, 0.0f, 1.0f);

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
        
        
        glm::mat4 VP = View;
        glm::vec3 pos = position;
        glm::vec3 scale(1, 1, 1);
        glm::vec3 rot = direction;
        
        glm::mat4 posMat = glm::translate(pos);
        glm::mat4 scaleMat = glm::scale(scale);
        glm::mat4 rotX = glm::rotate(rot.x, glm::vec3(1.0, 0.0, 0.0));
        glm::mat4 rotY = glm::rotate(rot.y, glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 rotZ = glm::rotate(rot.z, glm::vec3(0.0, 0.0, 1.0));
        glm::mat4 rotMat = rotX * rotY * rotZ;
        glm::mat4 M = posMat * rotMat * scaleMat;
        //glm::mat4 MVP = VP * M;
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
        glm::mat4 Normal = Model;

        glUniformMatrix4fv(shader.m_uniforms[0], 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(shader.m_uniforms[1], 1, GL_FALSE, &Normal[0][0]);
        glUniform3f       (shader.m_uniforms[2], -0.5f, -3.0f, -3.0f);
        mesh2.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        // wait remainder of 1/300th second
        auto time = std::chrono::system_clock::now();
        while (time < target_time) { time = std::chrono::system_clock::now(); }
        target_time = time + std::chrono::microseconds((int)(1000000.f / globals.FPS_MAX));
        //say(time.time_since_epoch());

    }

    glfwTerminate();
    return 0;
}
