#include <iostream>
#include <engine/Spatial.h>

namespace Engine {

Spatial::Spatial()
:	mParent(0),
	mDirty(false)
{
	SetPos(Math::Vector3f(0, 0, 0));
	SetRotation(Math::Vector3f(0, 0, 0));
	SetScale(1.0f);
}

Spatial::~Spatial()
{
}

void Spatial::SetPos(const Math::Vector3f &pos)
{
	mLocalTranslate = pos;
	mDirty = true;
}

void Spatial::SetRotation(const Math::Vector3f &rot)
{
	// XXX make faster
	Math::Matrix4x4 rotx;
	Math::Matrix4x4 roty;
	Math::Matrix4x4 rotz;

	rotx.SetRotationX(rot.getx());
	roty.SetRotationY(rot.gety());
	rotz.SetRotationZ(rot.getz());

	mLocalRotationAngles = rot;
	mLocalRotation = rotx * roty * rotz;
	mDirty = true;
}

void Spatial::SetScale(float scale)
{
	mLocalScale = scale;
	mDirty = true;
}

Math::Vector3f Spatial::GetGlobalPos()
{
	if (mParent)
		return mLocalTranslate + mParent->GetGlobalPos();
	else
		return mLocalTranslate;
}

void Spatial::Move(const Math::Vector3f &trans)
{
	mLocalTranslate += trans;
	mDirty = true;
}

void Spatial::Rotate(const Math::Vector3f &rot)
{
	SetRotation(mLocalRotationAngles + rot);
	mDirty = true;
}

void Spatial::Scale(float scale)
{
	mLocalScale *= scale;
	mDirty = true;
}

void Spatial::UpdateTransform(bool force)
{
	if (mDirty || force)
		UpdateWorldMatrix();
}

void Spatial::UpdateWorldMatrix()
{
	// calculate the local transformation matrix
	Math::Matrix4x4 localTransform;

	// this is super lame
	Math::Matrix4x4 localTranslate;
	Math::Matrix4x4 localScaling;
	localTranslate.SetTranslation(mLocalTranslate);
	localScaling.SetScaling(Math::Vector3f(mLocalScale, mLocalScale, mLocalScale));

	localTransform = localTranslate * mLocalRotation * localScaling;

	if (mParent) {
		mWorldScale = mParent->mWorldScale * mLocalScale;
		mWorldTranslate = mParent->mWorldTranslate + mParent->mWorldScale * mParent->mWorldRotation.Transform(mLocalTranslate);
		mWorldRotation = mParent->mWorldRotation * mLocalRotation;
		mWorldTransform = mParent->mWorldTransform * localTransform; // the full transformation
	} else {
		// we're the top of the world
		mWorldScale = mLocalScale;
		mWorldTranslate = mLocalTranslate;
		mWorldRotation = mLocalRotation;
		mWorldTransform = localTransform;
	}
	mDirty = false;

//	std::cout << "UpdateWorldMatrix: mWorldTransform \n" << mWorldTransform << "\n";
}

void Spatial::PrintPosition()
{
	std::cout << mLocalTranslate << std::endl;
}

}

