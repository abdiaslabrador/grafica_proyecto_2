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
unsigned int loadTexture(char const * path);

const float orbitalRadius =  10.0f;
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.f,  orbitalRadius);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

//zoom
float fov = 45.0f;
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
signed int textureSelection=0;

//movimiento orbital
float _x = 0.0f;
float _y = 0.0f;
float _z = 0.0f;

float acumulador_h = 0.0f;
float acumulador_v = 0.0f;

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

glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

Shader centerCuboContainerShader("shaders/shader.vs", "shaders/shader.fs");
Shader lightStaticShader("shaders/cubolightStatic.vs", "shaders/cubolightStatic.fs");
Shader lightRoundShader("shaders/cubolightRound.vs", "shaders/cubolightRound.fs");

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

    //texturas
    unsigned int txs_difusas[3];
    unsigned int  txs_especulares[3];

    txs_difusas[0] = loadTexture("images/mosaic_figura3.jpg");
    txs_especulares[0] = loadTexture("images/mosaic_figura3_especular.jpg");
    txs_difusas[1] = loadTexture("images/grass_figura2.jpg");
    txs_especulares[1] = loadTexture("images/grass_figura2_especular.jpg");
    txs_difusas[2] = loadTexture("images/bloques_figura1.jpg");
    txs_especulares[2] = loadTexture("images/bloques_figura1_especular.jpg");

    //figuras
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
    
    centerCuboContainerShader.use();
    centerCuboContainerShader.setInt("material.diffuse", 0);
    centerCuboContainerShader.setInt("material.specular", 1);
    
   while (!glfwWindowShouldClose(window))
    {
        processInput(window);
    
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, txs_difusas[textureSelection]);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, txs_especulares[textureSelection]);

        glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);        
        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), cameraUp);
        glm::mat4 model;
        model         = glm::mat4(1.0f);

        centerCuboContainerShader.use();
        centerCuboContainerShader.setMat4("projection", projection);
        centerCuboContainerShader.setMat4("view", view);

        centerCuboContainerShader.setFloat("material.shininess", 64.0f);
        centerCuboContainerShader.setVec3("lightDirection.ambient",  0.2f, 0.2f, 0.2f);
        centerCuboContainerShader.setVec3("lightDirection.diffuse",  0.5f, 0.5f, 0.5f); 
        centerCuboContainerShader.setVec3("lightDirection.specular", 	1.0f, 1.0f, 1.0f);
        centerCuboContainerShader.setVec3("lightDirection.direction", lightDirectionalPos.x, lightDirectionalPos.y, lightDirectionalPos.z);

        centerCuboContainerShader.setVec3("lightPoints[0].ambient",  0.2f, 0.2f, 0.2f);
        centerCuboContainerShader.setVec3("lightPoints[0].diffuse",  0.5f, 0.5f, 0.5f); 
        centerCuboContainerShader.setVec3("lightPoints[0].specular", 	1.0f, 1.0f, 1.0f);
        centerCuboContainerShader.setVec3("lightPoints[0].position", lightStaticPos.x, lightStaticPos.y, lightStaticPos.z);
        centerCuboContainerShader.setFloat("lightPoints[0].constant", 	1.0f);
        centerCuboContainerShader.setFloat("lightPoints[0].linear", 	0.0014);
        centerCuboContainerShader.setFloat("lightPoints[0].cuadratic", 0.000007);

        const float radiusLightRound =  4.0f;
        const float lightRoundx = sin(glfwGetTime()) * radiusLightRound;
        const float lightRoundz = cos(glfwGetTime()) * radiusLightRound;
        centerCuboContainerShader.setVec3("lightPoints[1].ambient",  0.2f, 0.2f, 0.2f);
        centerCuboContainerShader.setVec3("lightPoints[1].diffuse",  0.5f, 0.5f, 0.5f); 
        centerCuboContainerShader.setVec3("lightPoints[1].specular", 	1.0f, 1.0f, 1.0f);
        centerCuboContainerShader.setVec3("lightPoints[1].position", lightRoundx, 0.0f, lightRoundz);

        centerCuboContainerShader.setFloat("lightPoints[1].constant", 	1.0f);
        centerCuboContainerShader.setFloat("lightPoints[1].linear", 	0.0014	);
        centerCuboContainerShader.setFloat("lightPoints[1].cuadratic", 0.000007);

        centerCuboContainerShader.setVec3("viewPos",  cameraPos.x, cameraPos.y, cameraPos.z);
        centerCuboContainerShader.setMat4("model", model);

        model = glm::translate(model,    glm::vec3(0.0f, 0.0f, 0.0f));
        centerCuboContainerShader.setMat4("model", model);
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

        glfwSetKeyCallback(window, key_callback);  
        glfwPollEvents();
        glfwSwapBuffers(window);
    } 
glDeleteVertexArrays(1, &cuadrado_vao);
glDeleteVertexArrays(1, &lightStatic_vao);
glDeleteVertexArrays(1, &lightRound_vao);
glDeleteBuffers(1, &vbo);
glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  
void processInput(GLFWwindow *window)
{
    const float cameraSpeed =2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        if (acumulador_v < 1.4f){
            acumulador_v += cameraSpeed;
        }
        _y = sin(acumulador_v) * orbitalRadius;
        _z = cos(acumulador_h) * orbitalRadius;
        cameraPos = glm::vec3(_x,  _y, _z);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        if (acumulador_v > -1.4f){
            acumulador_v -= cameraSpeed;
        }
        _y = sin(acumulador_v) * orbitalRadius;
        _z = cos(acumulador_h) * orbitalRadius;
        cameraPos = glm::vec3(_x,  _y, _z);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        acumulador_h += cameraSpeed;
        _x = sin(acumulador_h) * orbitalRadius;
        _z = cos(acumulador_h) * orbitalRadius;
        cameraPos = glm::vec3(_x ,  _y , _z );
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        acumulador_h -= cameraSpeed;
        _x = sin(acumulador_h) * orbitalRadius;
        _z = cos(acumulador_h) * orbitalRadius;
        cameraPos = glm::vec3(_x ,  _y, _z );
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   
     if(key == GLFW_KEY_1 || key == GLFW_KEY_KP_1){
        switch (action ) {
            case GLFW_PRESS:
                    textureSelection = 0;
                    break;
            case  GLFW_REPEAT:
                    break;
            }
    }
     if(key == GLFW_KEY_2 || key == GLFW_KEY_KP_2){
        switch (action ) {
            case GLFW_PRESS:
                    textureSelection = 1;
                    break;
            case  GLFW_REPEAT:
                    break;
            }
    }
     if(key == GLFW_KEY_3 || key == GLFW_KEY_KP_3){
        switch (action ) {
            case GLFW_PRESS:
                    textureSelection = 2;
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