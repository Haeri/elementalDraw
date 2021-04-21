import time
from elemd import *

red_color = color(220, 75, 92, 255)
blue_color = color(75, 148, 220, 255)
white_color = color(224, 236, 228, 255)
dark_color = color(56, 58, 61, 255)
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
	ctx.set_clear_color(red_color)
	#ctx.set_clear_color(blue_color)

    # Rectangle
	ctx.set_fill_color(dark_color)
	ctx.fill_rect(40, 40, 420, 150)

    # Runnter
	sec = (time.time() * 100) % 500
	ctx.set_fill_color(white_color)
	ctx.fill_rect(sec, 0, 25, 25)

    # Main text
	ctx.set_font_size(40)
	ctx.draw_text(70, 60, "Hello From Python!\nThis is fun :D")

	now = time.time()
	delta = now - last
	last = now

	if(accum > 1):
		fps = frames
		accum = 0
		frames = 0
	frames = frames + 1
	accum = accum + delta
	
	# FPS counter
	ctx.set_fill_color(fps_color)
	ctx.set_font_size(16)
	ctx.draw_text(6, 0, str(fps))

	ctx.draw_frame()

	#time.sleep(0.001)

