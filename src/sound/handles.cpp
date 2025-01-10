
#include "handles.hpp"
#include "debug.hpp"

namespace plgl::impl {

	/*
	 * SoundHandle
	 */

	SoundHandle::SoundHandle()
	: keep(true), sources(0) {
		alGenBuffers(1, &id);
		impl::alCheckError("alGenBuffers");
	}

	SoundHandle::~SoundHandle() {
		alDeleteBuffers(1, &id);
		impl::alCheckError("alDeleteBuffers");
		id = 0;
	}

	bool SoundHandle::should_remove() const {
		if (!keep) {
			return sources == 0;
		}

		return false;
	}

	/*
	 * SourceHandle
	 */

	SourceHandle::SourceHandle(ALuint buffer)
	: keep(false), buffer(buffer) {
		alGenSources(1, &id);
		impl::alCheckError("alGenSources");

		alSourcei(id, AL_BUFFER, buffer);
		impl::alCheckError("alSourcei");
	}

	SourceHandle::~SourceHandle() {
		alDeleteSources(1, &id);
		impl::alCheckError("alDeleteSources");
		id = 0;
	}

	bool SourceHandle::should_remove() const {
		if (!keep) {
			ALenum state = AL_STOPPED;
			alGetSourcei(id, AL_SOURCE_STATE, &state);
			return state == AL_STOPPED;
		}

		return false;
	}

}