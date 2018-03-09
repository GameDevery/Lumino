
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN

/**
	@brief		������ UIElement ��_���I�Ȏq�v�f�Ƃ��Ēǉ��ł���R���g���[���ł��B
*/
class UIContentsControl
	: public UIControl
{
	LN_OBJECT;
public:
	void addChild(UIElement* element);
	void removeChild(UIElement* element);
	void clearChildren();

	// TODO: temp
	const Ref<UIElementCollection>& getItems() const { return m_logicalChildren; }

	//void setContent(UIElement* element);

protected:
	virtual int getLogicalChildrenCount() const override;
	virtual UIElement* getLogicalChild(int index) override;

	// UIElement interface
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UIContentsControl();
	virtual ~UIContentsControl();
	void initialize();
	//virtual void onUpdatingLayout() override;


private:
	Ref<UIElementCollection>	m_logicalChildren;
};

/**
	@brief	�����̃R���g���[����g�ݍ��킹���R���g���[�����쐬���邽�߂̃N���X�ł��B
*/
class UIUserControl
	: public UIContentsControl
{
	LN_OBJECT;
	
public:

	/** UIUserControl �̃C���X�^���X���쐬���܂��B */
	static Ref<UIUserControl> create();

LN_CONSTRUCT_ACCESS:
	UIUserControl();
	virtual ~UIUserControl();
	void initialize();
};

LN_NAMESPACE_END
