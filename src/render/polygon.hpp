
namespace plgl {

	enum PolygonMode {
		FILL = GL_FILL,
		LINES = GL_LINE,
		POINTS = GL_POINT
	};

	/**
	 * Changes the way geometry is drawn to the screen
	 */
	void polygon(PolygonMode mode);

}
