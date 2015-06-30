
#pragma once
#include "../UIElement.h"

namespace Lumino
{
namespace GUI
{
class ScrollViewer;

/**
	@brief	ScrollViewer �R���g���[�����̃��C���̃X�N���[���\�̈��\���܂��B
*/
class IScrollInfo
{
public:
	//virtual bool CanVerticallyScroll() const = 0;
	//virtual bool CanHorizontallyScroll() const = 0;

	/// �G�N�X�e���g (�R���e���c�S��) �̉������擾���܂��B
	virtual float GetExtentWidth() const = 0;

	/// �G�N�X�e���g (�R���e���c�S��) �̏c�����擾���܂��B
	virtual float GetExtentHeight() const = 0;

	/// �R���e���c�ɑ΂�����ۂ̕\���̈�̉������擾���܂��B
	virtual float GetViewportWidth() const = 0;

	/// �R���e���c�ɑ΂�����ۂ̕\���̈�̏c�����擾���܂��B
	virtual float GetViewportHeight() const = 0;

	/// �X�N���[�������R���e���c�̐����I�t�Z�b�g���擾���܂��B
	virtual float GetHorizontalOffset() const = 0;

	/// �X�N���[�������R���e���c�̐����I�t�Z�b�g��ݒ肵�܂��B
	virtual void SetHorizontalOffset(float offset) const = 0;

	/// �X�N���[�������R���e���c�̐����I�t�Z�b�g���擾���܂��B
	virtual float GetVerticalOffset() const = 0;

	/// �X�N���[�������R���e���c�̐����I�t�Z�b�g��ݒ肵�܂��B
	virtual void SetVerticalOffset(float offset) const = 0;

	/// �X�N���[������𐧌䂷�� ScrollViewer �v�f��ݒ肵�܂��B
	virtual void SetScrollOwner(ScrollViewer* owner) const = 0;

	/// �R���e���c���� 1 �_���P�ʂ���ɃX�N���[�����܂��B
	virtual void LineUp() = 0;

	/// �R���e���c���� 1 �_���P�ʂ����ɃX�N���[�����܂��B
	virtual void LineDown() = 0;

	/// �R���e���c���� 1 �_���P�ʂ����ɃX�N���[�����܂��B
	virtual void LineLeft() = 0;

	/// �R���e���c���� 1 �_���P�ʂ��E�ɃX�N���[�����܂��B
	virtual void LineRight() = 0;
	
#if 0
	/// �R���e���c���� 1 �y�[�W����ɃX�N���[�����܂��B
	virtual void PageUp() = 0;

	/// �R���e���c���� 1 �y�[�W�����ɃX�N���[�����܂��B
	virtual void PageDown() = 0;

	/// �R���e���c���� 1 �y�[�W�����ɃX�N���[�����܂��B
	virtual void PageLeft() = 0;

	/// �R���e���c���� 1 �y�[�W���E�ɃX�N���[�����܂��B
	virtual void PageRight() = 0;

	/// �R���e���c�����}�E�X�z�C�[������ 1 �񕪁A��ɃX�N���[�����܂��B
	virtual void MouseWheelUp() = 0;

	/// �R���e���c�����}�E�X�z�C�[������ 1 �񕪁A���ɃX�N���[�����܂��B
	virtual void MouseWheelDown() = 0;

	/// �R���e���c�����}�E�X�z�C�[������ 1 �񕪁A���ɃX�N���[�����܂��B
	virtual void MouseWheelLeft() = 0;

	/// �R���e���c�����}�E�X�z�C�[������ 1 �񕪁A�E�ɃX�N���[�����܂��B
	virtual void MouseWheelRight() = 0;
#endif

	//Rect MakeVisible(UIElement visual, Rect rectangle);
};
	
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
	virtual Size MeasureOverride(Size availableSize);
	virtual Size ArrangeOverride(Size finalSize);
	//OnMouseWheel
};


} // namespace GUI
} // namespace Lumino
