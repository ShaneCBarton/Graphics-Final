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
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Error initializing SDL..." << std::endl;
	}

	m_window = SDL_CreateWindow("OpenGL Final", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

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

	std::string vertexShaderSource = LoadShaderSource("shaders\\default.vert");
	std::string fragmentShaderSource = LoadShaderSource("shaders\\default.frag");

	std::cout << "Vertex Shader Source:\n" << vertexShaderSource << std::endl;
	std::cout << "Fragment Shader Source:\n" << fragmentShaderSource << std::endl;

	GLuint shaderProgramID = glCreateProgram();

	GLuint vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	m_shaderProgramID = LinkShaders(vertexShaderID, fragmentShaderID);

	glUseProgram(shaderProgramID);
	GLuint iChannel10Location = glGetUniformLocation(shaderProgramID, "iChannel10");
	GLuint iResolutionLocation = glGetUniformLocation(shaderProgramID, "iResolution");
	GLuint iTimeLocation = glGetUniformLocation(shaderProgramID, "iTime");

	int textureUnit = 0;
	float resolution[2] = { 800, 600 };
	float currentTime = GetCurrentTime();

	glUniform1i(iChannel10Location, textureUnit);
	glUniform2fv(iResolutionLocation, 1, resolution);
	glUniform1f(iTimeLocation, currentTime);

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

	CheckGLErrors("After glCompileShader");

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

	CheckGLErrors("After glLinkProgram");

	GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(programID, sizeof(infoLog), NULL, infoLog);
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

void Application::CheckGLErrors(const char* location)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error at " << location << ": " << error << std::endl;
	}
}

float Application::GetCurrentTime()
{
	using namespace std::chrono;
	return duration_cast<duration<float>>(high_resolution_clock::now().time_since_epoch()).count();
}