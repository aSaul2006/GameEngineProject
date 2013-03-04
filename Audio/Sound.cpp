/************************************************************************
*  Program Name:		Sound.cpp  										*
*  Name:				          										*
*  Date:				January 25, 2013								*
*  Description:			Code file for Sound class						*
*						Plays sound                             		*
*						                								*
*  Update:																*
*																		*
************************************************************************/
//////////////////////////////////////////////////////////////////////////
// Initialize FMOD
//////////////////////////////////////////////////////////////////////////
// The following code is taken from the "Platform Specific Issues" section 
// and the "Getting Started" section of the FMOD documentation.


/**************************************************************************
/**************************************************************************

This code should be pasted in CDirectXFramework.h

/**************************************************************************
/**************************************************************************/

#include "Sound.h"

Sound::Sound(HWND hWnd)
{
	CreateFmod(hWnd);
}
void Sound::CreateFmod(HWND m_hWnd)
{
	hWnd = m_hWnd;
	result = FMOD::System_Create(&system);
	ERRCHECK(result);
	result = system->getVersion(&version);
	ERRCHECK(result);
	if (version < FMOD_VERSION)
	{
		printf("Error! You are using an old version of FMOD %08x. This program requires %08x\n", version, FMOD_VERSION);
		//              return 0;
	}
	result = system->getNumDrivers(&numdrivers);
	ERRCHECK(result);
	if (numdrivers == 0)
	{
		result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		ERRCHECK(result);
	}
	else
	{
		result = system->getDriverCaps(0, &caps, 0, &speakermode);
		ERRCHECK(result);
		//              result = system->getDriverCaps(0, &caps, 0,0, &speakermode);
		//              ERRCHECK(result);
		/*
		Set the user selected speaker mode.
		*/
		result = system->setSpeakerMode(speakermode);
		ERRCHECK(result);
		if (caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			/*
			The user has the 'Acceleration' slider set to off! This is really bad
			for latency! You might want to warn the user about this.
			*/
			result = system->setDSPBufferSize(1024, 10);
			ERRCHECK(result);
		}
		result = system->getDriverInfo(0, name, 256, 0);
		ERRCHECK(result);
		if (strstr(name, "SigmaTel"))
		{
			/*
			Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
			PCM floating point output seems to solve it.
			*/
			result = system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
			ERRCHECK(result);
		}
	}
	result = system->init(100, FMOD_INIT_NORMAL, 0);
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		/*
		Ok, the speaker mode selected isn't supported by this soundcard. Switch it
		back to stereo...
		*/
		result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		ERRCHECK(result);
		/*
		... and re-init.
		*/
		result = system->init(100, FMOD_INIT_NORMAL, 0);
		ERRCHECK(result);
	}

	//Create the different Sounds
	CreateSounds(hWnd);
	Playing1 = false;
	Playing2 = false;
}

void Sound::PlaySound(HWND hWnd, FMOD::Sound* sound,FMOD::Channel **channel)
{
	FMOD::Channel* temp = (FMOD::Channel*)channel;

	bool bOutcome = false;
	//if((result = temp->isPlaying(&bOutcome)) == FMOD_OK)
	{
		result = system->playSound(FMOD_CHANNEL_FREE, sound, false, channel);
		ERRCHECK(result);

		result = system->update();
		ERRCHECK(result);
	}
}

void Sound::PlaySoundFX(FMOD::Sound* sound,FMOD::Channel **channel)
{
	FMOD::Channel* temp = (FMOD::Channel*)channel;

	bool bOutcome = false;
	//if((result = temp->isPlaying(&bOutcome)) == FMOD_OK)
	{
		result = system->playSound(FMOD_CHANNEL_FREE, sound, false, channel);

		result = system->update();
	}
}

void Sound::CreateSounds(HWND hWnd)
{
	//Create the different Sounds
	result = system->createSound("sound1.wav", FMOD_DEFAULT, 0, &Sound1);
	ERRCHECK(result);
}


bool Sound::IsPlaying(FMOD::Channel *channel,HWND hWnd)
{
	if((result = channel->isPlaying(&Playing1)) == FMOD_OK)
	{
		return true;
	}
	return false;
}


void Sound::ChannelStop(FMOD::Channel *channel,HWND hWnd, FMOD::Sound *sound, FMOD::Channel **nextChannel)
{
	if(!system)
	{
		return;
	}
	bool bOutcome = false;
	if((result = channel->isPlaying(&bOutcome)) == FMOD_OK)
	{
		result = channel->stop();
		ERRCHECK(result);

		PlaySound(hWnd, sound, nextChannel);
	}
}
void Sound::SoundShutdown()
{
	//On Game End
	//to free the sounds
	result = Sound1->release();
	ERRCHECK(result);
}
Sound::~Sound()
{
	SoundShutdown();
}