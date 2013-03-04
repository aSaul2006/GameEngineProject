/************************************************************************
*  Program Name:		AudioManager.cpp  								*
*  Name:				          										*
*  Date:				January 25, 2013								*
*  Description:			Code file for Sound class						*
*						Plays sound                             		*
*						Singleton design								*
*  Update:																*
*																		*
************************************************************************/
#include "AudioManager.h"
#include "cTextureResource.h"

AudioManager* AudioManager::instance = 0;

AudioManager::AudioManager(void)
{
	soundSystem = NULL;
	bgmChannel = NULL;
	for(int n = 0; n < numSfxChannels; n++)
	{
		sfxChannels[n] = NULL;
	}
}

AudioManager::~AudioManager(void)
{
	Shutdown();
}

AudioManager* AudioManager::GetInstance(void)
{
	if(instance == 0)
		instance = new AudioManager();
	return instance;
}

void AudioManager::DestroyInstance(void)
{
	if(instance)
	{
		delete instance;
		instance = NULL;
	}
}

void AudioManager::Initialize(void)
{
	////////////////////////////////////////////////////////////////////////////////////////
	//// Initialize FMOD
	////////////////////////////////////////////////////////////////////////////////////////

	FMOD_CAPS caps;
	FMOD_SPEAKERMODE speakerMode;
	unsigned int version;
	int numDrivers;
	char name[256];

	/*
	Create a System object and initialize.
	*/
	result = FMOD::System_Create(&soundSystem);
	//ERRCHECK(result);
	result = soundSystem->getVersion(&version);
	//ERRCHECK(result);
	if (version < FMOD_VERSION)
	{
		printf("Error! You are using an old version of FMOD %08x. This program requires %08x\n",
			version, FMOD_VERSION);
		//return 0;
	}
	result = soundSystem->getNumDrivers(&numDrivers);
	//ERRCHECK(result);
	if (numDrivers == 0)
	{
		result = soundSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		//ERRCHECK(result);
	}
	else
	{
		result = soundSystem->getDriverCaps(0, &caps, 0, &speakerMode);
		//ERRCHECK(result);
		/*
		Set the user selected speaker mode.
		*/
		result = soundSystem->setSpeakerMode(speakerMode);
		//ERRCHECK(result);
		if (caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			/*
			The user has the 'Acceleration' slider set to off! This is really bad
			for latency! You might want to warn the user about this.
			*/
			result = soundSystem->setDSPBufferSize(1024, 10);
			//ERRCHECK(result);
		}
		result = soundSystem->getDriverInfo(0, name, 256, 0);
		//ERRCHECK(result);
		if (strstr(name, "SigmaTel"))
		{
			/*
			Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
			PCM floating point output seems to solve it.
			*/
			result = soundSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT,
				0,0, FMOD_DSP_RESAMPLER_LINEAR);
			//ERRCHECK(result);
		}
	}
	result = soundSystem->init(100, FMOD_INIT_NORMAL, 0);
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		/*
		Ok, the speaker mode selected isn't supported by this soundcard. Switch it
		back to stereo...
		*/
		result = soundSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		//ERRCHECK(result);
		/*
		... and re-init.
		*/
		result = soundSystem->init(100, FMOD_INIT_NORMAL, 0);
	}
}

// Shutdown sound components
void AudioManager::Shutdown(void)
{
	if(soundSystem)
	{
		soundSystem->release();
		soundSystem = NULL;
	}
}

void AudioManager::Play(FMOD::Sound* sound)
{
	result = soundSystem->playSound(FMOD_CHANNEL_FREE, sound, false, 0);
}

void AudioManager::PlaySFX(FMOD::Sound* sound)
{
	bool playing;
	for(int n = 0; n < numSfxChannels; n++)
	{
		sfxChannels[n]->isPlaying(&playing);
		if(!playing)
		{
			sfxChannels[n]->stop();
			result = soundSystem->playSound(FMOD_CHANNEL_FREE,
				sound, false, &sfxChannels[n]);
			break;
		}
	}
}

