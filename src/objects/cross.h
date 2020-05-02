#ifndef CROSS_H
#define CROSS_H

class cross {
    private:
        const float points_buffer[36] = 
        {
            -1, 10, 0,
            1, 10, 0,
            -1, -10, 0,

            1, 10, 0,
            1, -10, 0,
            -1, -10, 0,

            -10, 1, 0,
            10, 1, 0,
            -10, -1, 0,

            10, 1, 0,
            10, -1, 0,
            -10, -1, 0
        };

    public:
        GLuint vao;

        cross();
};

cross::cross() {
    GLuint points_vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &points_vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36, points_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

#endif 
