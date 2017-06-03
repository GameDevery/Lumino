
#pragma once

LN_NAMESPACE_BEGIN
class CameraComponent;

/**
	@brief		
*/
class OffscreenWorldView
	: public Object
{
public:

LN_INTERNAL_ACCESS:
	OffscreenWorldView();
	virtual ~OffscreenWorldView();
	void Initialize();

private:
	// TODO: ���_�����߂邽�߂̓���� CameraComponent�BWorld �ւ̒ǉ��͂ł��Ȃ��Ȃǂ̐��������Ă��������B
	RefPtr<CameraComponent>	m_cameraInfo;
};

LN_NAMESPACE_END
