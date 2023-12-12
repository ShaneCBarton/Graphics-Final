#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <glew.h>
#include <gl/GL.h>
#include <chrono>

#include "Application.h"

Application::Application() : m_window(nullptr),
	m_glContext(nullptr), m_shaderProgramID(0), m_isRunning(false)
{
}

Application::~Application()
{

}

void Application::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Error initializing SDL..." << std::endl;
	}

	m_window = SDL_CreateWindow("OpenGL Final", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1);

	m_glContext = SDL_GL_CreateContext(m_window);
	if (!m_glContext)
	{
		std::cout << "Error creating OpenGL context..." << std::endl;
	}

	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		std::cout << "Error initializing GLEW..." << std::endl;
	}

	glViewport(0, 0, 800, 600);

	// Loading and compiling shaders with cout for debugging.
	std::string vertexShaderSource = LoadShaderSource("shaders/default.vert");
	std::string fragmentShaderSource = LoadShaderSource("shaders/default.frag");

	std::cout << "Vertex Shader Source:\n" << vertexShaderSource << std::endl;
	std::cout << "Fragment Shader Source:\n" << fragmentShaderSource << std::endl;

	GLuint vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	m_shaderProgramID = glCreateProgram();
	m_shaderProgramID = LinkShaders(vertexShaderID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(m_shaderProgramID);

	// Generate vertex buffer and array
	glGenBuffers(1, &m_vertexBuffer);

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		3.0f, -1.0f, 0.0f,
		-1.0f,  3.0f, 0.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &m_vertexArray);
	glBindVertexArray(m_vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_isRunning = true;
}

void Application::Run()
{
	while (m_isRunning)
	{
		ProcessInput(); 
		RenderFrame();
		SDL_GL_SwapWindow(m_window);
	}
}

void Application::ProcessInput()
{
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			m_isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
			{
				m_isRunning = false;
			}
			break;
		}
	}
}

std::string Application::LoadShaderSource(const char* filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Error opening shader file: " << filePath << std::endl;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	return buffer.str();
}

GLuint Application::CompileShader(GLenum shaderType, const std::string& source)
{
	GLuint shaderID = glCreateShader(shaderType);
	const char* sourceCStr = source.c_str();
	glShaderSource(shaderID, 1, &sourceCStr, NULL);
	glCompileShader(shaderID);

	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "Shader compilation failed: \n" << infoLog << std::endl;
	}

	return shaderID;
}

GLuint Application::LinkShaders(GLuint vertexShaderID, GLuint fragmentShaderID)
{
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "Shader program linking failed: \n" << infoLog << std::endl;
	}

	return programID;
}

void Application::Destroy()
{
	glDeleteProgram(m_shaderProgramID);
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

float Application::GetCurrentTime()
{
	using namespace std::chrono;
	return duration_cast<duration<float>>(high_resolution_clock::now().time_since_epoch()).count();
}

void Application::RenderFrame()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_shaderProgramID);

	glBindVertexArray(m_vertexArray);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	SDL_GL_SwapWindow(m_window);
}

