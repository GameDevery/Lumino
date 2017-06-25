﻿
#pragma once
#include "../Mesh/Mesh.h"
#include "VisualNode.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
LN_CLASS()
class StaticMeshComponent
	: public VisualComponent
{
	LN_OBJECT();
public:

	static RefPtr<StaticMeshComponent> create(StaticMeshModel* staticMeshModel);

	static RefPtr<StaticMeshComponent> create(const StringRef& filePath);

	static RefPtr<StaticMeshComponent> createPlane(const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags = MeshCreationFlags::None);

	static RefPtr<StaticMeshComponent> createScreenPlane();

	/**
		@brief		サイズが 1.0 であるボックスのメッシュを作成します。
		@details	このメッシュは共有リソースです。
					頂点バッファやインデックスバッファを操作すると、このメソッドで作成したほかのメッシュの形状にも影響します。
					通常はこれらのリソースを変更するべきではありません。
	*/
	static RefPtr<StaticMeshComponent> createBox();

	/** 各軸に沿ったサイズを指定してボックスのメッシュを作成します。*/
	static RefPtr<StaticMeshComponent> createBox(const Vector3& size);

	/** 各軸に沿ったサイズを指定してボックスのメッシュを作成します。*/
	static RefPtr<StaticMeshComponent> createBox(float width, float height, float depth);
	
	/**
		@brief		半径が 0.5 である球のメッシュを作成します。
		@details	このメッシュは共有リソースです。
					頂点バッファやインデックスバッファを操作すると、このメソッドで作成したほかのメッシュの形状にも影響します。
					通常はこれらのリソースを変更するべきではありません。
	*/
	static RefPtr<StaticMeshComponent> createSphere();

	/** 半径と分割数を指定して球のメッシュを作成します。*/
	static RefPtr<StaticMeshComponent> createSphere(float radius, int tessellation = 16);

	/**
		@brief		サイズが 1.0 であるティーポットのメッシュを作成します。
		@details	このメッシュは共有リソースです。
					頂点バッファやインデックスバッファを操作すると、このメソッドで作成したほかのメッシュの形状にも影響します。
					通常はこれらのリソースを変更するべきではありません。
	*/
	static RefPtr<StaticMeshComponent> createTeapot();

	/** サイズとなめらかさを指定してメッシュを作成します。*/
	static RefPtr<StaticMeshComponent> createTeapot(float size, int tessellation = 8);

public:

	StaticMeshModel* getStaticMeshModel() const { return m_mesh; }

LN_INTERNAL_ACCESS:
	StaticMeshComponent();
	virtual ~StaticMeshComponent();
	void initialize(StaticMeshModel* meshModel);
	virtual void onRender2(RenderingContext* renderer) override;

private:
	RefPtr<StaticMeshModel>	m_mesh;
};



/**
	@brief		
*/
LN_CLASS()
class Rectangle
	: public VisualComponent		// TODO: Shape とか？
{
	LN_OBJECT();
public:

	static RefPtr<Rectangle> create(const Rect& rect);


LN_CONSTRUCT_ACCESS:
	Rectangle();
	virtual ~Rectangle();
	void initialize(const Rect& rect);
	virtual void onRender2(RenderingContext* renderer) override;

private:
	Rect	m_rect;
};

LN_NAMESPACE_END
