# -*- coding: utf-8 -*-

""" STL.binary file reader """

# Standard imports.
import numpy as np
import struct

def is_binary(stl_file):
	""" Detect if given file is binary or ascii STL

	ascii STL file always begins with "solid", while binary STL file
	cannot start with "solid".
	
	"""
	binary = True
	with open(stl_file, "rb") as stl_file:

		# Read first 5 bytes. In ascii encoding one char is one byte.
		id_string_binary = stl_file.read(5)
		id_string = id_string_binary.decode('ascii')
		if "solid" in id_string:
			binary = False
	return binary
	

def REAL32bin_to_float(n_bytes, bin_file):
	binary = bin_file.read(n_bytes)
	float_val = struct.unpack('f', binary)
	return float_val

def list_of_REAL32bin_to_list_of_float(list_n_bytes, bin_file):
	list_of_float_vals = []
	for n_bytes in list_n_bytes:
		float_val = REAL32bin_to_float(n_bytes, bin_file)
		list_of_float_vals.append(float_val)
	return list_of_float_vals
	
def read_binary(binary_stl_file):
	
	triangles = []

	# Open binary STL file.
	with open(binary_stl_file, "rb") as bin_stl_file:
		
		# First is UINT8[80] little-endian: header (80B).
		header_binary = bin_stl_file.read(80)

		# Second is UINT32 little-endian: number of triangles (4B).
		number_of_triangles_binary = bin_stl_file.read(4)
		number_of_triangles = int.from_bytes(number_of_triangles_binary, byteorder='little', signed=False)

		# Read every triangle information.
		for triangle in range(number_of_triangles):
			
			# Normal vector. REAL32[3] little-endian (x/y/z:4B/4B/4B).
			normal_vector = list_of_REAL32bin_to_list_of_float([4,4,4], bin_stl_file)

			# Vertex 1. REAL32[3] little-endian (x/y/z:4B/4B/4B).
			v1 = list_of_REAL32bin_to_list_of_float([4,4,4], bin_stl_file)

			# Vertex 2. REAL32[3] little-endian (x/y/z:4B/4B/4B).
			v2 = list_of_REAL32bin_to_list_of_float([4,4,4], bin_stl_file)

			# Vertex 3. REAL32[3] little-endian (x/y/z:4B/4B/4B).
			v3 = list_of_REAL32bin_to_list_of_float([4,4,4], bin_stl_file)

			# Attribute byte count. UINT16 little-endian (2B).
			attribute_byte_count_binary = bin_stl_file.read(2)
			attribute_byte_count = int.from_bytes(attribute_byte_count_binary, byteorder='little', signed=False)

			triangles.append([normal_vector, v1, v2, v3])
	
	return triangles

def read_ascii(ascii_stl_file):

	triangles = []
	
	# Read... TODO.
	print("Reading ascii...")

	return triangles

def write_to_obj_file(triangles, out_file):

	with open(out_file, "w") as out:

		for tri in triangles:

			# Basically some software uses the convention that the "Y" axis is UP 
			# (Unity, Gravity Sketch etc) whereas other software use the "Z" axis 
			# as UP (Blender etc). CAD tools tend to favour Z axis up (right handed) 
			# but because we are heavily tied to unity, we use Y UP (Left Handed).
			# https://steamcommunity.com/app/551370/discussions/0/2579854400753648953/
			y_up = False
			for idx in range(1,4):

				if y_up:
					out.write('v' + ' ' + str(tri[idx][0][0]) + ' ' + str(tri[idx][1][0]) + ' ' + str(tri[idx][2][0]) + '\n')

				else:
					out.write('v' + ' ' + str(triangle[vertex][0][0]) + ' ' + str(triangle[vertex][2][0]) + ' ' + str(triangle[vertex][1][0]) + '\n')

		idx = 1
		for triangle_idx in range(len(triangles)):
			if y_up:
				out.write('f' + ' ' + str(idx) + ' ' + str(idx+1) + ' ' + str(idx+2) + '\n')
			else:
				out.write('f' + ' ' + str(idx) + ' ' + str(idx+2) + ' ' + str(idx+1) + '\n')
			idx += 3
				
		
def main():

	stl_file = "../../triangulation_springLow.stl"
	out_file = "out.obj"
	triangles = []

	if is_binary(stl_file):
		triangles = read_binary(stl_file)

	else:
		triangles = read_ascii(stl_file)

	# Print the data.
	"""
	for triangle in triangles:
		print("Start Tri")
		print("	N", triangle[0])
		print("	V1", triangle[1])
		print("	V2", triangle[2])
		print("	V3", triangle[3])
		print("End Tri")
	"""

	write_to_obj_file(triangles, out_file)
	print(len(triangles))


if __name__ == "__main__":
	main()
			






	
