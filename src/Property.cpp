/*
	[2015/7/30] LN_PROPERTY_BEGIN �� LN_PROPERTY_END �͂���ς��߂�
		�x�[�X�N���X�̃v���p�e�B�ɃA�N�Z�X���Â炭�Ȃ�B
		�Ⴆ�� StackPanel::Properties::Size �Ə����Ȃ��B

	[2015/7/29] LN_PROPERTY_BEGIN �� LN_PROPERTY_END �ɂ���
		�����̓v���p�e�B��`������\���̂ɕ����߂���́B
		C# �ƈႢ�Aobj-> �Ə����ăA�N�Z�X�ł�����A�C���e���Z���X�����������
		C++ �ł� WPF �̂悤�Ȓ�`�͂�����ƌ����Ȃ��Ǝv�����̂ł������Ă���B
*/
/*
	�v���p�e�B�̎�ނ�
	- ���̂� C++ �l�C�e�B�u�̃����o�ϐ�
	- ���̂��O��(C#�Ƃ�) �̃����o�ϐ�	�� ��������_���Ƃ��Ɏg���B�K�{�ł͂Ȃ�
	- ���̂� map �ɓo�^����� Variant
	�ƁA���ꂼ��
	- ���ʂ̃v���p�e�B
	- �Y�t�v���p�e�B
*/
#include "Internal.h"
#include <Lumino/Variant.h>
#include <Lumino/Property.h>

namespace Lumino
{

//=============================================================================
// PropertyManager
//=============================================================================

PropertyManager::PropertyMap	PropertyManager::m_propertyMap;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/*AttachedProperty* */ void PropertyManager::RegisterAttachedProperty(TypeInfo* ownerClass, const String& propertyName, const Variant& defaultValue)
{
#if 0
	TypedNameKey key(ownerClass, propertyName);
	PropertyMap::iterator itr = m_propertyMap.find(key);
	if (itr == m_propertyMap.end())
	{
		std::shared_ptr<AttachedProperty> prop(LN_NEW AttachedProperty(propertyName, defaultValue));
		m_propertyMap[key] = prop;
		return prop.get();
	}
	return itr->second.get();
#endif;
}

} // namespace Lumino
