
#pragma once
#include "../CoreObjectList.h"
#include "UIElement.h"

namespace Lumino
{
namespace GUI
{

#if 0
/**
	@brief		
*/
template<class TObject>
class GUIObjectList
	: public GenericVariantList<TObject>
	, public IAddChild
{
public:
	GUIObjectList() {}
	virtual ~GUIObjectList() {}

public:
	///// �w��C���f�b�N�X�ɗv�f���i�[����
	//void SetAt(int index, TObject* item) { SetAtBase(index, item); }

	///// �w��C���f�b�N�X�̗v�f���擾����
	//TObject* GetAt(int index) const { return static_cast<TObject*>(m_list.GetAt(index)); }

public:
	// IAddChild Interface
	virtual void AddChild(const Variant& value)
	{
		if (value.GetType() == VariantType_Object)
		{
			TObject* obj = dynamic_cast<TObject*>(value.GetObject());
			LN_THROW(obj != NULL, ArgumentException);
			Add(obj);
		}
		else {
			LN_THROW(0, ArgumentException);
		}
	}

	virtual void AddText(const String& text)
	{
		LN_THROW(0, InvalidOperationException);
	}
};
#endif

} // namespace GUI
} // namespace Lumino
