#==============================================================================
# �����t�@�C�������[�v�Đ����܂��B
#==============================================================================
require 'Lumino'
include Lumino

# (�����ݒ�) Midi �̍Đ��� DirectMusic ���g�p����
Config.set_direct_music_initialize_mode(DirectMusicMode::NORMAL)

# �����@�\������������
Application.initialize_audio

# �����t�@�C������ Sound �I�u�W�F�N�g�����
sound = Sound.new("Media/ln21.wav")
sound.volume = 1.0				# ����
sound.pitch = 1.0				# �s�b�`
sound.is_loop_enabled = true	# ���[�vON

# �Đ�
sound.play

# Enter �҂�
gets

# ��~
sound.stop

# ���C�u�����̏I������
Application.terminate

