#pragma once

#include "event.hpp"
#include "internal.hpp"
#include "color.hpp"
#include "render/renderer.hpp"

namespace plgl {

	/**
	 * @brief Open window.
	 *
	 * This needs to be called before any drawing can start,
	 * typically should be somewhere near the start of your program.
	 *
	 * @note One application can only have one window, invoking open()
	 *       multiple times will result in an error.
	 *
	 * @example
	 * @code{.cpp}
	 * int main() {
	 *
	 *    // open the application window
	 *    open("My PLGL Application", 400, 300);
	 *
	 *    // run until the user tries to close the window
	 *    while (!should_close) {
	 *        // put your drawing code here
	 *    }
	 *
	 *    // when the program is about to close call close()
	 *    close();
	 *
	 * }
	 * @endcode
	 *
	 * @see plgl::close()
	 *
	 * @param[in] title   Title of the window to be shown on the bar
	 * @param[in] width   Window width, in pixels
	 * @param[in] height  Window height, in pixels
	 */
	void open(const std::string& title, int width, int height);

	/**
	 * @brief Open window without title.
	 *
	 * A shorthand for opening windows without a given title, see
	 * the linked function for full documentation.
	 *
	 * @note One application can only have one window, invoking open()
	 *       multiple times will result in an error.
	 *
	 * @see plgl::open(const std::string&, int, int)
	 *
	 * @param[in] width   Window width, in pixels
	 * @param[in] height  Window height, in pixels
	 */
	void open(int width, int height);

	/**
	 * @brief Open window without size.
	 *
	 * A shorthand for opening windows without a given size, see
	 * the linked function for full documentation.
	 *
	 * @note One application can only have one window, invoking open()
	 *       multiple times will result in an error.
	 *
	 * @see plgl::open(const std::string&, int, int)
	 *
	 * @param[in] title  Title of the window to be shown on the bar
	 */
	void open(const std::string& title);

	/**
	 * @brief Open some window.
	 *
	 * A shorthand for opening windows without a given size or title, see
	 * the linked function for full documentation.
	 *
	 * @note One application can only have one window, invoking open()
	 *       multiple times will result in an error.
	 *
	 * @see plgl::open(const std::string&, int, int)
	 */
	void open();

	/**
	 * @brief Sets the listener for a particular event type.
	 *
	 * @example
	 * @code{.cpp}
	 * int main() {
	 *
	 *     open();
	 *
	 *     listen(WINDOW_CLOSE, [&] () {
	 *         // the used click the 'X' on the window bar
	 *     });
	 *
	 *     listen(MOUSE_RELEASED, [&] () {
	 *         // mouse was released
	 *     });
	 *
	 *     // ...
	 *
	 * }
	 * @endcode
	 *
	 * @see plgl::Event
	 *
	 * @param[in] event     The event to listen for
	 * @param[in] callback  The code to run when the event happens
	 */
	void listen(Event event, EventHandler callback);

	/**
	 * @brief Close currently open window.
	 *
	 * Must be called after a successful open(). After close has been
	 * called another window may be opened again using open(), as long as there
	 * isn't more then one open window at a time.
	 *
	 * @example
	 * @code{.cpp}
	 * int main() {
	 *
	 *    // open the application window
	 *    open("My PLGL Application", 400, 300);
	 *
	 *    // run until the user tries to close the window
	 *    while (!should_close) {
	 *        // put your drawing code here
	 *    }
	 *
	 *    // call close() to make the window close
	 *    close();
	 *
	 *    // here another open() can be called
	 *
	 * }
	 * @endcode
	 *
	 * @see plgl::open(const std::string&, int, int)
	 */
	void close();

	/**
	 * @brief Change window title.
	 *
	 * Changes the title bar string (window title) of
	 * the currently open window to the given string.
	 * Window must be already open when this function is called.
	 *
	 * @see plgl::open(const std::string&, int, int)
	 *
	 * @param[in] title The new title to set
	 */
	void title(const std::string& title);

	/**
	 * @brief Set background color.
	 *
	 * Contrary to how a similar function in Processing works
	 * this doesn't actually clear the screen, just sets the clear color.
	 * Screen is always empty when a rendering of a new frame begins,
	 * that is, when you invoke a first draw command after swap() the whole screen
	 * is at that point filled with the clear color.
	 *
	 * @note If you want the background color to be the same each frame,
	 *       you can call this once before rendering starts (for example after plgl::open())
	 *
	 * @param[in] r  The red component of the color, between 0 and 255
	 * @param[in] g  The green component of the color, between 0 and 255
	 * @param[in] b  The blue component of the color, between 0 and 255
	 */
	void background(float r, float g, float b);

	/**
	 * @brief Set background color.
	 *
	 * Contrary to how a similar function in Processing works
	 * this doesn't actually clear the screen, just sets the clear color.
	 * Screen is always empty when a rendering of a new frame begins,
	 * that is, when you invoke a first draw command after swap() the whole screen
	 * is at that point filled with the clear color.
	 *
	 * @note If you want the background color to be the same each frame,
	 *       you can call this once before rendering starts (for example after plgl::open())
	 *
	 * @param[in] color  The clear color to use
	 */
	void background(const Color& color);

	/**
	 * @brief End frame rendering
	 *
	 * Submits the currently drawn frame to the display,
	 * clears the canvas, and processes input events. Must be called
	 * in the rendering loop.
	 *
	 * @example
	 * @code{.cpp}
	 * int main() {
	 *
	 *    open("My PLGL Application", 400, 300);
	 *
	 *    while (!should_close) {
	 *        // put your drawing code here
	 *
	 *        // end of frame
	 *        swap();
	 *    }
	 *
	 *    close();
	 *
	 * }
	 * @endcode
	 *
	 * @see plgl::background(float, float, float)
	 */
	void swap();

	/**
	 * @brief Do nothing and wait for user to close the window
	 *
	 * Doesn't clear the screen and waits for user to close the window
	 * using the close button. Can be useful if you want to draw a single image.
	 * Don't use this if you intend to draw continuously. You should still call plgl::close(),
	 * after this method returns.
	 *
	 * @see plgl::close()
	 */
	void window_pause();

	/**
	 * @brief Forces the mouse cursor to stay within the bounds of the window.
	 *
	 * Makes the cursor unable to move
	 * outside the window bounds, as if the window border was
	 * the edge of the screen.
	 *
	 * @see plgl::cursor_hide(bool)
	 *
	 * @param[in] capture Whether the mouse should stay in the window bounds.
	 */
	void cursor_capture(bool capture);

	/**
	 * @brief Makes the cursor invisible while it is over the application window.
	 *
	 * Makes mouse cursor not appear as long as it's
	 * placed over the application window.
	 *
	 * @see plgl::cursor_capture(bool)
	 *
	 * @param[in] hidden Whether the mouse should be invisible while in the window bounds.
	 */
	void cursor_hide(bool hidden);

}
