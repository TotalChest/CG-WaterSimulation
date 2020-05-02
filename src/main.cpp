#include <math.h>
#include <stdio.h>
#include "common.h"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.hpp"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "objects/water_surface.h"
#include "objects/plane.h"
#include "objects/cross.h"
#include "objects/boat.h"
#include "functions.h"
#include <GLFW/glfw3.h>
#include <random>
#include "properties.h"

glm::vec3 cam_pos(0.0f, 5.0f, 5.0f); 
glm::vec3 cam_front(0.0f, -1.0f, -1.0f);
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);

// Обработка нажатий
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_R && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_Q && action == GLFW_PRESS)
        carcass = !carcass;
    if(key == GLFW_KEY_E && action == GLFW_PRESS) 
        boat_ex = !boat_ex;
    if(action == GLFW_PRESS)
		keys[key] = true;
	else if(action == GLFW_RELEASE)
	 	keys[key] = false;  
}

// Движение камеры
void do_movement()
{
    GLfloat cameraSpeed = 4.0f * deltaTime;
    if(keys[GLFW_KEY_W])
        cam_pos += cameraSpeed * cam_front;
    if(keys[GLFW_KEY_S])
        cam_pos -= cameraSpeed * cam_front;
    if(keys[GLFW_KEY_A])
        cam_pos -= glm::normalize(glm::cross(cam_front, cam_up)) * cameraSpeed;
    if(keys[GLFW_KEY_D])
        cam_pos += glm::normalize(glm::cross(cam_front, cam_up)) * cameraSpeed;
}

// Вращение камеры
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw   += xoffset;
	pitch += yoffset;
	if(pitch > 89.0f)
  		pitch =  89.0f;
	if(pitch < -89.0f)
 		pitch = -89.0f;
 	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cam_front = glm::normalize(front);
}

void raise_water(water_surface &W, float x, float z, float force = 0.3)
{
    int i = (x + h) / (2.0 * h) * W.N;
    int j = (z + h) / (2.0 * h) * W.N;

    if (i > 0 && j > 0 && i < W.N - 1 && j < W.N - 1) { 
        W.B[i-1][j-1] = force;
        W.B[i-1][j] = force;
        W.B[i-1][j+1] = force;
        W.B[i+1][j-1] = force;
        W.B[i+1][j] = force;
        W.B[i+1][j+1] = force;
        W.B[i][j+1] = force;
        W.B[i][j-1] = force;
    }
}

