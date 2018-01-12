#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "../include/u-gine.h"


int main(int argc, char* argv[]) {

	Screen::Instance().Open(1280, 720, false);

	uint16 screenWidth = Screen::Instance().GetWidth();
	uint16 screenHeight = Screen::Instance().GetHeight();

	AudioManager::Instance().Init();
	AudioSource* audioSource = new AudioSource("data/mutant.ogg");

	Listener::Instance().SetPosition(0, 0, 0);

	double position = 0;
	double pitch = 1;
	double maxPitch = 3.5;
	double minPitch = 0.65;
	double pitchIncrement = 0.1;

	audioSource->Play();

	while (Screen::Instance().IsOpened() && !Screen::Instance().KeyPressed(GLFW_KEY_ESC)) {

		Renderer::Instance().Clear(20, 20, 20);

		// Ellipse
		Renderer::Instance().SetColor(61, 164, 171, 0);
		Renderer::Instance().DrawEllipse(screenWidth / 2 + position * 10, screenHeight / 2 / pitch, 20, 20);

		// Position
		if (Screen::Instance().KeyPressed(GLFW_KEY_RIGHT)) {
			position++;
		}
		if (Screen::Instance().KeyPressed(GLFW_KEY_LEFT)) {
			position--;
		}

		// Pitch
		if (Screen::Instance().KeyPressed(GLFW_KEY_UP) &&
			pitch < maxPitch) {
			pitch += pitchIncrement;
		}
		if (Screen::Instance().KeyPressed(GLFW_KEY_DOWN) &&
			pitch > minPitch) {
			pitch -= pitchIncrement;
		}

		audioSource->SetPosition(position, 0, 0);
		audioSource->SetPitch(pitch);

		AudioStream::UpdateAll();

		// Refresh
		Screen::Instance().Refresh();
	}

	AudioManager::Instance().Finish();
	ResourceManager::Instance().FreeResources();

	return 0;
}