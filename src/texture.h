#pragma once

#include "common.h"

class Texture {
public:
	explicit Texture(Image image)
		: image_(std::move(image)) {
	}

	Size GetSize() const {
		Size result = {0, 0};
		if (!image_.empty()) {
			result.width = static_cast<int>(image_[0].size());
			result.height = static_cast<int>(image_.size());
		}
		return result;
	}

	char GetPixelColor(Point p) const {
		if (!image_.empty()) {
			if (static_cast<int>(image_.size()) > p.y) {
				if (static_cast<int>(image_[0].size()) > p.x) {
					return image_[p.y][p.x];
				}
			}
		}
		return '.';
	}

private:
	Image image_;
};