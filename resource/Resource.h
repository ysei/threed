#ifndef __RESOURCE_H
#define __RESOURCE_H

#include <utils/IRefcounted.h>
#include <list>

enum ResourceType {
	RT_MESH,
	RT_TEXTURE,

	RT_MAX
};

class Resource : public IRefcounted
{
public:
	Resource(enum ResourceType type);

	ResourceType GetType() { return m_Type; }

	// from IRefcounted
	virtual int AddRef();
	virtual int RemoveRef();

private:
	int mRefCount;
	enum ResourceType m_Type;

protected:
	virtual ~Resource();
};

// helpful typedefs
typedef std::list<Resource*> ResourceList;
typedef std::list<Resource*>::iterator ResourceListIterator;
typedef std::list<Resource*>::const_iterator ResourceListConstIterator;

#endif
