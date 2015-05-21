
#pragma once
#include "Common.h"

namespace Lumino
{
namespace GUI
{
	
/**
	@brief		GUI �@�\�̊Ǘ��N���X�ł��B
*/
class GUIManager
	: public RefObject
{
public:
	GUIManager();
	virtual ~GUIManager();

public:
	Workbench* CreateWorkbench();

private:
};

} // namespace GUI
} // namespace Lumino
