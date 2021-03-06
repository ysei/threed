#ifndef __CAMERA_H
#define __CAMERA_H

#include <math/Vector3.h>
#include <math/Matrix4x4.h>
#include <engine/Spatial.h>

namespace Engine {

class Camera {
public:
	Camera();
	virtual ~Camera();

	void Update();
	void Render(Renderer *r);

	void SetPos(const Math::Vector3f &pos);
	void Move(const Math::Vector3f &trans);
	void Rotate(const Math::Vector3f &rot);
	void PrintPosition();

	void SetZoom(float zoom) { mZoom = zoom; }
	void Zoom(float zoom) { mZoom += zoom; }
	float GetZoom() const { return mZoom; }

	void LookAt(Spatial &obj);

protected:
	Math::Vector3f mPos;
	Math::Vector3f mUp;
	Math::Matrix4x4 mTransform;
	float mZoom;
	bool dirty;
};

}

#endif
