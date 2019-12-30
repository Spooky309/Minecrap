#include "Texture.h"
std::shared_ptr<Texture> TextureLoader::LoadTexture(const std::string& path) {
	for (auto it = loadedtexs.begin(); it < loadedtexs.end(); it++) {
		std::shared_ptr<Texture> t = *it;
		if (path == t->p) {
			return t;
		}
	}
	std::shared_ptr<Texture> newt = std::make_shared<Texture>(path);
	loadedtexs.push_back(newt);
	return newt;
}
