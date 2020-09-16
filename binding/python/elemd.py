from ctypes import *

lib = cdll.LoadLibrary("./Debug/elemd.dll")

class WindowConfig(Structure):
	_fields_ = [
		("title", c_char_p),
		("width", c_int),
		("height", c_int),
		("position_x", c_int),
		("position_y", c_int),
		("x_scale", c_float),
		("y_scale", c_float),
		("decorated", c_bool),
		("transparent", c_bool),
		("resizeable", c_bool),
		("visible", c_bool),
		("vsync", c_bool),
		("icon_file", c_char_p)
	]

	def __init__(self, title="Python Window", width=400, height=600, position_x=-1, position_y=-1, x_scale=-1, y_scale=-1, decorated=True, transparent=False, resizeable=True, visible=True, vsync=True, icon_file="./elemd_res/elemd_icon.png"):
		super(WindowConfig, self).__init__(title.encode('utf-8'), width, height, position_x, position_y, x_scale, y_scale, decorated, transparent, resizeable, visible, vsync, icon_file.encode("utf-8"))


class color(Structure):
	def __init__(self, r, g, b, a):
		lib.color_new.argtypes = [c_int, c_int, c_int, c_int]
		lib.color_new.restype = POINTER(color)
		
		lib.color_delete.argtypes = [POINTER(color)]

		self.obj = lib.color_new(r, g, b, a)

	def __del__(self):
		return lib.color_delete(self.obj)


class Context(Structure):
	def __init__(self, object):
		lib.prepare.argtypes = [POINTER(Context)]

		lib.draw_frame.argtypes = [POINTER(Context)]

		lib.fill_rect.argtypes = [POINTER(Context), c_float, c_float, c_float, c_float]

		lib.draw_text.argtypes = [POINTER(Context), c_float, c_float, c_char_p]

		lib.set_fill_color.argtypes = [POINTER(Context), POINTER(color)]

		lib.set_clear_color.argtypes = [POINTER(Context), POINTER(color)]

		self.obj = object

	def prepare(self):
		return lib.prepare(self.obj)
	
	def draw_frame(self):
		return lib.draw_frame(self.obj)

	def fill_rect(self, x, y, width, height):
		return lib.fill_rect(self.obj, x, y, width, height)
	
	def draw_text(self, x, y, text):
		return lib.draw_text(self.obj, x, y, text.encode("utf-8"))
	
	def set_fill_color(self, color):
		return lib.set_fill_color(self.obj, color.obj)
	
	def set_clear_color(self, color):
		return lib.set_clear_color(self.obj, color.obj)
		

class Window(Structure):
	def __init__(self, window_config):
		lib.Window_new.argtypes = [POINTER(WindowConfig)]
		lib.Window_new.restype = POINTER(Window)
		
		lib.Window_delete.argtypes = [POINTER(Window)]

		lib.create_context.argtypes = [POINTER(Window)]
		lib.create_context.restype = POINTER(Context)

		lib.poll_events.argtypes = [POINTER(Window)]

		lib.is_running.argtypes = [POINTER(Window)]

		self.obj = lib.Window_new(window_config)

	def create_context(self):
		return Context(lib.create_context(self.obj))

	def poll_events(self):
		return lib.poll_events(self.obj)

	def is_running(self):
		return lib.is_running(self.obj)
		
	def destroy(self):
		return lib.Window_delete(self.obj)