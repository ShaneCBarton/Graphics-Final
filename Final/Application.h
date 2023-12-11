#pragma once

#include <SDL.h>
#include <SDL_opengl.h>

class Application
{

public:
	Application();
	~Application();
	void Initialize();
	void Destroy();
	void Run();
	void ProcessInput();
	std::string LoadShaderSource(const char* filePath);
	GLuint CompileShader(GLenum shaderType, const std::string& source);
	GLuint LinkShaders(GLuint vertexShaderID, GLuint fragmentShaderID);

private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool m_isRunning;
};

