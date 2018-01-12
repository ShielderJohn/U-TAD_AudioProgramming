#include "../include/audiomanager.h"
#include "../openal/AL/alc.h"
#include "../openal/AL/al.h"

AudioManager* AudioManager::manager = nullptr;

AudioManager::~AudioManager(void) {

}

AudioManager& AudioManager::Instance() {
	if (!manager) {
		manager = new AudioManager();
	}
	
	return *manager;
}

void AudioManager::Init() {
	device = alcOpenDevice(NULL);
	context = alcCreateContext(static_cast<ALCdevice*>(device), NULL);
	alcMakeContextCurrent(static_cast<ALCcontext*>(context));
}

void AudioManager::Finish() {
	alcDestroyContext(static_cast<ALCcontext*>(context));
	alcCloseDevice(static_cast<ALCdevice*>(device));
}

void AudioManager::SetDopplerFactor(float factor) {
	alDopplerFactor(factor);
}