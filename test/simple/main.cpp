
#include "context.hpp"

using namespace plgl;

std::vector<const char*> welcome = {
	"Hello World!",
	"Cześć Świecie!",
	"¡Hola Mundo!",
	"Hallo Welt!",
	"Привіт, світ!",
	"Ahoj světe!",
	"Bonjour!",
	"Hej världen!",
	"Hallo wereld!",
	"Ciao mondo!",
	"Γεια σου Κόσμο!",
	"Halló Heimur!",
	"Hei maailma!",
	"123456789"
};

int main() {

	open("Processing-Like Graphics Library", 400, 300);

	bool clip_mode = false;
	Texture cat {"assets/cat.png"};
	Font sans {"default"};
	Sound coin {"assets/coin.ogg"};

	listen(WINDOW_CLOSE, [] () {
		printf("Goodbye!\n");
	});

	listen(MOUSE_RELEASED, [&] () {
		coin.play();
		clip_mode = !clip_mode;
	});

	HSLA blue {rad(200), 1.0f, 0.8f};
	RGBA bg = blue.rgba();
	printf("Background: 0x%x, HSLA=%s, RGBA=%s\n", bg.pack_argb(), blue.str().c_str(), bg.str().c_str());

	if (!RGBA(100, 100, 100).hsla().achromatic()) {
		printf("Oof!\n");
	}

	cursor_hide(true);
	cursor_capture(false);

	while(!should_close) {

		if (focused) background(bg); else background(HSLA(rad(210), 0.8f, 0.75f));
		if (clip_mode) { clip(mouse_x - 100, mouse_y - 100, mouse_x + 100, mouse_y + 100); polygon(LINES); }

		fill(100, 100, 100);
		stroke(10, 150, 120);
		weight(2);
		point(10, 10);

		weight(5);
		ellipse(100, 50, 10, 30);
		circle(200, 70, noise(frame_count * 0.1f) * 8 + 8);
		stroke(200, 0, 0, 200);
		line(100, 50, 200, 70);

		float ox = 140;
		stroke(50, 0, 200);
		bezier(170 + ox, 40, 20 + ox, 20, 180 + ox, 180, 30 + ox, 160);

		stroke(200, 0, 0, 200);
		fill(OFF);
		circle(200, 150, 60);

		texture(cat);
		tint(255, 200, 0);
		image(80, 280);
		image(160, 280, 30, 30);

		tint(255, 255, 255);
		texture(cat, 20, 20, 60, 60);
		image(200, 280, 30, 30);

		// pick different hello message every 3s
		int idx = (frame_count / 60 / 3) % welcome.size();

		tint(255, 70, 70);
		font(sans);
		size(80);
		text(10, height - 40, welcome[idx]);

		size(30);
		textf(10, height - 10, "Quick {} jumps over the {}", "brown fox", "lazy dog!", "This one will not be shown!");

		stroke(100, 100, 0, 100);
		fill(0, 0, 200, 100);
		weight(10);

		quad(100, 25, 25, 100, 50, 125, 100, 100);
		trig(200, 50, 50, 200, 200, 200);
		square(250, 250, 30);
		rect(260, 100, 100, 80, 10, 20, 40, 0);

		stroke(250, 80, 80, 200);
		arc(320, 160, 25, 25, rad(((int) (1.4f * frame_count) % 360)), TAU / 3 + PI * sin(frame_count * 0.01), OPEN_PIE);

		fill(39, 114, 198);
		stroke(OFF);
		circle(mouse_x, mouse_y, 5);

		clip(OFF);
		polygon(FILL);
		swap();
	}

	cat.close();
	close();

}
