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

glm::vec3 position = glm::vec3(-8.0f, 8.0f, -8.0f);
glm::vec3 viewDirection = glm::vec3(1.0f, -1.0f, 1.0f);
glm::vec3 strafeDirection = glm::vec3(-1.0f, 0.0f, 1.0f);
glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec2 oldMousePosition;

bool ponce = false;
bool tonce = false;
bool wonce = false;

bool pause = false;
bool perspective1 = false;
bool perspective2 = false;
bool perspective3 = false;
bool perspective4 = false;
bool reset = false;
bool wireframe = false;

double time = 0.0f;

const float MOVEMENT_SPEED = 0.1f;
const float ROTATIONAL_SPEED = 0.005f;
GLfloat zoom = 45.0f;

GLfloat angle = 0.0f;

int r = 0;

GLfloat cx, cy, cz;

GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat z = 0.0f;

GLfloat dy = 0.0f;

GLfloat Svertices[] = {
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,

    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f
};

GLfloat Svertices_color[] =  {
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f
};

GLfloat Svertices_normal[] = {
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,

    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,

    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f
};

GLfloat Tvertices[] = {
    -1.0f, -2.0f, 1.0f,
    1.0f, -2.0f, 1.0f,
    1.0f, -2.0f, -1.0f,
    -1.0f, -2.0f, -1.0f
};

GLfloat Tvertices_color[] = {
    1.0f, 0.5f, 0.0f,
    1.0f, 0.5f, 0.0f,
    1.0f, 0.5f, 0.0f,
    1.0f, 0.5f, 0.0f
};

GLfloat Tvertices_normal[] = {
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};

