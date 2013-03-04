/************************************************************************
*  Program Name:		Sound.h  										*
*  Name:				          										*
*  Date:				January 25, 2013								*
*  Description:			Code file for Sound class						*
*						Plays sound                             		*
*						                								*
*  Update:																*
*																		*
************************************************************************/
#pragma once
#include "fmod\include\fmod.hpp"
#include <windows.h>
#include <iostream>
#pragma comment(lib, "fmodex_vc.lib")
#define ERRCHECK(x) if(x != FMOD_OK) {MessageBox(hWnd, (LPCWSTR)x, L"Fmod Error", MB_OK);}
class Sound
{

public:
	FMOD::System            *system;
	FMOD_RESULT                     result;
	FMOD::Channel           *channelFX;
	FMOD::Channel           *channelMM;
	FMOD::Channel           *channelCS;
	FMOD::Channel           *channelL1;
	FMOD::Channel           *channelL2;
	FMOD::Channel           *channelL3;
	FMOD::Channel           *channelBS;
	FMOD::Channel           *channelPS;
	FMOD::Channel           *channelWS;
	FMOD::Channel           *channelDS;
	FMOD::Channel           *channelPC; // previous channel
	FMOD_SPEAKERMODE        speakermode;
	FMOD_CAPS                       caps;
	unsigned int            version;
	int                                     numdrivers;
	char                            name[256];
	bool                            Playing1;
	bool                            Playing2;
	HWND                            hWnd;

	// Game Sounds
	FMOD::Sound                     *Sound1;

public:
	Sound(void);
	Sound(HWND hWnd);
	~Sound(void);
	void CreateFmod(HWND m_hWnd);
	void PlaySound(HWND hWnd, FMOD::Sound* sound,FMOD::Channel **channel);
	void PlaySoundFX(FMOD::Sound* sound,FMOD::Channel **channel);
	void CreateSounds(HWND hWnd);
	void SoundShutdown();
	bool IsPlaying(FMOD::Channel *channel,HWND hWnd);
	void ChannelStop(FMOD::Channel* channel,HWND hWnd, FMOD::Sound *sound, FMOD::Channel** nextChannel);

	//Create a system objects and initialize
};

