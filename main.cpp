#include "canvas.h"
#include "shapes.h"

#include <cassert>
#include <sstream>

#include <gtest/gtest.h>

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

std::unique_ptr<Texture> MakeTextureCow() {
	Image image = {R"(^__^            )",//
				   R"((oo)\_______    )",//
				   R"((__)\       )\/\)",//
				   R"(    ||----w |   )",//
				   R"(    ||     ||   )"};
	return std::make_unique<Texture>(move(image));
}

std::unique_ptr<Texture> MakeTextureSolid(Size size, char pixel) {
	Image image(size.height, std::string(size.width, pixel));
	return std::make_unique<Texture>(move(image));
}

std::unique_ptr<Texture> MakeTextureCheckers(Size size, char pixel1, char pixel2) {
	Image image(size.height, std::string(size.width, pixel1));

	for (int i = 0; i < size.height; ++i) {
		for (int j = 0; j < size.width; ++j) {
			if ((i + j) % 2 != 0) {
				image[i][j] = pixel2;
			}
		}
	}

	return std::make_unique<Texture>(move(image));
}

TEST(TestsShapes, TestCpp) {
	Canvas canvas(Size{77, 17});

	// Буква "C" как разность двух эллипсов, один из которых нарисован цветом фона
	canvas.AddShape(ShapeType::ELLIPSE, {2, 1}, {30, 15},
					MakeTextureCheckers({100, 100}, 'c', 'C'));
	canvas.AddShape(ShapeType::ELLIPSE, {8, 4}, {30, 9}, MakeTextureSolid({100, 100}, ' '));

	// Горизонтальные чёрточки плюсов
	auto h1 = canvas.AddShape(ShapeType::RECTANGLE, {54, 7}, {22, 3},
							  MakeTextureSolid({100, 100}, '+'));
	canvas.DuplicateShape(h1, {30, 7});

	// Вертикальные чёрточки плюсов
	auto v1 = canvas.DuplicateShape(h1, {62, 3});
	canvas.ResizeShape(v1, {6, 11});
	canvas.DuplicateShape(v1, {38, 3});

	std::stringstream output;
	canvas.Print(output);

	const auto answer = "###############################################################################\n"
						"#                                                                             #\n"
						"#            cCcCcCcCcC                                                       #\n"
						"#        CcCcCcCcCcCcCcCcCc                                                   #\n"
						"#      cCcCcCcCcCcCcCcCcCcCcC          ++++++                  ++++++         #\n"
						"#    CcCcCcCcCcCc                      ++++++                  ++++++         #\n"
						"#   CcCcCcCcC                          ++++++                  ++++++         #\n"
						"#   cCcCcCc                            ++++++                  ++++++         #\n"
						"#  cCcCcC                      ++++++++++++++++++++++  ++++++++++++++++++++++ #\n"
						"#  CcCcCc                      ++++++++++++++++++++++  ++++++++++++++++++++++ #\n"
						"#  cCcCcC                      ++++++++++++++++++++++  ++++++++++++++++++++++ #\n"
						"#   cCcCcCc                            ++++++                  ++++++         #\n"
						"#   CcCcCcCcC                          ++++++                  ++++++         #\n"
						"#    CcCcCcCcCcCc                      ++++++                  ++++++         #\n"
						"#      cCcCcCcCcCcCcCcCcCcCcC          ++++++                  ++++++         #\n"
						"#        CcCcCcCcCcCcCcCcCc                                                   #\n"
						"#            cCcCcCcCcC                                                       #\n"
						"#                                                                             #\n"
						"###############################################################################\n";

	ASSERT_EQ(answer, output.str());
}

TEST(TestsShapes, TestCow) {
	Canvas canvas{{18, 5}};

	canvas.AddShape(ShapeType::RECTANGLE, {1, 0}, {16, 5}, MakeTextureCow());

	std::stringstream output;
	canvas.Print(output);

	// clang-format off
    // Здесь уместно использовать сырые литералы, т.к. в текстуре есть символы '\'
    const auto answer =
            R"(####################)""\n"
            R"(# ^__^             #)""\n"
            R"(# (oo)\_______     #)""\n"
            R"(# (__)\       )\/\ #)""\n"
            R"(#     ||----w |    #)""\n"
            R"(#     ||     ||    #)""\n"
            R"(####################)""\n";
	// clang-format on

	ASSERT_EQ(answer, output.str());
}

TEST(TestsShapes, TestSimple) {
	Canvas canvas{{3, 3}};
	Image image = {""};
	canvas.AddShape(ShapeType::RECTANGLE, {0, 0}, {2, 2}, std::make_unique<Texture>(move(image)));

	std::stringstream output;
	canvas.Print(output);

	// clang-format off
	// Здесь уместно использовать сырые литералы, т.к. в текстуре есть символы '\'
	const auto answer =
			R"(#####)""\n"
			R"(#   #)""\n"
			R"(#   #)""\n"
			R"(#   #)""\n"
			R"(#####)""\n";
	// clang-format on

	ASSERT_EQ(answer, output.str());
}

TEST(TestsShapes, TestSimple_next) {
	Canvas canvas{{3, 3}};
	Image image = {"oo", "oo"};
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(move(image));
	ASSERT_EQ(texture->GetSize().width, 2);
	ASSERT_EQ(texture->GetSize().height, 2);
	ASSERT_EQ(texture->GetPixelColor({0,0}), 'a');
	canvas.AddShape(ShapeType::RECTANGLE, {0, 0}, {2, 2}, texture);

	std::stringstream output;
	canvas.Print(output);

	// clang-format off
	// Здесь уместно использовать сырые литералы, т.к. в текстуре есть символы '\'
	const auto answer =
			R"(#####)""\n"
			R"(#oo #)""\n"
			R"(#oo #)""\n"
			R"(#   #)""\n"
			R"(#####)""\n";
	// clang-format on

	ASSERT_EQ(answer, output.str());
}