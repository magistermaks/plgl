#pragma once

#include "external.hpp"

namespace plgl {

	/**
	 * Sound waveform generator meant for writing into WaveWriter
	 */
	using WaveformGenerator = std::function<void(std::vector<short>& buffer, int frequency)>;

	/**
	 * Waveform shape
	 */
	enum Waveform {
		SQUARE_WAVE,
		TRIANGLE_WAVE,
		SAWTOOTH_WAVE,
		SINE_WAVE,
	};

	/// Generate square wave into the given buffer
	void generate_square_wave(std::vector<short>& buffer, int period, int amplitude);

	/// Generate triangle wave into the given buffer
	void generate_triangle_wave(std::vector<short>& buffer, int period, int amplitude);

	/// Generate sawtooth wave into the given buffer
	void generate_sawtooth_wave(std::vector<short>& buffer, int period, int amplitude);

	/// Generate sine wave into the given buffer
	void generate_sine_wave(std::vector<short>& buffer, int period, int amplitude);

}

