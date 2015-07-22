

#pragma once

namespace Lumino
{
namespace GUI
{
	
// �l�X�ȃI�u�W�F�N�g�� private �����o�ɃA�N�Z�X��������N���X�B
// ���̃N���X�͐�΂Ƀ��C�u�����̃��[�U�[�Ɍ�����Ƃ���Ɍ��J���Ă͂Ȃ�Ȃ��B
// �t���[�����[�N�̓s����A�����̃����o�� public �Ō��J���Ȃ���΂Ȃ�Ȃ����A����������͂��܂�ɂ��댯�Ȏ����B
// ��ނ𓾂� friend class �ɓ������B
// C# �� internal ���g����Ίy�Ȃ̂����E�E�E�B
class GUIHelper
{
public:
	static void UIElement_SetKeyName(UIElement* element, const String& name) { element->m_keyName = name; }
	static void UIElement_SetTemplateParent(UIElement* element, UIElement* templateParent) { element->m_templateParent = templateParent; }
};

} // namespace GUI
} // namespace Lumino
