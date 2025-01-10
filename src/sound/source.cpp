
#include "source.hpp"
#include "debug.hpp"
#include "sound.hpp"

namespace plgl {

	Source::Source(const Sound& sound) {
		alGenSources(1, &al_source);
		impl::alCheckError("alGenSources");

		alSourcei(al_source, AL_BUFFER, sound.handle());
		impl::alCheckError("alSourcei");

		alSourcePlay(al_source);
		impl::alCheckError("alSourcePlay");
	}

	Source::~Source() {
		alDeleteSources(1, &al_source);
	}

	ALint Source::state() const {
		ALint state;
		alGetSourcei(al_source, AL_SOURCE_STATE, &state);
		impl::alCheckError("alGetSourcei");
		return state;
	}

	void Source::resume() {
		alSourcePlay(al_source);
		impl::alCheckError("alSourcePlay");
	}

	void Source::pause() {
		alSourcePause(al_source);
		impl::alCheckError("alSourcePause");
	}

	void Source::stop() {
		alSourceStop(al_source);
		impl::alCheckError("alSourceStop");
	}

	void Source::loop(bool enable) {
		alSourcei(al_source, AL_LOOPING, enable);
		impl::alCheckError("alSourcei");
	}

}