GLfloat Temp_Cube_Color[24*3];
GLfloat Temp_Plane_Color[4*3];

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

    for (int v = 0; v < 24; v++) {
        Temp_Cube_Color[3 * v + 0] = Svertices_color[3 * v + 0];
        Temp_Cube_Color[3 * v + 1] = Svertices_color[3 * v + 1];
        Temp_Cube_Color[3 * v + 2] = Svertices_color[3 * v + 2];
    }
    for (int v = 0; v < 4; v++) {
        Temp_Plane_Color[3 * v + 0] = Tvertices_color[3 * v + 0];
        Temp_Plane_Color[3 * v + 1] = Tvertices_color[3 * v + 1];
        Temp_Plane_Color[3 * v + 2] = Tvertices_color[3 * v + 2];
    }

	GLuint VAO0, VBO0, VBO0C, VBO0N;
    glGenVertexArrays(1, &VAO0);
    glGenBuffers(1, &VBO0);

    glBindVertexArray(VAO0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Svertices), Svertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &VBO0C);
    glBindBuffer(GL_ARRAY_BUFFER, VBO0C);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Svertices_color), Svertices_color, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(2, &VBO0N);
    glBindBuffer(GL_ARRAY_BUFFER, VBO0N);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Svertices_normal), Svertices_normal, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(2);

    GLuint VAO1, VBO1, VBO1C, VBO1N;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Tvertices), Tvertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &VBO1C);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1C);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Tvertices_color), Tvertices_color, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(2, &VBO1N);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1N);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Tvertices_normal), Tvertices_normal, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(2);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    while(!glfwWindowShouldClose(window)){
        if(wireframe && wonce) {
            for (int v = 0; v < 24; v++) {
                Svertices_color[3 * v + 0] = 1.0f;
                Svertices_color[3 * v + 1] = 1.0f;
                Svertices_color[3 * v + 2] = 1.0f;
            }
            for (int v = 0; v < 4; v++) {
                Tvertices_color[3 * v + 0] = 1.0f;
                Tvertices_color[3 * v + 1] = 1.0f;
                Tvertices_color[3 * v + 2] = 1.0f;
            }
            wonce = false;
            cout << "Passed" << endl;
        }
        else if(!wireframe && ponce) {
            for (int v = 0; v < 24; v++) {
                Svertices_color[3 * v + 0] = Temp_Cube_Color[3 * v + 0];
                Svertices_color[3 * v + 1] = Temp_Cube_Color[3 * v + 1];
                Svertices_color[3 * v + 2] = Temp_Cube_Color[3 * v + 2];
            }
            for (int v = 0; v < 4; v++) {
                Tvertices_color[3 * v + 0] = Temp_Plane_Color[3 * v + 0];
                Tvertices_color[3 * v + 1] = Temp_Plane_Color[3 * v + 1];
                Tvertices_color[3 * v + 2] = Temp_Plane_Color[3 * v + 2];
            }
            ponce = false;
            cout << "Passed" << endl;
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO0C);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Svertices_color), Svertices_color, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO1C);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Tvertices_color), Tvertices_color, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

        if(!pause){
            if(tonce){ glfwSetTime(time); tonce = false;}
            time = glfwGetTime();
        }
        else {time = time;}

        if (!pause) {
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
            angle = time * -1.0f;
        }

        glfwPollEvents();
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();

        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 tempview;

        projection = glm::perspective(glm::radians(zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        if(reset) {
            position = glm::vec3(-8.0f, 8.0f, -8.0f);
            viewDirection = glm::vec3(1.0f, -1.0f, 1.0f);
            strafeDirection = glm::vec3(-1.0f, 0.0f, 1.0f);
            UP = glm::vec3(0.0f, 1.0f, 0.0f);
        }

        if(perspective1){
            view = glm::lookAt(
                glm::vec3(16.0f, 16.0f, 16.0f), // Camera is at (4,3,3), in World Space
                glm::vec3(0.0f, 0.0f, 0.0f), // and looks at the origin
                glm::vec3(0.0f, 1.0f, 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
            );
        }
        else if(perspective2){
            view = glm::lookAt(
                glm::vec3(16.0f, 16.0f, -16.0f), // Camera is at (4,3,3), in World Space
                glm::vec3(0.0f, 0.0f, 0.0f), // and looks at the origin
                glm::vec3(0.0f, 1.0f, 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
            );
        }
        else if(perspective3){
            view = glm::lookAt(
                glm::vec3(-16.0f, 16.0f, -16.0f), // Camera is at (4,3,3), in World Space
                glm::vec3(0.0f, 0.0f, 0.0f), // and looks at the origin
                glm::vec3(0.0f, 1.0f, 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
            );
        }
        else if(perspective4){
            view = glm::lookAt(
                glm::vec3(-16.0f, 16.0f, 16.0f), // Camera is at (4,3,3), in World Space
                glm::vec3(0.0f, 0.0f, 0.0f), // and looks at the origin
                glm::vec3(0.0f, 1.0f, 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
            );
        }
        else {
            view = glm::lookAt(position, position + viewDirection, UP);
        }

        model = glm::translate(model, glm::vec3(0.0f, dy, 0.0f));
        model *= glm::rotate(model, angle, glm::vec3(x, y, z));

        glm::mat4 MVP = projection * view * model;

        GLint mvpLoc = glGetUniformLocation(ourShader.Program, "MVP");
        GLint vLoc = glGetUniformLocation(ourShader.Program, "V");
        GLint mLoc = glGetUniformLocation(ourShader.Program, "M");
        GLint lightLoc = glGetUniformLocation(ourShader.Program, "LightPosition_worldspace");

        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(MVP));
        glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO0);
        glDrawArrays(GL_QUADS, 0, 24);
        //glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, 0);

        glm::mat4 idlemodel;

        idlemodel = glm::mat4(1.0f);

        MVP = projection * view * idlemodel;

        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(MVP));
        glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(idlemodel));

        glBindVertexArray(VAO1);
        glDrawArrays(GL_QUADS, 0, 4);

        glm::vec3 lightPos = glm::vec3(4,8,4);
        glUniform3f(lightLoc, lightPos.x, lightPos.y, lightPos.z);

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

    if(key == GLFW_KEY_P && action == GLFW_PRESS) {pause = pause ^ true; tonce = true;}

    if(key == GLFW_KEY_1 && action == GLFW_PRESS){
            perspective1 = true;
            perspective2 = false;
            perspective3 = false;
            perspective4 = false;
            viewDirection = viewDirection;
    }
    if(key == GLFW_KEY_2 && action == GLFW_PRESS){
            perspective2 = true;
            perspective1 = false;
            perspective3 = false;
            perspective4 = false;
    }
    if(key == GLFW_KEY_3 && action == GLFW_PRESS){
            perspective3 = true;
            perspective1 = false;
            perspective2 = false;
            perspective4 = false;
    }
    if(key == GLFW_KEY_4 && action == GLFW_PRESS){
            perspective4 = true;
            perspective1 = false;
            perspective2 = false;
            perspective3 = false;
    }
    if(key == GLFW_KEY_0 && action == GLFW_PRESS) {
            perspective1 = false;
            perspective2 = false;
            perspective3 = false;
            perspective4 = false;
    }

    if(key == GLFW_KEY_R){ reset = reset ^ true; }

    if(key == GLFW_KEY_Q || key == GLFW_KEY_E){
        if (key == GLFW_KEY_Q) {
            wireframe = false;
            ponce = true;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            cout << "Q Pressed" << endl;
        }
        else if (key == GLFW_KEY_E) {
            wireframe = true;
            wonce = true;
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
    if (!perspective1 && !perspective2 && !perspective3 && !perspective4) {
        viewDirection = glm::mat3(rotator) * viewDirection;
    }
	oldMousePosition = newMousePosition;
}
