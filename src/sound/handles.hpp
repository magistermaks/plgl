#pragma once

#include "external.hpp"

namespace plgl::impl {

	/// Wrapper around OpenAL Buffer object, managed by sound system
	struct SoundHandle {
		bool keep;
		ALuint id;

		std::atomic_int sources;

		SoundHandle();
		~SoundHandle();

		bool should_remove() const;
	};

	/// Wrapper around OpenAL Source object, managed by sound system
	struct SourceHandle {
		bool keep;
		ALuint id;

		ALuint buffer;

		SourceHandle(ALuint buffer);
		~SourceHandle();

		bool should_remove() const;
	};

}