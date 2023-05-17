#include "stdafx.h"
#include "MyGame.h"

CMyGame::CMyGame(void) :
	player(400, 300, "player.bmp", CColor::White(), GetTime()),
	enemyOG(400, 300, "enemy.bmp", CColor::White(), GetTime()),
	goal(400, 300, "goal.bmp", CColor::White(), GetTime()),
	wallOG(500, 300, "wall.bmp", CColor::White(), GetTime()),
	shotOG(0, 0, "shot.bmp", CColor::White(), GetTime()),
	floorOG(263, 263, "floor.png", CColor::White(), GetTime())
{

}

CMyGame::~CMyGame(void)
{
	
}

void CMyGame::OnUpdate()
{
	Uint32 t = GetTime();

	player.Update(t);

	PlayerControl(t);

	GunControl(t);

	EnemyControl(t);

	for (CSprite* enemy : enemyList) {
		enemy->Update(t);
	}

	for (CSprite* floor : floorList) {
		floor->Update(t);
	}

	for (CSprite* wall : wallList) {
		wall->Update(t);
	}

	goal.Update(t);

}

void CMyGame::OnDraw(CGraphics* g)
{

	CameraControl(g);

	for (CSprite* floor : floorList) {
		floor->Draw(g);
	}

	goal.Draw(g);

	for (CSprite* shot : shotList) {
		shot->Draw(g);
	}

	player.Draw(g);

	for (CSprite* enemy : enemyList) {
		enemy->Draw(g);
	}

	for (CSprite* wall : wallList) {
		wall->Draw(g);
	}

}

void CMyGame::PlayerControl(Uint32 t) {
	CVector position(CVector(400, 300) - CVector (mx, my));
	CVector playerPosition(player.GetPos());
	CVector dir = CVector(0, 0);

	//cout << player.GetPos().m_x << " " << player.GetPos().m_y << "::" << position.m_x << " " << position.m_y << endl;

	player.SetRotation(position.m_x, position.m_y);

	if (IsKeyDown(SDLK_w)) dir.m_y += 1;
	if (IsKeyDown(SDLK_s)) dir.m_y -= 1;
	if (IsKeyDown(SDLK_a)) dir.m_x -= 1;
	if (IsKeyDown(SDLK_d)) dir.m_x += 1;

	player.SetDirection(dir);

	if ((IsKeyDown(SDLK_w) || IsKeyDown(SDLK_a) || IsKeyDown(SDLK_s) || IsKeyDown(SDLK_d))) {
		if (IsKeyDown(SDLK_LSHIFT)) player.SetSpeed(300);
		else player.SetSpeed(200);
	}
	else player.SetSpeed(0);

	for each (CSprite * wall in wallList) {

		float Pi = 3.141;
		float R = player.GetWidth() / 2;
		float X = wall->GetWidth() / 2;
		float Y = wall->GetHeight() / 2;
		CVector V = player.GetVelocity() * GetDeltaTime() / 1000;
		CVector T = wall->GetPos() - player.GetPos();

		// Up

		CVector N = CVector(sin(wall->GetRotation() * Pi / 180), cos(wall->GetRotation() * Pi / 180));
		if (Dot(V, N) < 0) {

			// Perpendicular component (oncoming)
			float vy = Dot(V, N); // velocity component
			CVector d = T + (Y + R) * N; // distance vector between edges
			float dy = Dot(d, N); // perpendicular space between
			float f1 = dy / vy;

			// Parallel component (breadth control)
			float vx = Cross(V, N); // velocity component
			float tx = Cross(T, N); // distance between centres
			float f2 = (tx - vx * f1) / (X + R);
			if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1) player.SetVelocity(Reflect(player.GetVelocity() * damp, N));
		}

		// Down

		if (Dot(V, N) > 0) {

			// Perpendicular component (oncoming)
			float vy = Dot(V, N); // velocity component
			CVector d = T - (Y + R) * N; // distance vector between edges
			float dy = Dot(d, N); // perpendicular space between
			float f1 = dy / vy;

			// Parallel component (breadth control)
			float vx = Cross(V, N); // velocity component
			float tx = Cross(T, N); // distance between centres
			float f2 = (tx - vx * f1) / (X + R);
			if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1) player.SetVelocity(Reflect(player.GetVelocity() * damp, N));
		}

		// Left

		N = CVector(-cos(wall->GetRotation() * Pi / 180), sin(wall->GetRotation() * Pi / 180));
		if (Dot(V, N) < 0) {

			// Perpendicular component (oncoming)
			float vy = Dot(V, N); // velocity component
			CVector d = T + (X + R) * N; // distance vector between edges
			float dy = Dot(d, N); // perpendicular space between
			float f1 = dy / vy;

			// Parallel component (breadth control)
			float vx = Cross(V, N); // velocity component
			float tx = Cross(T, N); // distance between centres
			float f2 = (tx - vx * f1) / (Y + R);
			if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1) player.SetVelocity(Reflect(player.GetVelocity() * damp, N));
		}

		// Right

		N = CVector(cos(wall->GetRotation() * Pi / 180), -sin(wall->GetRotation() * Pi / 180));
		if (Dot(V, N) < 0) {

			// Perpendicular component (oncoming)
			float vy = Dot(V, N); // velocity component
			CVector d = T + (X + R) * N; // distance vector between edges
			float dy = Dot(d, N); // perpendicular space between
			float f1 = dy / vy;

			// Parallel component (breadth control)
			float vx = Cross(V, N); // velocity component
			float tx = Cross(T, N); // distance between centres
			float f2 = (tx - vx * f1) / (Y + R);
			if (f1 >= 0 && f1 <= 1 && f2 >= -1 && f2 <= 1) player.SetVelocity(Reflect(player.GetVelocity() * damp, N));
		}
	}
}

