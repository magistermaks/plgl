
#include "context.hpp"

using namespace plgl;

int main() {

	open("Processing-Like Graphics Library", 400, 300);

	Atlas atlas;
	Sprite grass = atlas.submitFile("assets/grass.png");
	Sprite dirt = atlas.submitFile("assets/dirt.png");
	Sprite plant = atlas.submitFile("assets/plant.png");

	atlas.upload();
	atlas.save("atlas.png");
	atlas.freeze();

	while(!should_close) {

		background(0, 0, 0);

		// draw stars
		fill(130, 130, 150);
		stroke(30, 30, 50);
		weight(3);

		circle(60, 30, 5 + 1.2 * sin(0.134 + frame_count / 40.f));
		circle(200, 60, 4 + 1 * sin(0.678 + frame_count / 50.f));
		circle(330, 43, 3 + 1 * sin(0.901 + frame_count / 87.f));

		// draw terrain
		for (int x = 0; x < 400; x += 50) {

			random_seed(x);

			int h = uniform_random(0, 2);

			if ((int) uniform_random(0, 2)) {
				texture(plant);
				image(x, 150 + h * 50, 50, 50);
			}

			texture(grass);
			image(x, 200 + h * 50, 50, 50);

			texture(dirt);

			for (int y = 300; y > (200 + h * 50); y -= 50) {
				image(x, y, 50, 50);
			}

		}

		swap();
	}

	atlas.close();
	close();

}
