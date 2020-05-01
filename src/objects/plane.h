#ifndef PLANE_H
#define PLANE_H

class plane {
    private:
        const float points_buffer[18] = 
        {
            -1, 1, 0,
            1, 1, 0,
            -1, -1, 0,

            1, 1, 0,
            1, -1, 0,
            -1, -1, 0
        };

        const float normals_buffer[18] = 
        {
            0, 0, 1, 
            0, 0, 1, 
            0, 0, 1, 

            0, 0, 1, 
            0, 0, 1, 
            0, 0, 1
        };

    public:
        GLuint vao;

        plane();
};

plane::plane() {
    GLuint points_vbo, normals_vbo, texture_coords_vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &normals_vbo);
    glGenBuffers(1, &points_vbo);
    glGenBuffers(1, &texture_coords_vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, points_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, normals_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

#endif 
