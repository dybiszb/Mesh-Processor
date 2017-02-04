// author: dybisz

#include "rendering/gl_box.h"

glBox::glBox() {
    // Init Buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Init Vertices/Indices
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0); // Unbind VAO

}

glBox::~glBox() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void glBox::render(glShaderProgram &shader, Matrix4f& view, Matrix4f &
projection) {
    shader.use();

    glBindVertexArray(vao);

    glUniformMatrix4fv(glGetUniformLocation(shader.getId(),"model"), 1,
                       GL_FALSE, model.data());
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(),"view"), 1,
                      GL_FALSE, view.data());
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(),"projection"), 1,
                       GL_FALSE, projection.data());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader.unuse();
}