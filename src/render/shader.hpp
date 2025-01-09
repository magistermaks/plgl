#pragma once

#include "external.hpp"

namespace plgl {

	class Shader {

		private:

			GLuint program;

			static GLuint compile_shader(GLenum type, const char* source);
			static GLuint link_shader(GLuint vertex, GLuint fragment);

		public:

			Shader(const char* vertex_source, const char* fragment_source);
			~Shader();

			/// Bind this OpenGL Shader
			void use();

			/// Get OpenGL uniform location
			int uniform(const char* name);

	};

}
