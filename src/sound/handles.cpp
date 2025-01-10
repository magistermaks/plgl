
#include "handles.hpp"
#include "debug.hpp"

namespace plgl::impl {

	/*
	 * SoundHandle
	 */

	SoundHandle::SoundHandle() {
		alGenBuffers(1, &id);
		impl::alCheckError("alGenBuffers");
	}

	SoundHandle::~SoundHandle() {
		alDeleteBuffers(1, &id);
		impl::alCheckError("alDeleteBuffers");
		id = 0;
	}

	/*
	 * SourceHandle
	 */

	SourceHandle::SourceHandle() {
		alGenSources(1, &id);
		impl::alCheckError("alGenSources");
	}

	SourceHandle::~SourceHandle() {
		alDeleteSources(1, &id);
		impl::alCheckError("alDeleteSources");
		id = 0;
	}

}