#pragma once

#include <SDL.h>

class Application
{

public:
	Application();
	~Application();
	void Initialize();
	void Destroy();
	void Run();
	void ProcessInput();

private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool m_isRunning;
};

