#ifndef BOAT_H
#define BOAT_H

class boat {
    private:
        GLuint points_vbo, normals_vbo;

        const float points_buffer[117] = 
        {
            0, 0.5, 2,
            1, 0.5, 1,
            -1, 0.5, 1,

            1, 0.5, 1,
            1, 0.5, -2,
            -1, 0.5, 1,

            -1, 0.5, 1,
            -1, 0.5, -2,
            1, 0.5, -2,

            0, 0, 2,
            0, 0.5, 2,
            1, 0.5, 1,

            0, 0, 2,
            1, 0, 1,
            1, 0.5, 1,

            0, 0, 2,
            0, 0.5, 2,
            -1, 0.5, 1,

            0, 0, 2,
            -1, 0, 1,
            -1, 0.5, 1,

            1, 0, 1,
            1, 0.5, 1,
            1, 0.5, -2,

            1, 0, 1,
            1, 0, -2,
            1, 0.5, -2,

            -1, 0, 1,
            -1, 0.5, 1,
            -1, 0.5, -2,

            -1, 0, 1,
            -1, 0, -2,
            -1, 0.5, -2,

            1, 0, -2,
            1, 0.5, -2,
            -1, 0.5, -2,

            1, 0, -2,
            -1, 0, -2,
            -1, 0.5, -2
        };

        const float normals_buffer[117] = 
        {
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,

            0, 1, 0,
            0, 1, 0,
            0, 1, 0,

            0, 1, 0,
            0, 1, 0,
            0, 1, 0,

            1, 0, 1,
            1, 0, 1,
            1, 0, 1,

            1, 0, 1,
            1, 0, 1,
            1, 0, 1,

            -1, 0, 1,
            -1, 0, 1,
            -1, 0, 1,

            -1, 0, 1,
            -1, 0, 1,
            -1, 0, 1,

            1, 0, 0,
            1, 0, 0,
            1, 0, 0,

            1, 0, 0,
            1, 0, 0,
            1, 0, 0,

            -1, 0, 0,
            -1, 0, 0,
            -1, 0, 0,

            -1, 0, 0,
            -1, 0, 0,
            -1, 0, 0,

            0, 0, -1,
            0, 0, -1,
            0, 0, -1,

            0, 0, -1,
            0, 0, -1,
            0, 0, -1
        };

    public:
        GLuint vao;

        glm::vec3 position;
        glm::vec2 front;

        boat(glm::vec3 pos, glm::vec2 fnt);
};

boat::boat(glm::vec3 pos, glm::vec2 fnt) {

    position = pos;
    front = fnt;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &points_vbo);
    glGenBuffers(1, &normals_vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 117, points_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 117, normals_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
#endif 