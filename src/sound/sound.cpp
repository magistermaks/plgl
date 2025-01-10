
#include "sound.hpp"
#include "debug.hpp"
#include "util.hpp"
#include "source.hpp"
#include "globals.hpp"
#include "system.hpp"

namespace plgl {

	ALenum Sound::format(uint32_t channels) {
		return (channels > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	}

	Sound::Sound(const std::string& path) {

		alGenBuffers(1, &al_buffer);
		impl::alCheckError("alGenBuffers");

		int channels;
		int samples;
		short* data;

		long count = stb_vorbis_decode_filename(path.c_str(), &channels, &samples, &data);

		if (count == -1) {
			fault("Failed to load sound: '{}'!", path);
		}

		int length = count * channels * sizeof(short);
		alBufferData(al_buffer, format(channels), data, length, samples);
		impl::alCheckError("alBufferData");

		// the documentation for STB vorbis is non-existent
		// i assume this is what should be done, other STB libraries typically had a
		// custom free function but for this one i couldn't find any
		free(data);
	}

	Sound::~Sound() {
		alDeleteBuffers(1, &al_buffer);
	}

	ALuint Sound::handle() const {
		return al_buffer;
	}

	Source& Sound::play() const {
		return plgl::sound_system->play(*this);
	}

}