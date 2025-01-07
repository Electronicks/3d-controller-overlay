#if defined(__linux__) 
#elif __FreeBSD__
#elif __ANDROID__
#elif __APPLE__
#elif _WIN32
#define SDL_MAIN_HANDLED
#include <windows.h>
#else //some other operating system
#endif

#include <cstdio>
#include <SDL2/SDL.h>
#include <GLFW/glfw3.h>

import controller_window;
import settings_window;

#ifdef _WIN32
int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
#else
int main(int argc, char* argv[])
#endif
{
	// Initialize Program
#if defined(WIN32) && defined(DEBUG)
	AllocConsole();
	SetConsoleTitle(L"3D Controller Overlay");
	// https://stackoverflow.com/a/15547699/1130520
	freopen_s((FILE**)stdin, "conin$", "r", stdin);
	freopen_s((FILE**)stdout, "conout$", "w", stdout);
	freopen_s((FILE**)stderr, "conout$", "w", stderr);
	//SetConsoleCtrlHandler(&ConsoleCtrlHandler, TRUE);
#endif
	if(SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0){
        printf("Error: %s\n", SDL_GetError());
        exit(1);
    }
	createSettingsWindow();
	loadTabs();
	
	// Main Loop
	bool gQuit = false;
	while (!gQuit)
	{
		// Input
		glfwPollEvents();

		settings_window_input(gQuit);
		controller_window_input();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			settings_sdl_events(&event);
			controller_sdl_events(&event);
		}

		// Draw
		drawSettingsWindow();
		drawControllerWindows();
	}

	// Cleanup
	saveTabs();
	removeSettingsWindow();
	destroyWindows();
	SDL_Quit();
	glfwTerminate();

	return 0;
}
