
#pragma once
#include "../UIElement.h"
#include "ButtonBase.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class Track
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Track);
public:
	static PropertyID	ValueProperty;			///< �X�N���[���ʒu�ɑ΂���l
	static PropertyID	MaximumProperty;		///< �w��\�ȍő�l
	static PropertyID	MinimumProperty;		///< �w��\�ȍŏ��l
	static PropertyID	OrientationProperty;	///< Track �̕���
	static PropertyID	DecreaseButtonProperty;	///< �l������������ Button �R���g���[��
	static PropertyID	IncreaseButtonProperty;	///< �l�𑝉������� Button �R���g���[��

public:
	Track(GUIManager* manager);
	virtual ~Track();

	/// �X�N���[���ʒu�ɑ΂���l��ݒ肵�܂��B
	void SetValue(double value) { m_value = value; }

	/// �X�N���[���ʒu�ɑ΂���l���擾���܂��B�K��l�� 0 �ł��B
	double GetValue() const { return m_value; }

	/// �w��\�ȍő�l��ݒ肵�܂��B
	void SetMaximum(double value) { m_maximum = value; }

	/// �w��\�ȍő�l���擾���܂��B�K��l�� 1 �ł��B
	double GetMaximum() const { return m_maximum; }

	/// �w��\�ȍŏ��l��ݒ肵�܂��B
	void SetMinimum(double value) { m_minimum = value; }

	/// �w��\�ȍŏ��l���擾���܂��B�K��l�� 0 �ł��B
	double GetMinimum() const { return m_minimum; }

	/// Track �̕������w�肵�܂��B
	void SetOrientation(Orientation orientation) { m_orientation = orientation; }

	/// Track �̕������擾���܂��B�K��l�� Orientation::Horizontal �ł��B
	Orientation GetOrientation() const { return m_orientation; }

	/// �l������������ Button �R���g���[����ݒ肵�܂��B
	void SetDecreaseButton(ButtonBase* button);

	/// �l������������ Button �R���g���[�����擾���܂��B
	ButtonBase* GetDecreaseButton() const{ return m_decreaseButton; }

	/// �l�𑝉������� Button �R���g���[����ݒ肵�܂��B
	void SetIncreaseButton(ButtonBase* button);

	/// �l�𑝉������� Button �R���g���[�����擾���܂��B
	ButtonBase* GetIncreaseButton() const { return m_increaseButton; }

protected:
	virtual Size ArrangeOverride(Size finalSize);
	virtual Size MeasureOverride(Size availableSize);

private:
	double		m_value;
	double		m_maximum;
	double		m_minimum;
	Orientation	m_orientation;
	RefPtr<ButtonBase>	m_decreaseButton;
	RefPtr<ButtonBase>	m_increaseButton;
};


} // namespace GUI
} // namespace Lumino
