// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Thetraedra", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");




    const GLfloat g_vertex_buffer_data[] = {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f,

        -1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f,

        1.f, 0.f, 0.f,
        0.f, -1.f, 0.f,
        0.f, 0.f, 1.f,

        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, -1.f,

        -1.f, 0.f, 0.f,
        0.f, -1.f, 0.f,
        0.f, 0.f, 1.f,

        -1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, -1.f,

        1.f, 0.f, 0.f,
        0.f, -1.f, 0.f,
        0.f, 0.f, -1.f,

        -1.f, 0.f, 0.f,
        0.f, -1.f, 0.f,
        0.f, 0.f, -1.f,
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


    // One color for each vertex. They were generated randomly.
    static const GLfloat g_color_buffer_data[] = {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f,

        0.f, 0.5f, 0.5f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f,

        1.f, 0.f, 0.f,
        0.5f, 0.f, 0.5f,
        0.f, 0.f, 1.f,

        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.5f, 0.5f, 0.f,

        0.f, 0.5f, 0.5f,
        0.5f, 0.f, 0.5f,
        0.f, 0.f, 1.f,

        0.f, 0.5f, 0.5f,
        0.f, 1.f, 0.f,
        0.5f, 0.5f, 0.f,

        1.f, 0.f, 0.f,
        0.5f, 0.f, 0.5f,
        0.5f, 0.5f, 0.f,

        0.f, 0.5f, 0.5f,
        0.5f, 0.f, 0.5f,
        0.5f, 0.5f, 0.f,
    };



    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 Model      = glm::mat4(1.0f);


    GLfloat alpha = 0.0f;

    do{

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 View = glm::lookAt(
            glm::vec3(3 * cos(3* alpha) * std::cos(alpha), 3 * sin(3* alpha)  * std::cos(alpha) , 3 * std::sin(alpha)),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
        );

        alpha += 0.01;

        glm::mat4 MVP        = Projection * View * Model;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

        glUseProgram(programID);
        glDrawArrays(GL_TRIANGLES, 0, 24);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );


    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);


    glfwTerminate();

    return 0;
}

