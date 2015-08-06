
#pragma once

namespace Lumino
{
namespace Graphics { class GraphicsManager; }
namespace GUI { class GUIManager; }
class Application;

// Application �N���X�̎��e�� Manager ���e���W���[���Ɍ��J������́B
// Application �𒼐ڌ��J����� include �̊֌W�Ńr���h���Ԃ����������ƂɂȂ�̂ŗp�ӂ����B
class ApplicationContext
{
public:
	static void							SetCurrent(Application* app);
	static Application*					GetCurrent();
	static Graphics::GraphicsManager*	GetGraphicsManager();
	static GUI::GUIManager*				GetGUIManager();

	template<class T, typename... TArgs>
	static T* CreateObject(TArgs... args)
	{
		return LN_NEW T(args...);
	}

	template<class T, typename... TArgs>
	static RefPtr<T> CreateRefObject(TArgs... args)
	{
		return RefPtr<T>(LN_NEW T(args...));
	}

};

} // namespace Lumino
