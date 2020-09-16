import time
from elemd import *

red_color = color(220, 75, 92, 255)
white_color = color(224, 236, 228, 255)
fps_color = color(60, 236, 50, 255)

sec = 0
now = time.time()
last = time.time()
frames = 0
accum = 0
fps = 0

def run(win, ctx):
	global sec, now, last, frames, accum, fps

	win.poll_events()

	ctx.set_fill_color(red_color)
	ctx.fill_rect(0, 0, 1000, 1000)
	ctx.set_clear_color(red_color)

	ctx.set_fill_color(white_color)
	
	sec = (time.time() * 100) % 500
	ctx.fill_rect(30 + sec, 100, 25, 25)

	ctx.draw_text(70, 40, "Hello From Python!")

	now = time.time()
	delta = now - last
	last = now

	if(accum > 1):
		fps = frames
		accum = 0
		frames = 0
	frames = frames + 1
	accum = accum + delta
	
	ctx.set_fill_color(fps_color)
	ctx.draw_text(6, 0, str(fps))

	ctx.draw_frame()

	#time.sleep(0.001)

