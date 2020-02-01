﻿
#pragma once
#include "../VisualObject.hpp"

namespace ln {
class BoxMeshComponent;

class BoxMesh
	: public VisualObject
{
public:
	static Ref<BoxMesh> create();

	BoxMeshComponent* boxMeshComponent() const;

protected:

LN_CONSTRUCT_ACCESS:
	BoxMesh();
	virtual ~BoxMesh();
	void init();

private:
    Ref<BoxMeshComponent> m_component;
};

} // namespace ln
