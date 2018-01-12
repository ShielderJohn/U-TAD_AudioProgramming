#include "../include/listener.h"
#include "../openal/AL/al.h"

Listener* Listener::listener = nullptr;

Listener::Listener() {
	SetPosition(0, 0, 0);
	SetVelocity(0, 0, 0);
	SetOrientation(0, 0, 0);
}

Listener& Listener::Instance() {
	if (!listener) {
		listener = new Listener();
	}

	return *listener;
}

void Listener::SetPosition(float x, float y, float z) {
	alListener3f(AL_POSITION, x, y, z);
}

void Listener::SetVelocity(float x, float y, float z) {
	alListener3f(AL_VELOCITY, x, y, z);
}

void Listener::SetOrientation(float x, float y, float z) {
	alListener3f(AL_ORIENTATION, x, y, z);
}
