
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

	std::shared_ptr<impl::SoundHandle> SoundSystem::load() {

		std::shared_ptr<impl::SoundHandle> sound;

		// Try to recycle sound buffer,
		// or create new one if that can't be done
		if (!unused.empty()) {
			sound = unused.back();
			unused.pop_back();
			sounds.emplace_back(sound);
		} else {
			sound = sounds.emplace_back(std::make_shared<impl::SoundHandle>());
		}

		return sound;

	}

	std::shared_ptr<impl::SourceHandle> SoundSystem::play(const std::shared_ptr<impl::SoundHandle>& sound) {
		if (!sound) {
			return {};
		}

		sound->sources ++;
		sound_count ++;
		return sources.emplace_back(std::make_shared<impl::SourceHandle>(sound->id));
	}

	void SoundSystem::update() {
		sources.remove_if([&] (const std::shared_ptr<impl::SourceHandle>& source) {
			if (!source || source->should_remove()) {
				for (auto& sound : sounds) {
					if (sound->id == source->buffer) {
						sound->sources --;
					}
				}

				return true;
			}

			return false;
		});

		sounds.remove_if([this] (const std::shared_ptr<impl::SoundHandle>& sound) {
			if (!sound || sound->should_remove()) {
				unused.push_back(sound);
				return true;
			}

			return false;
		});

		sound_count = sources.size();
	}

	int SoundSystem::frequency() {
		int samples;
		alcGetIntegerv(device, ALC_FREQUENCY, 1, &samples);
		return samples;
	}

	void SoundSystem::unload_all() {
		stop_all();
		unused.clear();
		sounds.clear();
	}

	void SoundSystem::stop_all() {
		sound_count = 0;

		for (auto& ptr : sources) {
			if (ptr) {
				Source source {ptr};
				source.stop();
			}
		}
	}

	void SoundSystem::pause_all() {
		for (auto& ptr : sources) {
			if (ptr) {
				Source source {ptr};
				source.pause();
			}
		}
	}

	void SoundSystem::resume_all() {
		for (auto& ptr : sources) {
			if (ptr) {
				Source source {ptr};
				source.resume();
			}
		}
	}

	Source SoundSystem::tone(Waveform waveform, float volume, float frequency, int milliseconds) {
		Sound sound {[frequency, waveform] (std::vector<short>& buffer, int sps) {

			// number of samples per period
			const int period = sps / frequency;
			const int amplitude = 8000;

			if (waveform == SQUARE_WAVE) generate_square_wave(buffer, period, amplitude);
			if (waveform == TRIANGLE_WAVE) generate_triangle_wave(buffer, period, amplitude);
			if (waveform == SAWTOOTH_WAVE) generate_sawtooth_wave(buffer, period, amplitude);
			if (waveform == SINE_WAVE) generate_sine_wave(buffer, period, amplitude);

		}, milliseconds};

		sound.discard();
		return sound.play(volume);
	}

}
