import time
from elemd import *
import example
import importlib
import os

winc = WindowConfig("Python Hello", 600)
win = Window(winc)
ctx = win.create_context()

ctx.prepare()

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