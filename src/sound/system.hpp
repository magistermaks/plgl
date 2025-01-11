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

			std::vector<std::shared_ptr<impl::SoundHandle>> unused;
			std::list<std::shared_ptr<impl::SoundHandle>> sounds;
			std::list<std::shared_ptr<impl::SourceHandle>> sources;

		public:

			static SoundSystem* get();

			SoundSystem();
			~SoundSystem();

			std::shared_ptr<impl::SoundHandle> load();
			std::shared_ptr<impl::SourceHandle> play(const std::shared_ptr<impl::SoundHandle>& sound);

			void update();

		public:

			/**
			 * @brief Get preferred sampling rate
			 *
			 * Get desired frequency of the
			 * underlying sound device.
			 */
			int frequency();

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

			/**
			 * @brief Play a tone using the selected waveform preset
			 *
			 * @param[in] waveform     Shape of the wave to play
			 * @param[in] volume       Loudness of the tone to play
			 * @param[in] frequency    Hertz, you would want to use values in range 50 to 15000
			 * @param[in] milliseconds Optionally, number of milliseconds to play, should be below 1000
			 */
			Source tone(Waveform waveform, float volume, float frequency, int milliseconds = 100);

	};

}
