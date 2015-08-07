
#pragma once
#include "../UIElement.h"
#include "../ContentPresenter.h"
#include "ScrollBar.h"
#include "IScrollInfo.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class ScrollContentPresenter
	: public ContentPresenter
	, public IScrollInfo
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ScrollContentPresenter);
public:
	static ScrollContentPresenterPtr Create();

	ScrollContentPresenter(GUIManager* manager);
	virtual ~ScrollContentPresenter();

protected:
	// IScrollInfo override
	virtual void SetCanHorizontallyScroll(bool enabled) { m_scrollData.CanHorizontallyScroll = enabled; }
	virtual bool CanHorizontallyScroll() const { return m_scrollData.CanHorizontallyScroll; }
	virtual void SetCanVerticallyScroll(bool enabled) { m_scrollData.CanVerticallyScroll = enabled; }
	virtual bool CanVerticallyScroll() const { return m_scrollData.CanVerticallyScroll; }
	virtual float GetExtentWidth() const { return m_scrollData.Extent.Width; }
	virtual float GetExtentHeight() const { return m_scrollData.Extent.Height; }
	virtual float GetViewportWidth() const { return m_scrollData.Viewport.Width; }
	virtual float GetViewportHeight() const { return m_scrollData.Viewport.Height; }
	virtual void SetHorizontalOffset(float offset) { m_scrollData.Offset.X = offset; /*TODO: InvalidateArrange();*/ }
	virtual float GetHorizontalOffset() const { return m_scrollData.Offset.X; }
	virtual void SetVerticalOffset(float offset) { m_scrollData.Offset.Y = offset; /*TODO: InvalidateArrange();*/ }
	virtual float GetVerticalOffset() const { return m_scrollData.Offset.Y; }
	virtual void SetScrollOwner(ScrollViewer* owner) { m_scrollData.ScrollOwner = owner; }
	virtual void LineUp() { if (IsScrollClient()){ SetHorizontalOffset(GetHorizontalOffset() - m_scrollLineDelta); } }
	virtual void LineDown() { if (IsScrollClient()){ SetHorizontalOffset(GetHorizontalOffset() + m_scrollLineDelta); } }
	virtual void LineLeft()	{ if (IsScrollClient()){ SetVerticalOffset(GetVerticalOffset() - m_scrollLineDelta); } }
	virtual void LineRight() { if (IsScrollClient()){ SetVerticalOffset(GetVerticalOffset() + m_scrollLineDelta); } }

	// UIElement override
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);

private:
	bool IsScrollClient() const;
	void HookupScrollingComponents();

private:
	static const float m_scrollLineDelta;
	ScrollData		m_scrollData;
	IScrollInfo*	m_scrollInfo;		///< �r�W���A���̎q�v�f�� IScrollInfo �������ꍇ�͂�����w���B���Ȃ���� this ���w���B
};

/**
	@brief

	@par	ScrollViewer �R���g���[���̖��O�t���p�[�c
	PART_ScrollContentPresenter	ScrollContentPresenter	ScrollViewer �̃R���e���c�̃v���[�X�z���_�[�ł��B
	PART_VerticalScrollBar	ScrollBar	�R���e���c�𐂒������ɃX�N���[�����邽�߂Ɏg�p���� ScrollBar �ł��B
	PART_HorizontalScrollBar	ScrollBar	�R���e���c�𐅕������ɃX�N���[�����邽�߂Ɏg�p���� ScrollBar �ł��B
*/
class ScrollViewer
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ScrollViewer);
public:
	static const String PART_ScrollContentPresenterTemplateName;
	static const String PART_VerticalScrollBarTemplateName;
	static const String PART_HorizontalScrollBarTemplateName;

public:
	static ScrollViewerPtr Create();

	ScrollViewer(GUIManager* manager);
	virtual ~ScrollViewer();

	//-------------------------------------------------------------------
	/** @name Properties */
	/** @{ */


	void SetHorizontalOffset(float offset);

	void SetVerticalOffset(float offset);


	/** @} */

	/// �r���[�ɕ\�����Ă���͈͂̕����擾���܂��B(�ʏ�AActualSize - ScrollBarSize �ƂȂ�)
	float GetViewportWidth() const;

	/// �r���[�ɕ\�����Ă���͈͂̍������擾���܂��B(�ʏ�AActualSize - ScrollBarSize �ƂȂ�)
	float GetViewportHeight() const;

	/// �X�N���[���ł���R���e���c�v�f�̕����擾���܂��B(�ʏ�AExtentSize - ViewportSize �ƂȂ�)
	float GetScrollableWidth() const;

	/// �X�N���[���ł���R���e���c�v�f�̍������擾���܂��B(�ʏ�AExtentSize - ViewportSize �ƂȂ�)
	float GetScrollableHeight() const;

	/// �X�N���[�������������̃I�t�Z�b�g���擾���܂��B
	float GetHorizontalOffset() const;

	/// �X�N���[�������c�����̃I�t�Z�b�g���擾���܂��B
	float GetVerticalOffset() const;




	/// �X�N���[���ΏۂƂȂ� Panel �S�̂̃T�C�Y�B�q�v�f��ǉ����A���C�A�E�g���X�V����Ƃ��̒l���X�V�����B
	const Size& GetExtentSize() const;



	/// �R���e���c�� 1 �s�����ɃX�N���[�����܂��B
	void LineDown();

	/// �R���e���c�� 1 �񕪍��ɃX�N���[�����܂��B
	void LineLeft();

	/// �R���e���c�� 1 �񕪉E�ɃX�N���[�����܂��B
	void LineRight();

	/// �R���e���c�� 1 �s����ɃX�N���[�����܂��B
	void LineUp();

	/// �R���e���c�̖����܂Ő��������ɃX�N���[�����܂��B
	void ScrollToBottom();

	/// �R���e���c�̐擪�܂Ő��������ɃX�N���[�����܂��B
	void ScrollToLeftEnd();

	/// �R���e���c�̖����܂Ő��������ɃX�N���[�����܂��B
	void ScrollToRightEnd();

	/// �R���e���c�̐擪�܂Ő��������ɃX�N���[�����܂��B
	void ScrollToTop();

	/// �w�肳�ꂽ�����I�t�Z�b�g�ʒu�܂ŃX�N���[�����܂��B
	void ScrollToHorizontalOffset(double offset);

	/// �w�肳�ꂽ�����I�t�Z�b�g�ʒu�܂ŃX�N���[�����܂��B
	void ScrollToVerticalOffset(double offset);

protected:
	void OnLayoutUpdated();
	virtual void PollingTemplateChildCreated(UIElement* newElement);
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	//OnMouseWheel

private:
	friend class ScrollContentPresenter;
	void SetScrollInfo(IScrollInfo* scrollInfo);

	void Handler_ScrollBar_Scroll(ScrollEventArgs* e);


private:
	ScrollContentPresenter*	m_scrollContentPresenter;	///< VisualTree ���� ScrollContentPresenter
	ScrollBar*				m_verticalScrollBar;		///< VisualTree ���� VerticalScrollBar
	ScrollBar*				m_horizontalScrollBar;		///< VisualTree ���� HorizontalScrollBar
	IScrollInfo*			m_scrollInfo;				///< �X�N���[������Ώ�
};


} // namespace GUI
} // namespace Lumino
