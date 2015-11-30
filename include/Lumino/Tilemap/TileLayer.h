
#pragma once

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief	
*/
class TileLayer
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	TileLayer();
	virtual ~TileLayer();
	void CreateCore(SceneGraphManager* manager);
	
	void SetTileId(int x, int y, int id);
	int GetTileId(int x, int y);

private:
	Array<int>	m_data;		// �}�b�v�f�[�^�{�́Bm_size.X*m_size.Y �̃T�C�Y�����B
	Size		m_size;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
