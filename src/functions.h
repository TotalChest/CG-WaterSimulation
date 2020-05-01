#ifndef FUNCTIONS_H
#define FUNCTIONS_H

GLuint create_texture(const char *file_name) {
    stbi_set_flip_vertically_on_load(1);
    int x, y, n;
    int force_channels = 3;
    unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);

    if (!image_data) {
        printf("Could not load texture file %s\n", file_name);
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image_data);

    return texture;
}

#endif 