/*
	@file	SoundListener.h
*/
#pragma once
#include "Common.h"

namespace Lumino
{
LN_NAMESPACE_AUDIO_BEGIN

/**
	@brief		3D �T�E���h�̃��X�i�[�̃N���X�ł��B
*/
class SoundListener
{
public:
	
	/**
		@brief		3D�����̃��X�i�[�̈ʒu��ݒ肵�܂��B
		@param[in]	position	: 3D ��ԏ�̍��W
	*/
	static void SetPosition(const Vector3& position);

	/**
		@brief		3D�����̃��X�i�[�̈ʒu��ݒ肵�܂��B
		@param[in]	x			: 3D ��ԏ�� X ���W
		@param[in]	y			: 3D ��ԏ�� Y ���W
		@param[in]	z			: 3D ��ԏ�� Z ���W
	*/
	static void SetPosition(float x, float y, float z);
	
	/**
		@brief		3D�����̃��X�i�[�̈ʒu���擾���܂��B
	*/
	static const Vector3& GetPosition();

	/**
		@brief		3D�����̃��X�i�[�̐��ʕ�����ݒ肵�܂��B
		@param[in]	direction		: ����
	*/
	static void SetDirection(const Vector3& direction);

	/**
		@brief		3D�����̃��X�i�[�̐��ʕ�����ݒ肵�܂��B
		@param[in]	x			: ������ X ����
		@param[in]	y			: ������ Y ����
		@param[in]	z			: ������ Z ����
	*/
	static void SetDirection(float x, float y, float z);

	/**
		@brief		3D�����̃��X�i�[�̐��ʕ������擾���܂��B
	*/
	static const Vector3& GetDirection();

	/**
		@brief		3D�����̃��X�i�[�̏������ݒ肵�܂��B(���ʕ����Ƃ͒����ł��邱��)
		@param[in]	direction	: �����
	*/
	static void SetUpDirection(const Vector3& direction);

	/**
		@brief		3D�����̃��X�i�[�̏������ݒ肵�܂��B(���ʕ����Ƃ͒����ł��邱��)
		@param[in]	x			: ������ X ����
		@param[in]	y			: ������ Y ����
		@param[in]	z			: ������ Z ����
	*/
	static void SetUpDirection(float x, float y, float z);
	
	/**
		@brief		3D�����̃��X�i�[�̏�������擾���܂��B
	*/
	static const Vector3& GetUpDirection(const Vector3& direction);

	/**
		@brief		3D�����̃��X�i�[�̑��x��ݒ肵�܂��B
		@param[in]	velocity	: ���x
	*/
	static void SetVelocity(const Vector3& velocity);

	/**
		@brief		3D�����̃��X�i�[�̑��x��ݒ肵�܂��B
		@param[in]	x			: ���x�� X ����
		@param[in]	y			: ���x�� Y ����
		@param[in]	z			: ���x�� Z ����
	*/
	static void SetVelocity(float x, float y, float z);

	/**
		@brief		3D�����̃��X�i�[�̑��x��ݒ肵�܂��B
		@param[in]	velocity	: ���x
	*/
	static const Vector3& GetVelocity();

};

LN_NAMESPACE_AUDIO_END
} // namespace Lumino
