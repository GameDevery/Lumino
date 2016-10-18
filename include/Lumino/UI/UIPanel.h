
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

/**
	@brief		1�ȏ�̎q UIElement ������ UI �v�f�̃x�[�X�N���X�ł��B

	@details	HAlignment �y�� VAlignment �̋K��l�� Stretch �ł��B
*/
class UIPanel
	: public UIElement
	, public tr::IUIElementCollectionOwner
{
	LN_UI_TYPEINFO_DECLARE();
public:
	void AddChild(UIElement* element);
	void RemoveChild(UIElement* element);

LN_PROTECTED_INTERNAL_ACCESS:
	UIPanel();
	virtual ~UIPanel();
	void Initialize(detail::UIManager* manager);
	UIElementCollection* GetChildren() const { return m_children; }

	// UIElement interface
	virtual int GetVisualChildrenCount() const override;
	virtual UIElement* GetVisualChildOrderd(int index) const override;
	virtual SizeF MeasureOverride(const SizeF& constraint) override;
	virtual SizeF ArrangeOverride(const SizeF& finalSize) override;

	// IUIElementCollectionOwner interface
	virtual void OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e) override;

private:
	RefPtr<UIElementCollection>	m_children;
};

/**
	@brief		
*/
class UIStackPanel
	: public UIPanel
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
	virtual SizeF MeasureOverride(const SizeF& constraint) override;
	virtual SizeF ArrangeOverride(const SizeF& finalSize) override;

private:
	Orientation	m_orientation;
};

/**
	@brief		
*/
class UICanvas
	: public UIPanel
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static UICanvasPtr Create();

LN_PROTECTED_INTERNAL_ACCESS:
	UICanvas();
	virtual ~UICanvas();
	void Initialize(detail::UIManager* manager);

	// UIElement interface
	virtual SizeF MeasureOverride(const SizeF& constraint) override;
	virtual SizeF ArrangeOverride(const SizeF& finalSize) override;

private:
};


/**
	@brief		
*/
class UIGridLayout
	: public UIPanel
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static UIGridLayoutPtr Create();

LN_PROTECTED_INTERNAL_ACCESS:
	UIGridLayout();
	virtual ~UIGridLayout();
	void Initialize(detail::UIManager* manager);

	// UIElement interface
	virtual SizeF MeasureOverride(const SizeF& constraint) override;
	virtual SizeF ArrangeOverride(const SizeF& finalSize) override;

private:
	class DefinitionBase;
	class ColumnDefinition;
	class RowDefinition;
	Array<RefPtr<ColumnDefinition>>	m_columnDefinitions;
	Array<RefPtr<RowDefinition>>	m_rowDefinitions;
};

LN_NAMESPACE_END
