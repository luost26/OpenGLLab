
#include "App/GymApp.hpp"

using namespace Showcase;

int main() {
	GymApp * app = new GymApp;
	app->run();
	delete app;
	return 0;
}