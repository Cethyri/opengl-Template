#include <fstream>
#include <iostream>
#include "stdafx.h"
#include "image.h"

namespace Image
{
	static const int WIDTH_OFF = 18, HEIGHT_OFF = 22, BPP_OFF = 28, SIZE_OFF = 34, OFFSET_OFF = 10;
	char header[54];
	int size;
	int offset;

	const unsigned char * Image::LoadBMP(const std::string & filename, int & width, int & height, int & bpp)
	{
		std::ifstream fileStream(filename, std::ios::binary);

		if (!fileStream)
		{
			std::cout << "can't open file at: " << filename << std::endl;
			return nullptr;
		}

		fileStream.read(&header[0], sizeof(header));

		width = *(int*)&(header[WIDTH_OFF]);
		height = *(int*)&(header[HEIGHT_OFF]);
		bpp = *(short*)&(header[BPP_OFF]);
		size = *(int*)&(header[SIZE_OFF]);
		offset = *(int*)&(header[OFFSET_OFF]);

		unsigned char * data = new unsigned char[size];


		fileStream.seekg(offset);

		fileStream.read((char*) (&data[0]), size);

		//unsigned char temp;
		//for (int i = 0; i < size - 2; i += bpp / 8)
		//{
		//	temp = data[i];
		//	data[i] = data[i + 2];
		//	data[i + 2] = temp;
		//}

		fileStream.close();

		return data;
	}
}
