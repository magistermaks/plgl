
#include "system.hpp"
#include "util.hpp"
#include "debug.hpp"
#include "globals.hpp"

namespace plgl {

	SoundSystem* SoundSystem::get() {
		if (plgl::sound_system) {
			return plgl::sound_system;
		}

		fault("Can't use sound system when it's not loaded!");
	}

	SoundSystem::SoundSystem() {
		const char* name = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
		this->device = alcOpenDevice(name);

		if (device == nullptr) {
			fault("Sound system failed to start, unable to open device '{}'!", name);
		}

		this->context = alcCreateContext(device, nullptr);

		if (context == nullptr) {
			fault("Sound system failed to start, unable to create context!");
		}

		alcMakeContextCurrent(context);
		impl::alCheckError("alcMakeContextCurrent");
	}

	SoundSystem::~SoundSystem() {
		unload_all();

		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	std::weak_ptr<impl::SoundHandle> SoundSystem::load(const std::string& path) {

		// Create new buffer
		auto& sound = sounds.emplace_back(std::make_shared<impl::SoundHandle>());

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

		return {sound};

	}

	std::weak_ptr<impl::SourceHandle> SoundSystem::play() {
		sound_count ++;
		return {sources.emplace_back(std::make_shared<impl::SourceHandle>())};
	}

	void SoundSystem::update() {
		sources.remove_if([] (const std::shared_ptr<impl::SourceHandle>& source) {
			ALenum state = AL_STOPPED;
			alGetSourcei(source->id, AL_SOURCE_STATE, &state);
			return state == AL_STOPPED;
		});

		sound_count = sources.size();
	}

	void SoundSystem::unload_all() {
		stop_all();
		sounds.clear();
	}

	void SoundSystem::stop_all() {
		sound_count = 0;
		sources.clear();
	}

	void SoundSystem::pause_all() {
		for (auto& shared_handle : sources) {
			Source source {shared_handle};
			source.pause();
		}
	}

	void SoundSystem::resume_all() {
		for (auto& shared_handle : sources) {
			Source source {shared_handle};
			source.resume();
		}
	}

}
