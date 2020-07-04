#pragma once

#include <vector>
#include "common.h"

class frame { 
private:
	int width;
	int height;
	std::vector<color> buffer;

public:

	frame(int width, int height) 
		: width(width)
		, height(height)
	{
		buffer.resize(width * height);
	}

	void write_color(int x, int y, color pixel_color, int samples_per_pixel) {
		auto scale = 1.0 / samples_per_pixel;
		color corrected_color = pixel_color;
		gamma_correct(corrected_color, scale);

		buffer[x + y * width] = corrected_color; 
	}

	void save_to_ppm(std::ostream& out) {
		std::cout << "P3\n" << width << " " << height << "\n255\n";
		for(size_t i = 0; i < buffer.size(); i++) {
			color& color = buffer[i];

			out << static_cast<int>(255.999 * clamp01(color.r)) << ' '
				<< static_cast<int>(255.999 * clamp01(color.g)) << ' '
				<< static_cast<int>(255.999 * clamp01(color.b)) << std::endl;
		}
	}

	inline int get_width() const { return width; }
	inline int get_height() const { return height; }

private:
	inline void gamma_correct(color& col, double scale) {
		// why do i scale here?
		col.r = sqrt(col.r * scale);
		col.g = sqrt(col.g * scale);
		col.b = sqrt(col.b * scale);
	}
};
