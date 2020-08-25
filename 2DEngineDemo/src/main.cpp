#include <DxLib.h>

#include "Engine.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	auto& engine = Engine::Instance();
	engine.Start();
	engine.Run();
	engine.Stop();

	return 0;
}