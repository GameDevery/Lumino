#==============================================================================
# 3D �T�E���h�̃T���v���ł��B
#==============================================================================
require './Lumino'
include Lumino

# �����@�\������������
Application.initialize_audio

# �����t�@�C������ Sound �I�u�W�F�N�g�����
sound = Sound.new("Media/water02.wav")
sound.is_3d_enabled = true                      # 3D �����Ƃ��čĐ�����
sound.emitter_position = Vector3.new(12.0, 0, 0)# �����ʒu
sound.emitter_max_distance = 10.0               # ���̋����܂ŕ�������
sound.is_loop_enabled = true                    # ���[�vON

# ���X�i�[�̈ʒu�E�����E���x���w�肷��
SoundListener.position = Vector3.new(5, 0, 0)
SoundListener.direction = Vector3.new(0, 0, 1)
SoundListener.velocity = Vector3.new(2, 0, 0)

# �Đ�
sound.play

# Enter �҂�
gets

# ��~
sound.stop

# ���C�u�����̏I������
Application.terminate

