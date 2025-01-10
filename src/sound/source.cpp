
#include "source.hpp"
#include "debug.hpp"

namespace plgl {

	Source::Source(const std::shared_ptr<impl::SourceHandle>& source)
	: handle(source) {}

	Source& Source::resume() {
		if (std::shared_ptr<impl::SourceHandle> source = handle.lock()) {
			alSourcePlay(source->id);
			impl::alCheckError("alSourcePlay");
		}

		return *this;
	}

	Source& Source::pause() {
		if (std::shared_ptr<impl::SourceHandle> source = handle.lock()) {
			alSourcePause(source->id);
			impl::alCheckError("alSourcePause");
		}

		return *this;
	}

	Source& Source::gain(float value) {
		if (std::shared_ptr<impl::SourceHandle> source = handle.lock()) {
			alSourcef(source->id, AL_GAIN, value);
			impl::alCheckError("alSourcef");
		}

		return *this;
	}

	Source& Source::pitch(float value) {
		if (std::shared_ptr<impl::SourceHandle> source = handle.lock()) {
			alSourcef(source->id, AL_PITCH, value);
			impl::alCheckError("alSourcef");
		}

		return *this;
	}

	Source& Source::loop(bool enable) {
		if (std::shared_ptr<impl::SourceHandle> source = handle.lock()) {
			alSourcei(source->id, AL_LOOPING, enable);
			impl::alCheckError("alSourcei");
		}

		return *this;
	}

	ALint Source::state() const {
		ALint state = AL_STOPPED;

		if (std::shared_ptr<impl::SourceHandle> source = handle.lock()) {
			alGetSourcei(source->id, AL_SOURCE_STATE, &state);
			impl::alCheckError("alGetSourcei");
		}

		return state;
	}

	void Source::stop() {
		if (std::shared_ptr<impl::SourceHandle> source = handle.lock()) {
			alSourceStop(source->id);
			impl::alCheckError("alSourceStop");
		}
	}

	bool Source::is_playing() {
		return state() == AL_PLAYING;
	}

	bool Source::is_paused() {
		return state() == AL_PAUSED;
	}

	bool Source::is_stopped() {
		return state() == AL_STOPPED;
	}

}