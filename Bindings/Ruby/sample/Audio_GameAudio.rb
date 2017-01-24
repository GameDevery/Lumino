#! ruby -EWindows-31J
# -*- mode:ruby; coding:Windows-31J -*-
#==============================================================================
# �Q�[���p�̉����Đ��@�\�̃T���v���ł��B
#==============================================================================
require './Lumino'
include Lumino

# �����@�\������������
Engine.initialize_audio

p "Enter �������� BGM ���Đ����܂�..."
gets	# Enter �҂�

# BGM ���Đ�����
GameAudio.play_bgm("Media/ln23.ogg");

p "Enter �������� BGS ���Đ����܂�..."
gets	# Enter �҂�

# BGS ���Đ�����
GameAudio.play_bgs("Media/water02.wav", 0.5);

p "Enter �������� ME ���Đ����܂�..."
gets	# Enter �҂�

# ME ���Đ�����
GameAudio.play_me("Media/lnme_victory1.ogg");

p "Enter �������� SE ���Đ����܂�..."
gets	# Enter �҂�

# SE ���Đ�����
GameAudio.play_se("Media/ln_cursor_1.wav");

p "Enter �������� BGM �� BGS ���~���܂�..."
gets	# Enter �҂�

# BGM ���~���� (5�b�����ăt�F�[�h�A�E�g)
GameAudio.stop_bgm(5.0);

# BGS ���~����
GameAudio.stop_bgs;

p "Enter �������ƏI�����܂�..."
gets	# Enter �҂�

# ���C�u�����̏I������
Engine.terminate

