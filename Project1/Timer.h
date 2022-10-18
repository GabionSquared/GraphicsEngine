#pragma once


class Timer {
public:

	Timer();
	void Start();
	void Stop();
	void Pause();
	void Unpause();
	int GetTicks();
	bool IsPaused();
	bool IsStarted();

};