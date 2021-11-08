#pragma once
#include <vector>
#include <cmath>

struct vec3
{
    float x, y, z;
    vec3() :x(0.0), y(0.0), z(0.0){}
    vec3(float x_, float y_, float z_):x(x_),y(y_),z(z_){}
};

/* assembly cube vertices */
void rect(vec3 p1, vec3 p2, vec3 p3, vec3 p4, color color)
{
    glColor3fv(color);
    glBegin(GL_QUADS);
    glVertex3fv(&p1.x);
    glVertex3fv(&p2.x);
    glVertex3fv(&p3.x);
    glVertex3fv(&p4.x);
    glEnd();
}

/* draw cube */
void drawCube(GLuint id, float c)
{
    float d = c / 2.0;

    vec3 v1( -d, d, d );
    vec3 v2( - d, -d, d );
    vec3 v3( d, -d, d );
    vec3 v4( d, d, d );

    vec3 v5( d, d, -d );
    vec3 v6( d, -d, -d );
    vec3 v7( - d, -d, -d );
    vec3 v8( - d, d, -d );

    glNewList(id, GL_COMPILE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // front
    rect(v1, v2, v3, v4, red);

    // right
    rect(v4, v3, v6, v5, blue);

    // back
    rect(v5, v8, v7, v6, yellow);

    // left
    rect(v1, v8, v7, v2, green);

    // top
    rect(v1, v4, v5, v8, orange);

    // bottom
    rect(v2, v7, v6, v3, violet);

    glEndList();
}

void drawSphere(unsigned int& id, GLfloat ray, GLuint nStacks, GLuint nSectors)
{
    std::vector< std::vector<GLuint> > indexes;
    std::vector<vec3> points;

    const GLfloat PI = 3.14159265359;

    GLfloat deltaPhi = PI / nStacks;
    GLfloat deltaTheta = 2 * PI / nSectors;

    for (GLuint i = 0; i <= nStacks; i++)
    {
        GLfloat phi = -PI / 2.0 + i * deltaPhi;
        GLfloat temp = ray * cos(phi);
        GLfloat y = ray * sin(phi);

        std::vector<GLuint> pt;

        for (GLuint j = 0; j < nSectors; j++)
        {
            GLfloat theta = j * deltaTheta;
            GLfloat x = temp * sin(theta);
            GLfloat z = temp * cos(theta);

            points.push_back(vec3(x, y, z));
            GLuint index = points.size() - 1;
            pt.push_back(index);
        } // next j

        indexes.push_back(pt);
    } // next i

    /* points plot
    glColor3fv(red);
    glPointSize(2.5);
    glBegin(GL_POINTS);
    for (GLuint i = 0; i < points.size(); i++)
    {
        glVertex3fv(&points[i].x);
    }
    glEnd();
    */

    /* points plot */
    // glColor3fv(orange);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glNewList(id, GL_COMPILE);

    for (GLuint i = 0; i < nStacks; i++)
    {
        if (i % 3 == 0)
            glColor3fv(white_ice);
        else if (i % 3 == 1)
            glColor3fv(grey);
        else
            glColor3fv(grey_dark);

        glBegin(GL_TRIANGLE_STRIP);

        for (GLuint j = 0; j < nSectors; j++)
        {
            GLuint index = indexes[i][j];
            glVertex3fv(&points[index].x);
            index = indexes[i + 1][j];
            glVertex3fv(&points[index].x);

            if (j == nSectors - 1)
            {
                index = indexes[i][0];
                glVertex3fv(&points[index].x);
                index = indexes[i + 1][0];
                glVertex3fv(&points[index].x);
            }
        }
        glEnd();
    }
    glEndList();
}

void drawTerrain(unsigned int id)
{
    float L = 500.0;
    float incr = 1.0;
    float y = -0.5;

    glNewList(id, GL_COMPILE);
    glColor3fv(white_ice);
    glBegin(GL_LINES);
    for (float i = -L; i <= L; i += incr)
    {
        // vertical
        glVertex3f(i, y, -L);
        glVertex3f(i, y, L);

        // horizontal
        glVertex3f(-L, y, i);
        glVertex3f(L, y, i);
    }
    glEnd();
    glEndList();
}