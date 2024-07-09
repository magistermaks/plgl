#pragma once

#include "external.hpp"

namespace plgl {

	class Random {

		private:

			std::mt19937 random;

		public:

			inline Random(std::random_device& source)
			: random(source()) {}

			inline Random(unsigned int seed)
			: random(seed) {}

		public:

			inline void seed(unsigned int seed) {
				random.seed(seed);
			}

			inline float uniform(float min, float max) {
				std::uniform_real_distribution distribution {min, max};
				return distribution(this->random);
			}

			inline float gaussian(float mean, float deviation) {
				std::normal_distribution distribution {mean, deviation};
				return distribution(this->random);
			}

	};

	namespace impl {

		extern std::random_device random_device;
    	extern Random random;
		extern siv::PerlinNoise perlin;
		extern unsigned int octaves;

	}

	/// sets the random generator seed
	inline void random_seed(unsigned int seed) {
		impl::random.seed(seed);
	}

	/// returns a uniformly distributed random float in range [min, max)
	inline float uniform_random(float min, float max) {
		return impl::random.uniform(min, max);
	}

	/// returns a uniformly distributed random float in range [0, max)
	inline float uniform_random(float max) {
		return impl::random.uniform(0, max);
	}

	/// returns a normally (gaussian) distributed float around mean, with specificed deviation
	inline float gaussian_random(float mean, float deviation) {
		return impl::random.gaussian(mean, deviation);
	}

	/// sets the noise generator seed
	inline void noise_seed(unsigned int seed) {
		impl::perlin.reseed(seed);
	}

	/// sets the number of noise layers
	inline void noise_octaves(unsigned int octaves) {
		impl::octaves = std::max(1u, octaves);
	}

	/// query the noise value [0, 1] at the given 1D point
	inline float noise(float x) {
		return impl::perlin.octave1D_01(x, impl::octaves);
	}

	/// query the noise value [0, 1] at the given 2D point
	inline float noise(float x, float y) {
		return impl::perlin.octave2D_01(x, y, impl::octaves);
	}

	/// query the noise value [0, 1] at the given 3D point
	inline float noise(float x, float y, float z) {
		return impl::perlin.octave3D_01(x, y, z, impl::octaves);
	}

};
