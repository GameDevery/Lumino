
#pragma once
#include <Lumino/Base/Environment.h>

namespace Lumino
{

class MouseCursorVisibility
{
public:
	MouseCursorVisibility()
		: m_visibleMouseCursor(true)
		, m_mouseVisibleTime(0)
		, m_startTime(0)
		, m_requestForced(false)
	{}

	~MouseCursorVisibility()
	{}

public:

	/// ���̊֐��� false ��Ԃ��Ă���Ԃ͔�\���ɂ��� (���[�J�[�X���b�h�Ń`�F�b�N)
	bool CheckVisible() const
	{
		if (m_requestForced) return true;
		if (m_visibleMouseCursor) return true;
		if (m_mouseVisibleTime == 0) return false;
		return ((Environment::GetTickCount() - m_startTime) < m_mouseVisibleTime);
	}

	/// �}�E�X�J�[�\���̕\���ݒ� (count �� �}�E�X�J�[�\�����u���A�����܂ł̎���(ms)�B0 �ŏ�ɏ���)
	void SetMouseCursorVisibleState(bool visible, int time)
	{
		m_visibleMouseCursor = visible;
		m_mouseVisibleTime = std::max(0, time);
		m_startTime = Environment::GetTickCount();
		m_requestForced = false;
	}

	/// �}�E�X�J�[�\���̑����ɌĂԂ���
	void OnMoveCursor(bool on_client_area)
	{
		// �\�����Ȃ��ꍇ
		//if (!m_visibleMouseCursor && m_mouseVisibleTime == 0)
		//{
		//	m_requestForced = false;
		//}
		//else
		{
			if (on_client_area)
			{
				m_startTime = Environment::GetTickCount();
				m_requestForced = false;
			}
			else
			{
				m_requestForced = true;
			}
		}
	}

private:
	bool		m_visibleMouseCursor;
	uint64_t	m_mouseVisibleTime;
	uint64_t	m_startTime;
	bool		m_requestForced;

};

} // namespace Lumino
