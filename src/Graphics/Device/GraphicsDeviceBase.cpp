
#pragma once 

#include "GraphicsDeviceBase.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{

//=============================================================================
// GraphicsDeviceBase
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsDeviceBase::GraphicsDeviceBase()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsDeviceBase::~GraphicsDeviceBase()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsDeviceBase::Finalize()
{
	Threading::MutexScopedLock lock(m_deviceObjectListMutex);
	LN_FOREACH(IDeviceObject* obj, m_deviceObjectList) {
		obj->Release();
	}
	m_deviceObjectList.Clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsDeviceBase::AddDeviceResource(IDeviceObject* obj)
{
	Threading::MutexScopedLock lock(m_deviceObjectListMutex);
	m_deviceObjectList.Add(obj);
	obj->AddRef();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void GraphicsDeviceBase::RemoveDeviceResource(IDeviceObject* obj)
//{
//	m_deviceObjectList.Remove(obj);
//}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GraphicsDeviceBase::GCDeviceResource()
{
	/*
	���̊֐��͕`��X���b�h�́ASwapChain::Present() �̒���Ŏ��s�����B
	���\�[�X�����̃N���X�ȊO����Q�Ƃ���Ă��Ȃ���ΊJ������B

	�Ȃ��A���̂悤�Ȏd�g�݂ɂ����̂́A���\�[�X�̊J����e�Ղɂ��邽�߁B
	Create �n�̓X���b�h�Z�[�t���� GPU �ŗL�̕s��ɔ����邽�߁A
	�쐬�̑O��ŃR���e�L�X�g���A�N�e�B�u/�f�B�A�N�e�B�u���Ă��邪�A
	�J���� (glDelete�`) �̑O��ł����R�K�v�ɂȂ�B

	�x���`��̓s����A���C���X���b�h�ŕs�v�ɂȂ��Ă��`��X���b�h�ł͂܂��g���Ă��邱�Ƃ͕��ʂɂ���B
	�`��X���b�h�ł��K�v�Ȃ��Ȃ������_�Ń��\�[�X�� Release ����Ηǂ��̂����A
	���ꂾ�ƃf�X�g���N�^�Łu���݂̃X���b�h�����C���X���b�h�ł���� MakeCurrent ����v�̂悤�ȏ������K�v�ɂȂ�B
	���̏ꍇ���ƂȂ�̂́A
	�ECreate�n�̓����ŃG���[���������ARelease �������Ƃ��Ƀf�b�h���b�N�̊댯��������
	�E�f�X�g���N�^�ŗ�O�𔭐�������\��������
	���悤�͂�����ł����邪�A�V���v���Ɏ�������̂͏�������B
	*/
	Threading::MutexScopedLock lock(m_deviceObjectListMutex);

	ArrayList<IDeviceObject*>::iterator itr = m_deviceObjectList.begin();
	ArrayList<IDeviceObject*>::iterator end = m_deviceObjectList.end();
	for (; itr != end;)
	{
		if ((*itr)->GetRefCount() == 1) {
			(*itr)->Release();
			itr = m_deviceObjectList.erase(itr);
			end = m_deviceObjectList.end();
		}
		else {
			++itr;
		}
	}
}


} // namespace Device
} // namespace Graphics
} // namespace Lumino
