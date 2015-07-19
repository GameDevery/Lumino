
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
LN_CORE_OBJECT_TYPE_INFO_IMPL(Track, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(Track);


LN_DEFINE_PROPERTY_2		(Track, float, ValueProperty, "Value", 0.0f, &Track::SetValue, &Track::GetValue);
LN_DEFINE_PROPERTY_2		(Track, float, MinimumProperty, "Minimum", 0.0f, &Track::SetMinimum, &Track::GetMinimum);
LN_DEFINE_PROPERTY_2		(Track, float, MaximumProperty, "Maximum", 1.0f, &Track::SetMaximum, &Track::GetMaximum);
LN_DEFINE_PROPERTY_ENUM_2	(Track, Orientation, OrientationProperty,"Orientation", Orientation::Horizontal, &Track::SetOrientation, &Track::GetOrientation);
LN_DEFINE_PROPERTY_2		(Track, ButtonBase*, DecreaseButtonProperty, "DecreaseButton", NULL, &Track::SetDecreaseButton, &Track::GetDecreaseButton);
LN_DEFINE_PROPERTY_2		(Track, Thumb*, ThumbProperty, "Thumb", NULL, &Track::SetThumb, &Track::GetThumb);
LN_DEFINE_PROPERTY_2		(Track, ButtonBase*, IncreaseButtonProperty, "IncreaseButton", NULL, &Track::SetIncreaseButton, &Track::GetIncreaseButton);

//PropertyID	Track::ValueProperty(_T("Value"));
//PropertyID	Track::MinimumProperty(_T("Minimum"));
//PropertyID	Track::MaximumProperty(_T("Maximum"));
//PropertyID	Track::OrientationProperty(_T("Orientation"));
//PropertyID	Track::DecreaseButtonProperty(_T("DecreaseButton"));
//PropertyID	Track::ThumbProperty(_T("Thumb"));
//PropertyID	Track::IncreaseButtonProperty(_T("IncreaseButton"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Track* Track::Create(GUIManager* manager)
{
	auto obj = RefPtr<Track>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

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
	, m_density(1.0f)
{	
	// Register property
	//LN_DEFINE_PROPERTY		(Track, float, ValueProperty, &Track::SetValue, &Track::GetValue, 0.0f);
	//LN_DEFINE_PROPERTY		(Track, float, MinimumProperty, &Track::SetMinimum, &Track::GetMinimum, 0.0f);
	//LN_DEFINE_PROPERTY		(Track, float, MaximumProperty, &Track::SetMaximum, &Track::GetMaximum, 1.0f);
	//LN_DEFINE_PROPERTY_ENUM	(Track, Orientation, OrientationProperty, &Track::SetOrientation, &Track::GetOrientation, Orientation::Horizontal);
	//LN_DEFINE_PROPERTY		(Track, ButtonBase*, DecreaseButtonProperty, &Track::SetDecreaseButton, &Track::GetDecreaseButton, NULL);
	//LN_DEFINE_PROPERTY		(Track, Thumb*, ThumbProperty, &Track::SetThumb, &Track::GetThumb, NULL);
	//LN_DEFINE_PROPERTY		(Track, ButtonBase*, IncreaseButtonProperty, &Track::SetIncreaseButton, &Track::GetIncreaseButton, NULL);

	// Register handler
	LN_REGISTER_ROUTED_EVENT_HANDLER(Track, DragEventArgs, Thumb::DragStartedEvent, Handler_Thumb_DragStarted);
	LN_REGISTER_ROUTED_EVENT_HANDLER(Track, DragEventArgs, Thumb::DragDeltaEvent, Handler_Thumb_DragDelta);
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
	UpdateComponent(m_decreaseButton, button);
	m_decreaseButton = button;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::SetThumb(Thumb* thumb)
{
	UpdateComponent(m_thumb, thumb);
	m_thumb = thumb;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::SetIncreaseButton(ButtonBase* button)
{
	UpdateComponent(m_increaseButton, button);
	m_increaseButton = button;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::ApplyTemplateHierarchy(/*CombinedLocalResource* parent*/)
{
	Control::ApplyTemplateHierarchy(/*parent*/);
	if (m_decreaseButton != NULL) { m_decreaseButton->ApplyTemplate(); }
	if (m_thumb != NULL) { m_thumb->ApplyTemplate(); }
	if (m_increaseButton != NULL) { m_increaseButton->ApplyTemplate(); }
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
void Track::ArrangeLayout(const RectF& finalLocalRect)
{
	SizeF renderSize = ArrangeOverride(finalLocalRect.GetSize());
	m_finalLocalRect.X = finalLocalRect.X;
	m_finalLocalRect.Y = finalLocalRect.Y;
	m_finalLocalRect.Width = renderSize.Width;
	m_finalLocalRect.Height = renderSize.Height;
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
	else
	{
		RectF rect(0.0f, 0.0f, finalSize.Width, 0.0f);

		// �f�N�������g�{�^��
		if (m_decreaseButton != NULL)
		{
			rect.Y = 0.0f;
			rect.Height = decreaseButtonLength;
			m_decreaseButton->ArrangeLayout(rect);
		}
		// �܂�
		if (m_thumb != NULL)
		{
			rect.Y = decreaseButtonLength;
			rect.Height = thumbLength;
			m_thumb->ArrangeLayout(rect);
		}
		// �C���N�������g�{�^��
		if (m_increaseButton != NULL)
		{
			rect.Y = decreaseButtonLength + thumbLength;
			rect.Height = increaseButtonLength;
			m_increaseButton->ArrangeLayout(rect);
		}
	}

	return finalSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::Render()
{
	Control::Render();
	if (m_decreaseButton != NULL) { m_decreaseButton->Render(); }
	if (m_thumb != NULL) { m_thumb->Render(); }
	if (m_increaseButton != NULL) { m_increaseButton->Render(); }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::UpdateComponent(Control* oldValue, Control* newValue)
{
	if (oldValue != NULL) {
		m_visualChildren.Remove(oldValue);
	}
	if (newValue != NULL) {
		m_visualChildren.Add(newValue);
	}
	if (newValue != NULL) {
		newValue->SetParent(this);
		newValue->SetTemplateModified(true);
	}
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

	m_density = range / remainingTrackLength;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::Handler_Thumb_DragStarted(DragEventArgs* e)
{
	m_dragStartValue = m_value;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::Handler_Thumb_DragDelta(DragEventArgs* e)
{
	if (m_orientation == Orientation::Horizontal) {
		m_value = m_dragStartValue + e->XOffset * m_density;
	}
	else {
		m_value = m_dragStartValue + e->YOffset * m_density;
	}

	if (m_value < m_minimum) { m_value = m_minimum; }
	if (m_value > m_maximum) { m_value = m_maximum; }
}

} // namespace GUI
} // namespace Lumino

