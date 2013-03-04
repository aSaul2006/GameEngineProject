#pragma once
#include <windows.h>
#include <stdio.h> //for swprintf_s
#include <math.h>

#include <d3d9.h>
class InputManager;
class Background;

class Menu
{
private:

	InputManager		*menuInput;
	Background			*background;
	Background			*newGame;
	Background			*newGameH;
	Background			*options;
	Background			*optionsH;
	Background			*exit;
	Background			*exitH;
	int					menuItemSelect;
	int					message;

public:

	Menu();	
	~Menu();

	bool initialize(InputManager *Input, LPDIRECT3DDEVICE9 Device);
	void update();
	void render(LPDIRECT3DDEVICE9 Device);
	int getMessage();

	//Enumerator
	enum{DEFAULT = 0, NEW_GAME = 1, OPTIONS = 2, EXIT = 3};


};
