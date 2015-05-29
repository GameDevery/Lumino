
#pragma once
#include "Common.h"
#include "../Variant.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		
	@note	WPF �� FrameworkElementFactory �ɑ�������B
*/
class UIElementFactory
	: public CoreObject
{
public:
	UIElementFactory();
	virtual ~UIElementFactory();
};

/**
	@brief		
	@note	ContentPresenter ���K�v�B
*/
class ControlTemplate
	: public CoreObject
{
public:
	ControlTemplate();
	virtual ~ControlTemplate();

private:
	String				m_targetType;		///< �ΏۃR���g���[���� ("Button" ��)
	UIElementFactory*	m_visualTreeRoot;	///< �e���v���[�g�� VisualTree �̃��[�g�m�[�h
};


	

} // namespace GUI
} // namespace Lumino
