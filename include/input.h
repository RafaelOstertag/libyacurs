// -*- mode: c++ -*-
//
// $Id$

#ifndef INPUT_H
#define INPUT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include "widget.h"

/**
 * Input line.
 *
 * Widget providing one line for text input.
 *
 * If not length is specified, it behaves like a dynamic
 * Widget. Providing a length make it behave non-dynamic.
 *
 * Following cursor motion keys are supported:
 *
 *  - Key left: move cursor to the left
 *  - Key right: move cursor to the right
 *  - Ctrl-A: move to the beginning of line
 *  - Ctrl-E: move to the end of line
 *
 * Following editing keys are supported:
 *
 *  - Ctrl-U: delete all text
 *  - Ctrl-K: delete text from current cursor position to end of text.
 *  - Backspace: delete character left to the cursor
 *  - Delete: delete character right to the cursor
 *
 * Misc keys:
 *
 *  - Key Down, Enter, Tab: focus next Widget.
 *  - Key Up, Shift-Tab: focus previous Widget.
 *
 * When Input is put in read-only mode, all editing keys keys are
 * disabled. Only cursor motion is available.
 */
class Input: public Widget {
    private:
	/**
	 * Focus indication.
	 */
	bool __focus;

	/**
	 * Offset into the buffer. Used when not the entire buffer can
	 * be displayed in the Input Widget, i.e. scrolling.
	 */
	std::string::size_type __offset;

	/**
	 * Cursor position relative to offset, absolute to Widget.
	 */
	std::string::size_type __curs_pos;

	/**
	 * Maximum size of input.
	 *
	 * If it is 0, there is no limit.
	 */
	std::string::size_type __max_size;

	/**
	 * Length of the input line.
	 *
	 * May be zero when dynamically sized.
	 */
	int __length;

	/**
	 * Mode, whether or not editing is allowed.
	 */
	bool __read_only;

	/**
	 * The input
	 */
	std::string __buffer;

	/**
	 * The size of the Input Widget.
	 *
	 * May be Size::zero() when dynamically sized, else the
	 */
	Size __size;

    protected:
	virtual void key_handler(Event& _e);

	// From Realizeable
	void realize();
	void unrealize();

    public:
	/**
	 * @param _length the length (columns) the widget requires. If
	 * zero, it will dynamically size. Default 0.
	 *
	 * @param _max_size maximum size of input. Default 255.
	 *
	 * @param _t text to display initially. Default empty.
	 */
	Input(int _length=0, std::string::size_type _max_size=255, const std::string& _t=std::string());
	Input(const Input& _i);
	virtual ~Input();
	Input& operator=(const Input& _i);

	/**
	 * Set the content of the input buffer.
	 *
	 * @param _i the content of the input buffer.
	 */
	virtual void input(const std::string& _i);

	/**
	 * Return the content of the input buffer.
	 *
	 * @return the content of the input buffer.
	 */
	virtual std::string input() const;

	/**
	 * Set readonly mode.
	 *
	 * @param _s @c true, no editing allowed. @c false, editing
	 * allowed.
	 */
	void readonly(bool _s);

	/**
	 * Get read-only mode
	 *
	 * @return @c true if Input Widget is in read-only mode, @c
	 * false otherwise.
	 */
	bool readonly() const;

	// From WidgetBase

	void size_available(const Size& _s);

	/**
	 * Size the Input Widget requires.
	 *
	 * @return Either Size::zero() if __length is 0, or the
	 * Size(1, __length).
	 */
	Size size() const;

	Size size_hint() const;

	/**
	 * Dummy. Does nothing.
	 *
	 * Input Widget is not a container Widget, hence it may not
	 * notified of size changes().
	 *
	 * @return always @false
	 */
	bool size_change();

	/**
	 * Reset size.
	 *
	 * If __length is zero, resets __size. Else does nothing.
	 */
	void reset_size();

	bool can_focus() const;
	void focus(bool);
	bool focus() const;

	// From Realizeable
	/**
	 * Refresh the Input.
	 *
	 * @param immediate not directly used by Label::refresh() but
	 * passed to Widget::refresh().
	 */
	void refresh(bool immediate);
};

#endif // INPUT_H
