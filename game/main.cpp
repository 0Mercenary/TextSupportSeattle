#include "stdafx.h"
#include "MyGame.h"

CMyGame game;
CGameApp app;

int main(int argc, char* argv[])
{
	//app.DebugMemoryLeaks();
	app.OpenWindow(800, 600, "Tutorial 1");
	//app.OpenFullScreen(800, 600, 24);
	//wapp.OpenConsole();
	app.SetClearColor(CColor::DarkGray());
	app.Run(&game);
	return(0);
}
