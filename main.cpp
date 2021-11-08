#include <Windows.h>
#include <GLFW/glfw3.h>
#include <gl/GLU.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "colors.h"
#include "primitives.h"

static float angle = 0.0;

/* display list */
struct Object {
    GLuint id;
    vec3 pos;
    Object():id(0), pos(vec3(0.0,0.0,0.0)){}
};

const GLuint totalObjects = 20;
std::vector<Object> objects;


/* responsive resize */
void resize(int w, int h)
{
    glViewport(0, 0, w, h);

    /* fix deformation */
    float aspect = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, aspect, 0.1, 500.0);

    glMatrixMode(GL_MODELVIEW);
}

/* draw in screen */
void draw(float dt)
{
    /* Create Triangle
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
        glVertex3f(0.0, 0.5, 0.0);
        glVertex3f(-0.5, -0.5, 0.0);
        glVertex3f(0.5, -0.5, 0.0);
    glEnd();
    */

    /* Create Square
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
        glVertex3f(-2.5, -2.5, 0.0);
        glVertex3f(2.5, -2.5, 0.0);
        glVertex3f(2.5, 2.5, 0.0);
        glVertex3f(-2.5, 2.5, 0.0);
    glEnd();
    */

    /* Create Cube
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -50.0);
    glRotatef(30.0, 1.0, 1.0, 0.0);
    glScalef(2.0, 1.0, 2.0);
    */

    /* Create Cubes
    glLoadIdentity();
    glPushMatrix();
        glTranslatef(-20.0, 0.0, -50.0);
        glRotatef(angle, 1.0, 1.0, 0.0);
        drawCube(8.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(20.0, 0.0, -50.0);
        glRotatef(-angle, 1.0, 1.0, 0.0);
        drawCube(8.0);
    glPopMatrix();
    */

    /* Create Spheres */
    float veloc_ang = 60.0 * dt; // 60 degrees per second
    glLoadIdentity();

    glCallList(objects[0].id);

    for (int i = 1; i < totalObjects; i++)
    {
        float x = objects[i].pos.x;
        float y = objects[i].pos.y;
        float z = objects[i].pos.z;

        glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(angle, 1.0, 1.0, 0.0);
        glCallList(objects[i].id);
        glPopMatrix();
    }

    angle += veloc_ang;
    if (angle >= 360.0)
        angle = 0.0;
}

float random(float a, float b)
{
    float n = (float)rand() / RAND_MAX;
    float t = b - a;
    float r = a + n * t;
    return r;
}

void init()
{
    glClearColor(0.0, 0.15, 0.25, 1.0); // background color
    glEnable(GL_DEPTH_TEST);

    for (int i = 0; i < totalObjects; i++)
    {
        Object p;
        objects.push_back(p);
    }

    objects[0].id = glGenLists(totalObjects);
    drawTerrain(objects[0].id);

    for (int i = 1; i < totalObjects; i++)
    {
        objects[i].id = objects[0].id + i;
        float x = random(-100, 100);
        float y = random(8, 20);
        float z = random(-450, -20);
        objects[i].pos = vec3(x, y, z);

        float k = (float)rand() / RAND_MAX;
        if(k <= 0.5)
        { 
            drawSphere(objects[i].id, 5.0, 20.0, 20.0);
        }
        else
        {
            drawCube(objects[i].id, 5.0);
        }
    }
}

int main(void)
{
    /* Initialize the library */
    glfwInit();

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(800, 600, "Project comp graphic", NULL, NULL);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* init */
    init();

    float value = 0.0;
    float lastTime = 0.0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* FPS control */
        float currentTime = (float)glfwGetTime();
        float dt = currentTime - lastTime;
        lastTime = currentTime;

        /* Poll for and process events */
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            draw(dt);
        }
        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        resize(width, height);

        draw(dt);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    glDeleteLists(objects[0].id, totalObjects);
    glfwTerminate();
    return 0;
}