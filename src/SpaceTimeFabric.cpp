#include "../include/SpaceTimeFabric.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Fabric::Fabric(float gridStep, int gridSquares, glm::vec3 position, std::vector<float> color, float y_value, GLuint shader) {

    this->gridStep = gridStep;
    this->position = position;

    if (gridSquares % 2 == 0) {
        this->gridSquares = gridSquares;
    }
    else {
        this->gridSquares = gridSquares + 1;
    }

    this->y_value = y_value;
    
    this->color = color;
    this->shader = shader;

    this->compute_vertices();
    this->create_fabric();

    return;
}

void Fabric::compute_vertices() {
    // Emptzing vertices vector before calculation
    std::vector<float>().swap(this->vertices);

    float x, z_1, z_2;
    float x_1, x_2, z;

    // Forming vertices for top-down lines of grid
    for (int idx = -this->gridSquares; idx <= this->gridSquares; idx++) {
        for (int idz = -this->gridSquares; idz < this->gridSquares; idz++) {

            // Calculating first coordinate for a line segment
            x = idx * this->gridStep;
            z_1 = idz * this->gridStep;

            // Calculating second coordinate for a line segment
            z_2 = (idz + 1) * this->gridStep;

            this->vertices.push_back(x);
            this->vertices.push_back(this->y_value);
            this->vertices.push_back(z_1);
            this->vertices.push_back(x);
            this->vertices.push_back(this->y_value);
            this->vertices.push_back(z_2);
        }
    }

    // Forming vertices for left-right lines of grid
    for (int idz = -this->gridSquares; idz <= this->gridSquares; idz++) {
        for (int idx = -this->gridSquares; idx < this->gridSquares; idx++) {

            // Calculating first coordinate for a line segment
            x_1 = idx * this->gridStep;
            z = idz * this->gridStep;

            // Calculating second coordinate for a line segment
            x_2 = (idx + 1) * this->gridStep;

            this->vertices.push_back(x_1);
            this->vertices.push_back(this->y_value);
            this->vertices.push_back(z);
            this->vertices.push_back(x_2);
            this->vertices.push_back(this->y_value);
            this->vertices.push_back(z);
        }
    }

    return;
}

void Fabric::draw_fabric() {
    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::translate(Model, this->position);
    glUniformMatrix4fv(glGetUniformLocation(this->shader, "Model"), 1, GL_FALSE, glm::value_ptr(Model));

    glUniform4f(glGetUniformLocation(this->shader, "currentColor"), this->color[0], this->color[1], this->color[2], this->color[3]);
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_LINES, 0, vertices.size() / 3);

    return;
}

void Fabric::create_fabric() {
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return;
}