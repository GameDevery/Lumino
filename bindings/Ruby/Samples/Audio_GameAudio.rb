#==============================================================================
# �Q�[���p�̉����Đ��@�\�̃T���v���ł��B
#==============================================================================
require 'Lumino'
include Lumino

# (�����ݒ�) Midi �̍Đ��� DirectMusic ���g�p����
Config.set_direct_music_initialize_mode(DirectMusicMode::NORMAL)

# �����@�\������������
Application.initialize_audio

# BGM ���Đ�����
GameAudio.play_bgm("Media/ln20.wav");

gets	# Enter �҂�

# BGS ���Đ�����
GameAudio.play_bgs(Media/water01.wav);

gets	# Enter �҂�

# ME ���Đ�����
GameAudio.play_me(Media/me.wav);

gets	# Enter �҂�

# SE ���Đ�����
GameAudio.play_se(Media/se.wav);

# BGM ���~���� (5�b�����ăt�F�[�h�A�E�g)
GameAudio.stop_bgm("Media/ln20.wav", 5.0);

# BGS ���~����
GameAudio.stop_bgs(Media/water01.wav);

gets	# Enter �҂�

# ���C�u�����̏I������
Application.terminate

