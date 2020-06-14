import platform
import subprocess
import re
import os

def do_step(title, cmd, regex, isFile = True):
	print(title + " " + ("." * (30 - len(title))) + " ", end='')
	err, msg = subprocess.getstatusoutput(cmd)
	if err == 0:
		m = re.search(regex, msg)
		print("OK " + m.group(1))
		return 0
	else:
		if isFile:
			print("NOT FOUND!")
		else:
			print("FAILED!")
			print(msg)
		return err

def do_custon_step(title, fn, errMsg):
	print(title + " " + ("." * (30 - len(title))) + " ", end='')
	if fn():
		print("OK")
		return 0
	else:
		print("FAILED!")
		print(errMsg)
		return 1

err = 0

if do_step("CMake", "cmake --version", 'cmake version (.*)') != 0:
	err = err + 1
if do_step("Vcpkg", "vcpkg version", 'Vcpkg package management program version (.*)') != 0:
	err = err + 1
else:
	if do_step("Install glfw3", "vcpkg install glfw3:x64-" + platform.system(), '()', False) != 0:
		err = err + 1
	if do_custon_step("Vulkan SDK", lambda : "VULKAN_SDK" in os.environ, "Vulkan SDK not installed!") != 0:
		err = err + 1
	else:
		if do_step("Install Vulkan", "vcpkg install vulkan:x64-" + platform.system(), '()', False) != 0:
			err = err + 1
	if do_custon_step("Vcpkg root", lambda : "VCPKG_ROOT" in os.environ, "VCPKG_ROOT environment variable not set!") != 0:
			err = err + 1


exit(err)