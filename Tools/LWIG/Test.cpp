
#ifndef LUMINO_MATH_VECTOR3_H
#define LUMINO_MATH_VECTOR3_H

#include <stdio.h>
#include "Common.h"

LN_NAMESPACE_BEGIN
struct Vector2;
struct Vector4;
struct Quaternion;
struct Matrix;

/**
	@brief	3�����̃x�N�g�����`���܂��B
*/
LN_STRUCT()
struct LUMINO_EXPORT Vector3
{
public:

	/** X �v�f */
	LN_FIELD()
	float	x;

	/** Y �v�f */
	LN_FIELD()
	float	y;

	/** Z �v�f */
	LN_FIELD()
	float	z;

public:

	static const Vector3   Zero;		///< Vector3(0, 0, 0)
	static const Vector3   UnitX;		///< Vector3(1, 0, 0)
	static const Vector3   UnitY;		///< Vector3(0, 1, 0)
	static const Vector3   UnitZ;		///< Vector3(0, 0, 1)
	static const Vector3   Ones;		///< Vector3(1, 1, 1)

public:

	/**
		@brief	���ׂĂ̗v�f�� 0.0 �ɐݒ肵�ăC���X�^���X�����������܂��B
	*/
	Vector3();

	/**
		@brief	�w�肵���l���g�p���ăC���X�^���X�����������܂��B
	*/
	Vector3(float x, float y, float z);
	
	/**
		@brief	Vector2 �� z �l���w�肵�ăC���X�^���X�����������܂��B
	*/
	Vector3(const Vector2& vec, float z);

public:
	
	/**
		@brief		�e�v�f�ɒl��ݒ肵�܂��B
	*/
	void set(float x, float y, float z);
	
	/**
		@brief		���̃x�N�g���� X Y �v�f�� Vector2 �Ƃ��ĕԂ��܂��B
	*/
	const Vector2& getXY() const;

	/**
		@brief		�x�N�g���̒�����Ԃ��܂��B
	*/
	LN_METHOD()
	float getLength(int* test) const;

	/**
		@brief		�x�N�g���̒����� 2 ���Ԃ��܂��B
	*/
	LN_METHOD()
	float getLengthSquared() const;

	/**
		@brief		���̃x�N�g���𐳋K�����܂��B
		@details	�x�N�g���̒����� 0 �̏ꍇ�͐��K�����s���܂���B
	*/
	LN_METHOD()
    void normalize();

	/**
		@brief		���̃x�N�g�����w�肳�ꂽ�ő�l�ƍŏ��l�͈̔͂ɃN�����v���܂��B
		@param[in]	minVec	: �ŏ��l
		@param[in]	maxVec	: �ő�l
	*/
	LN_METHOD()
	void clamp(const Vector3& minVec, const Vector3& maxVec);

	/**
		@brief		�w�肳�ꂽ�s����g�p���Ă��̃x�N�g�������W�ϊ����܂��B
		@param[in]	mat		: �����̊�ɂȂ�s��
		@details	�x�N�g���� (X, Y, Z, 1.0) �Ƃ��č��W�ϊ����s���A���ʂ� w �ŏ��Z���܂��B
	*/
	void transformCoord(const Matrix& mat);
	
	/**
		@brief		�v�f�̂����ꂩ�� NaN �܂��� Inf ���𔻕ʂ��܂��B
	*/
	bool isNaNOrInf() const;

	/**
		@brief		�f�o�b�O�p�ɕ������W���o�͂��܂��B
		@param[in]	format	: �����w�蕶����
		@param[in]	stream	: �o�͐�X�g���[��
		@details	format �� NULL �̏ꍇ�A������ "%f, %f, %f\n" ���g�p���܂��B
	*/
	void print(const char* format = NULL, FILE* stream = NULL) const;
	
public:

	/**
		@brief		�w��x�N�g���𐳋K�������x�N�g����Ԃ��܂��B
		@param[in]	x		: �����̊�ɂȂ�x�N�g���� X �v�f
		@param[in]	y		: �����̊�ɂȂ�x�N�g���� Y �v�f
		@param[in]	z		: �����̊�ɂȂ�x�N�g���� Z �v�f
		@return		���K�����ꂽ�x�N�g��
	*/
	LN_METHOD()
	static Vector3 normalize(float x, float y, float z);

	/**
		@brief		�w��x�N�g���𐳋K�������x�N�g����Ԃ��܂��B
		@param[in]	vec		: �����̊�ɂȂ�x�N�g��
		@return		���K�����ꂽ�x�N�g��
	*/
	LN_METHOD()
	static Vector3 normalize(const Vector3& vec);

	static Vector3 safeNormalize(const Vector3& vec);
	static Vector3 safeNormalize(const Vector3& vec, const Vector3& alt);

	/** �w�肵���l�����ׂĂ̗v�f�ɐݒ肵�܂��B */
	static Vector3 replicate(float value);
	
	/**
		@brief		2�̃x�N�g���Ԃ̋������v�Z���܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		2�̃x�N�g���Ԃ̋���
	*/
	static float distance(const Vector3& vec1, const Vector3& vec2);
	
