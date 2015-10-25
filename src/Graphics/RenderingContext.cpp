/*
	���O�̌��l�^�͂��̂����肩��B
	https://developer.mozilla.org/ja/docs/Web/API/RenderingContext

	���̃��W���[���� Scene �� GUI �ŋ��L����邪�A
	DrawingContext �� Scene ���� GUI ���B
	���C�u�����Ƃ��Ă� Scene �̕����Ӗ����傫���BDraw ��� Render ���ȁA�ƁB


	Scene �ł́A���炩���� SceneNode �̃V�F�[�_�����[�U�[�V�F�[�_�Ƃ���
	�ݒ肵�����̂� OnRender() �ɓn���B



	���[�U�[�V�F�[�_�ɗv�����钸�_�錾�́A
	�EPos0
	�ETexUV0
	�EColor0

	���[�U�[�V�F�[�_�ɗv������p�����[�^�́A
	�EVIEWPORTPIXELSIZE
	�ELNRC_TONE
		UI_TONE�ANODE_TONE �Ƃ��̂ق������������H
	��Tone �̌v�Z�̓��[�U�[�V�F�[�_�ŐF�����܂�����ɍs��Ȃ���΂Ȃ�Ȃ��B
	�i��Z�����ŕ\���ł��Ȃ��̂Œ��_�錾�����ł͕s���j
	#include "lumino.fx" �Ƃ����āA�s�N�Z���V�F�[�_�� LNUI_PostPixelShader �݂����Ȋ֐���
	���[�U�[�ɌĂ�ł��炤�K�v������B

	�`�������3�B
	�E�v���~�e�B�u�̂�
	�E�g�ݍ��݃V�F�[�_���g��
	�E���[�U�[�V�F�[�_���g��

	RenderingContext
	BasicRenderingContext
	UIRenderingContext
	SceneNodeRenderingContext
*/