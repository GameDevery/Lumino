
#include <iostream>
#include <Lumino/Text/Encoding.h>
#include <Lumino/IO/StreamReader.h>
#include <Lumino/IO/FileManager.h>
#include "../../Modeling/ModelManager.h"
#include "MqoImporter.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// ModelManager
//==============================================================================

//------------------------------------------------------------------------------
MqoImporter::MqoImporter()
{
}

//------------------------------------------------------------------------------
RefPtr<MeshResource> MqoImporter::Import(ModelManager* manager, const StringRef& filePath)
{
	LN_FAIL_CHECK_ARG(manager != nullptr) return nullptr;

	m_filePath = PathName(filePath);
	m_parentDir = m_filePath.GetParent();

	RefPtr<Stream> stream(manager->GetFileManager()->CreateFileStream(filePath), false);

	// Metasequoia4 �ŏo�͂���� .mqo �t�@�C���̕����R�[�h�� Shift_JIS ������
	// TODO: GenericStreamReader �Ƃ��ɂ��� char �̂܂܏����ł���΂����ƍ����ɂȂ�
	StreamReader reader(stream, Encoding::GetEncoding(EncodingType::SJIS));

	String line;
	while (reader.ReadLine(&line))
	{
		// Material
		int index = line.IndexOf(_T("Material"));
		if (index > -1)
		{
			int count = StringTraits::ToInt32(line.c_str() + index + 9);
			LoadMaterials(&reader);
		}

		// Object
		index = line.IndexOf(_T("Object"));
		if (index > -1)
		{
			int count = StringTraits::ToInt32(line.c_str() + index + 7);
			LoadObject(&reader);
		}
	}

	LN_NOTIMPLEMENTED();
	return nullptr;
}

//------------------------------------------------------------------------------
void MqoImporter::LoadMaterials(StreamReader* reader)
{
	String line;
	while (reader->ReadLine(&line))
	{
		if (line.IndexOf(_T("}")) > -1) break;

		// �}�e���A��1�̏��́A\s ��؂�̂P�s�ɂȂ��Ă���B
		// �ŏ��̃f�[�^�̓}�e���A�����B����͔�΂��B
		int dataHead = 0;
		{
			int first = line.IndexOf('"');
			int second = line.IndexOf('"', first + 1);
			dataHead = second + 2;
		}

		Color color = Color::White;
		float diffuse = 1.0f;
		float ambient = 1.0f;
		float emissive = 1.0f;
		float specular = 1.0f;
		float power = 1.0f;
		RefPtr<Texture> texture;

		while (dataHead < line.GetLength())
		{
			int numHead = line.IndexOf('(', dataHead) + 1;
			int numEnd = line.IndexOf(')', dataHead);
			int dataEnd = numEnd + 1;
			if (line[numHead] == '"')	// tex("�t�@�C����") �ɔ�����
			{
				numHead++;
				numEnd = line.IndexOf("\")", dataHead);
			}

			//if (line.IndexOf(_T("shader"), dataHead, CaseSensitivity::CaseInsensitive)
			//if (line.IndexOf(_T("vcol"), dataHead, CaseSensitivity::CaseInsensitive)	���_�J���[
			//if (line.IndexOf(_T("dbls"), dataHead, CaseSensitivity::CaseInsensitive)	���ʕ\��
			if (line.IndexOf(_T("col"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	�F�i�q�f�a�j�A�s�����x
			{
				const char * pp = line.c_str() + numHead;
				sscanf(line.c_str() + numHead, "%f %f %f %f", &color.r, &color.g, &color.b, &color.a);
			}
			if (line.IndexOf(_T("dif"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	�g�U��	0�`1
			{
				diffuse = StringTraits::ToDouble(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.IndexOf(_T("amb"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	���͌�	0�`1
			{
				ambient = StringTraits::ToDouble(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.IndexOf(_T("emi"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	���ȏƖ�	0�`1
			{
				emissive = StringTraits::ToDouble(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.IndexOf(_T("spc"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	���ˌ�	0�`1
			{
				specular = StringTraits::ToDouble(line.c_str() + numHead, dataEnd - numHead);
			}
			if (line.IndexOf(_T("power"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	���ˌ��̋���	0�`100
			{
				power = StringTraits::ToDouble(line.c_str() + numHead, dataEnd - numHead);
			}
			//if (line.IndexOf(_T("reflect"), dataHead, CaseSensitivity::CaseInsensitive)	���ʔ��� �iVer4.0�ȍ~)	0�`1
			//if (line.IndexOf(_T("refract"), dataHead, CaseSensitivity::CaseInsensitive)	���ܗ� �iVer4.0�ȍ~)	1�`5
			if (line.IndexOf(_T("tex"), dataHead, CaseSensitivity::CaseInsensitive) == dataHead) //	�͗l�}�b�s���O��	���΃p�X�܂��͐�΃p�X�ŋL�q
			{
				texture = m_manager->CreateTexture(m_parentDir, StringRef(line.c_str() + numHead, line.c_str() + numEnd), ModelCreationFlag::IgnoreTextureNotFound);
			}
			//if (line.IndexOf(_T("aplane"), dataHead, CaseSensitivity::CaseInsensitive)	�����}�b�s���O��	���΃p�X�܂��͐�΃p�X�ŋL�q
			//if (line.IndexOf(_T("bump"), dataHead, CaseSensitivity::CaseInsensitive)	���ʃ}�b�s���O��	���΃p�X�܂��͐�΃p�X�ŋL�q
			//if (line.IndexOf(_T("proj_type"), dataHead, CaseSensitivity::CaseInsensitive)	�}�b�s���O����
			//if (line.IndexOf(_T("proj_pos"), dataHead, CaseSensitivity::CaseInsensitive)	���e�ʒu�i�w�x�y�j
			//if (line.IndexOf(_T("proj_scale"), dataHead, CaseSensitivity::CaseInsensitive)	���e�g�嗦�i�w�x�y�j
			//if (line.IndexOf(_T("proj_angle"), dataHead, CaseSensitivity::CaseInsensitive)	���e�p�x�i�g�o�a�j - 180�`180

			dataHead = dataEnd + 1;
		}
		
		Color c;
		c.a = color.a;

		auto material = RefPtr<DiffuseMaterial>::MakeRef();
		material->Initialize();

		c.r = diffuse * color.r;
		c.g = diffuse * color.g;
		c.b = diffuse * color.b;
		material->SetDiffuse(c);

		c.r = ambient * color.r;
		c.g = ambient * color.g;
		c.b = ambient * color.b;
		material->SetAmbient(c);

		c.r = emissive * color.r;
		c.g = emissive * color.g;
		c.b = emissive * color.b;
		material->SetEmissive(c);

		c.r = specular * color.r;
		c.g = specular * color.g;
		c.b = specular * color.b;
		material->SetSpecular(c);

		material->SetSpecularPower(power);
	}
}

//------------------------------------------------------------------------------
void MqoImporter::LoadObject(StreamReader* reader)
{

}

} // namespace detail
LN_NAMESPACE_END