	/**
		@brief		2�̃x�N�g���Ԃ̋�����2����v�Z���܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		2�̃x�N�g���Ԃ̋�����2��
	*/
	static float distanceSquared(const Vector3& vec1, const Vector3& vec2);
	
	/**
		@brief		2�̃x�N�g���̓��ς��v�Z���܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		2�̃x�N�g���̓���
	*/
	static float dot(const Vector3& vec1, const Vector3& vec2);

	/**
		@brief		2�̃x�N�g���̊O�ς��v�Z���܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		2�̃x�N�g���̊O��
	*/
	static Vector3 cross(const Vector3& vec1, const Vector3& vec2);

	/**
		@brief		2�̃x�N�g���̍ŏ��l�ō\�������x�N�g����Ԃ��܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		�ŏ��l����쐬���ꂽ�x�N�g��
	*/
	static Vector3 min(const Vector3& vec1, const Vector3& vec2);

	/**
		@brief		�w�肳�ꂽ�����̃x�N�g���̍ŏ��l�ō\�������x�N�g����Ԃ��܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g���̔z��
		@param[in]	count	: �����̊�ɂȂ�x�N�g���̐�
		@return		�ŏ��l����쐬���ꂽ�x�N�g��
	*/
	static Vector3 min(const Vector3* vectors, int count);

	/**
		@brief		2�̃x�N�g���̍ő�l�ō\�������x�N�g����Ԃ��܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g��
		@param[in]	vec2	: �����̊�ɂȂ�x�N�g��
		@return		�ő�l����쐬���ꂽ�x�N�g��
	*/
	static Vector3 max(const Vector3& vec1, const Vector3& vec2);

	/**
		@brief		�w�肳�ꂽ�����̃x�N�g���̍ő�l�ō\�������x�N�g����Ԃ��܂��B
		@param[in]	vec1	: �����̊�ɂȂ�x�N�g���̔z��
		@param[in]	count	: �����̊�ɂȂ�x�N�g���̐�
		@return		�ő�l����쐬���ꂽ�x�N�g��
	*/
	static Vector3 max(const Vector3* vectors, int count);

	/**
		@brief		���˃x�N�g���Ɩ@���x�N�g�����甽�˃x�N�g�����v�Z���܂��B
		@param[in]	vec		: ���˃x�N�g��
		@param[in]	normal	: �@���x�N�g��
		@return		���˃x�N�g��
	*/
	static Vector3 reflect(const Vector3& vec, const Vector3& normal);

	/**
		@brief		���˃x�N�g���Ɩ@���x�N�g�����犊��x�N�g�����v�Z���܂��B
		@param[in]	vec		: ���˃x�N�g��
		@param[in]	normal	: �@���x�N�g��
		@return		����x�N�g��
	*/
	static Vector3 slide(const Vector3& vec, const Vector3& normal);

	/**
		@brief		�w�肳�ꂽ�N�H�[�^�j�I�����g�p���ăx�N�g�������W�ϊ����܂��B
		@param[in]	vec		: �����̊�ɂȂ�x�N�g��
		@param[in]	qua		: �����̊�ɂȂ�N�H�[�^�j�I��
		@return		�ϊ����ꂽ�x�N�g��
	*/
	static Vector3 transform(const Vector3& vec, const Quaternion& qua);

	/**
		@brief		�w�肳�ꂽ�s����g�p���ăx�N�g�������W�ϊ����܂��B
		@param[in]	vec		: �����̊�ɂȂ�x�N�g��
		@param[in]	mat		: �����̊�ɂȂ�s��
		@return		�ϊ����ꂽ�x�N�g��
	*/
	static Vector4 transform(const Vector3& vec, const Matrix& mat);

	/**
		@brief		�w�肳�ꂽ�s����g�p���ăx�N�g�������W�ϊ����܂��B
		@param[in]	vec		: �����̊�ɂȂ�x�N�g��
		@param[in]	mat		: �����̊�ɂȂ�s��
		@details	�x�N�g���� (X, Y, Z, 1.0) �Ƃ��č��W�ϊ����s���A���ʂ� w �ŏ��Z���܂��B
		@return		�ϊ����ꂽ�x�N�g��
	*/
	static Vector3 transformCoord(const Vector3& vec, const Matrix& mat);

	/**
		@brief		2 �̃x�N�g���Ԃ̐��`��Ԃ��s���܂��B
		@param[in]	start	: �J�n�x�N�g�� (t = 0.0 �̂Ƃ��̒l)
		@param[in]	end		: �I���x�N�g�� (t = 1.0 �̂Ƃ��̒l)
		@param[in]	t		: ���d�W��
		@return		��Ԍ��ʂ̃x�N�g��
		@details	t �͒ʏ�A0.0�`1.0 ���w�肵�܂��B
	*/
	static Vector3 lerp(const Vector3& start, const Vector3& end, float t);

