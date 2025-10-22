//Práctica 07
// Sánchez Gachuz  Jenyfer Estefanya
// 319025901
// Fecha de entrega 28 de septiembre del 2025
// 
//Std. Includes
#include <string>
#include <iostream>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f)); // Cámara atrás en Z
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, " Jenyfer Sanchez_Practica7 Texturizado", nullptr, nullptr);
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");

    // Load models
    ////Model dog((char*)"Models/RedDog.obj");
    ////Model Table((char*)"Models/Table.obj");
    ////Model Corona((char*)"Models/Corona.obj"); // Carga el modelo de la botella
    ////Model newPlant((char*)"Models/indoor plant_02.obj"); // Carga el nuevo modelo de planta
    ////Model DesignChair((char*)"Models/DesignChair1.obj");
    ////Model Plate((char*)"Models/plate.obj"); // Carga el modelo del plato
    ////Model cup((char*)"Models/cup.obj");

    Model Prac7((char*)"Models/Prac7.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
       

       // // Dibujar la taza 
       // glm::mat4 cupModel(1);
       // // Escala  más pequeña
       // float cupScale = 0.12f;
       // cupModel = glm::scale(cupModel, glm::vec3(cupScale, cupScale, cupScale));
       // cupModel = glm::translate(cupModel, glm::vec3(0.0f, -0.53f / cupScale, 9.0f));
       // glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(cupModel));
       // cup.Draw(shader);

       // // Draw dog
       // glm::mat4 dogModel(1);
       // dogModel = glm::scale(dogModel, glm::vec3(1.5f, 1.5f, 1.5f));
       // glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(dogModel));
       // dog.Draw(shader);

       // // Draw table
       // glm::mat4 TableModel(1);
       // // Escala: Reduce el tamaño de la mesa drásticamente para que sea visible
       // TableModel = glm::scale(TableModel, glm::vec3(0.015f, 0.015f, 0.015f));
       // // Traslación: Mueve la mesa hacia abajo para que se vea junto al perro
       // TableModel = glm::translate(TableModel, glm::vec3(0.0f, -89.0f, 0.0f));
       // glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(TableModel));
       // Table.Draw(shader);

       /////*  Draw Corona
       //// glm::mat4 CoronaModel(1);
       ////  La escala del perro es 1.5. Para que la botella sea la mitad, ajustamos su escala.
       //// CoronaModel = glm::scale(CoronaModel, glm::vec3(0.75f, 0.75f, 0.75f));
       ////  La colocamos en el otro lado del perro.
       //// CoronaModel = glm::translate(CoronaModel, glm::vec3(-2.0f, 0.0f, 0.0f));
       //// glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(CoronaModel));
       //// Corona.Draw(shader);*/

       // //// Draw chair next to the table
       // //glm::mat4 ChairModel(1);
       // //ChairModel = glm::scale(ChairModel, glm::vec3(0.015f));
       // //ChairModel = glm::translate(ChairModel, glm::vec3(-30.0f, -89.0f, 0.0f));
       // //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ChairModel));
       // //DesignChair.Draw(shader);

       // // Draw the new plant
       // glm::mat4 NewPlantModel(1);
       // // Ajusta este valor hasta que la planta tenga un tamaño razonable en tu escena.
       // NewPlantModel = glm::scale(NewPlantModel, glm::vec3(0.10f, 0.10f, 0.10f));
       // // Traslación: Mueve la planta a un lado del perro y la mesa.
       // // Ajusta los valores para que la planta aparezca donde deseas.
       // NewPlantModel = glm::translate(NewPlantModel, glm::vec3(-22.0f, -15.0f, 0.0f));
       // glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(NewPlantModel));
       // newPlant.Draw(shader);

       // // Dibujar la nueva planta a la derecha
       // NewPlantModel = glm::mat4(1); // 👈 reiniciar la matriz
       // NewPlantModel = glm::translate(NewPlantModel, glm::vec3(3.0f, -1.5f, 0.0f));
       // NewPlantModel = glm::scale(NewPlantModel, glm::vec3(0.1f, 0.1f, 0.1f));
       // glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(NewPlantModel));
       // newPlant.Draw(shader);

        // // Draw Prac7 dado
        glm::mat4 Prac7Model(1);
        Prac7Model = glm::scale(Prac7Model, glm::vec3(0.1f, 0.1f, 0.1f)); // Escala más pequeña
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(Prac7Model));
        Prac7.Draw(shader);


        

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}