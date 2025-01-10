
#include "sound.hpp"
#include "debug.hpp"
#include "source.hpp"
#include "system.hpp"

namespace plgl {

	/*
	 * Sound
	 */

	Sound::Sound(const std::string& path)
	: handle(SoundSystem::get()->load(path)) {
		// we only hold a weak ref to the buffer
	}

	Source Sound::play(float volume, float pitch) {
		return Source {SoundSystem::get()->play().lock(), handle.lock()}.gain(volume).pitch(pitch).resume();
	}

}