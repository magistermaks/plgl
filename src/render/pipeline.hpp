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
			Texture* texture;

			Pipeline(Shader& shader, Texture* texture);

		public:

			void draw();
			void flush();

	};

}
