#include <SDL_main.h>
#include "Engine.h"

int main(int argc, char* argv[]) // char** argv
{
	Engine engine; // Allocates memory for object and all instance properties but NOT static properties.
	return engine.Run();
}
