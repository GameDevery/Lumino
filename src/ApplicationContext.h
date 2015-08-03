
#pragma once

namespace Lumino
{
namespace Graphics { class GraphicsManager; }
class Application;

// Application �N���X�̎��e�� Manager ���e���W���[���Ɍ��J������́B
// Application �𒼐ڌ��J����� include �̊֌W�Ńr���h���Ԃ����������ƂɂȂ�̂ŗp�ӂ����B
class ApplicationContext
{
public:
	static void SetCurrent(Application* app);
	static Application* GetCurrent();
	static Graphics::GraphicsManager* GetGraphicsManager();
};

} // namespace Lumino
