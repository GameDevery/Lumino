
#pragma once
#include "UIElement.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		
*/
class UIElementCollection
	: public CoreObject		// TODO:�v���p�e�B�͂���Ȃ��̂ł�����i�T�u�N���X�������Ă�����
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	UIElementCollection(UIElement* logicalParent)
		: m_logicalParent(logicalParent)
	{}
	virtual ~UIElementCollection(){}

	/// �����ɗv�f��ǉ����܂��B
	void Add(UIElement* item)
	{
		if (item != NULL) {
			item->SetParent(m_logicalParent);
			m_visualChildren.Add(item);

			// �q�v�f�̃e�[�}�𒼂��ɍX�V
			item->ApplyTemplate();
		}
	}

public:
	//typedef ArrayList<UIElement*>::iterator	iterator;
	//typedef ArrayList<UIElement*>::const_iterator	const_iterator;
	typedef ArrayList< RefPtr<UIElement> >::iterator	iterator;
	typedef ArrayList< RefPtr<UIElement> >::const_iterator	const_iterator;
	iterator		begin()			{ return m_visualChildren.begin(); }
	const_iterator	begin() const	{ return m_visualChildren.begin(); }
	iterator		end()			{ return m_visualChildren.end(); }
	const_iterator	end() const		{ return m_visualChildren.end(); }

public:
	UIElement*						m_logicalParent;
	ArrayList< RefPtr<UIElement> >	m_visualChildren;	// ArrayList<RefPtr<UIElement> >  �͂�߂��B�C�e���[�g����Ƃ��ɏ璷�ɂȂ�B
};

} // namespace GUI
} // namespace Lumino
