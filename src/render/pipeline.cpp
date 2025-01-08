
#include "pipeline.hpp"

namespace plgl {

	/*
	 * Pipeline
	 */

	Shader& Pipeline::getColorShader() {
		static const char* vertex = R"(
			#version 330 core

			layout (location = 0) in vec2 iPos;
			layout (location = 2) in vec4 iColor;

			out vec4 vColor;

			void main(){
				gl_Position = vec4(iPos.xy, -1.0, 1.0);
				vColor = iColor;
			}
		)";

		static const char* fragment = R"(
			#version 330 core

			in vec4 vColor;

			out vec4 fColor;

			void main(){
				fColor = vColor;
			}
		)";

		static Shader shader {vertex, fragment};
		return shader;
	}

	Shader& Pipeline::getImageShader() {
		static const char* vertex = R"(
			#version 330 core

			layout (location = 0) in vec2 iPos;
			layout (location = 1) in vec2 iTex;
			layout (location = 2) in vec4 iColor;

			out vec4 vColor;
			out vec2 vTex;

			void main(){
				gl_Position = vec4(iPos.xy, -1.0, 1.0);
				vColor = iColor;
				vTex = iTex;
			}
		)";

		static const char* fragment = R"(
			#version 330 core

			uniform sampler2D uSampler;

			in vec4 vColor;
			in vec2 vTex;

			out vec4 fColor;

			void main(){
				fColor = texture(uSampler, vTex).rgba * vColor;
			}
		)";

		static Shader shader {vertex, fragment};
		return shader;
	}

	Shader& Pipeline::getFontShader() {
		static const char* vertex = R"(
			#version 330 core

			layout (location = 0) in vec2 iPos;
			layout (location = 1) in vec2 iTex;
			layout (location = 2) in vec4 iColor;

			out vec4 vColor;
			out vec2 vTex;

			void main(){
				gl_Position = vec4(iPos.xy, -1.0, 1.0);
				vColor = iColor;
				vTex = iTex;
			}
		)";

		static const char* fragment = R"(
			#version 330 core

			uniform sampler2D uSampler;

			in vec4 vColor;
			in vec2 vTex;

			out vec4 fColor;

			float median(float r, float g, float b) {
				return max(min(r, g), min(max(r, g), b));
			}


			// font_size / 64 * 6
			float range() {
				// this, i think, is both incorrect and unadvised, but it works for now
				vec2 unitRange = vec2(6)/vec2(textureSize(uSampler, 0));
				vec2 screenTexSize = vec2(1.0)/fwidth(vTex);
				return max(0.5*dot(unitRange, screenTexSize), 1.0);
			}

			void main() {
				vec3 fields = texture(uSampler, vTex).rgb;
				float distance = median(fields.x, fields.y, fields.z);
				float screen = range() * (distance - 0.5);
				float opacity = clamp(screen + 0.5, 0.0, 1.0);
				fColor = vec4(vColor.rgb, vColor.a * opacity);
			}

		)";

		static Shader shader {vertex, fragment};
		return shader;
	}

	Pipeline::Pipeline(Shader& shader, Texture* texture)
	: buffer({}), shader(shader), texture(texture) {
		shader.use();

		if (texture) {
			glUniform1i(shader.uniform("uSampler"), 0);
		}
	}

	void Pipeline::draw() {
		if (texture) {
			texture->use();
		}

		shader.use();
		buffer.draw();
	}

	void Pipeline::flush() {
		if (!buffer.empty()) {
			draw();
			buffer.clear();
		}
	}

}