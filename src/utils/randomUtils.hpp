#include <cassert>
#include <raylib.h>

namespace util {
template <typename T> T GetRandom(T min, T max) {
	return static_cast<T>(
		GetRandomValue(static_cast<int>(min), static_cast<int>(max)));
}

template <typename T> T GetRandom(int min, T max) {
	return static_cast<T>(GetRandomValue(min, static_cast<int>(max)));
}
} // namespace util
