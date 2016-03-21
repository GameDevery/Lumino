
#pragma once
LN_NAMESPACE_BEGIN

/**
	@brief		�Q�[�����̎��ԏ���\���܂��B
*/
class GameTime final
{
public:
	
	/**
		@brief		�O��̃t���[������o�߂����Q�[������ (�b) ���擾���܂��B
	*/
	static double GetElapsedGameTime();

	/**
		@brief		�O��̃t���[������o�߂��������� (�b) ���擾���܂��B
	*/
	static double GetElapsedRealTime();

	/**
		@brief		�J�n����̑��Q�[������ (�b) ���擾���܂��B
	*/
	static double GetTotalGameTime();

	/**
		@brief		�J�n����̑������� (�b) ���擾���܂��B
	*/
	static double GetTotalRealTime();
};

LN_NAMESPACE_END
