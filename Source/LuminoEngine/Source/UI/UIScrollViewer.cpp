
#include "Internal.h"
#include <Lumino/UI/UIScrollViewer.h>
#include <Lumino/UI/UILayoutPanel.h>
#include "UIManager.h"
#include "EventArgsPool.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIDragDeltaEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIDragDeltaEventArgs, UIEventArgs)

//==============================================================================
// UIThumb
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIThumb, UIElement)
LN_ROUTED_EVENT_IMPLEMENT2(UIThumb, UIDragDeltaEventArgs, DragStartedEvent);
LN_ROUTED_EVENT_IMPLEMENT2(UIThumb, UIDragDeltaEventArgs, DragDeltaEvent);
LN_ROUTED_EVENT_IMPLEMENT2(UIThumb, UIDragDeltaEventArgs, DragCompletedEvent);
LN_ROUTED_EVENT_IMPLEMENT2(UIThumb, UIDragDeltaEventArgs, DragCanceledEvent);

//------------------------------------------------------------------------------
RefPtr<UIThumb> UIThumb::Create()
{
	auto ptr = RefPtr<UIThumb>::MakeRef();
	ptr->Initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UIThumb::UIThumb()
	: m_lastScreenPosition()
	, m_isDragging(false)
{
}

//------------------------------------------------------------------------------
UIThumb::~UIThumb()
{
}

//------------------------------------------------------------------------------
void UIThumb::Initialize()
{
	UIElement::Initialize();
}

//------------------------------------------------------------------------------
void UIThumb::OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e)
{
	if (ev == UIElement::MouseDownEvent)
	{
		if (!m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			PointF pos = mouseEvent->GetPosition(GetVisualParent());

			m_lastScreenPosition = pos;
			m_isDragging = true;
			CaptureMouse();

			// �h���b�O�J�n�C�x���g
			detail::EventArgsPool* pool = GetManager()->GetEventArgsPool();
			RefPtr<UIDragDeltaEventArgs> args(pool->Create<UIDragDeltaEventArgs>(), false);
			args->horizontalChange = pos.x - m_lastScreenPosition.x;
			args->verticalChange = pos.y - m_lastScreenPosition.y;
			OnDragStarted(args);

			e->handled = true;
			return;
		}
	}
	else if (ev == UIElement::MouseUpEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			PointF pos = mouseEvent->GetPosition(GetVisualParent());

			m_isDragging = false;
			ReleaseMouseCapture();

			// �h���b�O�I���C�x���g
			// TODO: template ��
			detail::EventArgsPool* pool = GetManager()->GetEventArgsPool();
			RefPtr<UIDragDeltaEventArgs> args(pool->Create<UIDragDeltaEventArgs>(), false);
			args->horizontalChange = pos.x - m_lastScreenPosition.x;
			args->verticalChange = pos.y - m_lastScreenPosition.y;
			OnDragCompleted(args);

			e->handled = true;
			return;
		}
	}
	else if (ev == UIElement::MouseMoveEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			PointF pos = mouseEvent->GetPosition(GetVisualParent());

			// �h���b�O���C�x���g
			detail::EventArgsPool* pool = GetManager()->GetEventArgsPool();
			RefPtr<UIDragDeltaEventArgs> args(pool->Create<UIDragDeltaEventArgs>(), false);
			args->horizontalChange = pos.x - m_lastScreenPosition.x;
			args->verticalChange = pos.y - m_lastScreenPosition.y;
			OnDragDelta(args);

			e->handled = true;
			return;
		}
	}

	UIElement::OnRoutedEvent(ev, e);
}

//==============================================================================
// UITrack
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UITrack, UIElement)

