
/*
	DecreaseButton �� IncreaseButton �� WPF �ł�
	<Track>
		<Track.DecreaseButton>
			<RepertButton />
		</Track.DecreaseButton>
		<Track.IncreaseButton>
			<RepertButton />
		</Track.IncreaseButton>
	</Track>
	�̂悤�Ƀv���p�e�B�ɒ��ړ���Ă���B

	������������
	PART_DecreaseButton �̂悤�Ȗ��O�����Č������Ă��ǂ���������Ȃ��B
*/

#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/Track.h>

namespace Lumino
{

namespace GUI
{

//=============================================================================
// Track
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Track);
LN_UI_ELEMENT_SUBCLASS_IMPL(Track);

PropertyID	Track::ValueProperty(_T("Value"));
PropertyID	Track::MinimumProperty(_T("Minimum"));
PropertyID	Track::MaximumProperty(_T("Maximum"));
PropertyID	Track::OrientationProperty(_T("Orientation"));
PropertyID	Track::DecreaseButtonProperty(_T("DecreaseButton"));
PropertyID	Track::ThumbProperty(_T("Thumb"));
PropertyID	Track::IncreaseButtonProperty(_T("IncreaseButton"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Track::Track(GUIManager* manager)
	: Control(manager)
	, m_value(0.0f)
	, m_minimum(0.0f)
	, m_maximum(1.0f)
	, m_orientation(Orientation::Horizontal)
	, m_decreaseButton()
	, m_thumb()
	, m_increaseButton()
	, m_viewportSize(0.0f)
{	
	// Register property
	LN_DEFINE_PROPERTY		(Track, float, ValueProperty, &Track::SetValue, &Track::GetValue, 0.0f);
	LN_DEFINE_PROPERTY		(Track, float, MinimumProperty, &Track::SetMinimum, &Track::GetMinimum, 0.0f);
	LN_DEFINE_PROPERTY		(Track, float, MaximumProperty, &Track::SetMaximum, &Track::GetMaximum, 1.0f);
	LN_DEFINE_PROPERTY_ENUM	(Track, Orientation, OrientationProperty, &Track::SetOrientation, &Track::GetOrientation, Orientation::Horizontal);
	LN_DEFINE_PROPERTY		(Track, ButtonBase*, DecreaseButtonProperty, &Track::SetDecreaseButton, &Track::GetDecreaseButton, NULL);
	LN_DEFINE_PROPERTY		(Track, Thumb*, ThumbProperty, &Track::SetThumb, &Track::GetThumb, NULL);
	LN_DEFINE_PROPERTY		(Track, ButtonBase*, IncreaseButtonProperty, &Track::SetIncreaseButton, &Track::GetIncreaseButton, NULL);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Track::~Track()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::SetDecreaseButton(ButtonBase* button)
{
	m_decreaseButton = button;
	AddVisualChild(button);		// VisualChild �Ƃ��Ă������Ƃ� OnRender() ���Ă΂��悤�ɂȂ�
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::SetThumb(Thumb* thumb)
{
	m_thumb = thumb;
	AddVisualChild(thumb);		// VisualChild �Ƃ��Ă������Ƃ� OnRender() ���Ă΂��悤�ɂȂ�
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::SetIncreaseButton(ButtonBase* button)
{
	m_increaseButton = button;
	AddVisualChild(button);		// VisualChild �Ƃ��Ă������Ƃ� OnRender() ���Ă΂��悤�ɂȂ�
}

//-----------------------------------------------------------------------------
// Note: �����ł͎q�v�f�� Measure ���Ȃ�
//-----------------------------------------------------------------------------
void Track::MeasureLayout(const SizeF& availableSize)
{
	m_desiredSize = MeasureOverride(availableSize);
}

//-----------------------------------------------------------------------------
// Note: �����ł͎q�v�f�� Arrange ���Ȃ�
//-----------------------------------------------------------------------------
void Track::ArrangeLayout(const RectF& finalRect)
{
	SizeF renderSize = ArrangeOverride(finalRect.GetSize());
	m_finalRect.Width = renderSize.Width;
	m_finalRect.Height = renderSize.Height;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF Track::MeasureOverride(const SizeF& availableSize)
{
	SizeF desiredSize(0.0, 0.0);

	if (!m_thumb.IsNull())
	{
		m_thumb->MeasureLayout(availableSize);
		desiredSize = m_thumb->GetDesiredSize();	// Thumb �͂��̃T�C�Y��v�����Ă���B(���[�U�[�����ڃT�C�Y�������������ꍇ��)
	}

	if (!Math::IsNaN(m_viewportSize))
	{
		if (m_orientation == Orientation::Vertical)
			desiredSize.Height = 0.0;	// �c�����͌�̌v�Z�ɔC����
		else
			desiredSize.Width = 0.0;	// �������͌�̌v�Z�ɔC����
	}

	return desiredSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF Track::ArrangeOverride(const SizeF& finalSize)
{
	float decreaseButtonLength;
	float thumbLength;
	float increaseButtonLength;
	float trackLength = (m_orientation == Orientation::Vertical) ? finalSize.Height : finalSize.Width;

	if (Math::IsNaN(m_viewportSize))
	{
		// �r���[�T�C�Y���֌W�Ȃ��ꍇ�̌v�Z�B�܂�ASlider �R���g���[���p
		//CalcSliderComponentsSize
	}
	else
	{
		// �r���[�T�C�Y���֌W�Ȃ��ꍇ�̌v�Z�B�܂�ASlider �R���g���[���p
		CalcScrollBarComponentsSize(trackLength, m_viewportSize, &decreaseButtonLength, &thumbLength, &increaseButtonLength);
	}

	if (m_orientation == Orientation::Horizontal)
	{
		RectF rect(0.0f, 0.0f, 0.0f, finalSize.Height);

		// �f�N�������g�{�^��
		if (m_decreaseButton != NULL)
		{
			rect.X = 0.0f;
			rect.Width = decreaseButtonLength;
			m_decreaseButton->ArrangeLayout(rect);
		}
		// �܂�
		if (m_thumb != NULL)
		{
			rect.X = decreaseButtonLength;
			rect.Width = thumbLength;
			m_thumb->ArrangeLayout(rect);
		}
		// �C���N�������g�{�^��
		if (m_increaseButton != NULL)
		{
			rect.X = decreaseButtonLength + thumbLength;
			rect.Width = increaseButtonLength;
			m_increaseButton->ArrangeLayout(rect);
		}
	}

	return finalSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::CoerceLength(float& componentLength, float trackLength)
{
	if (componentLength < 0)
	{
		componentLength = 0.0f;
	}
	else if (componentLength > trackLength || Math::IsNaN(componentLength))
	{
		componentLength = trackLength;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::CalcScrollBarComponentsSize(
	float trackLength,
	float viewportSize, 
	float* outDecreaseButtonLength, 
	float* outThumbLength,
	float* outIncreaseButtonLength)
{
	/*
	 ViewportSize �Ɏg�p�����P�ʂ́A�R���e���c�̒������L�q���邽�߂Ɏg�p�����P�ʂƓ����ł��B
	 https://msdn.microsoft.com/ja-jp/library/system.windows.controls.primitives.scrollbar.viewportsize%28v=vs.110%29.aspx
	
	https://msdn.microsoft.com/ja-jp/library/system.windows.controls.primitives.track.maximum%28v=vs.110%29.aspx
	*/

	float min = m_minimum;
	float range = std::max(0.0f, m_maximum - min);
	float offset = std::min(range, m_value - min);			// m_value �̈ʒu
	float extent = std::max(0.0f, range) + viewportSize;	// �R���e���c�S�̂̃T�C�Y

	//float trackLength = finalLength;
	float thumbMinLength = 16.0f;

	// Thumb �T�C�Y���v�Z����
	float thumbLength = trackLength * viewportSize / extent;	// �R���e���c�S�̂̓��A�ǂ̕�����\�����Ă���̂��A���̊����� Thumb �̒��������
	CoerceLength(thumbLength, trackLength);
	thumbLength = std::max(thumbMinLength, thumbLength);

	// �c��̕����̃T�C�Y
	float remainingTrackLength = trackLength - thumbLength;

	// DecreaseButton
	float decreaseButtonLength = remainingTrackLength * offset / range;
	CoerceLength(decreaseButtonLength, remainingTrackLength);

	// IncreaseButton
	float increaseButtonLength = remainingTrackLength - decreaseButtonLength;
	CoerceLength(increaseButtonLength, remainingTrackLength);

	*outDecreaseButtonLength = decreaseButtonLength;
	*outThumbLength = thumbLength;
	*outIncreaseButtonLength = increaseButtonLength;
}

} // namespace GUI
} // namespace Lumino

