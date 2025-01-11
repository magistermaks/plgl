#pragma once

#include "external.hpp"
#include "handles.hpp"
#include "generator.hpp"

namespace plgl {

	class Source;

	/**
	 * Represents a loaded sound data (buffer), allows you to play a sound and then interact with it
	 * using the returned plgl::Source object.
	 *
	 * @example
	 * @code{.cpp}
	 * int main() {
	 *
	 *    // Open window and start sound system first
	 *    open("My Sound Application");
	 *
	 *    // Load and play sound
	 *    Sound my_sound {"assets/my_sound.ogg"};
	 *    my_sound.play();
	 *
	 *    // Once you play a sound you can modify it using the returned 'Source' object
	 *    Source my_playing_sound = my_sound.play();
	 *    my_playing_sound.loop();
	 *    my_playing_sound.pitch(0.8);
	 *
	 *    // Just wait for the user to close the window
	 *    pause();
	 *
	 *    // This will both close the window as well as stop the sound system
	 *    close();
	 *
	 * }
	 * @endcode
	 *
	 * This objects implements fearless memory safety, you can copy it safely and quickly.
	 * Though using references is, as always, recommended when applicable.
	 */
	class Sound {

		private:

			std::weak_ptr<impl::SoundHandle> handle;

		public:

			/**
			 * @brief Load new sound from file
			 *
			 * Create new sound from a sound file (Only .OGG Vorbis sound files are supported)
			 * Remember to only load the sound once, then you can play it using play().
			 * Sounds can only be loaded after the window is opened using plgl::open()
			 *
			 * @example
			 * @code{.cpp}
			 * int main() {
			 *
			 *    // Open window and start sound system first
			 *    open("My Sound Application");
			 *
			 *    // Load sound file
			 *    Sound my_sound {"assets/my_sound.ogg"};
			 *
			 *    // play every second
			 *    while (!should_close) {
			 *       my_sound.play();
			 *       sleep(1s);
			 *    }
			 *
			 *    // This will both close the window as well as stop the sound system
			 *    close();
			 *
			 * }
			 * @endcode
			 *
			 * @param[in] path Path to the .ogg sound file
			 */
			Sound(const std::string& path);

			/**
			 * @brief Create sound using waveform generator
			 *
			 * Create new sound dynamically using waveform generator.
			 * Sounds can only be loaded after the window is opened using plgl::open()
			 *
			 * @example
			 * @code{.cpp}
			 * int main() {
			 *
			 *    // Open window and start sound system first
			 *    open("My Sound Application");
			 *
			 *    // Dynamically create sound data
			 *    Sound my_sound {[] (std::vector<short>& buffer, int frequency) {
			 *       // generate sound data
			 *    }, 100};
			 *
			 *    // play every second
			 *    while (!should_close) {
			 *       my_sound.play();
			 *       sleep(1s);
			 *    }
			 *
			 *    // This will both close the window as well as stop the sound system
			 *    close();
			 *
			 * }
			 * @endcode
			 *
			 * @see plgl::tone()
			 *
			 * @param[in] generator    Predefined generator or a custom function
			 * @param[in] milliseconds Number of milliseconds to record
			 */
			Sound(const WaveformGenerator& generator, int milliseconds);

			/**
			 * @brief Discard and delete this buffer
			 *
			 * Delete this buffer and free related resources as soon as
			 * no sound source uses it anymore. After that it will become unusable.
			 */
			void discard();

			/**
			 * @brief Play this sound
			 *
			 * Starts playing the sound and returns the plgl::Source object
			 * that allows one to control the sound playback.
			 *
			 * @example
			 * @code{.cpp}
			 * // Note that you should only load the sound once
			 * Sound my_sound {"assets/my_sound.ogg"};
			 *
			 * // Once you play a sound you can modify it using the returned 'Source' object
			 * Source my_source = my_sound.play();
			 * my_source.loop();
			 * my_source.pitch(0.8);
			 * my_source.gain(0.5);
			 * @endcode
			 *
			 * @see plgl::Source
			 *
			 * @param[in] volume Optionally pass desired sound volume
			 * @param[in] pitch  Optionally pass desired sound pitch
			 *
			 * @return The plgl::Source sound control object
			 */
			Source play(float volume = 1.0f, float pitch = 1.0f);

	};

}
