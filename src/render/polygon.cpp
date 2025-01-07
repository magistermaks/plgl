
#include "external.hpp"
#include "polygon.hpp"

namespace plgl {

	void polygon(PolygonMode mode) {
		// TODO this should be moved into the renderer and
		//      and trigger a pipeline flush
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}

}