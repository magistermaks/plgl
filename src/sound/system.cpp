
#include "system.hpp"
#include "util.hpp"
#include "debug.hpp"
#include "globals.hpp"

namespace plgl {

	SoundSystem::SoundSystem() {
		const char* name = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
		this->al_device = alcOpenDevice(name);

		if (al_device == nullptr) {
			fault("Sound system failed to start, unable to open device '{}'!", name);
		}

		this->al_context = alcCreateContext(al_device, nullptr);

		if (al_context == nullptr) {
			fault("Sound system failed to start, unable to create context!");
		}

		alcMakeContextCurrent(al_context);
		impl::alCheckError("alcMakeContextCurrent");
	}

	SoundSystem::~SoundSystem() {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(al_context);
		alcCloseDevice(al_device);
	}

	Source& SoundSystem::play(const plgl::Sound& sound) {
		sound_count ++;
		return sources.emplace_back(sound);
	}

	void SoundSystem::update() {
		sources.remove_if([] (const Source& source) {
			return source.state() == AL_STOPPED;
		});

		sound_count = sources.size();
	}

	void SoundSystem::stop_all() {
		sound_count = 0;
		sources.clear();
	}

	void SoundSystem::pause_all() {
		for (Source& source : sources) source.pause();
	}

	void SoundSystem::resume_all() {
		for (Source& source : sources) source.resume();
	}

}