	/**
		@brief		�w�肳�ꂽ�x�N�g�����g�p���� �G���~�[�g�X�v���C����Ԃ����s���܂��B
		@param[in]	v1	: �J�n�x�N�g��
		@param[in]	a1	: �J�n�x�N�g���̐ڐ��x�N�g��(���x)
		@param[in]	v2	: �I���x�N�g��
		@param[in]	a2	: �I���x�N�g���̐ڐ��x�N�g��(���x)
		@param[in]	t	: ���d�W��
		@return		��Ԍ��ʂ̒l
		@details	t �͒ʏ�A0.0�`1.0 ���w�肵�܂��B
	*/
	static Vector3 hermite(const Vector3& v1, const Vector3& a1, const Vector3& v2, const Vector3& a2, float t);

	/**
		@brief		�w�肳�ꂽ�x�N�g�����g�p���� Catmull-Rom ��Ԃ��s���܂��B
		@param[in]	vec1	: 1�Ԗڂ̈ʒu
		@param[in]	vec2	: 2�Ԗڂ̈ʒu (t = 0.0 �̂Ƃ��̒l)
		@param[in]	vec3	: 3�Ԗڂ̈ʒu (t = 1.0 �̂Ƃ��̒l)
		@param[in]	vec4	: 4�Ԗڂ̈ʒu
		@param[in]	t		: ���d�W��
		@return		��Ԍ��ʂ̃x�N�g��
		@details	t �͒ʏ�A0.0�`1.0 ���w�肵�܂��B
	*/
	static Vector3 catmullRom(const Vector3& vec1, const Vector3& vec2, const Vector3& vec3, const Vector3& vec4, float t);
	
	/**
		@brief		�I�u�W�F�N�g���(3D���)�̃x�N�g�����X�N���[�����(2D���)�̃x�N�g���ɕϊ�����B
		@param[in]	point			: �I�u�W�F�N�g��ԏ�̍��W
		@param[in]	worldViewProj	: �����ς݂� ���[���h - �r���[ - �v���W�F�N�V�����s��
		@param[in]	x				: �r���[�|�[�g�̍��� X ���W
		@param[in]	y				: �r���[�|�[�g�̍��� Y ���W
		@param[in]	width			: �r���[�|�[�g�̕�
		@param[in]	height			: �r���[�|�[�g�̍���
		@param[in]	minZ			: �r���[�|�[�g�̍ŏ��[�x
		@param[in]	maxZ			: �r���[�|�[�g�̍ő�[�x
	*/
	static Vector3 project(const Vector3& point, const Matrix& worldViewProj, float x, float y, float width, float height, float minZ = 0.0f, float maxZ = 1.0f);

	/**
		@brief		�X�N���[�����(2D���)�̃x�N�g�����I�u�W�F�N�g���(3D���)�̃x�N�g���ɕϊ�����B
		@param[in]	point			: �X�N���[����ԏ�̍��W
		@param[in]	worldViewProj	: �����ς݂� ���[���h - �r���[ - �v���W�F�N�V�����s��
		@param[in]	x				: �r���[�|�[�g�̍��� X ���W
		@param[in]	y				: �r���[�|�[�g�̍��� Y ���W
		@param[in]	width			: �r���[�|�[�g�̕�
		@param[in]	height			: �r���[�|�[�g�̍���
		@param[in]	minZ			: �r���[�|�[�g�̍ŏ��[�x
		@param[in]	maxZ			: �r���[�|�[�g�̍ő�[�x
	*/
	static Vector3 unproject(const Vector3& point, const Matrix& worldViewProj, float x, float y, float width, float height, float minZ = 0.0f, float maxZ = 1.0f);

	static bool nearEqual(const Vector3& value1, const Vector3& value2);

public:

	Vector3& operator += (const Vector3& v);
	Vector3& operator += (float v);
	Vector3& operator -= (const Vector3& v);
	Vector3& operator -= (float v);
	Vector3& operator *= (const Vector3& v);
	Vector3& operator *= (float v);
	Vector3& operator /= (const Vector3& v);
	Vector3& operator /= (float v);

	friend Vector3 operator + (const Vector3& v1,	const Vector3& v2);
	friend Vector3 operator + (const Vector3& v1,	float v2);
	friend Vector3 operator + (float v1,			const Vector3& v2);
	friend Vector3 operator - (const Vector3& v1,	const Vector3& v2);
	friend Vector3 operator - (const Vector3& v1,	float v2);
	friend Vector3 operator - (float v1,			const Vector3& v2);
	friend Vector3 operator * (const Vector3& v1,	const Vector3& v2);
	friend Vector3 operator * (const Vector3& v1,	float v2);
	friend Vector3 operator * (float v1,			const Vector3& v2);
	friend Vector3 operator / (const Vector3& v1,	const Vector3& v2);
	friend Vector3 operator / (const Vector3& v1,	float v2);
	friend Vector3 operator / (float v1,			const Vector3& v2);

	friend Vector3 operator - (const Vector3& v1);

	bool operator == (const Vector3& v) const;
	bool operator != (const Vector3& v) const;

#ifdef LN_MATH_VECTOR3_EXTENSION
	LN_MATH_VECTOR3_EXTENSION
#endif
};

LN_NAMESPACE_END

//#include "Vector3.inl"

#endif // LUMINO_MATH_VECTOR3_H
