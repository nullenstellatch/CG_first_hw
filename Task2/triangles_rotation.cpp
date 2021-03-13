#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <dirent.h>
#include <unistd.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
using namespace glm;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <common/shader.hpp>

int main( void )
{

    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( 1024, 768, "TrianglesRotations", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window.\n" );
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

    // White
    glClearColor(1.f, 1.f, 1.f, 0.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programLeft = LoadShaders("VertexMVP.vertexshader", "LeftTriangle.fragmentshader");
    GLuint MatrixLeft = glGetUniformLocation(programLeft, "MVP");

    GLuint programRight = LoadShaders("VertexMVP.vertexshader", "RightTriangle.fragmentshader");
    GLuint MatrixRight = glGetUniformLocation(programRight, "MVP");


    static const GLfloat g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.3f,  0.0f, 0.0f,
        -0.3f, -0.0f, 0.0f,
        0.6f, 0.3f, 0.0f,
        0.6f, -0.3f, 0.0f,
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 Model = glm::mat4(1.0f);

    GLfloat alpha = 0.0f;

    do{
        glClear( GL_COLOR_BUFFER_BIT );


        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glm::mat4 View = glm::lookAt(
            glm::vec3(5 * std::cos(alpha), 0.0 , 5 * std::sin(alpha)),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
        );

        alpha += 0.01;

        glm::mat4 MVP = Projection * View * Model;

        glEnableVertexAttribArray(0);
        glUseProgram(programLeft);
        glUniformMatrix4fv(MatrixLeft, 1, GL_FALSE, &MVP[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(programRight);
        glUniformMatrix4fv(MatrixRight, 1, GL_FALSE, &MVP[0][0]);
        glDrawArrays(GL_TRIANGLES, 3, 3);

        glDisableVertexAttribArray(0);


        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );


    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programLeft);
    glDeleteProgram(programRight);


    glfwTerminate();

    return 0;
}
