#ifndef ELEMD_WINDOW_IMPL_HPP
#define ELEMD_WINDOW_IMPL_HPP

#include <vector>
#include <unordered_map>

#include "elemd/window.h"

struct GLFWwindow;
struct GLFWcursor;

typedef struct WindowImpl {
	GLFWwindow* _glfw_window = NULL;
	uint32_t _windowCount = 0;

	float _x_scale = 1;
	float _y_scale = 1;
	float _x_offset = 0;
	float _y_offset = 0;
	float _dpi_scale = 1;
	int _last_x_pos = -1;
	int _last_y_pos = -1;
	int _last_x_scale = -1;
	int _last_y_scale = -1;

	std::vector<std::function<void(resize_event)>> _resize_callbacks;
	std::vector<std::function<void(mouse_move_event)>> _mouse_move_callbacks;
	std::vector<std::function<void(mouse_button_event)>> _mouse_button_callbacks;
	std::vector<std::function<void(key_event)>> _key_callbacks;
	std::vector<std::function<void(char_event)>> _char_callbacks;
	std::vector<std::function<void(scroll_event)>> _scroll_callbacks;

	std::unordered_map<Cursor* , GLFWcursor*> _cursors;
	std::unordered_map<CURSOR_TYPE, GLFWcursor*> _default_cursors;
} WindowImpl;



WindowImpl* _windowImpl_create(const WindowConfig& config);
void _windowImpl_destroy(const WindowConfig& config);

GLFWwindow* _windowImpl_getGLFWWindow();
void _windowImpl_load_icon(const WindowConfig& config);

   

#endif // ELEMD_WINDOW_IMPL_HPP