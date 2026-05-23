#pragma once
#include "myImageIO.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

template <typename T> using vec2d = std::vector<std::vector<T>>;

vec2d<double> gaussian_kernel(int kernel_size, double sigma,
							  bool normalize = true) {
	// initialize kernel
	vec2d<double> kernel(kernel_size, std::vector<double>(kernel_size));
	const int center = kernel_size / 2;
	double sum = 0;
	for (int kx = 0; kx < kernel_size; kx++) {
		for (int ky = 0; ky < kernel_size; ky++) {
			const auto dx = kx - center;
			const auto dy = ky - center;

			const auto value =
				std::exp(-(dx * dx + dy * dy) / (2.0 * sigma * sigma));
			kernel[ky][kx] = value;
			sum += value;
		}
	}

	// normalize
	if (normalize) {
		std::for_each(
			kernel.begin(), kernel.end(), [&sum](std::vector<double>& row) {
				std::for_each(row.begin(), row.end(), [&sum](double& element) {
					element = element / sum;
				});
			});
	}

	return kernel;
}

void apply_kernel(myImageData* img1, myImageData* img2,
				  const vec2d<double>& kernel) {

	const auto& H = img1->getHeight();
	const auto& W = img1->getWidth();

	const int& kernel_size_half = kernel.size() / 2;
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {

			double sum = 0;
			// following kernel size
			for (int ky = -kernel_size_half; ky <= kernel_size_half; ky++) {
				for (int kx = -kernel_size_half; kx <= kernel_size_half; kx++) {
					const int px = x + kx;
					const int py = y + ky;
					if (px < 0 || px >= W || py < 0 || py >= H) continue;
					const auto& value = img1->get(px, py);
					sum += value *
						   kernel[ky + kernel_size_half][kx + kernel_size_half];
				}
			}
			img2->set(x, y, sum);
		}
	}
}

void apply_kernel_with_range_weight(myImageData* img1, myImageData* img2,
									int kernel_size, double sigma_s,
									double sigma_r) {
	const auto H = img1->getHeight();
	const auto W = img1->getWidth();
	// get Gaussian Kernel
	const auto kernel = gaussian_kernel(kernel_size, sigma_s, false);

	const int kernel_size_half = kernel.size() / 2;
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {

			double sum = 0;
			double weight_total = 0;
			const auto center = img1->get(x, y);
			// following kernel size
			for (int ky = -kernel_size_half; ky <= kernel_size_half; ky++) {
				for (int kx = -kernel_size_half; kx <= kernel_size_half; kx++) {
					const int px = x + kx;
					const int py = y + ky;
					if (px < 0 || px >= W || py < 0 || py >= H) continue;

					// spatial weight (use one from Gaussian kernel)
					const auto spatial_weight =
						kernel[ky + kernel_size_half][kx + kernel_size_half];

					const auto neighbor = img1->get(px, py);
					const double intensity_diff =
						(double)neighbor - (double)center;

					// range weight
					const auto range_weight =
						std::exp(-(intensity_diff * intensity_diff) /
								 (2.0 * sigma_r * sigma_r));

					const auto weight = spatial_weight * range_weight;
					// apply weight
					sum += neighbor * weight;
					weight_total += weight;
				}
			}
			if (weight_total > 0) {
				img2->set(x, y, (int)(std::round(sum / weight_total)));
			} else {
				img2->set(x, y, center);
			};
		}
	}
}

double window_average(myImageData* full_img, int x, int y, int window_size) {
	double sum = 0;
	int count = 0;
	const int radius = window_size / 2;
	for (int ky = -radius; ky <= radius; ky++) {
		for (int kx = -radius; kx <= radius; kx++) {
			if (x + kx < 0 || x + kx >= full_img->getWidth() || y + ky < 0 ||
				y + ky >= full_img->getHeight())
				continue;
			const auto value = full_img->get(x + kx, y + ky);
			sum += value;
			count++;
		}
	}
	const double average = sum / count;
	return average;
}

double window_average(vec2d<double>& coefficients, int x, int y,
					  int window_size) {
	double sum = 0;
	int count = 0;

	const int radius = window_size / 2;
	for (int ky = -radius; ky <= radius; ky++) {
		for (int kx = -radius; kx <= radius; kx++) {
			if (x + kx < 0 || x + kx >= coefficients[0].size() || y + ky < 0 ||
				y + ky >= coefficients.size())
				continue;
			const auto value = coefficients[y + ky][x + kx];
			sum += value;
			count++;
		}
	}
	const double average = sum / count;
	return average;
}

double window_variance(myImageData* full_img, int x, int y, int window_size) {
	const auto average = window_average(full_img, x, y, window_size);
	const int radius = window_size / 2;
	double sum = 0;
	int count = 0;

	for (int ky = -radius; ky <= radius; ky++) {
		for (int kx = -radius; kx <= radius; kx++) {
			if (x + kx < 0 || x + kx >= full_img->getWidth() || y + ky < 0 ||
				y + ky >= full_img->getHeight())
				continue;
			const auto value = full_img->get(x + kx, y + ky);
			const auto mean_diff = value - average;
			sum += (mean_diff * mean_diff);
			count++;
		}
	}

	const double variance = sum / count;
	return variance;
}

void apply_guided_filter(myImageData* img1, myImageData* img2, int window_size,
						 double epsilon) {
	const auto H = img1->getHeight();
	const auto W = img1->getWidth();

	vec2d<double> a(H, std::vector<double>(W));
	vec2d<double> b(H, std::vector<double>(W));
	// find coefficients
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			const auto mean = window_average(img1, x, y, window_size);
			const auto var = window_variance(img1, x, y, window_size);

			a[y][x] = var / (var + epsilon);
			b[y][x] = epsilon * mean / (var + epsilon);
		}
	}

	// find coefficients average and apply
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			const auto a_mean = window_average(a, x, y, window_size);
			const auto b_mean = window_average(b, x, y, window_size);

			const auto value = img1->get(x, y);

			img2->set(x, y, a_mean * value + b_mean);
		}
	}
}