int initGL()
{
	int res = 0;

	// Загрузка функции OpenGL через glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: "   << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: "  << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: "     << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	return 0;
}

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;

	// Создание окна
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

  	GLFWwindow*  window = glfwCreateWindow(WIDTH, HEIGHT, "Water Simulation", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if(initGL() != 0) 
		return -1;

	// Настройки
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetKeyCallback(window, key_callback); 
    glfwSetCursorPosCallback(window, mouse_callback);
	
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	// Загрузка шейдеров
	std::unordered_map<GLenum, std::string> surface_shaders;
	surface_shaders[GL_VERTEX_SHADER]   = "surface.vert";
	surface_shaders[GL_FRAGMENT_SHADER] = "surface.frag";
	ShaderProgram surface_shader(surface_shaders);
	std::unordered_map<GLenum, std::string> floor_shaders;
	floor_shaders[GL_VERTEX_SHADER]   = "floor.vert";
	floor_shaders[GL_FRAGMENT_SHADER] = "floor.frag";
	ShaderProgram floor_shader(floor_shaders);
	std::unordered_map<GLenum, std::string> target_shaders;
	target_shaders[GL_VERTEX_SHADER]   = "target.vert";
	target_shaders[GL_FRAGMENT_SHADER] = "target.frag";
	ShaderProgram target_shader(target_shaders);
    std::unordered_map<GLenum, std::string> boat_shaders;
    boat_shaders[GL_VERTEX_SHADER]   = "boat.vert";
    boat_shaders[GL_FRAGMENT_SHADER] = "boat.frag";
    ShaderProgram boat_shader(boat_shaders);

	// Загрузка объектов
    water_surface water_surface(h);
    plane plane;
    cross cross;
    boat boat(glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, -1.0));

    // Загрузка текстур
   	GLuint floor_texture = create_texture("../textures/floor.jpg");

    while (!glfwWindowShouldClose(window))
    {
    	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Камера
		glm::mat4 view = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);
		glm::mat4 inv_view = glm::inverse(view);
		glm::mat4 projection = glm::perspective(PI/4, (float)WIDTH / HEIGHT, 0.1f, 100.0f);
		glm::mat4 inv_projection = glm::inverse(projection);

		// Отррисовка центра
		{
			target_shader.StartUseShader();

        	glm::mat4 model(1);
        	model = glm::scale(model, glm::vec3(3.0/WIDTH, 3.0/HEIGHT, 1));
        	target_shader.SetUniformMatrix("model_mat", glm::value_ptr(model));

			glBindVertexArray(cross.vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);

			target_shader.StopUseShader();
		}

        // Отрисовка бассейна
        {
            floor_shader.StartUseShader();

            // Управление камерой
        	floor_shader.SetUniformMatrix("view_mat", glm::value_ptr(view));
        	floor_shader.SetUniformMatrix("proj_mat", glm::value_ptr(projection));

         	// Пол
            glm::mat4 model(1);
            model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
            model = glm::scale(model, glm::vec3(h, 1, h));
			model = glm::rotate(model, -PI/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

			floor_shader.SetUniformMatrix("model_mat", glm::value_ptr(model));
			floor_shader.SetUniformVec4("text_coord", 0.0f, 0.0f, 1.0f, 1.0f);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, floor_texture);

            glBindVertexArray(plane.vao);
            glDrawArrays(GL_TRIANGLES, 0, 18);
            glBindVertexArray(0);

            // Задняя стена
            model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -h));
			model = glm::scale(model, glm::vec3(h, 2.0, 1));

            floor_shader.SetUniformMatrix("model_mat", glm::value_ptr(model));
            floor_shader.SetUniformVec4("text_coord", 0.0f, 0.0f, 1.0f, 0.5f);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, floor_texture);

            glBindVertexArray(plane.vao);
            glDrawArrays(GL_TRIANGLES, 0, 18);
            glBindVertexArray(0);

            // Левая стена
            model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-h, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1, 2.0, h));
			model = glm::rotate(model, PI/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            floor_shader.SetUniformMatrix("model_mat", glm::value_ptr(model));
            floor_shader.SetUniformVec4("text_coord", 0.0f, 0.0f, 1.0f, 0.5f);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, floor_texture);

            glBindVertexArray(plane.vao);
            glDrawArrays(GL_TRIANGLES, 0, 18);
            glBindVertexArray(0);

            // Правая стена
            model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(h, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1, 2.0, h));
			model = glm::rotate(model, -PI/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            floor_shader.SetUniformMatrix("model_mat", glm::value_ptr(model));
            floor_shader.SetUniformVec4("text_coord", 0.0f, 0.0f, 1.0f, 0.5f);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, floor_texture);

            glBindVertexArray(plane.vao);
            glDrawArrays(GL_TRIANGLES, 0, 18);
            glBindVertexArray(0);

            // Передняя стена
            model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, h));
			model = glm::scale(model, glm::vec3(h, 2.0, 1));
			model = glm::rotate(model, PI, glm::vec3(0.0f, 1.0f, 0.0f));
            floor_shader.SetUniformMatrix("model_mat", glm::value_ptr(model));
            floor_shader.SetUniformVec4("text_coord", 0.0f, 0.0f, 1.0f, 0.5f);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, floor_texture);

            glBindVertexArray(plane.vao);
            glDrawArrays(GL_TRIANGLES, 0, 18);
            glBindVertexArray(0);

            floor_shader.StopUseShader();
        }


        // Отрисовка поверхности
        {
        	// Обновление карты высот
        	water_surface.update();

            surface_shader.StartUseShader(); 

            // Управление камерой
        	surface_shader.SetUniformMatrix("view_mat", glm::value_ptr(view));
        	surface_shader.SetUniformMatrix("proj_mat", glm::value_ptr(projection));

        	glm::mat4 model(1);
        	surface_shader.SetUniformMatrix("model_mat", glm::value_ptr(model));
        	surface_shader.SetUniform("h", h);

        	surface_shader.SetUniformVec4("cam_pos",  cam_pos[0],  cam_pos[1],  cam_pos[2], 1.0f);
        	glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, floor_texture);

            if(carcass)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            glBindVertexArray(water_surface.vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, water_surface.elements_vbo);
            glDrawElements(GL_TRIANGLES, (water_surface.N - 1) * (water_surface.N - 1) * 2 * 3, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            if(carcass)
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

         	surface_shader.StopUseShader();
        }


        // Обработка лодки
        if(boat_ex) {
            boat_shader.StartUseShader();

            if(keys[GLFW_KEY_UP]) {
                boat.position += 1.0f * deltaTime * glm::vec3(boat.front[0], 0, boat.front[1]);
                if(boat.position[0] >= h-0.7)
                    boat.position[0] = h-0.7;
                if(boat.position[2] >= h-0.7)
                    boat.position[2] = h-0.7;
                if(boat.position[0] <= -h+0.7)
                    boat.position[0] = -h+0.7;
                if(boat.position[2] <= -h+0.7)
                    boat.position[2] = -h+0.7;
                raise_water(water_surface, boat.position[0] + 0.5*boat.front[0], boat.position[2] + 0.5*boat.front[1], 0.2);
            }
            if(keys[GLFW_KEY_LEFT]) {
                boat.front = glm::rotate(boat.front, -PI/120.0f);
                raise_water(water_surface, boat.position[0], boat.position[2], 0.2);
            }
            if(keys[GLFW_KEY_RIGHT]) {
                boat.front = glm::rotate(boat.front, PI/120.0f);
                raise_water(water_surface, boat.position[0], boat.position[2], 0.2);
            }

            // Управление камерой
            boat_shader.SetUniformMatrix("view_mat", glm::value_ptr(view));
            boat_shader.SetUniformMatrix("proj_mat", glm::value_ptr(projection));

            glm::mat4 model(1);
            model = glm::translate(model, boat.position + glm::vec3(0.0, -0.1, 0.0));
            model = glm::scale(model, glm::vec3(0.35, 1, 0.35));
            model = glm::rotate(model, glm::atan(boat.front[0], boat.front[1]), glm::vec3(0.0f, 1.0f, 0.0f));
            boat_shader.SetUniformMatrix("model_mat", glm::value_ptr(model));

            glBindVertexArray(boat.vao);
            glDrawArrays(GL_TRIANGLES, 0, 117);
            glBindVertexArray(0);

            boat_shader.StopUseShader();
        }


        // Обработка нажатия
 		{
            static bool is_mouse_down = false;

            glm::vec3 mouse_world = glm::normalize(cam_front);
            glm::vec3 mouse_intersection = cam_pos + (-cam_pos.y / mouse_world.y) * mouse_world;

            int mouse_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
            if (mouse_state == GLFW_PRESS && !is_mouse_down) {
                is_mouse_down = true;

                if (mouse_intersection.x > -h &&
                        mouse_intersection.x < h &&
                        mouse_intersection.z > -h &&
                        mouse_intersection.z < h) {
                    raise_water(water_surface, mouse_intersection.x, mouse_intersection.z, 0.4);
                }
            }
            else if (mouse_state == GLFW_RELEASE && is_mouse_down) {
                is_mouse_down = false;
            }
        }

        glfwPollEvents();
        do_movement();
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwSwapBuffers(window); 

	}

	glfwDestroyWindow(window);
    glfwTerminate();
	return 0;
}