#pragma once
#include "texture.h"

#include "iostream"
#include <memory>
// Поддерживаемые виды фигур: прямоугольник и эллипс
enum class ShapeType { RECTANGLE,
					   ELLIPSE };

class Shape {
public:
	// Фигура после создания имеет нулевые координаты и размер,
	// а также не имеет текстуры
	explicit Shape(ShapeType type) : type_(type) {
	}

	void SetPosition(Point pos) {
		position_ = pos;
	}

	void SetSize(Size size) {
		size_ = size;
	}

	void SetTexture(std::shared_ptr<Texture> texture) {
		texture_ = std::move(texture);
	}

	// Рисует фигуру на указанном изображении
	// В зависимости от типа фигуры должен рисоваться либо эллипс, либо прямоугольник
	// Пиксели фигуры, выходящие за пределы текстуры, а также в случае, когда текстура не задана,
	// должны отображаться с помощью символа точка '.'
	// Части фигуры, выходящие за границы объекта image, должны отбрасываться.
	void Draw(Image &image) const {
		auto img_size = GetImageSize(image);
		for (auto y = 0; y < size_.height; ++y) {
			for (auto x = 0; x < size_.width; ++x) {
				if (IsPointInEllipse({x, y}, size_) && type_ == ShapeType::ELLIPSE) {
					image[y + position_.y][x + position_.x] = texture_->GetPixelColor({x, y});
					continue;
				} else {
					if (type_ == ShapeType::RECTANGLE) {
						image[y + position_.y][x + position_.x] = texture_->GetPixelColor({x, y});
						continue;
					}
				}
			}
		}
	}

private:
	ShapeType type_{};
	Point position_{};
	Size size_{};
	std::shared_ptr<Texture> texture_ = nullptr;
};