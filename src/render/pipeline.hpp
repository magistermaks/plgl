#pragma once

#include "buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "polygon.hpp"

namespace plgl {

	class Pipeline {

		public:

			static Shader& getColorShader();
			static Shader& getImageShader();
			static Shader& getFontShader();

		public:

			Buffer buffer;
			Shader& shader;
			PixelBuffer* texture;

			Pipeline(Shader& shader, PixelBuffer* texture);

		public:

			void draw();
			void flush();

	};

}
