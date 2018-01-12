#ifndef UGINE_AUDIOMANAGER_H
#define UGINE_AUDIOMANAGER_H

#include "../include/glinclude.h"

class AudioManager {
public:
	static AudioManager& Instance();

	void Init();
	void Finish();

	void SetDopplerFactor(float factor);

protected:
	AudioManager(void) : device(NULL), context(NULL) {}
	~AudioManager(void);

private:
	static AudioManager* manager;
	void* device;
	void* context;
};

#endif