#include "TextureDictionary.h"
#include "Filesystem.h"
TextureDictionary::TextureDictionary(const std::string& texName)
{
	m_tex = textureloader::instance().load_tex(Filesystem::Instance().GetAbsPathTo(texName));
	xUVs = m_tex->width / 16;
	yUVs = m_tex->height / 16;
	numUVs = xUVs * yUVs;
	UVs = (FaceUVCoord*)malloc(sizeof(FaceUVCoord) * numUVs);
	size_t i = 0;
	for (size_t y = 0; y < m_tex->height; y += 16)
	{
		for (size_t x = 0; x < m_tex->width; x += 16)
		{
			float bottom = ((float)y / (float)m_tex->height);
			float top = ((float)y + 16 / (float)m_tex->height);

			UVs[i].tl = glm::vec2((float)x / (float)m_tex->width, top);
			UVs[i].tr = glm::vec2((float)(x + 16) / (float)m_tex->width, top);
			UVs[i].bl = glm::vec2((float)(x) / (float)m_tex->width, bottom);
			UVs[i].br = glm::vec2((float)(x + 16) / (float)m_tex->width, bottom);
			i++;
		}
	}
	glBindTexture(GL_TEXTURE_2D, m_tex->glTex);
}