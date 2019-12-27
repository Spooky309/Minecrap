#include "Texture.h"
std::shared_ptr<texture> textureloader::load_tex(const std::string& path) {
	for (auto it = loadedtexs.begin(); it < loadedtexs.end(); it++) {
		std::shared_ptr<texture> t = *it;
		if (path == t->p) {
			return t;
		}
	}
	std::shared_ptr<texture> newt = std::make_shared<texture>(path);
	loadedtexs.push_back(newt);
	return newt;
}
