#pragma once

#include "external.hpp"

namespace plgl::impl {

	/// Wrapper around OpenAL Buffer object, managed by sound system
	struct SoundHandle {
		ALuint id;

		SoundHandle();
		~SoundHandle();
	};

	/// Wrapper around OpenAL Source object, managed by sound system
	struct SourceHandle {
		ALuint id;

		SourceHandle();
		~SourceHandle();
	};

}