
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
	// TODO: 視点を決めるための特殊な CameraComponent。World への追加はできないなどの制限をしておきたい。
	RefPtr<CameraComponent>	m_cameraInfo;
};

LN_NAMESPACE_END
