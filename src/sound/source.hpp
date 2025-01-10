#pragma once

#include "external.hpp"

namespace plgl {

	class Sound;

	class Source {

		private:

			ALuint al_source;

		public:

			Source(const Sound& sound);
			~Source();

			Source& resume();
			Source& pause();
			Source& gain(float value);
			Source& pitch(float value);
			Source& loop(bool enable = true);

			ALint state() const;
			void stop();

	};

}

