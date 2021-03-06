﻿
#pragma once
#include "../../Graphics/GeometryStructs.hpp"
#include "../../Visual/VisualComponent.hpp"

namespace ln {


class PlaneMeshComponent
    : public VisualComponent
{
    LN_OBJECT;
public:
    void setSize(const Vector2& size) { m_size = size; }
    void setSize(float sizeX, float sizeY) { setSize(Vector2(sizeX, sizeY)); }

    void setUVParUnit(const Vector2& value) { m_uvParUnit = value; }

    void setMaterial(Material* material);
    Material* material() const;

protected:
    void serialize(Serializer2& ar) override;
    void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    PlaneMeshComponent();
    virtual ~PlaneMeshComponent();
    void init();

private:
    Vector2 m_size;
    //float m_sizeX;
    //float m_sizeY;
    Vector2 m_uvParUnit;
    Ref<Material> m_material;
};

class SphereMeshComponent
	: public VisualComponent
{
public:
    void setMaterial(Material* material);

protected:
    void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    SphereMeshComponent();
    virtual ~SphereMeshComponent();
	void init();

private:
    Ref<Material> m_material;
};

class BoxMeshComponent
	: public VisualComponent
{
public:
	void setMaterial(Material* material);

protected:
	void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
	BoxMeshComponent();
	virtual ~BoxMeshComponent();
	void init(const Vector3& size);

private:
	Ref<Material> m_material;
	Box m_box;
};

} // namespace ln
