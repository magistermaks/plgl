
#include "shader.hpp"

namespace plgl {

	GLuint Shader::compile(GLenum type, const char* source) {
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);

		// error checking
		GLint compiled = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (compiled == GL_FALSE) {
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			std::vector<char> error (length);
			glGetShaderInfoLog(shader, length, &length, error.data());

			throw std::runtime_error {std::string ("Shader compilation failed with error: ") + error.data()};
		}

		return shader;
	}

	Shader::Shader(const char* vertex_source, const char* fragment_source) {
		GLuint vert = compile(GL_VERTEX_SHADER, vertex_source);
		GLuint frag = compile(GL_FRAGMENT_SHADER, fragment_source);

		program = glCreateProgram();
		glAttachShader(program, vert);
		glAttachShader(program, frag);
		glLinkProgram(program);

		glDeleteShader(vert);
		glDeleteShader(frag);
	}

	Shader::~Shader() {
		glDeleteProgram(program);
	}

	void Shader::use() {
		glUseProgram(program);
	}

	int Shader::uniform(const char* name) {
		return glGetUniformLocation(program, name);
	}

}
