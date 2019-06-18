
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

float REAL32bin_to_float(ifstream& stl_file)
{
	char binary[4];
	stl_file.read(binary, 4);	
	float float_val;
	memcpy(&float_val, binary, sizeof(float));
	return float_val;
}

int main(){

	string stl_file_path = "../../triangulation_springLow.stl";
	string obj_file_path = "out.obj";

	// Open stl file as: input, binary.
	ifstream stl_file(stl_file_path, ios::in|ios::binary);

	// Open obj file as: output
	ofstream obj_file(obj_file_path, ios::out);

	// Basically some software uses the convention that the "Y" axis is UP 
	// (Unity, Gravity Sketch etc) whereas other software use the "Z" axis 
	// as UP (Blender etc). CAD tools tend to favour Z axis up (right handed) 
	// but because we are heavily tied to unity, we use Y UP (Left Handed).
	// https://steamcommunity.com/app/551370/discussions/0/2579854400753648953/

	int number_of_triangles;
	bool y_up = false;
	if (stl_file.is_open() and obj_file.is_open())
	{

		// First is UINT8[80] little-endian: header (80B).
		char header_binary[80];
		stl_file.read(header_binary, 80);

		// Second is UINT32 little-endian: number of triangles (4B).
		char number_of_triangles_binary[4];
		stl_file.read(number_of_triangles_binary, 4);
		memcpy(&number_of_triangles, number_of_triangles_binary, sizeof(int));

		// Read every triangle information.
		for (int n_triangle = 0; n_triangle < number_of_triangles; ++n_triangle)
		{
			// Normal vector. REAL32[3] little-endian (x/y/z:4B/4B/4B).
			float normal_x = REAL32bin_to_float(stl_file);
			float normal_y = REAL32bin_to_float(stl_file);
			float normal_z = REAL32bin_to_float(stl_file);
			
			// Vertex 1. REAL32[3] little-endian (x/y/z:4B/4B/4B).
			float v1_x = REAL32bin_to_float(stl_file);
			float v1_y = REAL32bin_to_float(stl_file);
			float v1_z = REAL32bin_to_float(stl_file);

			// Vertex 2. REAL32[3] little-endian (x/y/z:4B/4B/4B).
			float v2_x = REAL32bin_to_float(stl_file);
			float v2_y = REAL32bin_to_float(stl_file);
			float v2_z = REAL32bin_to_float(stl_file);
	
			// Vertex 3. REAL32[3] little-endian (x/y/z:4B/4B/4B).
			float v3_x = REAL32bin_to_float(stl_file);
			float v3_y = REAL32bin_to_float(stl_file);
			float v3_z = REAL32bin_to_float(stl_file);

			// Attribute byte count. UINT16 little-endian (2B).
			char attribute_byte_count_binary[2];
			stl_file.read(attribute_byte_count_binary, 2);

			//cout << "Normal: " << normal_x << " " << normal_y << " " << normal_z << endl;
			//cout << "V1: " << v1_x << " " << v1_y << " " << v1_z << endl;
			//cout << "V2: " << v2_x << " " << v2_y << " " << v2_z << endl;
			//cout << "V3: " << v3_x << " " << v3_y << " " << v3_z << endl;
			//cout << endl;

			if (y_up)
			{
				obj_file << "v" << " " << to_string(v1_x) << " " << to_string(v1_y) << " " << to_string(v1_z) << endl;
				obj_file << "v" << " " << to_string(v2_x) << " " << to_string(v2_y) << " " << to_string(v2_z) << endl;
				obj_file << "v" << " " << to_string(v3_x) << " " << to_string(v3_y) << " " << to_string(v3_z) << endl;
			} else
			{
				obj_file << "v" << " " << to_string(v1_x) << " " << to_string(v1_z) << " " << to_string(v1_y) << endl;
				obj_file << "v" << " " << to_string(v2_x) << " " << to_string(v2_z) << " " << to_string(v2_y) << endl;
				obj_file << "v" << " " << to_string(v3_x) << " " << to_string(v3_z) << " " << to_string(v3_y) << endl;
			}
							
		}

		unsigned long idx = 1;
		for (int triangle_idx = 0; triangle_idx < number_of_triangles; ++triangle_idx)
		{	
			if (y_up)
			{
				obj_file << "f" << " " << to_string(idx) << " " << to_string(idx+1) << " " << to_string(idx+2) << endl;
			} else
			{
				obj_file << "f" << " " << to_string(idx) << " " << to_string(idx+2) << " " << to_string(idx+1) << endl;
			}
			idx = idx + 3;
		}

		stl_file.close();
		obj_file.close();

		
	}

	else 
	{
		cout << "Unable to open the file" << endl;
	}


	return 0;
}
