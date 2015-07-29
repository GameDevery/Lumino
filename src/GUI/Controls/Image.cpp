
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/Image.h>

namespace Lumino
{
namespace GUI
{
//=============================================================================
// Image
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Image, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(Image);

//LN_DEFINE_PROPERTY_2(Image, const Variant&, SourceProperty, "Source", Variant::Null, &Image::SetSource, NULL);
LN_DEFINE_PROPERTY_2(Image, Rect, SourceRectProperty, "SourceRect", Rect(), &Image::SetSourceRect, &Image::GetSourceRect);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Image* Image::Create(GUIManager* manager)
{
	auto obj = RefPtr<Image>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Image::Image(GUIManager* manager)
	: UIElement(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Image::~Image()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Image::SetSource(const Variant& value)
{
	if (value.GetType() == VariantType_Object) 
	{
		auto tex = dynamic_cast<Graphics::Texture*>(value.GetObject());
		if (tex != NULL)
		{
			SetSourceTexture(tex);
			return;
		}
	}

	LN_VERIFY(0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Image::SetSourceTexture(Graphics::Texture* texture)
{
	m_texture = texture;
	UpdateInternalSourceRect();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Image::SetSourceRect(const Rect& rect)
{
	m_srcRect = rect;
	UpdateInternalSourceRect();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Image::UpdateInternalSourceRect()
{
	if (m_texture != NULL)
	{
		if (m_srcRect.IsZero())
		{
			// �]������`���w�肳��Ă��Ȃ���΃e�N�X�`���T�C�Y���g�p����
			const Size& size = m_texture->GetSize();
			m_internalSrcRect.Set(0, 0, size.Width, size.Height);
		}
		else {
			m_internalSrcRect = m_srcRect;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF Image::MeasureOverride(const SizeF& availableSize)
{
	SizeF desiredSize = UIElement::MeasureOverride(availableSize);

	if (m_stretch == Stretch::None)
	{
		// �e�N�X�`���]���T�C�Y���R���g���[���̍ŏ��T�C�Y�ɂ���
		desiredSize.Width = std::max(desiredSize.Width, (float)m_internalSrcRect.Width);
		desiredSize.Height = std::max(desiredSize.Height, (float)m_internalSrcRect.Height);
	}
	return desiredSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF Image::ArrangeOverride(const SizeF& finalSize)
{
	SizeF size = UIElement::ArrangeOverride(finalSize);
	if (m_stretch == Stretch::None)
	{
		size.Width = std::min(size.Width, (float)m_internalSrcRect.Width);
		size.Height = std::min(size.Height, (float)m_internalSrcRect.Height);
	}
	return size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Image::OnRender(Graphics::Painter* painter)
{
	if (m_texture == NULL) { return; }

	painter->DrawTexture(m_finalGlobalRect, m_texture, m_internalSrcRect);
}

} // namespace GUI
} // namespace Lumino

