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

			ALint state() const;
			void resume();
			void pause();
			void stop();
			void loop(bool enable = true);

	};

}

