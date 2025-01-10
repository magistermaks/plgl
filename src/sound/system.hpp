#pragma once

#include "external.hpp"
#include "source.hpp"
#include "sound.hpp"

namespace plgl {

	class SoundSystem {

		private:

			ALCdevice* al_device;
			ALCcontext* al_context;
			std::mutex mutex;

			std::list<Source> sources;

		public:

			SoundSystem();
			~SoundSystem();

			Source& play(const Sound& sound);
			void update();

			void stop_all();
			void pause_all();
			void resume_all();
	};

}
