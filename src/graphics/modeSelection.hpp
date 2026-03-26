#pragma once
#include "button.hpp"
#include <raylib.h>

class ModeSelection {
	ModeSelection();
	~ModeSelection();

	void Update();
	void Render();

	NewButton TimeCountingDownRun;
	NewButton TimeIncreasingRun;
	NewButton NoTimeCounterRun;
};
