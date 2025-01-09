
#include "shader.hpp"
#include "util.hpp"

namespace plgl {

	GLuint Shader::compile_shader(GLenum type, const char* source) {
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

			fault("Shader compilation failed with error: {}", error.data());
		}

		return shader;
	}

	GLuint Shader::link_shader(GLuint vertex, GLuint fragment) {

		// create shader program
		GLuint program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);

		// error checking
		GLint linked = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);

		if(linked == GL_FALSE) {
			GLint length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

			std::vector<char> error (length);
			glGetProgramInfoLog(program, length, &length, error.data());

			fault("Shader linking failed with error: {}", error.data());
		}

		return program;

	}

	Shader::Shader(const char* vertex_source, const char* fragment_source) {
		GLuint vert = compile_shader(GL_VERTEX_SHADER, vertex_source);
		GLuint frag = compile_shader(GL_FRAGMENT_SHADER, fragment_source);
		program = link_shader(vert, frag);

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
