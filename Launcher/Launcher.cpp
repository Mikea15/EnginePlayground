
#include "Launcher.h"
#include "Game.h"

int main(int argc, char* argv[])
{
	StubState stubState;
	Game game(&stubState);
	return game.Execute();
}