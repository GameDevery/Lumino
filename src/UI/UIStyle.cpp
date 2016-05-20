/*
	[2016/5/13] m_image �ɂ���
		WPF �Ƃ��ɂ͗p�ӂ���Ă��Ȃ����A�Q�[������UI�Ƃ��Ă͗p�ӂ��Ă������ق����֗��Ȃ̂Ŏ����������́B
		��ʓI��GUI�t���[�����[�N�͐F����̎�ނŃX�^�C����\�����邪�A�Q�[���ł͉摜���g�����Ƃ������B
		�����A1���̉摜�����Ńt�F�[�h�Ȃǂ̑J�ڂ��s���ƁA�����x�̃u�����h���Ɉ�u���̃I�u�W�F�N�g�������Č����邱�ƂɂȂ�B
		�w�i�ƑO�i2��ޗp�ӂ��A�w�i�͊�{�I�ɓ����x��ύX���Ȃ��A�O�i��VisualState�̕ύX�ɍ��킹�Ă��낢��ς��悤�ɂ���ƈ��肵�Č�����B

		���Ȃ݂�Qt����image�v���p�e�B������B
		http://doc.qt.io/qt-4.8/stylesheet-examples.html
*/
#include "Internal.h"
#include <Lumino/UI/UIStyle.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIStyle
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIStyle, Object);

//------------------------------------------------------------------------------
UIStylePtr UIStyle::Create()
{
	return UIStylePtr::MakeRef();
}

//------------------------------------------------------------------------------
UIStyle::UIStyle()
	//: m_lastUpdateParent(nullptr)
	//m_revisionCount(0)
	: m_margin(ThicknessF(0, 0, 0, 0))
	, m_padding(ThicknessF(0, 0, 0, 0))
	, m_verticalAlignment(VerticalAlignment::Top)
	, m_horizontalAlignment(HorizontalAlignment::Left)
	, m_background(nullptr)
	, m_foreground(nullptr)
	, m_fontFamily(String::GetEmpty())
	, m_fontSize(0)
	, m_fontBold(false)
	, m_fontItalic(false)
{
}

//------------------------------------------------------------------------------
UIStyle::~UIStyle()
{
}

//------------------------------------------------------------------------------
detail::InvalidateFlags UIStyle::UpdateInherit(UIStyle* parent)
{
	detail::InvalidateFlags invalidate = detail::InvalidateFlags::None;

	if (parent == nullptr) return invalidate;

	bool changed = false;
	changed |= m_background.UpdateInherit(parent->m_background);
	//changed |= m_foreground.UpdateInherit(parent->m_foreground);
	if (changed) invalidate |= detail::InvalidateFlags::Rendering;

	changed = false;
	changed |= m_fontFamily.UpdateInherit(parent->m_fontFamily);
	changed |= m_fontSize.UpdateInherit(parent->m_fontSize);
	changed |= m_fontBold.UpdateInherit(parent->m_fontBold);
	changed |= m_fontItalic.UpdateInherit(parent->m_fontItalic);
	if (changed) invalidate |= detail::InvalidateFlags::Font;

	return invalidate;
}

//==============================================================================
// UIStyleTable
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIStyleTable, Object);

//------------------------------------------------------------------------------
UIStyleTable::UIStyleTable()
{
}

//------------------------------------------------------------------------------
UIStyleTable::~UIStyleTable()
{
}

//------------------------------------------------------------------------------
void UIStyleTable::AddStyle(const String& targetName, const String& targetState, UIStyle* style)
{
	LN_CHECK_ARG(style != nullptr);

	StyleKey k{ targetName, targetState };
	m_table.Add(k, style);
}

//------------------------------------------------------------------------------
UIStyle* UIStyleTable::FindStyle(const String& targetName, const String& targetState)
{
	StyleKey k{ targetName, targetState };
	RefPtr<UIStyle>* s = m_table.Find(k);
	if (s != nullptr) return s->Get();
	return nullptr;
}

LN_NAMESPACE_END
