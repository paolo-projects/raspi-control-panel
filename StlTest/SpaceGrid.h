#pragma once

class SpaceGrid5x3 {
public:
	SpaceGrid5x3(int col, int row) {
		x = H_LAT_PADD + (col) * (width + H_INT_PADD);
		y = V_LAT_PADD + (row) * (height + V_INT_PADD);
	}
	int getX() const {
		return x;
	}
	int getY() const {
		return y;
	}
	int getWidth() const {
		return width;
	}
	int getHeight() const {
		return height;
	}
private:
	static constexpr int H_LAT_PADD = 40, H_INT_PADD = 25, V_LAT_PADD = 40, V_INT_PADD = 30;
	static constexpr int width = 60, height = 60;
	int x, y;
};