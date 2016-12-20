#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdlib.h>

#include "shader.h"

using namespace std;

const int WIDTH = 800, HEIGHT = 600;

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);

glm::vec3 position = glm::vec3(-3.0f, 3.0f, -3.0f);
glm::vec3 viewDirection = glm::vec3(1.0f, -1.0f, 1.0f);
glm::vec3 strafeDirection;
glm::vec3 RIGHT;
glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec2 oldMousePosition;

bool perspective = false;

const float MOVEMENT_SPEED = 0.1f;
const float ROTATIONAL_SPEED = 0.005f;
GLfloat zoom = 45.0f;

int r = 0;

GLfloat cx, cy, cz;

GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat z = 0.0f;

GLfloat dy = 0.0f;

int main() {
    if (!glfwInit()){
        cout << "Failed to initialize GLEW" << endl;
        return -1;
    }
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Modern GL", NULL, NULL);

    int screenWidth, screenHeight;
	glfwGetFramebufferSize( window, &screenWidth, &screenHeight );

	if (!window){
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, KeyCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()){
		cout << "Failed to initialize GLEW" << endl;
		return -1;
	}

    glViewport(0,0, screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader ourShader("VertexShaderCode.vert", "FragmentShaderCode.frag");
/*
    GLfloat Svertices[] = {
        -1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f,

        -1.0f, 1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f
    };
*/

	GLfloat Svertices[] = {
        -1.0f, 1.0f, 1.0f,     1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,     1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, -1.0f,     1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, -1.0f,     1.0f, 0.0f, 0.0f,

        -1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 1.0f,     1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,     1.0f, 1.0f, 0.0f,

        1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f,     0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, -1.0f,     0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 1.0f,     0.0f, 1.0f, 0.0f,

        1.0f, 1.0f, -1.0f,     0.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,     0.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,     0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,     0.0f, 1.0f, 1.0f,

        -1.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,     0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,     0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,     0.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 1.0f,     1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f,     1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,     1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,     1.0f, 0.0f, 1.0f
    };

    GLfloat Tvertices[] = {
        -1.0f, -2.0f, 1.0f,     1.0f, 0.5f, 0.0f,
        1.0f, -2.0f, 1.0f,     1.0f, 0.5f, 0.0f,
        1.0f, -2.0f, -1.0f,     1.0f, 0.5f, 0.0f,
        -1.0f, -2.0f, -1.0f,     1.0f, 0.5f, 0.0f
    };

	GLuint VAO0, VBO0;
    glGenVertexArrays(1, &VAO0);
    glGenBuffers(1, &VBO0);

    glBindVertexArray(VAO0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Svertices), Svertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO0);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(Svertices_color), Svertices_color, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    GLuint VAO1, VBO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Tvertices), Tvertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    while(!glfwWindowShouldClose(window)){
        float cd;
        if (dy <= 0) {cd = 0.0005f;}
        else if (dy >= 5) {cd = -0.0005f;}
        dy += cd;

        r = rand() % 9;

        if (r >= 0 && r <= 2) {
            if (x <= 0) {cx = 0.0005f;}
            else if (x >= 1) {cx = -0.0005f;}
            x += cx;
        }
        if (r >= 2 && r <= 5) {
            if (y <= 0) {cy = 0.0005f;}
            else if (y >= 1) {cy = -0.0005f;}
            y += cy;
        }
        if (r >= 6 && r <= 8) {
            if (z <= 0) {cz = 0.0005f;}
            else if (z >= 1) {cz = -0.0005f;}
            z += cz;
        }

        glfwPollEvents();
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();

        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 tempview;

        projection = glm::perspective(glm::radians(zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        if(perspective) {
            view = glm::lookAt(
                glm::vec3(8,6,6), // Camera is at (4,3,3), in World Space
                glm::vec3(1,1,1), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
            );
            position = glm::vec3(8,6,6);
            RIGHT = glm::vec3(1,1,1);
            viewDirection = RIGHT - position;
        }
        else {
            view = glm::lookAt(position, RIGHT, UP);
        }

        if (!perspective) {
            model = glm::translate(model, glm::vec3(0.0f, dy, 0.0f));
            model *= glm::rotate(model, (GLfloat)glfwGetTime() * -1.0f, glm::vec3(x, y, z));
        }

        else {

        }

        glm::mat4 MVP = projection * view * model;

        GLint mvpLoc = glGetUniformLocation(ourShader.Program, "MVP");

        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(MVP));

        glBindVertexArray(VAO0);
        glDrawArrays(GL_QUADS, 0, 24);
        //glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, 0);

        glm::mat4 idlemodel;

        if (!perspective) { idlemodel = glm::mat4(1.0f); }
        else {

        }

        MVP = projection * view * idlemodel;

        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(MVP));

        glBindVertexArray(VAO1);
        glDrawArrays(GL_QUADS, 0, 4);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO0);
	glDeleteBuffers(1, &VBO0);
    glfwTerminate();
    return 0;
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {glfwSetWindowShouldClose(window, GL_TRUE);}

    if(key == GLFW_KEY_P && action == GLFW_PRESS){ perspective = perspective ^ true; }

    if(key == GLFW_KEY_Q || key == GLFW_KEY_E){
        if (key == GLFW_KEY_Q) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            cout << "Q Pressed" << endl;
        }
        else if (key == GLFW_KEY_E) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            cout << "E Pressed" << endl;
        }
    }

    if(key == GLFW_KEY_W) {position += MOVEMENT_SPEED * viewDirection;    cout << "W Pressed" << endl;}
    if(key == GLFW_KEY_S) {position -= MOVEMENT_SPEED * viewDirection;    cout << "S Pressed" << endl;}
    if(key == GLFW_KEY_A) {position -= MOVEMENT_SPEED * strafeDirection;  cout << "A Pressed" << endl;}
    if(key == GLFW_KEY_D) {position += MOVEMENT_SPEED * strafeDirection;  cout << "D Pressed" << endl;}
    if(key == GLFW_KEY_SPACE) {position += MOVEMENT_SPEED * UP;   cout << "Space Pressed" << endl;}
    if(key == GLFW_KEY_LEFT_SHIFT) {position -= MOVEMENT_SPEED * UP;   cout << "L Shift Pressed" << endl;}
}

void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    if ( zoom >= 1.0f && zoom <= 90.0f ) { zoom -= 2 * yOffset; }
    if ( zoom <= 1.0f ) { zoom = 1.0f; }
    if ( zoom >= 90.0f ) { zoom = 90.0f; }
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
    glm::vec2 newMousePosition;
    newMousePosition.x = xPos;
    newMousePosition.y = yPos;
    glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) > 50.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}

	strafeDirection = glm::cross(viewDirection, UP);
	glm::mat4 rotator = glm::rotate(-1 * mouseDelta.x * ROTATIONAL_SPEED, UP) *
			glm::rotate(-1 * mouseDelta.y * ROTATIONAL_SPEED, strafeDirection);

    viewDirection = glm::mat3(rotator) * viewDirection;
    RIGHT = position + viewDirection;
	oldMousePosition = newMousePosition;
}
