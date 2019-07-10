﻿
#pragma once
#include "../Graphics/GeometryStructs.hpp"
#include "Common.hpp"

namespace ln {
class RenderingContext;
class TilemapModel;

enum class TilemapOrientation
{
    UpFlow,
    DownFlow,
};

class AbstractTilemapLayer
	: public Object
{
public:

    //void resize(int width, int height);

    //void setTileId(int x, int y, int id);
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual int getTileId(int x, int y) const = 0;

    void setTileSize(const Size& size);

    void setOrientation(TilemapOrientation value) { m_orientation = value; }

public: // TODO: internal
    void render(TilemapModel* model, RenderingContext* context, const Matrix& transform, const detail::TilemapBounds& bounds);

LN_CONSTRUCT_ACCESS:
    AbstractTilemapLayer();
	virtual ~AbstractTilemapLayer();
	void init();
    //void init(int width, int height);

private:
    //List<int> m_data;
    //SizeI m_size;
    Size m_tileSize;
    TilemapOrientation m_orientation;
};

// タイルデータを内部に持つ AbstractTilemapLayer の実装
class TilemapLayer
    : public AbstractTilemapLayer
{
public:
    static Ref<TilemapLayer> create();

    void resize(int width, int height);

    void setTileId(int x, int y, int id);

    virtual int getWidth() const override { return m_size.width; }
    virtual int getHeight() const override { return m_size.height; }
    virtual int getTileId(int x, int y) const override;

LN_CONSTRUCT_ACCESS:
    TilemapLayer();
    virtual ~TilemapLayer();
    void init();
    void init(int width, int height);

private:
    List<int> m_data;
    SizeI m_size;
};

} // namespace ln