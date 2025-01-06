#pragma once

#include "external.hpp"

namespace plgl {

	class Shader {

		private:

			GLuint program;
			GLuint compile(GLenum type, const char* source);

		public:

			Shader(const char* vertex_source, const char* fragment_source);
			~Shader();

			/// Bind this OpenGL Shader
			void use();

			/// Get OpenGL uniform location
			int uniform(const char* name);

	};

}
