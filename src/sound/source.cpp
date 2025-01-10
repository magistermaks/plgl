
#include "source.hpp"
#include "debug.hpp"
#include "sound.hpp"

namespace plgl {

	Source::Source(const Sound& sound) {
		alGenSources(1, &al_source);
		impl::alCheckError("alGenSources");

		alSourcei(al_source, AL_BUFFER, sound.handle());
		impl::alCheckError("alSourcei");
	}

	Source::~Source() {
		alDeleteSources(1, &al_source);
	}

	Source& Source::resume() {
		alSourcePlay(al_source);
		impl::alCheckError("alSourcePlay");
		return *this;
	}

	Source& Source::pause() {
		alSourcePause(al_source);
		impl::alCheckError("alSourcePause");
		return *this;
	}

	Source& Source::gain(float value) {
		alSourcef(al_source, AL_GAIN, value);
		impl::alCheckError("alSourcef");
		return *this;
	}

	Source& Source::pitch(float value) {
		alSourcef(al_source, AL_PITCH, value);
		impl::alCheckError("alSourcef");
		return *this;
	}

	Source& Source::loop(bool enable) {
		alSourcei(al_source, AL_LOOPING, enable);
		impl::alCheckError("alSourcei");
		return *this;
	}

	ALint Source::state() const {
		ALint state;
		alGetSourcei(al_source, AL_SOURCE_STATE, &state);
		impl::alCheckError("alGetSourcei");
		return state;
	}

	void Source::stop() {
		alSourceStop(al_source);
		impl::alCheckError("alSourceStop");
	}

}