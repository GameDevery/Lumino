
@echo off
set MY_DOCUMENTS=%USERPROFILE%\Documents
set VS_TEMPLATES_DIR=%MY_DOCUMENTS%\Visual Studio 2015\Templates\ProjectTemplates\

echo %VS_TEMPLATES_DIR%

if exist "%VS_TEMPLATES_DIR%" (
copy "tools\LuminoProjectCS.zip" "%VS_TEMPLATES_DIR%\LuminoProjectCS.zip"
echo "VisualStudio2015 �Ƀv���W�F�N�g�e���v���[�g���C���X�g�[�����܂����B"
echo "[�V�����v���W�F�N�g] �� [�e���v���[�g] > [Visual C#] ���m�F���Ă��������B"
) else (
echo "VisualStudio2015 �̃v���W�F�N�g�e���v���[�g�t�H���_��������Ȃ��������߁A�e���v���[�g�̃C���X�g�[�����s���܂���ł����B"
)

pause
