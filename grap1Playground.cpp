#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>
#include <GLFW/glfw3.h>


double getVertex(int numPoint, char var)
{
    double x = 0;
    double z = 0;

    float angle = 2 * M_PI / 8;
    float radius = 0.5f;

    for (int i = 0; i < numPoint; i++)
    {
        x = radius * cos(angle * i); // left right
        z = radius * sin(angle * i); // up down
    }

    //std::cout << x << " " << z << std::endl;

    if (var == 'x')
    {
        return x;
    }

    else
    {
        return z;
    }
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Atasha Limcuando", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glLoadIdentity();
        glTranslatef(0.0f, 0.53f, 0.0f);
        glRotatef(23.0f, 0.0f, 0.0f, 1.0f);

        glBegin(GL_POLYGON);

        glVertex2f(getVertex(1, 'x'), getVertex(1, 'z'));
        glVertex2f(getVertex(2, 'x'), getVertex(2, 'z'));
        glVertex2f(getVertex(3, 'x'), getVertex(3, 'z'));
        glVertex2f(getVertex(4, 'x'), getVertex(4, 'z'));
        glVertex2f(getVertex(5, 'x'), getVertex(5, 'z'));
        glVertex2f(getVertex(6, 'x'), getVertex(6, 'z'));
        glVertex2f(getVertex(7, 'x'), getVertex(7, 'z'));
        glVertex2f(getVertex(8, 'x'), getVertex(8, 'z'));

        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}