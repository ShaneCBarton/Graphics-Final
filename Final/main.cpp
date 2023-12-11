#include <iostream>
#include "Application.h"

int main(int argc, char *argv[])
{

	Application application;

	application.Initialize();
	application.Run();
	application.Destroy();

	return 0;
}