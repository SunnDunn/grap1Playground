#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

float angle = 0.0f;
float radius = 0.75f;

float x_mod = 0;
float y_mod = 0;
float z_mod = 0;

float scale_x_mod = 0;
float scale_y_mod = 0;
float scale_z_mod = 0;

float axis_x_mod = 0;
float axis_y_mod = 0;
float axis_z_mod = 0;

float theta_x_mod = 0;
float theta_y_mod = 0;

glm::mat4 identity_matrix = glm::mat4(1.0);
glm::mat4 matrices[3];

float x = 0.1;
float y = 2.3;
float z = 0;

float scale_x = 0.05;
float scale_y = 0.05;
float scale_z = 0.05;

float axis_x = 0;
float axis_y = 1;
float axis_z = 0;

float theta = 0;

float theta_x = 0;
float theta_y = 0;
float theta_z = 0;

float theta_z_mod = 0;

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_D && action != GLFW_RELEASE)
    {
        x_mod += 0.1f;
    }

    if (key == GLFW_KEY_A && action != GLFW_RELEASE)
    {
        x_mod -= 0.1f;
    }

    if (key == GLFW_KEY_W && action != GLFW_RELEASE)
    {
        y_mod += 0.1f;
    }

    if (key == GLFW_KEY_S && action != GLFW_RELEASE)
    {
        y_mod -= 0.1f;
    }

    if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE)
    {
        theta_y_mod -= 5;
        axis_y_mod += 1;
    }

    if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE)
    {
        theta_y_mod += 5;
        //axis_y_mod -= 1;
    }

    if (key == GLFW_KEY_UP && action != GLFW_RELEASE)
    {
        theta_x_mod -= 5;
        //axis_x_mod += 1;
    }

    if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE)
    {
        theta_x_mod += 5;
        //axis_x_mod -= 1;
    }

    if (key == GLFW_KEY_Q && action != GLFW_RELEASE)
    {
        scale_x_mod += 0.01;
        scale_y_mod += 0.01;
    }

    if (key == GLFW_KEY_E && action != GLFW_RELEASE)
    {
        scale_x_mod -= 0.01;
        scale_y_mod -= 0.01;
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float height = 600.0f;
    float width = 600.0f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Atasha Limcuando", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //Initialization of glad
    gladLoadGL();

    GLfloat UV[]{
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        0.f, 0.f
    };

    glViewport(
        0,
        0,
        width,
        height
    );

    stbi_set_flip_vertically_on_load(true);
    int img_width, img_height, color_channels;
    // 3 == RGB JPG / PNG !Transparency
    // 4 == RGBA PNG Transparency

    unsigned char* tex_bytes = stbi_load(
        "3D/partenza.jpg",
        &img_width,
        &img_height,
        &color_channels,
        0
    );

    glfwSetKeyCallback(window, Key_Callback);

    std::fstream vertSrc("Shaders/Sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/Sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);

    glLinkProgram(shaderProg);

    std::string path = "3D/djSword.obj";
    std::vector<tinyobj::shape_t> shapes; //3D objects
    std::vector<tinyobj::material_t> material; //texture
    std::string warning, error; //when you fuck up

    tinyobj::attrib_t attributes; //position, texture data, etc

    //loads the mesh
    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    std::vector<GLfloat> fullVertexData;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        // Push the X position of the vertex
        fullVertexData.push_back(
            // Multiply the index by 3 to get the base offset
            attributes.vertices[(vData.vertex_index * 3)]
        );

        // Push the Y position of the vertex
        fullVertexData.push_back(
            // Add the base offset by 1 to get Y
            attributes.vertices[(vData.vertex_index * 3) + 1]
        );

        // Push the Z position of the vertex
        fullVertexData.push_back(
            // Add the base offset by 2 to get Z
            attributes.vertices[(vData.vertex_index * 3) + 2]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3)]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 1]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 2]
        );

        // Push the U of the tex coords
        fullVertexData.push_back(
            // Multiply the index by 2 to get the base offset
            attributes.texcoords[(vData.texcoord_index * 2)]
        );

        // Push the V of the tex coords
        fullVertexData.push_back(
            // Multiply the index by 1 to get V
            attributes.texcoords[(vData.texcoord_index * 2) + 1]
        );
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // currTex = texture = tex0

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB, // 4 channels == RGBA, 3 channels == RGB
        img_width,
        img_height,
        0,  // Border
        GL_RGB,
        GL_UNSIGNED_BYTE,
        tex_bytes
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes); // clean up


    //Do VAO and VBO til clean up per object
    //ID of VAO & VBO
    GLuint VAO, VBO, EBO, VBO_UV;
    //Generates 1 VAO and VBO and assigns the ID to the variable
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    /*glGenBuffers(1, &VBO_UV);
    glGenBuffers(1, &EBO);*/

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Ad in the new array of vertex data in the VBO
    glBufferData(
        GL_ARRAY_BUFFER,
        // size of the whole array in bytes
        sizeof(GLfloat) * fullVertexData.size(),
        //Data of the array
        fullVertexData.data(),
        GL_DYNAMIC_DRAW
    );

    // Add in how to get the postion data from our array
    glVertexAttribPointer(
        0, // index 0 is vertex position
        3, // Position is 3 floats xyz
        GL_FLOAT, // data type of array
        GL_FALSE,
        8 * sizeof(float), // size of vertex data
        (void*)0
    );

    GLuint normalPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        1, // index 1 is normal position
        3, // Position is 3 floats xyz
        GL_FLOAT, // data type of array
        GL_FALSE,
        8 * sizeof(float), // size of vertex data
        (void*)normalPtr
    );


    // Since our UV starts at index 3 or the 4th index of our vertex data
    GLintptr uvPtr = 6 * sizeof(float);

    // Add in how ot get the UV data from our array
    glVertexAttribPointer(
        2, // Index 2 is tex coords / 2
        2, // UV is 2 floats
        GL_FLOAT, // Data type of array
        GL_FALSE,
        8 * sizeof(float), // size of fullVertexData
        (void*)uvPtr
    );

    //Assigning data to VBO
    //glBufferData(GL_ARRAY_BUFFER,
    //    sizeof(GLfloat) * attributes.vertices.size(), //Size of the buffer in bytes
    //    &attributes.vertices[0], //Array itself
    //    GL_STATIC_DRAW //Static Object - does not need to move, use DYNAMIC_DRAW if otherwise
    //);

    // 0 Position
    // 1 ??
    // 2 UV / Texture data

    //Describes on how to read data
    //glVertexAttribPointer(
    //    0, //Attribute index or idex of VBO, 0 is for position
    //    3, //X, Y, Z
    //    GL_FLOAT, //Type of Array
    //    GL_FALSE, //Normalized or not
    //    3 * sizeof(GLfloat), //Size of component in bytes
    //    (void*)0 //Stride value
    //);

    // size of float * length of array
    // float bytes = (sizeof(GLfloat)) * (sizeof(UV) / sizeof(UV[0]));

    //glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
    //glBufferData(
    //    GL_ARRAY_BUFFER,
    //    bytes,
    //    &UV[0],
    //    GL_STATIC_DRAW
    //);

    //glVertexAttribPointer(
    //    2, // index
    //    2, //UV
    //    GL_FLOAT,
    //    GL_FALSE,
    //    2 * sizeof(GLfloat),
    //    (void*)0
    //);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    //    sizeof(GLuint) * mesh_indices.size(), //Size of indice array
    //    mesh_indices.data(), //Array itself
    //    GL_STATIC_DRAW
    //);

    //Enables attrib index 0
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //Cleanup after editing VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //// orthographic projection
    //glm::mat4 projection = glm::ortho(
    //    -2.0f,
    //    2.0f,
    //    -2.0f,
    //    2.0f,
    //    -1.0f,
    //    1.0f
    //);

    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),
        height / width,
        0.1f,
        100.0f
    );

    glm::vec3 lightPos = glm::vec3(-10, 3, 0);
    glm::vec3 lightColor = glm::vec3(1, 1, 1);
    float ambientStr = 0.1f;
    glm::vec3 ambientColor = glm::vec3(0, 1, 0);
    float specStr = 0.5f;
    float specPhong = 16;

    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Projection Matrix
        glm::vec3 cameraPos = glm::vec3(0, 0, 10.0f);
        glm::mat4 cameraPositionMatrix = glm::translate(
            glm::mat4(1.0f),
            cameraPos * -1.0f
        );

        glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 Center = glm::vec3(0.0f, 3.0f, 0.0f);

        glm::vec3 F = glm::vec3(Center - cameraPos);
        F = glm::normalize(F);

        glm::vec3 R = glm::normalize(
            glm::cross(F, WorldUp)
        );

        glm::vec3 U = glm::normalize(
            glm::cross(R, F)
        );

        glm::mat4 cameraOrientation = glm::mat4(1.0f);

        //Matrix[Col][Row]
        cameraOrientation[0][0] = R.x;
        cameraOrientation[1][0] = R.y;
        cameraOrientation[2][0] = R.z;

        cameraOrientation[0][1] = U.x;
        cameraOrientation[1][1] = U.y;
        cameraOrientation[2][1] = U.z;

        cameraOrientation[0][2] = -F.x;
        cameraOrientation[1][2] = -F.y;
        cameraOrientation[2][2] = -F.z;

        glm::mat4 viewMatrix = cameraOrientation * cameraPositionMatrix;

        theta_y_mod += 1;

        // transformation matrix
        glm::mat4 transformation_matrix = glm::translate(
            identity_matrix,
            glm::vec3(x + x_mod, y + y_mod, y + z_mod)
        );

        transformation_matrix = glm::scale(
            transformation_matrix,
            glm::vec3(scale_x + scale_x_mod, scale_y + scale_y_mod, scale_z + scale_z_mod)
        );

        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta_y + theta_y_mod),
            glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f))
        );

        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta_x + theta_x_mod),
            glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f))
        );

        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta_z + theta_z_mod),
            glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f))
        );

        glUseProgram(shaderProg);
        glBindVertexArray(VAO);

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        glBindTexture(GL_TEXTURE_2D, texture);
        GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");
        glUniform1i(tex0Address, 0);

        // Get the addess of the light position from the shader
        GLuint lightAddress = glGetUniformLocation(shaderProg, "lightPos");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));

        // get the address of the light color from the shader
        GLuint lightColorAddress = glGetUniformLocation(shaderProg, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

        // get the address of the ambient strength from the shader
        GLuint ambientStrAddress = glGetUniformLocation(shaderProg, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);

        // Get the address of the ambient color from the shader
        GLuint ambientColorAddress = glGetUniformLocation(shaderProg, "ambientColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

        // get the address of the ambient color form the shader
        GLuint cameraPosAddress = glGetUniformLocation(shaderProg, "cameraPos");
        glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));

        //get the address of the ambinet str from the shader
        GLuint specStrAddress = glGetUniformLocation(shaderProg, "specStr");
        glUniform1f(specStrAddress, specStr);

        // get the address of the spec phong form the shader
        GLuint specPhongAddress = glGetUniformLocation(shaderProg, "specPhong");
        glUniform1f(specPhongAddress, specPhong);

        //std::cout << x_mod << " " << y_mod << " " << z_mod << std::endl;

        //glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);

        /*angle += 0.01f;

        for (int i = 0; i < 3; i++)
        {
            float localAngle = angle + (i * M_PI * 2 / 3);

            float X = radius * cos(localAngle);
            float Z = radius * sin(localAngle);

            glm::mat4 transformation_matrix = glm::translate(
                identity_matrix,
                glm::vec3(X, Z, 0)
            );

            transformation_matrix = glm::rotate(
                transformation_matrix,
                angle,
                glm::vec3(0.0f, 0.0f, 1.0f)
            );

            unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

            glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
        }*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    //Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}