
#include "external.hpp"

namespace plgl::impl {

	/**
	 * Return last OpenAL error, or nullptr if there was no error
	 */
	const char* alStringError();

	/**
	 * Check if there was any OpenAL error, throw if there was
	 */
	void alCheckError(const char* usage);

}