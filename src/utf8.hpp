#pragma once

#include "external.hpp"

namespace plgl {

	/**
	 * @brief Read unicode from string
	 *
	 * Reads between 1 and 4 bytes from the given utf-8 string and returns the corresponding unicode
	 * codepoint as 32 bit integer. The Index into the string is read from, and saved to, given offset pointer.
	 *
	 * @param[in]     cstr    UTF-8 encoded c-string
	 * @param[in,out] offset  Pointer to an offset into the string
	 *
	 * @return Unicode codepoint as 32 bit integer
	 */
	int next_unicode(const char* cstr, int* offset);

}