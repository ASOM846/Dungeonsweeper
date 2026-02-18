#pragma once
// Prosty, engine-agnostyczny InputLocker.
// Użycie:
//   inputLocker.LockFor(0.15f); // blokuj na 150 ms
//   inputLocker.Update(dt);
//   if (!inputLocker.IsLocked()) { handle_input(); }

class InputLocker {
  public:
	InputLocker() : locked(false), remainingSeconds(0.0f) {}

	// Blokada na określony czas (sekundy)
	void LockFor(float seconds) {
		locked = true;
		remainingSeconds = seconds;
	}

	// Ręczna blokada (do odblokowania explicit)
	void Lock() {
		locked = true;
		remainingSeconds = -1.0f;
	}

	// Odblokuj natychmiast
	void Unlock() {
		locked = false;
		remainingSeconds = 0.0f;
	}

	// Wywołuj co klatkę z delta-time (sekundy)
	void Update(float dt) {
		if (!locked)
			return;
		if (remainingSeconds < 0.0f)
			return; // indefinite lock
		remainingSeconds -= dt;
		if (remainingSeconds <= 0.0f)
			Unlock();
	}

	bool IsLocked() const { return locked; }

  private:
	bool locked;
	// remainingSeconds < 0 => locked until Unlock() called
	float remainingSeconds;
};
