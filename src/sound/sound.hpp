#pragma once

#include "external.hpp"

namespace plgl {

	class Source;

	class Sound {

		private:

			ALuint al_buffer;

			ALenum format(uint32_t channels);

		public:

			Sound(const std::string& path);
			~Sound();

			ALuint handle() const;
			Source& play() const;

	};

}
