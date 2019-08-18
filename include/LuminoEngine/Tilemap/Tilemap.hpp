﻿
#pragma once
#include "../Scene/VisualObject.hpp"

namespace ln {
class TilemapComponent;

class Tilemap
	: public VisualObject
{
    LN_OBJECT;
public:
    static Ref<Tilemap> create();

    void setTilemapModel(TilemapModel* tilemapModel);

protected:

LN_CONSTRUCT_ACCESS:
    Tilemap();
	virtual ~Tilemap();
	void init();

private:
    Ref<TilemapComponent> m_component;
};

} // namespace ln
