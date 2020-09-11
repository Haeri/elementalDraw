import time
from elemd import *

winc = WindowConfig("Python Hello", 600)
win = Window(winc)
ctx = win.create_context()

red_color = color(255, 75, 92, 255)
white_color = color(224, 236, 228, 255)
	
ctx.set_clear_color(red_color)

ctx.prepare()

while win.is_running():
	win.poll_events()

	ctx.set_fill_color(white_color)
	ctx.fill_rect(30, 30, 25, 25)

	ctx.draw_text(70, 20, "Hello From Python!")

	ctx.draw_frame()

	time.sleep(0.1)

win.destroy()