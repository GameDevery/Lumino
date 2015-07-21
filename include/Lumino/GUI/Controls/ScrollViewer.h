
#pragma once
#include "../UIElement.h"

namespace Lumino
{
namespace GUI
{
class ScrollViewer;

	
/**
	@brief
*/
class ScrollContentPresenter
	: public ContentPresenter
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ScrollContentPresenter);
public:
	ScrollContentPresenter(GUIManager* manager);
	virtual ~ScrollContentPresenter();

private:
	void HookupScrollingComponents();
};

/**
	@brief

	@par	ScrollViewer �R���g���[���̖��O�t���p�[�c
	PART_ScrollContentPresenter	ScrollContentPresenter	ScrollViewer �̃R���e���c�̃v���[�X�z���_�[�ł��B
	PART_HorizontalScrollBar	ScrollBar	�R���e���c�𐅕������ɃX�N���[�����邽�߂Ɏg�p���� ScrollBar �ł��B
	PART_VerticalScrollBar	ScrollBar	�R���e���c�𐂒������ɃX�N���[�����邽�߂Ɏg�p���� ScrollBar �ł��B
*/
class ScrollViewer
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ScrollViewer);
public:
	static ScrollViewer* Create(GUIManager* manager);

	ScrollViewer(GUIManager* manager);
	virtual ~ScrollViewer();

	//-------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

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
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	//OnMouseWheel
};


} // namespace GUI
} // namespace Lumino
