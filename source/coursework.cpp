#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/maths.hpp>
#include <common/camera.hpp>
#include <common/model.hpp>
#include <common/light.hpp>

// Function prototypes
void keyboardInput(GLFWwindow *window);
void mouseInput(GLFWwindow* window);

// Frame timers
float previousTime = 0.0f;  // time of previous iteration of the loop
float deltaTime = 0.0f;  // time elapsed since the previous frame

Camera camera(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f));

struct Object
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    float angle = 0.0f;
    std::string name;
};

int main( void )
{
    // =========================================================================
    // Window creation - you shouldn't need to change this code
    // -------------------------------------------------------------------------
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "Computer Graphics Coursework", NULL, NULL);
    
    if( window == NULL ){
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    // -------------------------------------------------------------------------
    // End of window creation
    // =========================================================================
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Use back face culling
    glEnable(GL_CULL_FACE);

    // Ensure we can capture keyboard inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Capture mouse inputs
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    

    // Compile shader program
    unsigned int shaderID, lightShaderID;
    shaderID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
  
   //lightShaderID = LoadShaders("lightVertexShader.glsl", "lightFragmentShader.glsl");

    // Activate shader
    glUseProgram(shaderID);

    // Load models
    Model cube("../assets/cube.obj");
    Model sphere("../assets/sphere.obj");
    Model teapot("../assets/teapot.obj");
    //
    // Load the textures
    cube.addTexture("../assets/crate.jpg", "diffuse");
    sphere.addTexture("../assets/bricks_deffuse.png", "diffuse");
    teapot.addTexture("../assets/blue.bmp", "diffuse");




    // Define cube object lighting properties
    cube.ka = 1.0f;
    cube.kd = 0.0f;
    cube.ks = 0.0f;
    cube.Ns = 20.0f;
    sphere.ka = 1.0f;
    sphere.kd = 0.0f;
    sphere.ks = 0.0f;
    sphere.Ns = 20.0f;
    teapot.ka = 1.0f;
    teapot.kd = 0.0f;
    teapot.ks = 0.0f;
    teapot.Ns = 20.0f;

    // Add light sources
    Light lightSources;
    lightSources.addDirectionalLight(glm::vec3(1.0f, -1.0f, 0.0f),  // direction
        glm::vec3(1.0f, 1.0f, 0.0f));  // colour
    //lightSources.addSpotLight(
    //    glm::vec3(5.0f, 5.0f, 0.0f),            // position: above the origin
    //    glm::vec3(0.0f, -1.0f, 0.0f),           // direction: pointing down
    //    glm::vec3(1.0f, 1.0f, 1.0f),            // color: white light
    //    glm::cos(glm::radians(5.0f)),           // inner cutoff: tight beam
    //    glm::cos(glm::radians(8.0f)),           // outer cutoff: soft edge
    //    1.0f,                                   // intensity: normal brightness
    //    true                                    // enabled: spotlight is active
    //);



    // Cube positions
    glm::vec3 positions[] = {
        glm::vec3(1.0f,  -3.0f,  1.0f),
        glm::vec3(1.0f, -3.0f, 2.0f),
        glm::vec3(1.0f, -3.0f, 3.0f),
        glm::vec3(2.0f, -3.0f, 1.0f),
        glm::vec3(2.0f,  -3.0f, 2.0f),
        glm::vec3(2.0f,  -3.0f, 3.0f),
        glm::vec3(3.0f, -3.0f, 1.0f),
        glm::vec3(3.0f,  -3.0f, 2.0f),
        glm::vec3(3.0f,  -3.0f, 3.0f),
        glm::vec3(1.0f,  -3.0f, -1.0f),
        glm::vec3(4.0f,  -3.0f, 1.0f),
        glm::vec3(4.0f, -3.0f, 2.0f),
        glm::vec3(4.0f,  -3.0f, 3.0f),
        glm::vec3(1.0f,  -3.0f,  4.0f),
        glm::vec3(1.0f, -3.0f, 5.0f),
        glm::vec3(1.0f, -3.0f, 6.0f),
        glm::vec3(2.0f, -3.0f, 4.0f),
        glm::vec3(2.0f,  -3.0f, 5.0f),
        glm::vec3(2.0f,  -3.0f, 6.0f),
        glm::vec3(3.0f, -3.0f, 4.0f),
        glm::vec3(3.0f,  -3.0f, 5.0f),
        glm::vec3(3.0f,  -3.0f, 6.0f),
        glm::vec3(1.0f,  -3.0f, 0.0f),
        glm::vec3(4.0f,  -3.0f, 4.0f),
        glm::vec3(4.0f, -3.0f, 5.0f),
        glm::vec3(4.0f,  -3.0f, 6.0f)

    };
    // teapot positions
    glm::vec3 positionstea[] = {
        glm::vec3(0.0f,  0.0f,  1.0f),
        glm::vec3(2.0f,  0.0f, 3.0f),
        glm::vec3(5.0f, 0.0f, 2.0f),
    };
    // speare positions
    glm::vec3 positionsround[] = {
        glm::vec3(2.0f,  -1.0f,  2.0f),
        
    };

    // Add teapots to objects vector
    std::vector<Object> objects;
    Object object;
    Object objtea;
    Object round;
    object.name = "cube";
    objtea.name = "teapot";
    round.name = "sphere";
    for (unsigned int i = 0; i < 26; i++)
    {
        object.position = positions[i];
        object.rotation = glm::vec3(1.0f, 1.0f, 1.0f);
        object.scale = glm::vec3(0.5f, 0.5f, 0.5f);
        //object.angle = Maths::radians(20.0f * i);
        objects.push_back(object);
    }
    for (unsigned int i = 0; i < 3; i++)
    {
        objtea.position = positionstea[i];
        objtea.rotation = glm::vec3(1.0f, 1.0f, 1.0f);
        objtea.scale = glm::vec3(0.5f, 0.5f, 0.5f);
        objtea.angle = Maths::radians(20.0f * i);
        objects.push_back(objtea);
    }
    for (unsigned int i = 0; i < 1; i++)
    {
        round.position = positionsround[i];
        round.rotation = glm::vec3(1.0f, 1.0f, 1.0f);
        round.scale = glm::vec3(0.5f, 0.5f, 0.5f);
        round.angle = Maths::radians(20.0f * i);
        objects.push_back(round);
    }
    

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Update timer
        float time = glfwGetTime();
        deltaTime = time - previousTime;
        previousTime = time;

        // Get inputs
        keyboardInput(window);
        mouseInput(window);

        // Clear the window
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate view and projection matrices
        camera.target = camera.eye + camera.front;
        //camera.calculateMatrices();
        camera.quaternionCamera();
        // Activate shader
        glUseProgram(shaderID);

        // Send light source properties to the shader
        lightSources.toShader(shaderID, camera.view);

        // Send view matrix to the shader
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "V"), 1, GL_FALSE, &camera.view[0][0]);

        // Loop through objects
        for (unsigned int i = 0; i < static_cast<unsigned int>(objects.size()); i++)
        {
            // Calculate model matrix
            glm::mat4 translate = Maths::translate(objects[i].position);
            glm::mat4 scale = Maths::scale(objects[i].scale);
            glm::mat4 rotate = Maths::rotate(objects[i].angle, objects[i].rotation);
            glm::mat4 model = translate * rotate * scale;

            // Send the MVP and MV matrices to the vertex shader
            glm::mat4 MV = camera.view * model;
            glm::mat4 MVP = camera.projection * MV;
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MV"), 1, GL_FALSE, &MV[0][0]);

            // Draw the model
            if (objects[i].name == "cube")
                cube.draw(shaderID);
            if (objects[i].name == "teapot")
                teapot.draw(shaderID);
            if (objects[i].name == "sphere")
                sphere.draw(shaderID);
        }

        // Draw light sources
        //lightSources.draw(lightShaderID, camera.view, camera.projection, sphere);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    cube.deleteBuffers();
    glDeleteProgram(shaderID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

void keyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Move the camera using WSAD keys
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.eye += 5.0f * deltaTime * camera.front;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.eye -= 5.0f * deltaTime * camera.front;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.eye -= 5.0f * deltaTime * camera.right;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.eye += 5.0f * deltaTime * camera.right;
}

void mouseInput(GLFWwindow* window)
{
    // Get mouse cursor position and reset to centre
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Update yaw and pitch angles
    camera.yaw += 0.005f * float(xPos - 1024 / 2);
    camera.pitch += 0.005f * float(768 / 2 - yPos);

    // Calculate camera vectors from the yaw and pitch angles
    camera.calculateCameraVectors();
}