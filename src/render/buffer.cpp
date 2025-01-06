
#include "buffer.hpp"

namespace plgl {

	/*
	 * Vertex
	 */

	Vertex::Vertex(float x, float y, float u, float v, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	: x(x), y(y), u(u), v(v), r(r), g(g), b(b), a(a) {}

	/*
	 * Buffer
	 */

	void Buffer::vertexAttribute(int index, int count, int stride, long offset, GLenum type, bool normalize) {
		glVertexAttribPointer(index, count, type, normalize, stride, (void*) offset);
		glEnableVertexAttribArray(index);
	}

	void Buffer::upload() {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(Vertex), buffer.data(), GL_DYNAMIC_DRAW);
	}

	Buffer::Buffer() {
		// create and bind VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// create and fill VBO
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// configure VAO
		vertexAttribute(0, 2, stride, 0 * sizeof(float), GL_FLOAT, false); // vec2: xy
		vertexAttribute(1, 2, stride, 2 * sizeof(float), GL_FLOAT, false); // vec2: uv
		vertexAttribute(2, 4, stride, 4 * sizeof(float), GL_UNSIGNED_BYTE, true); // vec4: rgba

		// cleanup global state
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	Buffer::~Buffer() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

	void Buffer::clear() {
		buffer.clear();
	}

	bool Buffer::empty() {
		return buffer.empty();
	}

	void Buffer::draw() {
		upload();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, buffer.size());
	}

	void Buffer::vertex(float x, float y, float u, float v, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		buffer.emplace_back(
			plgl::impl::remapx(x),
			plgl::impl::remapy(y),
			u, v, r, g, b, a
		);
	}

	void Buffer::vertex(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		vertex(x, y, 0, 0, r, g, b, a);
	}

}