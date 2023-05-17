#pragma once

class CMyGame : public CGame
{
	
	int my;
	int mx;
	int fireTimer = 0;
	float damp = 0;

	bool lDown;

	CSprite player;
	CSprite goal;
	CSprite wallOG;
	CSprite shotOG;
	CSprite floorOG;
	CSprite enemyOG;

	CSpriteList wallList;
	CSpriteList shotList;
	CSpriteList floorList;
	CSpriteList enemyList;

public:
	CMyGame(void);
	~CMyGame(void);

	
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);

	virtual void PlayerControl(Uint32 t);
	virtual void CameraControl(CGraphics* g);
	virtual void GunControl(Uint32 t);
	virtual void EnemyControl(Uint32 t);
	virtual void LevelReading(string filename);
	
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartGame();
	virtual void OnStartLevel(Sint16 nLevel);
	virtual void OnGameOver();
	virtual void OnTerminate();

	
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);
};