//------------------------------------------------------------------------------
RefPtr<UITrack> UITrack::Create()
{
	auto ptr = RefPtr<UITrack>::MakeRef();
	ptr->Initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UITrack::UITrack()
	: m_orientation(Orientation::Horizontal)
	, m_value(0.0f)
	, m_minimum(0.0f)
	, m_maximum(1.0f)
	, m_density(1.0f)
	, m_viewportSize(0.0f)
	, m_decreaseButton(nullptr)
	, m_thumb(nullptr)
	, m_increaseButton(nullptr)
{
}

//------------------------------------------------------------------------------
UITrack::~UITrack()
{
}

//------------------------------------------------------------------------------
void UITrack::Initialize()
{
	UIElement::Initialize();

	m_decreaseButton = NewObject<UIButton>();
	m_thumb = NewObject<UIThumb>();
	m_increaseButton = NewObject<UIButton>();

	m_decreaseButton->SetStyleSubControlName(tr::TypeInfo::GetTypeInfo<UITrack>()->GetName(), _T("DecreaseButton"));
	m_increaseButton->SetStyleSubControlName(tr::TypeInfo::GetTypeInfo<UITrack>()->GetName(), _T("IncreaseButton"));

	AddVisualChild(m_decreaseButton);
	AddVisualChild(m_thumb);
	AddVisualChild(m_increaseButton);
}

//------------------------------------------------------------------------------
float UITrack::ValueFromDistance(float horizontal, float vertical)
{
	float scale = 1;//IsDirectionReversed ? -1 : 1;

	if (m_orientation == Orientation::Horizontal)
	{
		return scale * horizontal * m_density;
	}
	else
	{
		return scale * vertical * m_density;
	}
}

//------------------------------------------------------------------------------
UIThumb* UITrack::GetThumb() const
{
	return m_thumb;
}

//------------------------------------------------------------------------------
Size UITrack::MeasureOverride(const Size& constraint)
{
	m_decreaseButton->MeasureLayout(constraint);
	m_thumb->MeasureLayout(constraint);
	m_increaseButton->MeasureLayout(constraint);
	return UIElement::MeasureOverride(constraint);
}

//------------------------------------------------------------------------------
Size UITrack::ArrangeOverride(const Size& finalSize)
{
	float decreaseButtonLength;
	float thumbLength;
	float increaseButtonLength;
	float trackLength = (m_orientation == Orientation::Vertical) ? finalSize.height : finalSize.width;

	if (Math::IsNaN(m_viewportSize))
	{
		// �r���[�T�C�Y���֌W�Ȃ��ꍇ�̌v�Z�B�܂�ASlider �R���g���[���p
		CalcSliderComponentsSize(finalSize, (m_orientation == Orientation::Vertical), &decreaseButtonLength, &thumbLength, &increaseButtonLength);
	}
	else
	{
		CalcScrollBarComponentsSize(trackLength, m_viewportSize, &decreaseButtonLength, &thumbLength, &increaseButtonLength);
	}

	if (m_orientation == Orientation::Horizontal)
	{
		Rect rect(0.0f, 0.0f, 0.0f, finalSize.height);

		// PageUp Button
		if (m_decreaseButton != nullptr)
		{
			rect.x = 0.0f;
			rect.width = decreaseButtonLength;
			m_decreaseButton->ArrangeLayout(rect);
		}
		// Thumb
		if (m_thumb != nullptr)
		{
			rect.x = decreaseButtonLength;
			rect.width = thumbLength;
			m_thumb->ArrangeLayout(rect);
		}
		// PageDown Button
		if (m_increaseButton != nullptr)
		{
			rect.x = decreaseButtonLength + thumbLength;
			rect.width = increaseButtonLength;
			m_increaseButton->ArrangeLayout(rect);
		}
	}
	else
	{
		Rect rect(0.0f, 0.0f, finalSize.width, 0.0f);

		// PageUp Button
		if (m_decreaseButton != nullptr)
		{
			rect.y = 0.0f;
			rect.height = decreaseButtonLength;
			m_decreaseButton->ArrangeLayout(rect);
		}
		// Thumb
		if (m_thumb != nullptr)
		{
			rect.y = decreaseButtonLength;
			rect.height = thumbLength;
			m_thumb->ArrangeLayout(rect);
		}
		// PageDown Button
		if (m_increaseButton != nullptr)
		{
			rect.y = decreaseButtonLength + thumbLength;
			rect.height = increaseButtonLength;
			m_increaseButton->ArrangeLayout(rect);
		}
	}

	return finalSize;
}

//------------------------------------------------------------------------------
void UITrack::CoerceLength(float& componentLength, float trackLength)
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

//------------------------------------------------------------------------------
void UITrack::CalcSliderComponentsSize(
	const Size& arrangeSize,
	bool isVertical,
	float* outDecreaseButtonLength,
	float* outThumbLength,
	float* outIncreaseButtonLength)
{
	float min = m_minimum;
	float range = std::max(0.0f, m_maximum - min);
	float offset = std::min(range, m_value - min);

	float trackLength;
	float thumbLength;

	if (isVertical)
	{
		trackLength = arrangeSize.height;
		thumbLength = (m_thumb == nullptr) ? 0 : m_thumb->GetLayoutDesiredSize().height;
	}
	else
	{
		trackLength = arrangeSize.width;
		thumbLength = (m_thumb == nullptr) ? 0 : m_thumb->GetLayoutDesiredSize().width;
	}

	CoerceLength(thumbLength, trackLength);

	float remainingTrackLength = trackLength - thumbLength;

	float decreaseButtonLength = remainingTrackLength * offset / range;
	CoerceLength(decreaseButtonLength, remainingTrackLength);

	float increaseButtonLength = remainingTrackLength - decreaseButtonLength;
	CoerceLength(increaseButtonLength, remainingTrackLength);


	*outDecreaseButtonLength = decreaseButtonLength;
	*outThumbLength = thumbLength;
	*outIncreaseButtonLength = increaseButtonLength;

	m_density = range / remainingTrackLength;
}

//------------------------------------------------------------------------------
void UITrack::CalcScrollBarComponentsSize(
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
	float offset = std::min(range, m_value - min);            // m_value �̈ʒu
	float extent = std::max(0.0f, range) + viewportSize;    // �R���e���c�S�̂̃T�C�Y

	//float trackLength = finalLength;
	float thumbMinLength = 16.0f;

	// Thumb �T�C�Y���v�Z����
	float thumbLength = trackLength * viewportSize / extent;    // �R���e���c�S�̂̓��A�ǂ̕�����\�����Ă���̂��A���̊����� Thumb �̒��������
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

//==============================================================================
// UIScrollEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIScrollEventArgs, UIEventArgs)

//------------------------------------------------------------------------------
UIScrollEventArgsPtr UIScrollEventArgs::Create(Object* sender, float newValue, ScrollEventType type, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::GetInstance()->GetEventArgsPool();
		RefPtr<UIScrollEventArgs> ptr(pool->Create<UIScrollEventArgs>(sender, newValue, type), false);
		return ptr;
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

//------------------------------------------------------------------------------
UIScrollEventArgs::UIScrollEventArgs()
	: newValue(0.0f)
	, type(ScrollEventType::ThumbTrack)
{
}

//------------------------------------------------------------------------------
UIScrollEventArgs::~UIScrollEventArgs()
{
}

//------------------------------------------------------------------------------
void UIScrollEventArgs::Initialize(Object* sender, float newValue_, ScrollEventType type_)
{
	sender = sender;
	newValue = newValue_;
	type = type_;
}

//==============================================================================
// UIScrollBar
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIScrollBar, UIControl)
LN_ROUTED_EVENT_IMPLEMENT2(UIScrollBar, UIScrollEventArgs, ScrollEvent);

const String UIScrollBar::OrientationStates = _T("OrientationStates");
const String UIScrollBar::HorizontalState = _T("Horizontal");
const String UIScrollBar::VerticalState = _T("Vertical");

//------------------------------------------------------------------------------
RefPtr<UIScrollBar> UIScrollBar::Create()
{
	auto ptr = RefPtr<UIScrollBar>::MakeRef();
	ptr->Initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UIScrollBar::UIScrollBar()
	: m_track(nullptr)
	, m_dragStartValue(0)
	, m_lineUpButton(nullptr)
	, m_lineDownButton(nullptr)
{
}

//------------------------------------------------------------------------------
UIScrollBar::~UIScrollBar()
{
}

//------------------------------------------------------------------------------
void UIScrollBar::Initialize()
{
	UIControl::Initialize();

	// register VisualState
	auto* vsm = GetVisualStateManager();
	vsm->RegisterVisualState(OrientationStates, HorizontalState);
	vsm->RegisterVisualState(OrientationStates, VerticalState);

	m_track = NewObject<UITrack>();
	m_lineUpButton = NewObject<UIButton>();
	m_lineDownButton = NewObject<UIButton>();
	AddVisualChild(m_track);
	AddVisualChild(m_lineUpButton);
	AddVisualChild(m_lineDownButton);

	m_lineUpButton->SetStyleSubControlName(tr::TypeInfo::GetTypeInfo<UIScrollBar>()->GetName(), _T("LineUpButton"));
	m_lineDownButton->SetStyleSubControlName(tr::TypeInfo::GetTypeInfo<UIScrollBar>()->GetName(), _T("LineDownButton"));

	// TODO:
	m_lineUpButton->SetSize(Size(16, 16));
	m_lineDownButton->SetSize(Size(16, 16));

	SetOrientation(Orientation::Horizontal);
}

//------------------------------------------------------------------------------
void UIScrollBar::SetOrientation(Orientation orientation)
{
	m_track->SetOrientation(orientation);

	switch (orientation)
	{
	case Orientation::Horizontal:
		GoToVisualState(HorizontalState);
		break;
	case Orientation::Vertical:
		GoToVisualState(VerticalState);
		break;
	default:
		LN_NOTIMPLEMENTED();
		break;
	}
}

//------------------------------------------------------------------------------
Orientation UIScrollBar::GetOrientation() const
{
	return m_track->GetOrientation();
}

//------------------------------------------------------------------------------
void UIScrollBar::SetValue(float value)
{
	m_track->SetValue(value);
}

//------------------------------------------------------------------------------
float UIScrollBar::GetValue() const
{
	return m_track->GetValue();
}

//------------------------------------------------------------------------------
void UIScrollBar::SetMinimum(float value)
{
	m_track->SetMinimum(value);
}

//------------------------------------------------------------------------------
float UIScrollBar::GetMinimum() const
{
	return m_track->GetMinimum();
}

//------------------------------------------------------------------------------
void UIScrollBar::SetMaximum(float value)
{
	m_track->SetMaximum(value);
}

//------------------------------------------------------------------------------
float UIScrollBar::GetMaximum() const
{
	return m_track->GetMaximum();
}

//------------------------------------------------------------------------------
void UIScrollBar::SetViewportSize(float value)
{
	m_track->SetViewportSize(value);
}

//------------------------------------------------------------------------------
float UIScrollBar::GetViewportSize() const
{
	return m_track->GetViewportSize();
}

//------------------------------------------------------------------------------
void UIScrollBar::OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e)
{
	if (ev == UIThumb::DragStartedEventId)
	{
		m_dragStartValue = m_track->GetValue();
	}
	else if (ev == UIThumb::DragDeltaEventId)
	{
		auto* e2 = static_cast<UIDragDeltaEventArgs*>(e);
		UpdateValue(e2->horizontalChange, e2->verticalChange);

		auto args = UIScrollEventArgs::Create(this, m_track->GetValue(), ScrollEventType::ThumbTrack);
		RaiseEvent(ScrollEventId, this, args);

		//switch (m_track->GetOrientation())
		//{
		//case Orientation::Horizontal:

		//	break;
		//case Orientation::Vertical:
		//	break;
		//case Orientation::ReverseHorizontal:
		//case Orientation::ReverseVertical:
		//default:
		//	LN_NOTIMPLEMENTED();
		//	break;
		//}
	}
	else if (ev == UIThumb::DragCompletedEventId)
	{
		auto args = UIScrollEventArgs::Create(this, m_track->GetValue(), ScrollEventType::EndScroll);
		RaiseEvent(ScrollEventId, this, args);
	}
	UIControl::OnRoutedEvent(ev, e);
}

//------------------------------------------------------------------------------
Size UIScrollBar::MeasureOverride(const Size& constraint)
{
	m_track->MeasureLayout(constraint);
	m_lineUpButton->MeasureLayout(constraint);
	m_lineDownButton->MeasureLayout(constraint);
	return UIControl::MeasureOverride(constraint);
}

//------------------------------------------------------------------------------
Size UIScrollBar::ArrangeOverride(const Size& finalSize)
{
	Size upSize;
	Size downSize;
	Orientation orientation = GetOrientation();

	switch (orientation)
	{
	case Orientation::Horizontal:
		upSize = m_lineUpButton->GetDesiredSize();
		downSize = m_lineUpButton->GetDesiredSize();
		m_lineUpButton->ArrangeLayout(Rect(0, 0, upSize.width, finalSize.height));
		m_lineDownButton->ArrangeLayout(Rect(finalSize.width - downSize.width, 0, downSize.width, finalSize.height));
		m_track->ArrangeLayout(Rect(upSize.width, 0, finalSize.width - upSize.width - downSize.width, finalSize.height));
		break;
	case Orientation::Vertical:
		m_track->ArrangeLayout(Rect(0, 0, finalSize));
		m_lineUpButton->ArrangeLayout(Rect(0, 0, finalSize.width, m_lineUpButton->GetDesiredSize().height));
		m_lineDownButton->ArrangeLayout(Rect(0, finalSize.height - m_lineDownButton->GetDesiredSize().height, finalSize.width, m_lineDownButton->GetDesiredSize().height));
		break;
	default:
		LN_NOTIMPLEMENTED();
		break;
	}

	return UIControl::ArrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
//void UIScrollBar::GetStyleClassName(String* outSubStateName)
//{
//	if (m_track->GetOrientation() == Orientation::Horizontal)
//		*outSubStateName = _T("Horizontal");
//	else
//		*outSubStateName = _T("Vertical");
//}

//------------------------------------------------------------------------------
void UIScrollBar::UpdateValue(float horizontalDragDelta, float verticalDragDelta)
{
	float valueDelta = m_track->ValueFromDistance(horizontalDragDelta, verticalDragDelta);

	float newValue = m_dragStartValue + valueDelta;
	m_track->SetValue(newValue);

	// TODO:
	//RefPtr<ScrollEventArgs> args(m_manager->GetEventArgsPool()->Create<ScrollEventArgs>(newValue, ScrollEventType::ThumbTrack), false);
	//RaiseEvent(ScrollEvent, this, args);
}

//==============================================================================
// UIScrollViewer
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIScrollViewer, UIControl)

//------------------------------------------------------------------------------
RefPtr<UIScrollViewer> UIScrollViewer::Create()
{
	auto ptr = RefPtr<UIScrollViewer>::MakeRef();
	ptr->Initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UIScrollViewer::UIScrollViewer()
	: m_horizontalScrollBar(nullptr)
	, m_verticalScrollBar(nullptr)
	, m_scrollTarget(nullptr)
{
}

//------------------------------------------------------------------------------
UIScrollViewer::~UIScrollViewer()
{
}

//------------------------------------------------------------------------------
void UIScrollViewer::Initialize()
{
	UIControl::Initialize();

	m_horizontalScrollBar = NewObject<UIScrollBar>();
	m_horizontalScrollBar->SetOrientation(Orientation::Horizontal);
	m_verticalScrollBar = NewObject<UIScrollBar>();
	m_verticalScrollBar->SetOrientation(Orientation::Vertical);
	AddVisualChild(m_horizontalScrollBar);
	AddVisualChild(m_verticalScrollBar);
}

//------------------------------------------------------------------------------
Size UIScrollViewer::MeasureOverride(const Size& constraint)
{
	Size desiredSize = UIControl::MeasureOverride(constraint);

	desiredSize.width += m_verticalScrollBar->GetWidth();
	desiredSize.height += m_horizontalScrollBar->GetHeight();

	return desiredSize;
}

//------------------------------------------------------------------------------
Size UIScrollViewer::ArrangeOverride(const Size& finalSize)
{
	float barWidth = m_verticalScrollBar->GetWidth();
	float barHeight = m_horizontalScrollBar->GetHeight();


	Size childArea(finalSize.width - barWidth, finalSize.height - barHeight);
	Size actualSize = UIControl::ArrangeOverride(childArea);


	Rect rc;

	rc.width = barWidth;
	rc.height = finalSize.height - barHeight;
	rc.x = finalSize.width - barWidth;
	rc.y = 0;
	m_verticalScrollBar->ArrangeLayout(rc);

	rc.width = finalSize.width - barWidth;
	rc.height = barHeight;
	rc.x = 0;
	rc.y = finalSize.height - barHeight;
	m_horizontalScrollBar->ArrangeLayout(rc);

	if (m_scrollTarget != nullptr)
	{
		m_horizontalScrollBar->SetMinimum(0.0f);
		m_horizontalScrollBar->SetMaximum(m_scrollTarget->GetExtentWidth());
		m_horizontalScrollBar->SetViewportSize(m_scrollTarget->GetViewportWidth());
		m_verticalScrollBar->SetMinimum(0.0f);
		m_verticalScrollBar->SetMaximum(m_scrollTarget->GetExtentHeight());
		m_verticalScrollBar->SetViewportSize(m_scrollTarget->GetViewportHeight());
	}
	else
	{
		m_horizontalScrollBar->SetMinimum(0.0f);
		m_horizontalScrollBar->SetMaximum(0.0f);
		m_horizontalScrollBar->SetViewportSize(0.0f);
		m_verticalScrollBar->SetMinimum(0.0f);
		m_verticalScrollBar->SetMaximum(0.0f);
		m_verticalScrollBar->SetViewportSize(0.0f);
	}

	return actualSize;
}

//------------------------------------------------------------------------------
void UIScrollViewer::OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e)
{
	UIControl::OnRoutedEvent(ev, e);

	if (ev == UIScrollBar::ScrollEventId)
	{
		auto* e2 = static_cast<UIScrollEventArgs*>(e);

		if (m_scrollTarget != nullptr)
		{
			if (e->sender == m_horizontalScrollBar)
			{
				m_scrollTarget->SetHorizontalOffset(e2->newValue);
			}
			else if (e->sender == m_verticalScrollBar)
			{
				m_scrollTarget->SetVerticalOffset(e2->newValue);
			}
		}
	}
}

//------------------------------------------------------------------------------
void UIScrollViewer::OnLayoutPanelChanged(UILayoutPanel* newPanel)
{
	m_scrollTarget = newPanel;
}


LN_NAMESPACE_END
