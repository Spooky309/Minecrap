#pragma once
#include <memory>
#include <vector>
#include <fstream>
#include <png.h>
#include "OpenGLW.h"
class texture {
public:
	texture(const std::string& path) {
		p = path;
		png_byte header[8];
		auto fp = std::unique_ptr<std::FILE, decltype(&std::fclose)>(std::fopen(path.c_str(), "rb"), &std::fclose);
		if (fp == nullptr)
		{
			throw std::runtime_error("Requested PNG file either does not exist or is not accessible.");
		}
		fread(header, 1, 8, fp.get());

		if (png_sig_cmp(header, 0, 8))
		{
			throw std::runtime_error("Requested texture at " + path + " is not a valid PNG file.\n");
		}

		png_struct* png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (png_ptr == nullptr)
		{
			throw std::runtime_error("weird error: png_create_read_struct returned null");
		}
		png_info* info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == nullptr) {
			throw std::runtime_error("weird error: png_create_info_struct returned null");
		}

		if (setjmp(png_jmpbuf(png_ptr))) {
			throw std::runtime_error("weird error of some kind in LibPNG.");
		}
		png_init_io(png_ptr, fp.get());
		png_set_sig_bytes(png_ptr, 8);
		png_read_info(png_ptr, info_ptr);

		width = png_get_image_width(png_ptr, info_ptr);
		height = png_get_image_height(png_ptr, info_ptr);
		int colour_type = png_get_color_type(png_ptr, info_ptr);
		int bit_depth = png_get_bit_depth(png_ptr, info_ptr);

		png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
			&colour_type, nullptr, nullptr, nullptr);

		if (bit_depth == 16) {
#if PNG_LIBPNG_VER >= 10504
			png_set_scale_16(png_ptr);
#else
			png_set_strip_16(png_ptr);
#endif
		}
		if (colour_type == PNG_COLOR_TYPE_PALETTE) {
			png_set_palette_to_rgb(png_ptr);
		}
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
			png_set_tRNS_to_alpha(png_ptr);
		}
		if (colour_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
			png_set_expand_gray_1_2_4_to_8(png_ptr);
		}
		if (colour_type == PNG_COLOR_TYPE_RGB) {
			png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
		}
		png_read_update_info(png_ptr, info_ptr);
		switch (colour_type) {
		case PNG_COLOR_TYPE_RGB:
			format = GL_RGB;
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA: // It should always be this, but check anyway.
			format = GL_RGBA;
			break;
		default:
			throw std::runtime_error("png image was an unknown format");
			break;
		}


		std::size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);

		data.resize(height * rowbytes);
		for (std::size_t y = height; y > 0; y--) {
			png_read_row(png_ptr, &data[(y - 1) * rowbytes], nullptr);
		}
		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		//for (auto it = data.begin() + 3; it < data.end(); it += 4) {
		//	if (*it > 0) {
		//		transluc = true;
		//	}
		//}
		glGenTextures(1, &glTex);
		glBindTexture(GL_TEXTURE_2D, glTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	bool is_transluc() {
		return transluc;
	}
	bool transluc;
	bool transluc_checked;
	GLuint glTex;
	GLuint format;
	GLuint width;
	GLuint height;
	std::string p;
	std::vector<GLubyte> data;
};
class textureloader {
public:
	std::shared_ptr<texture> load_tex(const std::string& path);
	std::vector<std::shared_ptr<texture>> loadedtexs;

	static textureloader& instance() {
		static textureloader inst; //init once
		return inst;
	}
};
