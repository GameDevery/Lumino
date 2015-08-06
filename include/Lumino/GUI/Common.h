
#pragma once
#include "../Variant.h"

namespace Lumino
{
namespace GUI
{
class GUIManager;

class ResourceDictionary;
class UIElement;
class Control;
class RootFrame;

class Storyboard;

// internal
class CombinedLocalResource;
class VisualStateInstance;

typedef CoreObject* (*ObjectFactory)(GUIManager* manager);

/// �c�����̕\���ʒu�������܂��B
LN_ENUM(VerticalAlignment)
{
	Bottom = 0,			///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�̉��[�ɑ����Ĕz�u���܂��B
	Center,				///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�̒����ɑ����Ĕz�u���܂��B
	Stretch,			///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�S�̂Ɉ����L�΂��܂��B
	Top,				///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�̏�[�ɑ����Ĕz�u���܂��B
};
LN_ENUM_DECLARE(VerticalAlignment);

/// �������̕\���ʒu�������܂��B
LN_ENUM(HorizontalAlignment)
{
	Left = 0,			///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�̍����ɑ����Ĕz�u���܂��B
	Center,				///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�̒����ɑ����Ĕz�u���܂��B
	Right,				///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�̉E���ɑ����Ĕz�u���܂��B
	Stretch,			///< �q�v�f���A�e�̃��C�A�E�g �X���b�g�S�̂Ɉ����L�΂��܂��B
};
LN_ENUM_DECLARE(HorizontalAlignment);

/// �R���g���[���̃��C�A�E�g�����������܂��B
LN_ENUM(Orientation)
{
	Vertical = 0,		///< ���������ɔz�u���܂��B
	Horizontal,			///< ���������ɔz�u���܂��B
};
LN_ENUM_DECLARE(Orientation);



class IAddChild
{
public:
	virtual void AddChild(const Variant& value) = 0;
	virtual void AddText(const String& text) = 0;
};

} // namespace GUI
} // namespace Lumino
