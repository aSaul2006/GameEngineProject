#include "Menu.h"
#include "Keyboard_Input.h"
#include "Background.h"


Menu::Menu()
{
	menuInput = NULL;
	background = NULL;
	newGame = NULL;
	newGameH = NULL;
	options = NULL;
	optionsH = NULL;
	exit = NULL;
	exitH = NULL;
}

Menu::~Menu()
{
	if(menuInput != NULL) {delete menuInput;}
	if(background != NULL) {delete background;}
	if(newGame != NULL) {delete newGame;}
	if(newGameH != NULL) {delete newGameH;}
	if(options != NULL) {delete options;}
	if(optionsH != NULL) {delete optionsH;}
	if(exit != NULL) {delete exit;}
	if(exitH != NULL) {delete exitH;}

}

//Loads all images for the menu
bool Menu::initialize(InputManager *Input, LPDIRECT3DDEVICE9 Device)
{
	//0 is Default, 1 is New Game, 2 Options, 3 Exit

	menuInput = Input;
	menuItemSelect = 1;
	message = 1;
	//Load Menu Images
	//Load Background for Menu
	background = new Background();
	background->load(Device, L"mBackground.png");

	//Load New Game Image
	newGame = new Background();
	newGame->load(Device, L"NewGame_0.png");
	newGame->setPosition(340, 300);
	newGameH = new Background();
	newGameH->load(Device, L"NewGame_1.png");
	newGameH->setPosition(340,300);

	//Load Options (have no images so using new game for now)
	options = new Background();
	options->load(Device, L"NewGame_0.png");
	options->setPosition(340, 400);
	optionsH = new Background();
	optionsH->load(Device, L"NewGame_1.png");
	options->setPosition(340, 400);


	//Load Exit Image
	exit = new Background();
	exit->load(Device, L"Exit_0.png");
	exit->setPosition(340, 500);
	exitH = new Background();
	exitH->load(Device, L"Exit_1.png");
	exitH->setPosition(340, 500);

	return true;

}

void Menu::update()
{
	if((DIK_UP || DIK_W) || (DIK_DOWN))
	{
		if(menuItemSelect == 1)
		{
			menuItemSelect = 2;
		}
		if(menuItemSelect == 2)
		{
			menuItemSelect = 3;
		}

	}

	//Gets Menu Pressed Input
	if((DIK_RETURN))
	{
		switch (menuItemSelect)
		{
		case 0:
			{
				message = DEFAULT;
				break;
			}
		case 1:
			{
				message = NEW_GAME;
				break;
			}
		case 2:
			{
				message = OPTIONS;
				break;
			}
		case 3:
			{
				message = EXIT;
				break;
			}
		}
	}
}

void Menu::render(LPDIRECT3DDEVICE9 Device)
{
	//render Menu & Background
	background->render(Device);

	//Render menut items based on which is highlighted
	if(menuItemSelect == 1)
	{
		newGameH->render(Device);
		options->render(Device);
		exit->render(Device);
	}

	if(menuItemSelect == 2)
	{
		newGame->render(Device);
		optionsH->render(Device);
		exit->render(Device);
	}

	if(menuItemSelect == 3)
	{
		newGame->render(Device);
		options->render(Device);
		exitH->render(Device);
	}

}

int Menu::getMessage()
{
	return message;
}
