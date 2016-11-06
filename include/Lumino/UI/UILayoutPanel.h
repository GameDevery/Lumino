
#pragma once
#include "UIElement.h"
#include "UIElementCollection.h"

LN_NAMESPACE_BEGIN
class UICanvas;
class UIGridLayout;
using UICanvasPtr = RefPtr<UICanvas>;
using UIGridLayoutPtr = RefPtr<UIGridLayout>;

/** �R���g���[���̃��C�A�E�g�����������܂��B*/
LN_ENUM(Orientation)
{
	Vertical = 0,		/**< ���������ɔz�u���܂��B*/
	Horizontal,			/**< ���������ɔz�u���܂��B*/
};
LN_ENUM_DECLARE(Orientation);

/** �O���b�h���C�A�E�g�̃Z���T�C�Y���w�肷��l�̎�ނł��B*/
enum class GridLengthType
{
	Auto,				/**< �q�v�f�̃T�C�Y�ɍ��킹�� */
	Pixel,				/**< �T�C�Y�𒼐ڎw�肷�� */
	Ratio,				/**< ���C�A�E�g��A�c��̗̈���g���� */
};

/**
	@brief		1�ȏ�̎q UIElement ������ UI �v�f�̃x�[�X�N���X�ł��B

	@details	HAlignment �y�� VAlignment �̋K��l�� Stretch �ł��B
*/
class UILayoutPanel
	: public UIElement
	, public tr::IUIElementCollectionOwner
{
	LN_UI_TYPEINFO_DECLARE();
public:
	void AddChild(UIElement* element);
	void RemoveChild(UIElement* element);

LN_PROTECTED_INTERNAL_ACCESS:
	UILayoutPanel();
	virtual ~UILayoutPanel();
	void Initialize(detail::UIManager* manager);
	UIElementCollection* GetChildren() const { return m_children; }

	// UIElement interface
	virtual int GetVisualChildrenCount() const override;
	virtual UIElement* GetVisualChildOrderd(int index) const override;
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

	// IUIElementCollectionOwner interface
	virtual void OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e) override;

private:
	RefPtr<UIElementCollection>	m_children;
};

/**
	@brief		
*/
class UIStackPanel
	: public UILayoutPanel
{
	LN_UI_TYPEINFO_DECLARE();
public:
	void SetOrientation(Orientation orientation) { m_orientation = orientation; }
	Orientation GetOrientation() const { return m_orientation; }

LN_PROTECTED_INTERNAL_ACCESS:
	UIStackPanel();
	virtual ~UIStackPanel();
	void Initialize(detail::UIManager* manager);

	// UIElement interface
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

private:
	Orientation	m_orientation;
};

/**
	@brief		
*/
class UICanvas
	: public UILayoutPanel
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static UICanvasPtr Create();

LN_PROTECTED_INTERNAL_ACCESS:
	UICanvas();
	virtual ~UICanvas();
	void Initialize(detail::UIManager* manager);

	// UIElement interface
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

private:
};


/**
	@brief		
*/
class UIGridLayout
	: public UILayoutPanel
{
	LN_UI_TYPEINFO_DECLARE();
public:

	/**
		@brief		UIGridLayout �I�u�W�F�N�g���쐬���܂��B
	*/
	static UIGridLayoutPtr Create();

	/**
		@brief		�w�肵�����̍s������� UIGridLayout �I�u�W�F�N�g���쐬���܂��B
		@details	�e�Z���̕��ƍ����� GridLengthType::Ratio �ƂȂ�܂��B
	*/
	static UIGridLayoutPtr Create(int columnCount, int rowCount);

public:

	void SetGridSize(int columnCount, int rowCount);
	void AddColumnDefinition(GridLengthType type = GridLengthType::Ratio, float width = 1.0f, float minWidth = 0.0f, float maxWidth = FLT_MAX);
	void AddRowDefinition(GridLengthType type = GridLengthType::Ratio, float height = 1.0f, float minHeight = 0.0f, float maxHeight = FLT_MAX);

LN_PROTECTED_INTERNAL_ACCESS:
	UIGridLayout();
	virtual ~UIGridLayout();
	void Initialize(detail::UIManager* manager);

	// UIElement interface
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

private:
	class DefinitionBase;
	class ColumnDefinition;
	class RowDefinition;
	List<RefPtr<ColumnDefinition>>	m_columnDefinitions;
	List<RefPtr<RowDefinition>>	m_rowDefinitions;
};

LN_NAMESPACE_END
