
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
	LN_PROPERTY(float,			ValueProperty);				///< Value �v���p�e�B�̎��ʎq
	LN_PROPERTY(float,			MinimumProperty);			///< Minimum �v���p�e�B�̎��ʎq
	LN_PROPERTY(float,			MaximumProperty);			///< Maximum �v���p�e�B�̎��ʎq
	LN_PROPERTY(Orientation,	OrientationProperty);		///< Orientation �v���p�e�B�̎��ʎq
	LN_PROPERTY(ButtonBase*,	DecreaseButtonProperty);	///< DecreaseButton �v���p�e�B�̎��ʎq
	LN_PROPERTY(Thumb*,			ThumbProperty);				///< Thumb �v���p�e�B�̎��ʎq
	LN_PROPERTY(ButtonBase*,	IncreaseButtonProperty);	///< IncreaseButton �v���p�e�B�̎��ʎq

public:
	static TrackPtr Create();

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

public:
	float ValueFromDistance(float horizontal, float vertical);

protected:
	//virtual void ApplyTemplateHierarchy(/*CombinedLocalResource* parent*/);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);
	virtual SizeF MeasureOverride(const SizeF& availableSize);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	//virtual void Render();

private:
	void UpdateComponent(Control* oldValue, Control* newValue);
	void CoerceLength(float& componentLength, float trackLength);
	void CalcSliderComponentsSize(float trackLength, float* outDecreaseButtonLength, float* outThumbLength, float* outIncreaseButtonLength);
	void CalcScrollBarComponentsSize(float trackLength, float viewportSize, float* outDecreaseButtonLength, float* outThumbLength, float* outIncreaseButtonLength);

	void OnDecreaseButtonPropertyChanged(PropertyChangedEventArgs* e);
	void OnThumbPropertyChanged(PropertyChangedEventArgs* e);
	void OnIncreaseButtonPropertyChanged(PropertyChangedEventArgs* e);

#if 0
	void Handler_Thumb_DragStarted(DragEventArgs* e);
	void Handler_Thumb_DragDelta(DragEventArgs* e);
#endif

private:
	float				m_value;
	float				m_minimum;
	float				m_maximum;
	Orientation			m_orientation;
	RefPtr<ButtonBase>	m_decreaseButton;
	RefPtr<Thumb>		m_thumb;
	RefPtr<ButtonBase>	m_increaseButton;
	float				m_viewportSize;
	float				m_density;
#if 0
	float				m_dragStartValue;	///< �h���b�O�J�n���� m_value
#endif

	//Delegate01
};


} // namespace GUI
} // namespace Lumino
