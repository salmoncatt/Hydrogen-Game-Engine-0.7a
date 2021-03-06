#include "hfpch.h"

namespace HFR {

	Texture::Texture() {
		textureID = 0;
		image = Image();
	}

	Texture::Texture(const Image& _image) {
		image = _image;
		textureID = 0;
	}

	Texture::Texture(const Texture& texture) {
		image = Image(texture.image);
		textureID = 0;
		byteAlignment = texture.byteAlignment;
		wrapMode = texture.wrapMode;
		filterMode = texture.filterMode;
		textureType = texture.textureType;
		internalFormat = texture.internalFormat;
		format = texture.format;
		dataType = texture.dataType;
		generateMipmap = texture.generateMipmap;
		textureUnit = texture.textureUnit;
	}

	Texture::Texture(const std::string& path) {
		image = Image(path);
		textureID = 0;
	}

	Texture& Texture::operator=(const Texture& texture) {
		textureID = texture.textureID;
		image = texture.image;

		byteAlignment = texture.byteAlignment;
		wrapMode = texture.wrapMode;
		filterMode = texture.filterMode;
		textureType = texture.textureType;
		internalFormat = texture.internalFormat;
		format = texture.format;
		dataType = texture.dataType;
		generateMipmap = texture.generateMipmap;
		textureUnit = texture.textureUnit;

		return *this;
	}

	Texture::~Texture() {
		destroy();
	}

	void Texture::destroy() {
		glDeleteTextures(1, &textureID);
	}

	void Texture::create() {
		if (!isCreated) {

			glActiveTexture(GL_TEXTURE0 + textureUnit);
			
			glGenTextures(1, &textureID);
			glBindTexture(textureType, textureID);

			glPixelStorei(GL_UNPACK_ALIGNMENT, byteAlignment);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode.x);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode.y);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode.x);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode.y);

			/*if (image.data != NULL)
				glTexImage2D(textureType, 0, internalFormat, image.width, image.height, 0, format, dataType, image.data);
			else
				glTexImage2D(textureType, 0, internalFormat, image.width, image.height, 0, format, dataType, 0);*/

			glTexImage2D(textureType, 0, internalFormat, image.width, image.height, 0, format, dataType, image.data);

			if(generateMipmap)
				glGenerateMipmap(textureType);

			isCreated = true;

			glActiveTexture(GL_TEXTURE0);

			glBindTexture(textureType, 0);
		}
	}

	void Texture::setSubImage(const int& level, const Vec2i& offset, const Vec2i& size, const void* pixels) {
		glBindTexture(textureType, textureID);
		glTexSubImage2D(GL_TEXTURE_2D, level, offset.x, offset.y, size.x, size.y, format, dataType, pixels);
		glBindTexture(textureType, 0);
	}

}