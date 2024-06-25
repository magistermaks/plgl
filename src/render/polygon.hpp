
namespace plgl {

	enum PolygonMode {
		FILL = GL_FILL,
		LINES = GL_LINE,
		POINTS = GL_POINT
	};

	void polygon(PolygonMode mode) {
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}

}
