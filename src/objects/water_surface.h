#ifndef WATER_SURFACE_H
#define WATER_SURFACE_H

class water_surface {
    private:
        GLuint points_vbo, normals_vbo;

    public:
        static const int N = 150;

        float A[N][N];
        float B[N][N];
        float C[N][N];

        GLuint vao, elements_vbo;

        glm::vec3 points[N * N];
        glm::vec3 normals[N * N];
        glm::vec3 elements[(N - 1) * (N - 1) * 2];

        float points_buffer[N * N * 3];
        float normals_buffer[N * N * 3];
        int elements_buffer[(N - 1) * (N - 1) * 2 * 3];

        water_surface();

        void update();
};


water_surface::water_surface() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &elements_vbo);
    glGenBuffers(1, &normals_vbo);
    glGenBuffers(1, &points_vbo);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = 0.0;
            B[i][j] = 0.0;
            C[i][j] = 0.0;
        }
    }
}

void water_surface::update() {
    // Вычисление новых высот
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            float v1, v2, v3, v4;

            if (i == 0)
                v1 = B[i][j];
            else
                v1 = B[i - 1][j];

            if (i == N - 1)
                v2 = B[i][j];
            else
                v2 = B[i + 1][j];

            if (j == 0) 
                v3 = B[i][j];
            else
                v3 = B[i][j - 1];

            if (j == N - 1)
                v4 = B[i][j];
            else
                v4 = B[i][j + 1];

            C[i][j] = (1-1.985)*A[i][j] + 1.985*(v1 + v2 + v3 + v4)/4.0; 
        }
    }

    // Обновление матриц
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = B[i][j];
            B[i][j] = C[i][j];
        }
    }

    // Вычисление координат
    int point_index = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            float x = -3 + 6 * (i/(float)(N-1));
            float y = -3 + 6 * (j/(float)(N-1));    
            points[point_index++] = glm::vec3(x, C[i][j], y);
        }
    }

    // Вычисление нормалей
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            int p1_i = i + N * j;
            int p2_i = i + 1 + N * j;
            int p3_i = i - 1 + N * j;
            int p4_i = i + N * (j - 1);
            int p5_i = i + N * (j + 1);

            glm::vec3 normal;

            if (i == 0 && j == 0) {
                glm::vec3 p1 = points[p1_i];
                glm::vec3 p2 = points[p2_i];
                glm::vec3 p5 = points[p5_i];

                normal = glm::cross(p5 - p1, p2 - p1);
            } else if (i == 0 && j == N - 1) {
                glm::vec3 p1 = points[p1_i];
                glm::vec3 p2 = points[p2_i];
                glm::vec3 p4 = points[p4_i];

                normal = glm::cross(p2 - p1, p4 - p1);
            } else if (i == N - 1 && j == 0) {
                glm::vec3 p1 = points[p1_i];
                glm::vec3 p3 = points[p3_i];
                glm::vec3 p5 = points[p5_i];

                normal = glm::cross(p3 - p1, p5 - p1);
            } else if (i == N - 1 && j == N - 1) {
                glm::vec3 p1 = points[p1_i];
                glm::vec3 p3 = points[p3_i];
                glm::vec3 p4 = points[p4_i];

                normal = glm::cross(p4 - p1, p3 - p1);
            } else if (i == 0) {
                glm::vec3 p1 = points[p1_i];
                glm::vec3 p2 = points[p2_i];
                glm::vec3 p4 = points[p4_i];
                glm::vec3 p5 = points[p5_i];

                glm::vec3 n1 = glm::normalize(glm::cross(p2 - p1, p4 - p1));
                glm::vec3 n2 = glm::normalize(glm::cross(p5 - p1, p2 - p1));

                normal = (n1 + n2) * 0.5f;
            } else if (j == 0) {
                glm::vec3 p1 = points[p1_i];
                glm::vec3 p2 = points[p2_i];
                glm::vec3 p3 = points[p3_i];
                glm::vec3 p5 = points[p5_i];

                glm::vec3 n1 = glm::normalize(glm::cross(p3 - p1, p5 - p1));
                glm::vec3 n2 = glm::normalize(glm::cross(p5 - p1, p2 - p1));

                normal = (n1 + n2) * 0.5f;
            } else if (i == N - 1) {
                glm::vec3 p1 = points[p1_i];
                glm::vec3 p3 = points[p3_i];
                glm::vec3 p4 = points[p4_i];
                glm::vec3 p5 = points[p5_i];

                glm::vec3 n1 = glm::normalize(glm::cross(p4 - p1, p3 - p1));
                glm::vec3 n2 = glm::normalize(glm::cross(p3 - p1, p5 - p1));

                normal = (n1 + n2) * 0.5f;
            } else if (j == N - 1) {
                glm::vec3 p1 = points[p1_i];
                glm::vec3 p2 = points[p2_i];
                glm::vec3 p3 = points[p3_i];
                glm::vec3 p4 = points[p4_i];

                glm::vec3 n1 = glm::normalize(glm::cross(p2 - p1, p4 - p1));
                glm::vec3 n2 = glm::normalize(glm::cross(p4 - p1, p3 - p1));

                normal = (n1 + n2) * 0.5f;
            } else {
                glm::vec3 p1 = points[p1_i];
                glm::vec3 p2 = points[p2_i];
                glm::vec3 p3 = points[p3_i];
                glm::vec3 p4 = points[p4_i];
                glm::vec3 p5 = points[p5_i];

                glm::vec3 n1 = glm::normalize(glm::cross(p4 - p1, p3 - p1));
                glm::vec3 n2 = glm::normalize(glm::cross(p2 - p1, p4 - p1));
                glm::vec3 n3 = glm::normalize(glm::cross(p5 - p1, p2 - p1));
                glm::vec3 n4 = glm::normalize(glm::cross(p3 - p1, p5 - p1));

                normal = (n1 + n2 + n3 + n4) * 0.25f; 
            }

            normal = glm::normalize(normal);
            this->normals[p1_i] = normal; 
        }
    }

    // Вычисление индексов для треугольников
    int e_i = 0;
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - 1; j++) {

            int p1 = i + N * j;
            int p2 = i + 1 + N * j;
            int p3 = i + N * (j + 1);

            elements[e_i++] = glm::vec3(p1, p2, p3);

            int p4 = i + 1 + N * j;
            int p5 = i + 1 + N * (j + 1);
            int p6 = i + N * (j + 1);

            elements[e_i++] = glm::vec3(p4, p5, p6);
        }
    }

    glBindVertexArray(vao);

    // Загрузка точек
    for (int i = 0; i < N * N; i++) {
        points_buffer[i*3] = points[i].x;
        points_buffer[i*3 + 1] = points[i].y;
        points_buffer[i*3 + 2] = points[i].z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * N * N * 3, points_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // Загрузка нормалей
    for (int i = 0; i < N * N; i++) {
        normals_buffer[i*3] = normals[i].x;
        normals_buffer[i*3 + 1] = normals[i].y;
        normals_buffer[i*3 + 2] = normals[i].z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * N * N * 3, normals_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);


    // Загрузка индексов
    for (int i = 0; i < (N - 1) * (N - 1) * 2; i++) {
        elements_buffer[i*3] = elements[i].x;
        elements_buffer[i*3 + 1] = elements[i].y;
        elements_buffer[i*3 + 2] = elements[i].z;
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * (N - 1) * (N - 1) * 2 * 3, elements_buffer, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

#endif
