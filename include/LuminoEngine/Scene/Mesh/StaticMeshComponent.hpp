﻿
#pragma once
#include "../../Visual/VisualComponent.hpp"

namespace ln {
class StaticMeshModel;

class StaticMeshComponent
	: public VisualComponent
{
public:
    void setModel(StaticMeshModel* model);
    StaticMeshModel* model() const;

protected:
    void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    StaticMeshComponent();
	virtual ~StaticMeshComponent();
	void init();

private:
    Ref<StaticMeshModel> m_model;
};

} // namespace ln