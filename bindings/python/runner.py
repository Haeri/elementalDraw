import time
from elemd import *
import example
import importlib
import os

winc = WindowConfig("Python Hello", 500, 300, native_pixel_size = False, icon_file = "./examples/window_app/res/logo.png")
win = Window(winc)
ctx = win.create_context()

urbanist = font("./examples/window_app/res/font/Urbanist-Regular.ttf")
ctx.register_font(urbanist);

ctx.prepare()
ctx.set_font(urbanist)

module_time = os.path.getmtime("./example.py")

runner_module = example
while win.is_running():
	if(module_time != os.path.getmtime("./example.py")):
		try:
			print("Reloading..")
			module_time = os.path.getmtime("./example.py")
			tmp_module = importlib.reload(example)
			runner_module = tmp_module
		except Exception as e:
			print("Reloading Faild!\n" + str(e))

	runner_module.run(win, ctx)

win.destroy()