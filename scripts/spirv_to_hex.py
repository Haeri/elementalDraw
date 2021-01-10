#!/usr/bin/python

import binascii
import re


frag_file = '../data/shader/rounded_rect.frag';
vert_file = '../data/shader/rounded_rect.vert';
ogl_frag_file = '../data/shader/gl/rounded_rect.frag';
ogl_vert_file = '../data/shader/gl/rounded_rect.vert';
frag_file_bin = frag_file + '.spv';
vert_file_bin = vert_file + '.spv';


def read_file(file_name):
	ret = ""
	with open(file_name, 'rb') as f:
		while True:
			content = f.read(1)
			if not content:
				break
			fomrated_hex = "{0:#0{1}x}".format(ord(content),4)
			ret = ret + str(fomrated_hex) + ", "
	return ret

def spvToCore(file_name):
	ret = ""
	file = open(file_name, mode='r')

	content = file.read()
	'''
	# #version 450       ->  #version 410 core
	content = re.sub(r"#version 450", "#version 410 core", content)

	# #extension GL_..   ->  
	content = re.sub(r"#extension GL_ARB_separate_shader_objects : enable\n", "", content)

	# layout(set = 0,    ->  layout(
	content = re.sub(r"layout\( ?set ?= ?\d,? ?", "layout(", content)

	# layout(binding = 0,    ->  layout(
	content = re.sub(r"layout\( ?binding ?= ?\d,? ?", "layout(", content)

	# gl_InstanceIndex   ->  gl_InstanceID
	content = re.sub(r"gl_InstanceIndex", "gl_InstanceID", content)

	# gl_VertexIndex     ->  gl_VertexID
	content = re.sub(r"gl_VertexIndex", "gl_VertexID", content)

	# flat     ->  
	content = re.sub(r" ?flat ?", " ", content)

	# readonly     ->  
	content = re.sub(r" ?readonly ?", " ", content)

	# Invert 
	content = re.sub(r'vec4\(verts\[gl_VertexID\]\.xy, 0\.0, 1\.0\);', "vec4(verts[gl_VertexID].xy*vec2(1.0, -1.0), 0.0, 1.0);", content)	
	'''
	# pack into a string
	content = re.sub(r"^(.*)$", r'"\1\\n"', content, flags=re.MULTILINE)

	return content
	
print("std::vector<unsigned char> vk_fragment_code = {\n" + read_file(frag_file_bin) + "\n};\n")
print("std::vector<unsigned char> vk_vertex_code = {\n" + read_file(vert_file_bin) + "\n};\n")

print("const char* ogl_fragment_code = \n" + spvToCore(ogl_frag_file) + ";\n")
print("const char* ogl_vertex_code = \n" + spvToCore(ogl_vert_file) + ";\n")