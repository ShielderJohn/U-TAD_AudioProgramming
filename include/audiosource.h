#ifndef UGINE_AUDIOSOURCE_H
#define UGINE_AUDIOSOURCE_H

#include "audiobuffer.h"

class AudioStream;

class AudioSource {
public:
	AudioSource(AudioBuffer* buffer);
	AudioSource(const String& filename);
	~AudioSource();

	unsigned int GetSource();

	void SetPitch(float pitch);
	void SetGain(float gain);
	void SetLooping(bool loop);
	void SetPosition(float x, float y, float z);
	void SetVelocity(float x, float y, float z);

	void Play();
	void Stop();
	void Pause();
	bool IsPlaying() const;

private:
	unsigned int source;
	AudioBuffer* buffer;
	AudioStream* stream;
};

#endif