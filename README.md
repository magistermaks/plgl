## PLGL
Processing-Like Graphics Library  

A re-implementation of [Processing](https://processing.org/) in C++ using OpenGL.  
This project is not, and does not aim to be, a 1-to-1 or API compatible port,  
both do to the language differences between Java and C++, as well as divergent design decisions.  

#### Running
* Use `./compile --run main` to run the "main" example
* Use `./compile --list` to list avaible example programs
* Use `./compile --help` for aditional info

#### Example
**Processing**  
```Processing
void setup() {
	size(100, 100);
	windowTitle("Hello!");
}

void draw() {

	// clear screen
	background(200);

	// draw static red ring
	noFill();
	stroke(200, 0, 0);
	strokeWeight(10);
	circle(50, 50, 50);

	// draw moving blue dot
	fill(0, 0, 255);
	noStroke();
	circle(mouseX, mouseY, 10);

}
```

**PLGL**  
```C++
#include "context.hpp"

using namespace plgl;

int main() {
	open(100, 100, "Hello!");

	// set background color
	background(200, 200, 200);

	while(!should_close) {

		// draw static red ring
		fill(OFF);
		stroke(200, 0, 0);
		weight(10);
		circle(50, 50, 50);
		
		// draw moving blue dot
		fill(0, 0, 255);
		stroke(OFF);
		circle(mouse_x, mouse_y, 10);

		// end frame
		swap();

	}

	close();
}
```
