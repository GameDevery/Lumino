
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief	
*/
class TileLayer
	: public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	TileLayer();
	virtual ~TileLayer();
	
	void Resize(int width, int height);
	const Size& GetSize() const { return m_size; }

	void SetTileId(int x, int y, int id);
	int GetTileId(int x, int y);

LN_INTERNAL_ACCESS:

private:
	Array<int>	m_data;		// �}�b�v�f�[�^�{�́Bm_size.X*m_size.Y �̃T�C�Y�����B
	Size		m_size;
};

LN_NAMESPACE_END
