/************************************************************************
*  Program Name:		AudioManager.h  								*
*  Name:				          										*
*  Date:				January 25, 2013								*
*  Description:			Code file for Sound class						*
*						Plays sound                             		*
*						Singleton design								*
*  Update:																*
*																		*
************************************************************************/
#pragma once

#ifndef H_AUDIOMANAGER
#define H_AUDIOMANAGER

#include <string>
#include <Windows.h>

#include "ResourceManager.h"

typedef enum {AUDIO_TYPE_DEFAULT=0, AUDIO_TYPE_SFX=1, AUDIO_TYPE_BGM=2}AUDIO_TYPE;

//-------------------------------------------------------------------------------
//===============================================================================
//---------------------------Audio Resource class----------------------------
//===============================================================================
class cAudioResource : public cResource
{
public:
	inline cAudioResource()
	{
		m_ResourceID = m_Scope = 0;
		m_Type = RESOURCE_AUDIO;
	}

	~cAudioResource();
	void load();
	void unload();

	AUDIO_TYPE m_AudioType;
	// Get access for the sound variable
	FMOD::Sound* GetSound(void){ return p_Sound;}

private:
	FMOD::Sound* p_Sound;
};

//-------------------------------------------------------------------------------
//===============================================================================
//---------------------------Audio Resource class----------------------------
//===============================================================================
class AudioManager
{
public:
	// Destructor
	~AudioManager(void);

	void Initialize(void);	// Initialize sound components
	void Shutdown(void);	// Shutdown sound components

	// Singleton instance manip methods
	static AudioManager* GetInstance(void);
	static void DestroyInstance(void);

	// Accessors
	FMOD::System* GetSystem(void)
		{return soundSystem;}

	// Sound playing
	void Play(FMOD::Sound* sound);	// Play a sound/music with default channel
	void PlaySFX(FMOD::Sound* sound);	// Play a sound effect with custom channel
	void PlayBGM(FMOD::Sound* sound);	// Play background music with custom channel

	// Volume adjustment methods
	void SetBGMVolume(float volume);
	void SetSFXVolume(float volume);

	cResource* loadResourceFromXML(TiXmlElement *Element);
	cResource* loadTResourceFromXML(TiXmlElement *Element);

private:
	static AudioManager* instance;	// Singleton instance
	AudioManager(void);	 // Constructor

	FMOD::System* soundSystem;	// Sound system object
	FMOD_RESULT result;
	FMOD::Channel* bgmChannel;	// Channel for background music
	static const int numSfxChannels = 4;
	FMOD::Channel* sfxChannels[numSfxChannels];	// Channel for sound effects
};
#endif