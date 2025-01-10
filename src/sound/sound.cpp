
#include "sound.hpp"
#include "debug.hpp"
#include "source.hpp"
#include "system.hpp"
#include "util.hpp"

namespace plgl {

	/*
	 * Sound
	 */

	Sound::Sound(const std::string& path) {

		SoundSystem* system = SoundSystem::get();
		auto sound = system->load();
		this->handle = sound;

		int channels;
		int samples;
		short* data;

		long count = stb_vorbis_decode_filename(path.c_str(), &channels, &samples, &data);

		if (count == -1) {
			fault("Failed to load sound: '{}'!", path);
		}

		ALenum format = (channels > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		int length = count * channels * sizeof(uint16_t);

		alBufferData(sound->id, format, data, length, samples);
		impl::alCheckError("alBufferData");

		// the documentation for STB vorbis is non-existent
		// i assume this is what should be done, other STB libraries typically had a
		// custom free function but for this one i couldn't find any
		free(data);

	}

	Sound::Sound(const WaveformGenerator& generator, int milliseconds) {

		SoundSystem* system = SoundSystem::get();
		auto sound = system->load();
		this->handle = sound;

		const long frequency = system->frequency();
		const long per_ms = frequency / 1000;
		const long samples = per_ms * milliseconds;

		// Prepare buffer
		std::vector<short> buffer;
		buffer.resize(samples);
		generator(buffer, frequency);

		alBufferData(sound->id, AL_FORMAT_MONO16, buffer.data(), buffer.size() * sizeof(uint16_t), frequency);
		impl::alCheckError("alBufferData");

	}

	void Sound::discard() {
		if (std::shared_ptr<impl::SoundHandle> sound = handle.lock()) {
			sound->keep = false;
		}
	}

	Source Sound::play(float volume, float pitch) {
		return Source {SoundSystem::get()->play(handle.lock())}.gain(volume).pitch(pitch).resume();
	}

}