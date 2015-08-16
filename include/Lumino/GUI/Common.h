
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

class Style;

class ContentPresenter;
class Button;
class ButtonChrome;
class Image;
class StackPanel;
class Grid;
class ColumnDefinition;
class RowDefinition;
class Thumb;
class ThumbChrome;
class Track;
class ScrollBar;
class ScrollContentPresenter;
class ScrollViewer;
class ItemList;
class ItemsControl;
class ItemsPresenter;
class GroupItem;
class ListBox;
class ListBoxChrome;
class ListBoxItem;

class AnimationClock;

class VisualState;
class VisualStateGroup;


typedef RefPtr<Style>					StylePtr;

typedef RefPtr<UIElement>				UIElementPtr;
typedef RefPtr<ContentPresenter>		ContentPresenterPtr;
typedef RefPtr<Button>					ButtonPtr;
typedef RefPtr<ButtonChrome>			ButtonChromePtr;
typedef RefPtr<Image>					ImagePtr;
typedef RefPtr<StackPanel>				StackPanelPtr;
typedef RefPtr<Grid>					GridPtr;
typedef RefPtr<ColumnDefinition>		ColumnDefinitionPtr;
typedef RefPtr<RowDefinition>			RowDefinitionPtr;
typedef RefPtr<Thumb>					ThumbPtr;
typedef RefPtr<ThumbChrome>				ThumbChromePtr;
typedef RefPtr<Track>					TrackPtr;
typedef RefPtr<ScrollBar>				ScrollBarPtr;
typedef RefPtr<ScrollContentPresenter>	ScrollContentPresenterPtr;
typedef RefPtr<ScrollViewer>			ScrollViewerPtr;
typedef RefPtr<ItemList>				ItemListPtr;
typedef RefPtr<ItemsControl>			ItemsControlPtr;
typedef RefPtr<ItemsPresenter>			ItemsPresenterPtr;
typedef RefPtr<GroupItem>				GroupItemPtr;
typedef RefPtr<ListBox>					ListBoxPtr;
typedef RefPtr<ListBoxChrome>			ListBoxChromePtr;
typedef RefPtr<ListBoxItem>				ListBoxItemPtr;

typedef RefPtr<VisualState>				VisualStatePtr;
typedef RefPtr<VisualStateGroup>		VisualStateGroupPtr;


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

/** �K��̃}�E�X�J�[�\���C���[�W�̎�ނ������܂��B*/
LN_ENUM(CommonCursorImage)
{
	Arrow = 0,			/**< ���J�[�\�� */
};
LN_ENUM_REFLECTION(CommonCursorImage, Arrow)
LN_ENUM_DECLARE(CommonCursorImage);


class IAddChild
{
public:
	virtual void AddChild(const Variant& value) = 0;
	virtual void AddText(const String& text) = 0;
};

} // namespace GUI
} // namespace Lumino
