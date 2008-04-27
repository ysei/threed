#ifndef __GEOMETRY_H
#define __GEOMETRY_H

#include <engine/Spatial.h>
#include <math/Vector3.h>
#include <resource/Resource.h>
#include <renderer/Mesh.h>
#include <renderer/IndexBuffer.h>
#include <renderer/VertexBuffer.h>

namespace Engine {

class Geometry : public Spatial {
public:
	Geometry();
	virtual ~Geometry();

	static Spatial *BuildFromResource(Resource *r);

	virtual void Render(Renderer *r);

	// temporary hack
	void SetDefaultGeometry();

	Mesh *m_Mesh;
};

}

#endif
