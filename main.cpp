#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glad/glad.h>
#include <glad/glad.c>
#include <GLFW/glfw3.h>
#include <shaderClass/shaderClass.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const * path);

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.f,  0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

//mause
bool firstMouse = true;
float yaw   = -90.0f;	
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

//tiempo de velocidad de movimiento
float deltaTime = 0.0f;	
float lastFrame = 0.0f; 
float currentFrame;

// posición de la luz estatica y direccional
glm::vec3 lightStaticPos(0.0f, -4.0f, 0.0f);
glm::vec3 lightDirectionalPos(-1.0f, -1.0f, 0.0f);

//texturas
unsigned int texture1;
unsigned int texture2;
signed int textureSelection=1;

//movimiento orbital
int orbitalDirection= 1;
const int DERECHA = 1;
const int IZQUIERDA = 2;
const int ARRIBA = 3;
const int ABAJO = 4;

int main(){ 
        
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    if (!gladLoadGL()) {
        std::cout << "Failed to initalize glad\n";
        glfwDestroyWindow(window);
        glfwTerminate();
    } 

glfwSetCursorPosCallback(window, mouse_callback);
glfwSetScrollCallback(window, scroll_callback);
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

Shader centarCuboContainerShader("shaders/shader.vs", "shaders/shader.fs");
Shader lightStaticShader("shaders/cubolightStatic.vs", "shaders/cubolightStatic.fs");
Shader lightRoundShader("shaders/cubolightRound.vs", "shaders/cubolightRound.fs");
// Shader lightDirectionalShader("shaders/cubolightDirectional.vs", "shaders/cubolightDirectional.fs");

glEnable(GL_DEPTH_TEST);

float vertices_triangulo[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};
    
    texture1 = loadTexture("images/mosaic_figura3.jpg");
    texture2 = loadTexture("images/mosaic_figura3_especular.jpg");

    unsigned int cuadrado_vao;
    glGenVertexArrays(1, &cuadrado_vao);
    glBindVertexArray(cuadrado_vao);
    
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangulo), vertices_triangulo, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    unsigned int lightStatic_vao;
    glGenVertexArrays(1, &lightStatic_vao);
    glBindVertexArray(lightStatic_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangulo), vertices_triangulo, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int lightRound_vao;
    glGenVertexArrays(1, &lightRound_vao);
    glBindVertexArray(lightRound_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangulo), vertices_triangulo, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unsigned int lightDirectional_vao;
    // glGenVertexArrays(1, &lightDirectional_vao);
    // glBindVertexArray(lightDirectional_vao);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangulo), vertices_triangulo, GL_STATIC_DRAW); 
    
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    
    centarCuboContainerShader.use();
    centarCuboContainerShader.setInt("material.diffuse", 0);
    centarCuboContainerShader.setInt("material.specular", 1);
    
   while (!glfwWindowShouldClose(window))
    {
        processInput(window);
    
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        const float orbitalRadius =  10.0f;
        const float orbitalSpeed = 8.0f;
        const float camerax = sin(glfwGetTime()/ orbitalSpeed)  * orbitalRadius;
        const float cameraz = cos(glfwGetTime()/ orbitalSpeed) * orbitalRadius;

        switch ( orbitalDirection ) {
            case DERECHA:
                    cameraPos = glm::vec3(camerax ,  0.0f , cameraz );
                    break;
            case  IZQUIERDA:
                    cameraPos = glm::vec3(-camerax ,  0.0f , cameraz );
                    break;
            case  ARRIBA:
                    cameraPos = glm::vec3(camerax ,  cameraz , 0.0f );
                    break;
            case  ABAJO:
                    cameraPos = glm::vec3(camerax ,  -cameraz , 0.0f );
                    break;
            }
        
        glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);        
        glm::mat4 view = glm::lookAt(cameraPos, cameraFront, cameraUp);
        glm::mat4 model;
        model         = glm::mat4(1.0f);

        centarCuboContainerShader.use();
        centarCuboContainerShader.setMat4("projection", projection);
        centarCuboContainerShader.setMat4("view", view);

        centarCuboContainerShader.setFloat("material.shininess", 64.0f);
        centarCuboContainerShader.setVec3("lightDirection.ambient",  0.2f, 0.2f, 0.2f);
        centarCuboContainerShader.setVec3("lightDirection.diffuse",  0.5f, 0.5f, 0.5f); 
        centarCuboContainerShader.setVec3("lightDirection.specular", 	1.0f, 1.0f, 1.0f);
        centarCuboContainerShader.setVec3("lightDirection.direction", lightDirectionalPos.x, lightDirectionalPos.y, lightDirectionalPos.z);

        centarCuboContainerShader.setVec3("lightPoints[0].ambient",  0.2f, 0.2f, 0.2f);
        centarCuboContainerShader.setVec3("lightPoints[0].diffuse",  0.5f, 0.5f, 0.5f); 
        centarCuboContainerShader.setVec3("lightPoints[0].specular", 	1.0f, 1.0f, 1.0f);
        centarCuboContainerShader.setVec3("lightPoints[0].position", lightStaticPos.x, lightStaticPos.y, lightStaticPos.z);
        centarCuboContainerShader.setFloat("lightPoints[0].constant", 	1.0f);
        centarCuboContainerShader.setFloat("lightPoints[0].linear", 	0.0014);
        centarCuboContainerShader.setFloat("lightPoints[0].cuadratic", 0.000007);

        const float radiusLightRound =  4.0f;
        const float lightRoundx = sin(glfwGetTime()) * radiusLightRound;
        const float lightRoundz = cos(glfwGetTime()) * radiusLightRound;
        centarCuboContainerShader.setVec3("lightPoints[1].ambient",  0.2f, 0.2f, 0.2f);
        centarCuboContainerShader.setVec3("lightPoints[1].diffuse",  0.5f, 0.5f, 0.5f); 
        centarCuboContainerShader.setVec3("lightPoints[1].specular", 	1.0f, 1.0f, 1.0f);
        centarCuboContainerShader.setVec3("lightPoints[1].position", lightRoundx, 0.0f, lightRoundz);

        centarCuboContainerShader.setFloat("lightPoints[1].constant", 	1.0f);
        centarCuboContainerShader.setFloat("lightPoints[1].linear", 	0.0014	);
        centarCuboContainerShader.setFloat("lightPoints[1].cuadratic", 0.000007);

        centarCuboContainerShader.setVec3("viewPos",  cameraPos.x, cameraPos.y, cameraPos.z);
        centarCuboContainerShader.setMat4("model", model);

        model = glm::translate(model,    glm::vec3(0.0f, 0.0f, 0.0f));
        centarCuboContainerShader.setMat4("model", model);
        glBindVertexArray(cuadrado_vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //cubo estático abajo
        lightStaticShader.use();
        lightStaticShader.setMat4("projection", projection);
        lightStaticShader.setMat4("view", view);
        model         = glm::mat4(1.0f);
        model = glm::translate(model,    lightStaticPos);
        model = glm::scale(model, glm::vec3(0.2f)); 
        lightStaticShader.setMat4("model", model);
        glBindVertexArray(lightStatic_vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        //cubo giratorio
        lightRoundShader.use();
        lightRoundShader.setMat4("projection", projection);
        lightRoundShader.setMat4("view", view);
        model         = glm::mat4(1.0f);
        model = glm::translate(model,  glm::vec3(lightRoundx, 0.0f, lightRoundz));
        model = glm::scale(model, glm::vec3(0.2f));
        lightRoundShader.setMat4("model", model);
        glBindVertexArray(lightRound_vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //cubo que que muestra la direccion de la luz direccional
        // lightDirectionalShader.use();
        // lightDirectionalShader.setMat4("projection", projection);
        // lightDirectionalShader.setMat4("view", view);
        // model         = glm::mat4(1.0f);
        // model = glm::translate(model,  lightDirectionalPos);
        // model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(0.2f));
        // lightDirectionalShader.setMat4("model", model);
        // glBindVertexArray(lightDirectional_vao);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSetKeyCallback(window, key_callback);  
        glfwPollEvents();
        glfwSwapBuffers(window);
    } 
glDeleteVertexArrays(1, &cuadrado_vao);
glDeleteVertexArrays(1, &lightStatic_vao);
glDeleteVertexArrays(1, &lightRound_vao);
// glDeleteVertexArrays(1, &lightDirectional_vao);
glDeleteBuffers(1, &vbo);
glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if( textureSelection != 1){
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS ){
            texture1 = loadTexture("images/mosaic_figura3.jpg");
            texture2 = loadTexture("images/mosaic_figura3_especular.jpg");
            textureSelection = 1;
        }
    }
    if(textureSelection != 2){
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS  || glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS ){
            texture1 = loadTexture("images/grass_figura2.jpg");
            texture2 = loadTexture("images/grass_figura2_especular.jpg");
            textureSelection = 2;
        }
    }
    if(textureSelection != 3){
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS ){
            texture1 = loadTexture("images/bloques_figura1.jpg");
            texture2 = loadTexture("images/bloques_figura1_especular.jpg");
            textureSelection = 3;
        }
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   
    if(key == GLFW_KEY_RIGHT){
        switch (action ) {
            case GLFW_PRESS:
                    orbitalDirection = DERECHA;
                    break;
            case  GLFW_REPEAT:
                    break;
            }
    }
    if(key == GLFW_KEY_LEFT){
        switch (action ) {
            case GLFW_PRESS:
                    orbitalDirection = IZQUIERDA;
                    break;
            case  GLFW_REPEAT:
                    break;
            }
    }
    if(key == GLFW_KEY_UP){
        switch (action ) {
            case GLFW_PRESS:
                    orbitalDirection = ARRIBA;
                    break;
            case  GLFW_REPEAT:
                    break;
            }
    }
    if(key == GLFW_KEY_DOWN){
        switch (action ) {
            case GLFW_PRESS:
                    orbitalDirection = ABAJO;
                    break;
            case  GLFW_REPEAT:
                    break;
            }
    }
    if(key == GLFW_KEY_KP_ADD){
        switch (action ) {
            case GLFW_PRESS:
                    fov -= 5;
                    if (fov < 1.0f)
                        fov = 1.0f;
                    
                    break;
            case  GLFW_REPEAT:
                    break;
            }
    }
    if(key == GLFW_KEY_KP_SUBTRACT){
        switch (action ) {
            case GLFW_PRESS:
                    fov += 5;
                    if (fov > 45.0f)
                        fov = 45.0f;
                    break;
            case  GLFW_REPEAT:
                    break;
            }
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}