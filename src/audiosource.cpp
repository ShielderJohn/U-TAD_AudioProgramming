#include "../include/audiosource.h"
#include "../include/audiostream.h"
#include "../include/string.h"
#include "../openal/AL/al.h"

AudioSource::AudioSource(AudioBuffer* buffer) {
	this->buffer = buffer;
	stream = nullptr;

	alGenSources(1, &source);
	alSourcei(source, AL_LOOPING, 0);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_BUFFER, this->buffer->GetBuffer());
}

AudioSource::AudioSource(const String& filename) {
	alGenSources(1, &source);
	alSourcei(source, AL_LOOPING, 0);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);

	stream = new AudioStream(filename, this);
}

AudioSource::~AudioSource() {
	alDeleteSources(1, &source);
}

unsigned int AudioSource::GetSource() {
	return source;
}

void AudioSource::SetPitch(float pitch) {
	alSourcef(source, AL_PITCH, pitch);
}

void AudioSource::SetGain(float gain) {
	alSourcef(source, AL_GAIN, gain);
}

void AudioSource::SetLooping(bool loop) {
	alSourcei(source, AL_LOOPING, loop);
}

void AudioSource::SetPosition(float x, float y, float z) {
	alSource3f(source, AL_POSITION, x, y, z);
}

void AudioSource::SetVelocity(float x, float y, float z) {
	alSource3f(source, AL_VELOCITY, x, y, z);
}

void AudioSource::Play() {
	alSourcePlay(source);
}

void AudioSource::Stop() {
	alSourceStop(source);
}

void AudioSource::Pause() {
	alSourcePause(source);
}

bool AudioSource::IsPlaying() const {
	int state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);

	return state == AL_PLAYING;
}