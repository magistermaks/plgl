#pragma once

#include "internal.hpp"

namespace plgl {

	struct Vertex {
		float x, y, u, v, r, g, b, a;

		Vertex(float x, float y, float u, float v, float r, float g, float b, float a)
		: x(x), y(y), u(u), v(v), r(r), g(g), b(b), a(a) {}
	};

	class Buffer {

		private:

			constexpr static int stride = 2+4+2;

			GLuint vao;
			GLuint vbo;
			std::vector<Vertex> buffer;

			void vertexAttribute(int index, int count, int stride, long offset) {
				glVertexAttribPointer(index, count, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*) (offset * sizeof(float)));
				glEnableVertexAttribArray(index);
			}

			void upload() {
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(Vertex), buffer.data(), GL_DYNAMIC_DRAW);
			}

		public:

			Buffer() {
				// create and bind VAO
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);

				// create and fill VBO
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);

				// configure VAO
				vertexAttribute(0, 2, stride, 0); // vec2: xy
				vertexAttribute(1, 2, stride, 2); // vec2: uv
				vertexAttribute(2, 4, stride, 4); // vec4: rgba

				// cleanup global state
				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			~Buffer() {
				glDeleteVertexArrays(1, &vao);
				glDeleteBuffers(1, &vbo);
			}

			void clear() {
				buffer.clear();
			}

			bool empty() {
				return buffer.empty();
			}

			void draw() {
				upload();
				glBindVertexArray(vao);
				glDrawArrays(GL_TRIANGLES, 0, buffer.size());
			}

			void vertex(float x, float y, float u, float v, float r, float g, float b, float a = 1) {
				buffer.emplace_back(
					plgl::impl::remapx(x),
					plgl::impl::remapy(y),
					u, v, r, g, b, a
				);
			}

			void vertex(float x, float y, float r, float g, float b, float a = 1) {
				vertex(x, y, 0, 0, r, g, b, a);
			}

	};

}
