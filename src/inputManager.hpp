#pragma once
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

	void LockFor(float seconds = 0.1) {
		isLocked = true;
		timeToBeLocked = seconds;
	}

  private:
	float timeToBeLocked;
	float currentLockTime;
	bool isLocked;
};
