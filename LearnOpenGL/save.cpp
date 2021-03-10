/*
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "ChunkGenerator.h"

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 1024;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 20.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

const unsigned int Crow = 3;
const unsigned int Ccol = 3;

const unsigned int row = 4;
const unsigned int col = 4;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float Xposition = floor(cameraPos.x);
float Zposition = floor(cameraPos.z);

//mouse
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("3.3.shader.vs", "3.3.shader.fs");//,"3.3.shader.gs"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[Crow * Ccol * 6 * 3 * row * col];

    int count = 0;
    for (int y1 = 0; y1 < Crow; y1++)
    {
        float yoff = Zposition / -1.0f + Ccol - y1 * 2;
        for (int x1 = 0; x1 < Ccol; x1++)
        {
            float xoff = Xposition - Ccol + x1 * 2;
            for (int y = 0; y < row; y++)
            {
                for (int x = 0; x < col; x++)
                {
                    vertices[count] = xoff;
                    count++;
                    vertices[count] = yoff;
                    count++;
                    vertices[count] = 0.0f;
                    count++;

                    vertices[count] = xoff + 2.0f / col;
                    count++;
                    vertices[count] = yoff;
                    count++;
                    vertices[count] = 0.0f;
                    count++;

                    vertices[count] = xoff + 2.0f / col;
                    count++;
                    vertices[count] = yoff + 2.0f / row;
                    count++;
                    vertices[count] = 0.0f;
                    count++;


                    vertices[count] = xoff;
                    count++;
                    vertices[count] = yoff;
                    count++;
                    vertices[count] = 0.0f;
                    count++;


                    vertices[count] = xoff;
                    count++;
                    vertices[count] = yoff + 2.0f / row;
                    count++;
                    vertices[count] = 0.0f;
                    count++;

                    vertices[count] = xoff + 2.0f / col;
                    count++;
                    vertices[count] = yoff + 2.0f / row;
                    count++;
                    vertices[count] = 0.0f;
                    count++;

                    xoff += 2.0f / col;
                }
                xoff = Xposition - Ccol + x1 * 2;
                yoff -= 2.0f / row;
            }
            yoff = Zposition / -1.0f + Ccol - y1 * 2;
        }
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        if (floor(cameraPos.x) != Xposition || floor(cameraPos.z) != Zposition)
        {
            Xposition = floor(cameraPos.x);
            Zposition = floor(cameraPos.z);


            int count = 0;
            for (int y1 = 0; y1 < Crow; y1++)
            {
                float yoff = Zposition / -1.0f + Ccol - y1 * 2;
                for (int x1 = 0; x1 < Ccol; x1++)
                {
                    float xoff = Xposition - Ccol + x1 * 2;
                    for (int y = 0; y < row; y++)
                    {
                        for (int x = 0; x < col; x++)
                        {
                            vertices[count] = xoff;
                            count++;
                            vertices[count] = yoff;
                            count++;
                            vertices[count] = 0.0f;
                            count++;

                            vertices[count] = xoff + 2.0f / col;
                            count++;
                            vertices[count] = yoff;
                            count++;
                            vertices[count] = 0.0f;
                            count++;

                            vertices[count] = xoff + 2.0f / col;
                            count++;
                            vertices[count] = yoff + 2.0f / row;
                            count++;
                            vertices[count] = 0.0f;
                            count++;


                            vertices[count] = xoff;
                            count++;
                            vertices[count] = yoff;
                            count++;
                            vertices[count] = 0.0f;
                            count++;


                            vertices[count] = xoff;
                            count++;
                            vertices[count] = yoff + 2.0f / row;
                            count++;
                            vertices[count] = 0.0f;
                            count++;

                            vertices[count] = xoff + 2.0f / col;
                            count++;
                            vertices[count] = yoff + 2.0f / row;
                            count++;
                            vertices[count] = 0.0f;
                            count++;

                            xoff += 2.0f / col;
                        }
                        xoff = Xposition - Ccol + x1 * 2;
                        yoff -= 2.0f / row;
                    }
                    yoff = Zposition / -1.0f + Ccol - y1 * 2;
                }
            }

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        }

        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render the triangle
        ourShader.use();

        // create transformations
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, Crow * Ccol * 6 * row * col);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraSpeed = 5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
*/