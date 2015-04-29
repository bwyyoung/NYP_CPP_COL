#include "AL_Audio.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
using namespace std;

#define NUM_BUFFERS 1
#define NUM_SOURCES 1
#define NUM_ENVIRONMENTS 1

ALCdevice	*gDevice;
ALCcontext	*gContext;

ALfloat listenerPos[] = {0.0f, 0.0f, 0.4f};
ALfloat listenerVel[] = {0.0f, 0.0f, 0.0f};
ALfloat listenerOri[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};

ALfloat sourcePos[] = {-2.0f, 0.0f, 0.0f};
ALfloat sourceVel[] = {0.0f, 0.0f, 0.0f};

ALuint	buffer[NUM_BUFFERS];
ALuint	source[NUM_SOURCES];
ALuint	environments[NUM_ENVIRONMENTS];

ALsizei size, freq;
ALenum	format;
ALvoid	*data;

bool Init()
{
	/* OPEN DEVICE and CREATE CONTEXT */
	gDevice = alcOpenDevice(NULL);	// Get default device
	if (gDevice)
	{
		gContext = alcCreateContext(gDevice, NULL);
		alcMakeContextCurrent(gContext);
	}
	alIsExtensionPresent("EAX2.0"); // Check for EAX2.0 support

	/* BUFFER GENERATION */
	alGetError();	// Clear Error Messages
	alGenBuffers(NUM_BUFFERS, buffer);

	ALenum error;
	if (error = alGetError() != AL_NO_ERROR)
	{	// Error Generating Buffers
		cout<<"Error generating buffers! Error code: "<<error<<endl;
		return false;
	}

	/* SOURCES GENERATION */
	alGenSources(NUM_SOURCES, source);
	if (error = alGetError() != AL_NO_ERROR)
	{	// Error Generating Sources
		cout<<"Error generating sources! Error code: "<<error<<endl;
		return false;
	}

	/* ATTACH BUFFERS TO SOURCES */
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	if (error = alGetError() != AL_NO_ERROR)
	{	// Error attaching buffers to sources
		cout<<"Error attaching buffers to sources! Error code: "<<error<<endl;
		return false;
	}

	/* SET UP LISTENER */
	alListenerfv(AL_POSITION, listenerPos);
	if (error = alGetError() != AL_NO_ERROR) {
		cout<<"Error setting listener position! Error code: "<<error<<endl;
		return false;
	}
	alListenerfv(AL_VELOCITY, listenerVel);
	if (error = alGetError() != AL_NO_ERROR) {
		cout<<"Error setting listener velocity! Error code: "<<error<<endl;
		return false;
	}
	alListenerfv(AL_ORIENTATION, listenerOri);
	if (error = alGetError() != AL_NO_ERROR) {
		cout<<"Error setting listener orientation! Error code: "<<error<<endl;
		return false;
	}
	return true;
}

void Exit()
{
	gContext = alcGetCurrentContext();
	gDevice = alcGetContextsDevice(gContext);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(gContext);
	alcCloseDevice(gDevice);
}