void CMyGame::CameraControl(CGraphics* g) {
	CVector scroll = CVector (400, 300) - player.GetPos();

	g->SetScrollPos(scroll);
}

void CMyGame::GunControl(Uint32 t) {
	if (fireTimer > 0) {
		fireTimer--;
	}

	if (fireTimer <= 0 && lDown) {
		fireTimer = 5;
		CSprite* shot = new CSprite(0, 0, "shot.bmp", CColor::White(), GetTime());
		shot->SetPosition(player.GetPosition());
		shot->SetDirection(player.GetRotation() + 180);
		shot->SetRotation(player.GetRotation());
		shot->SetSpeed(500);
		shot->Update(t + 90);
		//cout << shot->GetPos().m_x << " " << shot->GetPos().m_y << endl;
		shotList.push_back(shot);
	}

	for (CSprite* shot : shotList) {
		shot->Update(t);
	}

	for (CSprite* shot : shotList) {
		for (CSprite* wall : wallList) {
			if (shot->HitTest(wall)) {
				shot->Delete();
			}
		}

		if (shot->GetY() > 2000 || shot->GetY() < -2000 || shot->GetX() > 2000 || shot->GetX() < -2000) shot->Delete();
	}

	shotList.delete_if(deleted);
}

void CMyGame::EnemyControl(Uint32 t) {

}

void CMyGame::OnInitialize()
{	
	
}

void CMyGame::OnDisplayMenu()
{   
	StartGame();
}

void CMyGame::OnStartGame()
{
	auto floor = floorOG.Clone();
	floorList.push_back(floor);

	LevelReading("level1.txt");
}

void CMyGame::OnStartLevel(Sint16 nLevel)
{	
}

void CMyGame::LevelReading(string filename) {

	fstream myfile;
	myfile.open(filename, ios_base::in);
	int type, x, y, z, rot;
	string text;
	bool neof = true;

	// Made to be compatible with a level editor for GTEC, making dev easier - This means a Z needs to be used instead of y, and y is unused as a 3rd coord.
	do
	{
		myfile >> type >> x >> y >> z >> rot;

		cout << type << endl;

		x -= 400;
		z -= 400;

		x *= 3;
		y *= 3;
		z *= 3;

		z = z / 7;
		x = x / 7;

		neof = myfile.good();
		if (neof)
		{
			switch (type) {
				case 1: //  Wall
				{
					cout << "wall set" << endl;
					CSprite* wall = wallOG.Clone();
					wall->SetPosition((float)x, (float)z);
					wall->SetRotation(float(rot));
					wallList.push_back(wall);
					break;
				}
				case 2: // Spawn
				{
					cout << "player set" << endl;
					player.SetPosition((float)x, (float)z);
					break;
				}
				case 3: // Enemy
				{
					cout << "enemy set" << endl;
					CSprite* enemy = enemyOG.Clone();
					enemy->SetPosition((float)x, (float)z);
					enemy->SetRotation(float(rot));
					enemyList.push_back(enemy);
					break;
				}
			}
		}
		cout << neof << endl;
	} while (neof);
	myfile.close();
}

void CMyGame::OnGameOver()
{
}

void CMyGame::OnTerminate()
{
}



void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	if (sym == SDLK_SPACE)
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
	my = y;
	mx = x;
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{
	lDown = true;
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
	lDown = false;
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}
