﻿
#include "Internal.hpp"
#include <LuminoCore/Serialization/Serialization.hpp>

namespace ln {

/*
	write 時の基本方針
	- 何か値を write しようとしたら、オブジェクト先頭など、何か「書き込み待ち」になっているものを書く

	文字列や配列扱いしたいオブジェクトの write 動作
	- まず普通の Object として writeValue		→ コンテナ扱い
	- serialize の中で setTag する


	read 時の基本方針
	- オブジェクト型ならまず setNextName()
	- read 使用としている型がプリミティブ型でなければ store->readContainer
	- 次に store->readValue すると、現在の Container と setNextName() した名前から値を取得する

*/



/*
	- in object, out object に関わらず、serialize() が呼ばれたとき、そのオブジェクト用の Node は作られている。
		- Write : Node の state は Ready になっている。まだ Object か Array かはわからない。→ 最初の serialize で nextName が呼ばれるかどうかで決まる。
		- Read  : Node の state は Object または Array になっている。

	- State
		- Value  : リーフノードを示す。state 変更に続いて、"1度だけ"、"プリミティブな Value" のみシリアライズできる。
		- Array  : makeArrayTag によって Node の状態を確定する。その後は任意の回数 serialize できるが、NVP を serialize することはできない。
		- Object : makeXXXXTag が呼ばれずに、子の serialize が行われた場合は Object となる。
		           NVP 以外の値を１度だけ serialize することができる。この場合、子は親 Node の一部として serialize される。(子の serialize 開始時は Ready (親Nodeのまま) となる。スマートポインタ・Variant用)

	- BaseClass
		- 考え方は NVP と同じ。NVP の name の部分が "_base_class" とかになった状態で別 Object としてシリアライズする。

	- makeStringTag
		- Write : 
*/

//==============================================================================
// Archive

const String Archive::ClassNameKey = _TT("lumino_class_name");
const String Archive::ClassVersionKey = _TT("lumino_class_version");
const String Archive::ClassBaseKey = _TT("lumino_base_class");

//==============================================================================
// JsonTextOutputArchive

JsonTextOutputArchive::JsonTextOutputArchive()
    : m_localDoc()
    , m_localStore(&m_localDoc)
    , m_processing(false)
{
    setup(&m_localStore, ArchiveMode::Save);
}

JsonTextOutputArchive::~JsonTextOutputArchive()
{
}

String JsonTextOutputArchive::toString(JsonFormatting formatting)
{
    return m_localDoc.toString(formatting);
}

//==============================================================================
// JsonTextInputArchive

JsonTextInputArchive::JsonTextInputArchive(const String& jsonText)
    : m_localDoc()
    , m_localStore(&m_localDoc)
    , m_processing(false)
{
    m_localDoc.parse(jsonText);
    setup(&m_localStore, ArchiveMode::Load);
}

JsonTextInputArchive::~JsonTextInputArchive()
{
}

} // namespace ln
