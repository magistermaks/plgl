#pragma once

#include "internal.hpp"

namespace plgl {

	struct Vertex {
		float x, y, u, v;
		uint8_t r, g, b, a;

		Vertex(float x, float y, float u, float v, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	};

	class Buffer {

		private:

			constexpr static int stride = sizeof(Vertex);

			GLuint vao;
			GLuint vbo;
			std::vector<Vertex> buffer;

			void vertexAttribute(int index, int count, int stride, long offset, GLenum type, bool normalize);
			void upload();

		public:

			Buffer();
			~Buffer();

			/// Erase buffer contents
			void clear();

			/// Check if buffer is empty
			bool empty();

			/// Draw this buffer using currently enabled pipeline
			void draw();

			/// Add vertex to the buffer
			void vertex(float x, float y, float u, float v, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

			/// Add vertex to the buffer
			void vertex(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

	};

}
