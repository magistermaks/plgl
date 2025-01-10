#pragma once

#include "external.hpp"
#include "source.hpp"
#include "sound.hpp"
#include "handles.hpp"

namespace plgl {

	class SoundSystem {

		private:

			ALCdevice* device;
			ALCcontext* context;

			std::list<std::shared_ptr<impl::SoundHandle>> sounds;
			std::list<std::shared_ptr<impl::SourceHandle>> sources;

		public:

			static SoundSystem* get();

			SoundSystem();
			~SoundSystem();

			std::weak_ptr<impl::SoundHandle> load(const std::string& path);
			std::weak_ptr<impl::SourceHandle> play();

			void update();

			/**
			 * @brief Unload all loaded sound buffers
			 *
			 * This frees the memory used by all the loaded sound files, this is
			 * done automatically once you call plgl::close()
			 */
			void unload_all();

			/**
			 * @brief Stop all sounds
			 *
			 * This has the same effect as calling Source::stop() on all sounds.
			 */
			void stop_all();

			/**
			 * @brief Pause all sounds
			 *
			 * This has the same effect as calling Source::pause() on all sounds.
			 */
			void pause_all();

			/**
			 * @brief Resume all paused sounds
			 *
			 * This has the same effect as calling Source::resume() on all paused sounds.
			 */
			void resume_all();

	};

}
