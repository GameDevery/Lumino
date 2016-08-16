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
#include <Lumino/UI/UIElement.h>
#include <Lumino/UI/UIStyle.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIStylePropertyTable
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIStylePropertyTable, Object);

//------------------------------------------------------------------------------
UIStylePropertyTable::UIStylePropertyTable()
	//: m_lastInheritedParent(nullptr)
{
}

//------------------------------------------------------------------------------
UIStylePropertyTable::~UIStylePropertyTable()
{
}

//------------------------------------------------------------------------------
void UIStylePropertyTable::Initialize(const StringRef& visualStateName)
{
	m_visualStateName = visualStateName;
}

//------------------------------------------------------------------------------
void UIStylePropertyTable::AddValue(const tr::Property* targetProperty, const tr::Variant& value)
{
	m_attributes.Add(UIStyleAttribute(targetProperty, value));
}

//------------------------------------------------------------------------------
detail::InvalidateFlags UIStylePropertyTable::UpdateInherit(UIStylePropertyTable* parent)
{
	bool changed = false;

	// parent �������Ă���l�̂����A���� targetProperty �̂��̂�T���B����Ȃɐ��͑����Ȃ��͂��Ȃ̂Ő��`�T���B
	for (UIStyleAttribute& parentAttr : parent->m_attributes)
	{
		parentAttr.m_mergedMark = false;

		for (UIStyleAttribute& attr : m_attributes)
		{
			if (attr.m_targetProperty == parentAttr.m_targetProperty)
			{
				changed |= attr.UpdateInherit(parentAttr);
				parentAttr.m_mergedMark = true;
				break;
			}
		}
	}

	// parent �������Ă���l�̂����A���� targetProperty �ł͂Ȃ��������̂�P�Ȃ�Q�ƂƂ��Čp������B
	m_parentRefAttributes.Clear();
	for (UIStyleAttribute& parentAttr : parent->m_attributes)
	{
		if (!parentAttr.m_mergedMark)
		{
			m_parentRefAttributes.Add(&parentAttr);
			changed = true;	// TODO: �قƂ�ǖ���X�V���ꂽ���ƂɂȂ��Ă��܂��B���r�W�����J�E���g�K�{���ȁE�E
		}
	}

	return (changed) ? detail::InvalidateFlags::All : detail::InvalidateFlags::None;
}

//------------------------------------------------------------------------------
void UIStylePropertyTable::Apply(UIElement* targetElement)
{
	for (UIStyleAttribute& setter : m_attributes)
	{
		tr::Property::SetPropertyValue(targetElement, setter.m_targetProperty, setter.value);
		// TODO: �A�j���[�V����
	}
	for (UIStyleAttribute* setter : m_parentRefAttributes)
	{
		tr::Property::SetPropertyValue(targetElement, setter->m_targetProperty, setter->value);
		// TODO: �A�j���[�V����
	}
}

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
	//: m_margin(ThicknessF(0, 0, 0, 0))
	//, m_padding(ThicknessF(0, 0, 0, 0))
	//, m_verticalAlignment(VerticalAlignment::Top)
	//, m_horizontalAlignment(HorizontalAlignment::Left)
	//, m_background(nullptr)
	//, m_foreground(nullptr)
	//, m_fontFamily(String::GetEmpty())
	//, m_fontSize(0)
	//, m_fontBold(false)
	//, m_fontItalic(false)
{
}

//------------------------------------------------------------------------------
UIStyle::~UIStyle()
{
}

//------------------------------------------------------------------------------
void UIStyle::AddValue(const StringRef& visualStateName, const tr::Property* targetProperty, const tr::Variant& value)
{
	RefPtr<UIStylePropertyTable> table;
	if (!m_propertyTableMap.TryGetValue(visualStateName, &table))
	{
		table = RefPtr<UIStylePropertyTable>::MakeRef();
		table->Initialize(visualStateName);
		table->AddValue(targetProperty, value);
		m_propertyTableMap.Add(visualStateName, table);
	}
}

//------------------------------------------------------------------------------
UIStylePropertyTable* UIStyle::FindStylePropertyTable(const String& visualStateName)
{
	RefPtr<UIStylePropertyTable> table;
	if (m_propertyTableMap.TryGetValue(visualStateName, &table))
	{
		return table;
	}
	return false;
}

//------------------------------------------------------------------------------
//detail::InvalidateFlags UIStyle::UpdateInherit(UIStyle* parent)
//{
//	// �e���Ȃ����[�g�v�f�X�^�C���͌p���ɂ��l�ύX�͍l������K�v�͂Ȃ�
//	if (parent == nullptr) return detail::InvalidateFlags::None;
//
//	detail::InvalidateFlags invalidate = detail::InvalidateFlags::None;
//
//	for (auto& pair : m_propertyTableMap)
//	{
//		// parent �������Ă���l�̂����A���� targetProperty �̂��̂�T���B����Ȃɐ��͑����Ȃ��͂��Ȃ̂Ő��`�T���B
//		for (auto& parentPair : parent->m_propertyTableMap)
//		{
//			if (pair.first == parentPair.first)
//			{
//				invalidate |= pair.second->UpdateInherit(parentPair.second);
//				break;
//			}
//		}
//	}
//
//	//bool changed = false;
//	//changed |= m_background.UpdateInherit(parent->m_background);
//	////changed |= m_foreground.UpdateInherit(parent->m_foreground);
//	//if (changed) invalidate |= detail::InvalidateFlags::Rendering;
//
//	//changed = false;
//	//changed |= m_fontFamily.UpdateInherit(parent->m_fontFamily);
//	//changed |= m_fontSize.UpdateInherit(parent->m_fontSize);
//	//changed |= m_fontBold.UpdateInherit(parent->m_fontBold);
//	//changed |= m_fontItalic.UpdateInherit(parent->m_fontItalic);
//	//if (changed) invalidate |= detail::InvalidateFlags::Font;
//
//	return invalidate;
//}

//------------------------------------------------------------------------------
//void UIStyle::Apply(UIElement* targetElement)
//{
//	LN_ASSERT(targetElement != nullptr);
//
//	RefPtr<UIStylePropertyTable> table;
//	if (m_propertyTableMap.TryGetValue(targetElement->GetCurrentVisualStateName(), &table))
//	{
//		table->Apply(targetElement);
//	}
//}

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
void UIStyleTable::AddStyle(const String& targetName, UIStyle* style)
{
	LN_CHECK_ARG(style != nullptr);

	StyleKey k{ targetName };
	m_table.Add(k, style);
}

//------------------------------------------------------------------------------
UIStyle* UIStyleTable::FindStyle(const String& targetName)
{
	StyleKey k{ targetName };
	RefPtr<UIStyle>* s = m_table.Find(k);
	if (s != nullptr) return s->Get();
	return nullptr;
}

LN_NAMESPACE_END
