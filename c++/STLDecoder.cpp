
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

	// Open stl file as: input, binary.
	ifstream stl_file(stl_file_path, ios::in|ios::binary);
	if (stl_file.is_open())
	{

		// First is UINT8[80] little-endian: header (80B).
		char header_binary[80];
		stl_file.read(header_binary, 80);

		// Second is UINT32 little-endian: number of triangles (4B).
		char number_of_triangles_binary[4];
		stl_file.read(number_of_triangles_binary, 4);
		int number_of_triangles;
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

			cout << "Normal: " << normal_x << " " << normal_y << " " << normal_z << endl;
			cout << "V1: " << v1_x << " " << v1_y << " " << v1_z << endl;
			cout << "V2: " << v2_x << " " << v2_y << " " << v2_z << endl;
			cout << "V3: " << v3_x << " " << v3_y << " " << v3_z << endl;
			cout << endl;
						
		}

		stl_file.close();

		
	}

	else 
	{
		cout << "Unable to open the file" << endl;
	}


	return 0;
}
