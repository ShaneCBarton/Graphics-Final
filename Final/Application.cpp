#include <iostream>
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

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	m_isRunning = true;
}

void Application::Run()
{
	while (m_isRunning)
	{
		ProcessInput();
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

void Application::Destroy()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}