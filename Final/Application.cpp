#include <iostream>
#include <fstream>
#include <sstream>
#include <glew.h>
#include <gl/GL.h>

#include "Application.h"

Application::Application()
{
	m_isRunning = false;
}

Application::~Application()
{

}

void Application::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Error initializing SDL..." << std::endl;
	}

	m_window = SDL_CreateWindow("OpenGL Final", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

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

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	std::string vertexShaderSource = LoadShaderSource("vertexShader.glsl");
	std::string fragmentShaderSource = LoadShaderSource("fragmentShader.glsl");

	GLuint vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	m_shaderProgramID = LinkShaders(vertexShaderID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	m_isRunning = true;
}

void Application::Run()
{
	while (m_isRunning)
	{
		ProcessInput();

		glUseProgram(m_shaderProgramID);

		glUseProgram(0);
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
	std::stringstream buffer;
	buffer << file.rdbuf();
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
		glGetShaderInfoLog(shaderID, sizeof(infoLog), NULL, infoLog);
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
		glGetShaderInfoLog(programID, sizeof(infoLog), NULL, infoLog);
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