void AudioManager::PlayBGM(FMOD::Sound* sound)
{
	bgmChannel->stop();
	result = soundSystem->playSound(FMOD_CHANNEL_FREE, sound, false, &bgmChannel);
}

void AudioManager::SetBGMVolume(float volume)
{
	result = bgmChannel->setPaused(true);
	result = bgmChannel->setVolume(volume);
	result = bgmChannel->setPaused(false);
}

void AudioManager::SetSFXVolume(float volume)
{
	for(int n = 0; n < numSfxChannels; n++)
	{
		result = sfxChannels[n]->setPaused(true);
		result = sfxChannels[n]->setVolume(volume);
		result = sfxChannels[n]->setPaused(false);
	}
}

//-------------------------------------------------------------------------------
//===============================================================================
//---------------------------Load Data from XML----------------------------------
//===============================================================================

//Instantiates variables every time cResourceManager finds an audio asset

cResource* AudioManager::loadResourceFromXML(TiXmlElement *Element)
{
	if(Element)
	{
		cAudioResource* Resource = new cAudioResource();

		for(TiXmlAttribute* ElementAttrib = Element->FirstAttribute(); ElementAttrib; ElementAttrib= ElementAttrib->Next())
		{
			//examine our audio resource object
			std::string AttribName = ElementAttrib->Name();
			std::string AttribValue = ElementAttrib->Value();

			if(AttribName=="UID")
			{
				Resource->m_ResourceID = atoi(AttribValue.c_str());
			}
			if(AttribName=="filename")
			{
				Resource->m_FileName = AttribValue;
			}
			if(AttribName=="scenescope")
			{
				Resource->m_Scope = atoi(AttribValue.c_str());
			}
			if(AttribName=="audio_type")
			{
				if(AttribValue=="default")
					Resource->m_AudioType = AUDIO_TYPE_DEFAULT;
				else if(AttribValue=="sfx")
					Resource->m_AudioType = AUDIO_TYPE_SFX;
				else if(AttribValue=="bgm")
					Resource->m_AudioType = AUDIO_TYPE_BGM;
			}

		}
		return Resource;
	}
	return NULL;
}

cResource* AudioManager::loadTResourceFromXML(TiXmlElement *Element)
{
	if(Element)
	{
		cTextureResource* Resource = new cTextureResource();

		for(TiXmlAttribute* ElementAttrib = Element->FirstAttribute(); ElementAttrib; ElementAttrib= ElementAttrib->Next())
		{
			//examine our audio resource object
			std::string AttribName = ElementAttrib->Name();
			std::string AttribValue = ElementAttrib->Value();

			if(AttribName=="UID")
			{
				Resource->m_ResourceID = atoi(AttribValue.c_str());
			}
			if(AttribName=="filename")
			{
				Resource->m_FileName = AttribValue;
			}
			if(AttribName=="scenescope")
			{
				Resource->m_Scope = atoi(AttribValue.c_str());
			}

		}
		return Resource;
	}
	return NULL;
}

//-------------------------------------------------------------------------------
//===============================================================================
//---------------------------Audio Resource class----------------------------
//===============================================================================

cAudioResource::~cAudioResource()
{
	unload();
}
	
void cAudioResource::load()
{
	FMOD_MODE mode = FMOD_DEFAULT;
	FMOD_RESULT result;
	
	result = AudioManager::GetInstance()->GetSystem()->
		createStream(m_FileName.c_str(), mode, 0, &p_Sound);

	// If sound loaded incorrectly, release it and set it to NULL
	if(result == FMOD_OK)
		std::string thisthing = m_FileName.c_str();
	else if(p_Sound != NULL)
	{
		p_Sound->release();
		p_Sound = NULL;
	}
	
}
	
void cAudioResource::unload()
{
	if (p_Sound)
	{
		p_Sound->release();
		p_Sound = NULL;
	}
}