
#pragma once
#include "../UIElement.h"
#include "ButtonBase.h"
#include "Thumb.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
	@note	Value �̓r���[�T�C�Y (�s�N�Z���P��)
*/
class Track
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Track);
public:
	static PropertyID	ValueProperty;			///< �X�N���[���ʒu�ɑ΂���l
	static PropertyID	MinimumProperty;		///< �w��\�ȍŏ��l
	static PropertyID	MaximumProperty;		///< �w��\�ȍő�l
	static PropertyID	OrientationProperty;	///< Track �̕���
	static PropertyID	DecreaseButtonProperty;	///< �l������������ Button �R���g���[��
	static PropertyID	ThumbProperty;			///< �l�𑀍삷�� Thumb �R���g���[��
	static PropertyID	IncreaseButtonProperty;	///< �l�𑝉������� Button �R���g���[��

public:
	Track(GUIManager* manager);
	virtual ~Track();

	/// �X�N���[���ʒu�ɑ΂���l��ݒ肵�܂��B
	void SetValue(float value) { m_value = value; }

	/// �X�N���[���ʒu�ɑ΂���l���擾���܂��B�K��l�� 0 �ł��B
	float GetValue() const { return m_value; }

	/// �w��\�ȍŏ��l��ݒ肵�܂��B
	void SetMinimum(float value) { m_minimum = value; }

	/// �w��\�ȍŏ��l���擾���܂��B�K��l�� 0 �ł��B
	float GetMinimum() const { return m_minimum; }

	/// �w��\�ȍő�l��ݒ肵�܂��B
	void SetMaximum(float value) { m_maximum = value; }

	/// �w��\�ȍő�l���擾���܂��B�K��l�� 1 �ł��B
	float GetMaximum() const { return m_maximum; }

	/// Track �̕������w�肵�܂��B
	void SetOrientation(Orientation orientation) { m_orientation = orientation; }

	/// Track �̕������擾���܂��B�K��l�� Orientation::Horizontal �ł��B
	Orientation GetOrientation() const { return m_orientation; }

	/// �l������������ Button �R���g���[����ݒ肵�܂��B
	void SetDecreaseButton(ButtonBase* button);

	/// �l������������ Button �R���g���[�����擾���܂��B
	ButtonBase* GetDecreaseButton() const{ return m_decreaseButton; }

	/// �l�𑀍삷�� Thumb �R���g���[�����擾���܂��B
	Thumb* GetThumb() const { return m_thumb; }

	/// �l�𑀍삷�� Thumb �R���g���[����ݒ肵�܂��B
	void SetThumb(Thumb* thumb);

	/// �l�𑝉������� Button �R���g���[����ݒ肵�܂��B
	void SetIncreaseButton(ButtonBase* button);

	/// �l�𑝉������� Button �R���g���[�����擾���܂��B
	ButtonBase* GetIncreaseButton() const { return m_increaseButton; }

	/// �X�N���[���\�ȃR���e���c�̒��ŕ\������镔���̃T�C�Y��ݒ肵�܂��B
	void SetViewportSize(float size) { m_viewportSize = size; }

	/// �X�N���[���\�ȃR���e���c�̒��ŕ\������镔���̃T�C�Y���擾���܂��B
	float GetViewportSize() const { return m_viewportSize; }

protected:
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);
	virtual SizeF MeasureOverride(const SizeF& availableSize);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	virtual void Render();

private:
	void UpdateComponent(Control* oldValue, Control* newValue);
	void CoerceLength(float& componentLength, float trackLength);
	void CalcSliderComponentsSize(float trackLength, float* outDecreaseButtonLength, float* outThumbLength, float* outIncreaseButtonLength);
	void CalcScrollBarComponentsSize(float trackLength, float viewportSize, float* outDecreaseButtonLength, float* outThumbLength, float* outIncreaseButtonLength);

	void Thumb_DragDelta(DragEventArgs* e);

private:
	float				m_value;
	float				m_minimum;
	float				m_maximum;
	Orientation			m_orientation;
	RefPtr<ButtonBase>	m_decreaseButton;
	RefPtr<Thumb>		m_thumb;
	RefPtr<ButtonBase>	m_increaseButton;
	float				m_viewportSize;

	//Delegate01
};


} // namespace GUI
} // namespace Lumino
