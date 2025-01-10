#pragma once

#include "external.hpp"
#include "handles.hpp"

namespace plgl {

	class Sound;

	/**
	 * Represents a playing (or paused) sound, allows you to interact with the sound
	 * changing its properties during playback.
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
	 *    // ... Or,
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
	class Source {

		private:

			std::weak_ptr<impl::SourceHandle> handle;

			/// Get OpenAL 'AL_SOURCE_STATE' property
			ALint state() const;

		public:

			Source(std::shared_ptr<impl::SourceHandle> source, std::shared_ptr<impl::SoundHandle> sound);
			Source(std::shared_ptr<impl::SourceHandle>& source);

			/**
			 * @brief Resume sound playback
			 *
			 * Resumes playback after it was paused using pause(),
			 * note that a sound cannot be resumed after it is stopped using stop().
			 */
			Source& resume();

			/**
			 * @brief Pause sound playback
			 *
			 * Pauses sound playback, it can then be resumed using resume()
			 * note that this is not the same thing as stop() that stops the sound for good.
			 */
			Source& pause();

			/**
			 * @brief Set sound gain (volume)
			 *
			 * Multiplies the innate sound volume by the given value (gain)
			 * multiplier, note that sound volume depends both on its gain and the volume of the sound file itself.
			 *
			 * @param[in] value The volume multiplier
			 */
			Source& gain(float value);

			/**
			 * @brief Set sound pitch (frequency)
			 *
			 * Multiples the innate sound pitch by the given value (pitch)
			 * multiplier, note that sound pitch depends both on its pitch and the pitch of the sound file itself.
			 *
			 * @see https://en.wikipedia.org/wiki/Pitch_(music)
			 *
			 * @param[in] value The pitch multiplier
			 */
			Source& pitch(float value);

			/**
			 * @brief Play sound in loop
			 *
			 * Allows playing the sound in an endless loop, if at some point
			 * you wish to stop the playback use pause(), stop(), or loop(false) depending on your needs.
			 *
			 * @param[in] enable Whether the sound should be played in a loop
			 */
			Source& loop(bool enable = true);

			/**
			 * @brief Stop the sound
			 *
			 * After calling this method the sound will stop, and it won't be
			 * possible to resume it. You will need to play it again. Note that sounds stop automatically when they end.
			 */
			void stop();

		public:

			/**
			 * @brief Check if the sound is playing and can be paused
			 *
			 * This will return true when the sound is not yet over, and
			 * can still be paused using pause()
			 */
			bool is_playing();

			/**
			 * @brief Check if the sound is paused and can be resumed
			 *
			 * This will return true when the sound is paused using pause() and
			 * can be resumed using resume()
			 */
			bool is_paused();

			/**
			 * @brief Check if the stopped and CAN'T be resumed
			 *
			 * This will return true when the sound finishes playing or
			 * is interrupted using stop()
			 */
			bool is_stopped();

	};

}

