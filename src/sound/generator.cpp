
#include "generator.hpp"
#include "math.hpp"

namespace plgl {

	void generate_square_wave(std::vector<short>& buffer, int period, int amplitude) {
		const int half = period / 2;

		for (int i = 0; i < buffer.size(); i++) {
			int duty = i % period;
			short value = amplitude;

			if (duty < half) {
				value = -amplitude;
			}

			buffer[i] = value;
		}
	}

	void generate_triangle_wave(std::vector<short>& buffer, int period, int amplitude) {
		for (int i = 0; i < buffer.size(); i++) {
			float x = i;

			float xq = x / period;
			float value = std::abs((xq - std::floor(xq + 0.5f)) * 2) - 0.5;

			buffer[i] = value * amplitude;
		}
	}

	void generate_sawtooth_wave(std::vector<short>& buffer, int period, int amplitude) {
		for (int i = 0; i < buffer.size(); i++) {
			float x = i;

			float xq = x / period;
			float value = (xq - std::floor(xq + 0.5f)) * 2;

			buffer[i] = value * amplitude;
		}
	}

	void generate_sine_wave(std::vector<short>& buffer, int period, int amplitude) {
		float offset = PI * 3.0f * 0.5f;

		for (int i = 0; i < buffer.size(); i++) {
			float x = i;
			buffer[i] = sin(x / period * TAU + offset) * amplitude;
		}
	}

}