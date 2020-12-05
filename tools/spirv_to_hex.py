#!/usr/bin/python

import binascii

frag_file = '../data/shader/rounded_rect.frag.spv';
veft_file = '../data/shader/rounded_rect.vert.spv';


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

print("std::vector<unsigned char> ContextImplVulkan::fragment_code = {\n" + read_file(frag_file) + "\n};")
print("\n")
print("std::vector<unsigned char> ContextImplVulkan::vertex_code = {\n" + read_file(veft_file) + "\n};")