
#include "external.hpp"
#include "polygon.hpp"

namespace plgl {

	void polygon(PolygonMode mode) {
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}

}