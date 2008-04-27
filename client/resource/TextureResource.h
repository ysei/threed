#ifndef __TEXTURE_RESOURCE_H
#define __TEXTURE_RESOURCE_H

#include <resource/Resource.h>

class TextureResource : public Resource {
private:
	TextureResource(ResourceManager &m, const char *name);
	~TextureResource();

	virtual int LoadFromStorage();

public:


private:

	friend class Resource;
};

#endif