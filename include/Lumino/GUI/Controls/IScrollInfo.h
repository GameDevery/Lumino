
#pragma once

namespace Lumino
{
namespace GUI
{
class ScrollViewer;

/**
	@brief		ScrollViewer �R���g���[�����̃��C���̃X�N���[���\�̈��\���܂��B
	@details	Extent�AViewport�AOffset �̒P�ʂ̓X�N���[���Ώۂ̐ݒ�ɂ��قȂ�܂��B
				�Y�t�v���p�e�B ScrollViewer.CanContentScroll �� true �̏ꍇ�̓A�C�e���P�ʁA
				false �̏ꍇ�̓s�N�Z���P�ʂł��B
*/
class IScrollInfo
{
public:
	/// ��������ɃX�N���[���ł��邩�ǂ����������l��ݒ肵�܂��B
	/// @note	�X�N���[���o�[��\�����Ȃ��ݒ�ɂȂ��Ă�����A�����������݂��Ȃ��ꍇ�� false ���Z�b�g�����B
	///			IScrollInfo �̎����N���X�ŕs�v�Ȍv�Z���s��Ȃ��Ȃǂ̂��߂ɎQ�Ƃ���B
	virtual void SetCanHorizontallyScroll(bool enabled) = 0;

	/// ��������ɃX�N���[���ł��邩�ǂ����������l���擾���܂��B
	virtual bool CanHorizontallyScroll() const = 0;

	/// ��������ɃX�N���[���ł��邩�ǂ����������l��ݒ肵�܂��B
	virtual void SetCanVerticallyScroll(bool enabled) = 0;

	/// ��������ɃX�N���[���ł��邩�ǂ����������l���擾���܂��B
	virtual bool CanVerticallyScroll() const = 0;

	/// �G�N�X�e���g (�R���e���c�S��) �̉������擾���܂��B
	virtual float GetExtentWidth() const = 0;

	/// �G�N�X�e���g (�R���e���c�S��) �̏c�����擾���܂��B
	virtual float GetExtentHeight() const = 0;

	/// �R���e���c�ɑ΂�����ۂ̕\���̈�̉������擾���܂��B
	virtual float GetViewportWidth() const = 0;

	/// �R���e���c�ɑ΂�����ۂ̕\���̈�̏c�����擾���܂��B
	virtual float GetViewportHeight() const = 0;

	/// �X�N���[�������R���e���c�̐����I�t�Z�b�g��ݒ肵�܂��B
	virtual void SetHorizontalOffset(float offset) = 0;

	/// �X�N���[�������R���e���c�̐����I�t�Z�b�g���擾���܂��B
	virtual float GetHorizontalOffset() const = 0;

	/// �X�N���[�������R���e���c�̐����I�t�Z�b�g��ݒ肵�܂��B
	virtual void SetVerticalOffset(float offset) = 0;

	/// �X�N���[�������R���e���c�̐����I�t�Z�b�g���擾���܂��B
	virtual float GetVerticalOffset() const = 0;

	/// �X�N���[������𐧌䂷�� ScrollViewer �v�f��ݒ肵�܂��B
	virtual void SetScrollOwner(ScrollViewer* owner) = 0;

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

struct ScrollData
{
	ScrollViewer*	ScrollOwner;
	bool			CanHorizontallyScroll;
	bool			CanVerticallyScroll;
	PointF			Offset;
	SizeF			Extent;
	SizeF			Viewport;

	ScrollData()
		: ScrollOwner(NULL)
		, CanHorizontallyScroll(false)
		, CanVerticallyScroll(false)
		, Offset(PointF::Zero)
		, Extent(SizeF::Zero)
		, Viewport(SizeF::Zero)
	{}
};

} // namespace GUI
} // namespace Lumino
