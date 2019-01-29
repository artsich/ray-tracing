#include "geometry.h"
#include <vector>
#include <algorithm>
#include <fstream>

class frame_buffer
{
public:
	int width;
	int height;
	std::vector<Vec3f> buffer;

	frame_buffer(int width, int height)
	{
		this->width = width;
		this->height = height;
		buffer.resize(width*height);
	}
};

struct Sphere
{	
	int radius;
	Vec3f center;
	Vec3f color;

	Sphere(Vec3f center, Vec3f color, int radius) : center(center), color(color), radius(radius) {}

};

void fill_image(frame_buffer* frame)
{
	std::ofstream ofs;
	ofs.open("./out.ppm");
	ofs << "P6\n" << frame->width << " " << frame->height << "\n255\n";
	for (size_t i = 0; i < frame->height*frame->width; ++i) {
		for (size_t j = 0; j<3; j++) {
			ofs << (char)(255 * std::max(0.f, std::min(1.f, frame->buffer[i][j])));
			//ofs << 0;
		}
	}
	ofs.close();
}

int collision(const Vec3f& origin, const Vec3f& dir, const Sphere& shape)
{
	Vec3f l = origin - shape.center;
	float k1 = dir*dir;
	float k2 = (dir * l) * 2;
	float k3 = l * l - (shape.radius*shape.radius);
	float discriminant = sqrt(k2 * k2 - 4 * k1 * k3);
	return discriminant;
}

Vec3f rayTrace(const Vec3f& origin, const Vec3f& dir, const std::vector<Sphere>& shapes)
{
	for (Sphere shape : shapes)
	{
		int hit = collision(origin, dir, shape);
		if (hit >= 0)
		{
			return shape.color;
		}
	}
	return Vec3f(0.f, 0.f, 0.f);
}

void render(frame_buffer* frame)
{
	int fov = 70;
	std::vector<Sphere> spheres;
	spheres.push_back(Sphere(Vec3f(-3, 0, -16), Vec3f(1, 0, 0), 2));
	spheres.push_back(Sphere(Vec3f(-5, 0, -10), Vec3f(1, 1, 0), 2));

//goto L;
	for (int j = 0; j < frame->height; ++j)
	{
		for (int i = 0; i < frame->width; ++i)
		{
			float x = (2 * (i + 0.5) / (float)frame->width - 1)*tan(fov / 2.)*frame->width / (float)frame->height;
			float y = -(2 * (j + 0.5) / (float)frame->height - 1)*tan(fov / 2.);
			float z = -1;
			Vec3f dir = Vec3f(x, y, z);
			frame->buffer[i + j * frame->width] = rayTrace(Vec3f(0, 0, 0), dir, spheres);
		}
	}
L:
	fill_image(frame);
}

int main()
{
	const int width = 1024;
	const int height = 768;
	frame_buffer frame(width, height);

	render(&frame);

	//system("PAUSE");
	return 0;
}