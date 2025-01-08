
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

			uniform sampler2D sampler;

			in vec4 vColor;
			in vec2 vTex;

			out vec4 fColor;

			void main(){
				fColor = texture(sampler, vTex).rgba * vColor;
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

			uniform sampler2D sampler;

			in vec4 vColor;
			in vec2 vTex;

			out vec4 fColor;

			float median(float r, float g, float b) {
				return max(min(r, g), min(max(r, g), b));
			}

			float screenPxRange() {
				// simple - 7.5
				// aqmol2 - 1.8
				return 7.5f;
			}

			void main() {
				vec3 msd = texture(sampler, vTex).rgb;
				float sd = median(msd.r, msd.g, msd.b);
				float screenPxDistance = screenPxRange()*(sd - 0.5);
				float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
				fColor = vec4(vColor.rgb, vColor.a * opacity);
			}

		)";

		static Shader shader {vertex, fragment};
		return shader;
	}

	Pipeline::Pipeline(Shader& shader, Texture* texture)
	: buffer({}), shader(shader), texture(texture) {
		if (texture) {
			glUniform1i(shader.uniform("sampler"), 0);
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