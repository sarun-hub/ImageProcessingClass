#pragma once

struct RGB {
	double R;
	double G;
	double B;
};

struct YUV {
	double Y;
	double U;
	double V;
};

inline YUV convert_rgb_to_yuv(const RGB& input) {
	const auto R = input.R;
	const auto G = input.G;
	const auto B = input.B;

	auto Y = 0.299 * R + 0.587 * G + 0.114 * B;
	auto U = -0.169 * R - 0.331 * G + 0.500 * B;
	auto V = 0.500 * R - 0.419 * G - 0.081 * B;

	const auto output = YUV{Y, U, V};
	return output;
}

inline RGB convert_yuv_to_rgb(const YUV& input) {
	const auto Y = input.Y;
	const auto U = input.U;
	const auto V = input.V;

	auto R = Y + 1.402 * V;
	auto G = Y - 0.334 * U - 0.714 * V;
	auto B = Y + 1.772 * U;
    
	const auto output = RGB{R, G, B};
	return output;
}