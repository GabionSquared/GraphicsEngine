#include "Timer.h"


int startTicks;
int pausedTicks;
bool paused;
bool started;


Timer::Timer() {
	startTicks = 0;
	pausedTicks = 0;
	paused = false;
	started = false;
}

void Timer::Start() {
	started = true;
	paused = false;
	startTicks = SDL_GetTicks(); //time since SDL was initialised
	pausedTicks = 0;
}

void Timer::Stop() {
	Timer();
}

void Timer::Pause() {
	if (started && !paused) {
		paused = true;

		pausedTicks = SDL_GetTicks() - startTicks;
		//essentially saves the position the timer was at

		startTicks = 0;
	}
}

void Timer::Unpause() {
	if (started && paused) {
		paused = false;

		startTicks = SDL_GetTicks() - pausedTicks;
		//essentially saves the position the timer was at

		startTicks = 0;
	}
}

//Say if you start the timer when SDL_GetTicks() reports 5000 ms and then you pause it at 10000ms.
//This means the relative time at the time of pausing is 5000ms. If we were to unpause it when
//SDL_GetTicks was at 20000, the new start time would be 20000 - 5000ms or 15000ms.
//This way the relative time will still be 5000ms away from the current SDL_GetTicks time.

int Timer::GetTicks() {
	//thousandths of a second

	if (started) {
		if (paused) {
			return pausedTicks;
		}
		else {
			return SDL_GetTicks() - startTicks;
		}
	}
	else {
		return -1;
	}
}

bool Timer::IsPaused() {
	return paused;
}

bool Timer::IsStarted() {
	return started;
}