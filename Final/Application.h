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
	void RenderFrame();
	std::string LoadShaderSource(const char* filePath);
	GLuint CompileShader(GLenum shaderType, const std::string& source);
	GLuint LinkShaders(GLuint vertexShaderID, GLuint fragmentShaderID);
	void CheckGLErrors(const char* location);
	float GetCurrentTime();

private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	GLuint m_shaderProgramID;
	bool m_isRunning;
};

