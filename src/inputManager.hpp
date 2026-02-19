#pragma once
#include <iostream>
#include <raylib.h>

class InputManager {
  public:
	InputManager() : isLocked(false) {}
	~InputManager() = default;

	bool IsLocked() { return isLocked; }

	void Update(float deltaTime) {
		if (!isLocked) {
			return;
		}

		DrawText("LOCKED INPUT", 10, 10, 30, RED);

		currentLockTime += deltaTime;
		if (currentLockTime >= timeToBeLocked) {
			isLocked = false;
			timeToBeLocked = 0.0f;
			currentLockTime = 0.0f;
		}
	}

	void LockFor(float seconds) {
		isLocked = true;
		timeToBeLocked = seconds;
	}

  private:
	float timeToBeLocked;
	float currentLockTime;
	bool isLocked;
};
