#include "geometry.h"
#include <vector>
#include <algorithm>
#include <fstream>

const int width = 1024;
const int height = 768;

void drawToFile(const std::vector<Vec3f>& buffer, int width, int height)
{
	std::ofstream ofs;
	ofs.open("./out.ppm");
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (size_t i = 0; i < height*width; ++i) {
		for (size_t j = 0; j<3; j++) {
			ofs << (char)(255 * std::max(0.f, std::min(1.f, buffer[i][j])));
		}
	}
	ofs.close();
}

int rayTrace(const Vec3f& dir, const std::vector<Vec3f>& shapes)
{
	for (Vec3f coord : shapes)
	{

	}
}

bool collision(const Vec3f& ray, const Vec3f& point)
{

}

void render(std::vector<Vec3f>& framebuffer, int width, int height)
{

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{

		}
	}

}


int main()
{
	render();

	//system("PAUSE");
	return 0;
}