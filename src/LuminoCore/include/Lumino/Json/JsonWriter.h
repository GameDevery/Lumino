﻿
#pragma once
#include <stack>
#include "../IO/TextWriter.hpp"
#include "Common.hpp"

namespace ln {
namespace tr {

/**
	@brief	
*/
class JsonWriter
{
public:
	JsonWriter(TextWriter* textWriter);
	virtual ~JsonWriter();

public:
	void setFormatting(JsonFormatting formatting);
	void writeStartObject();
	void writeEndObject();
	void writeStartArray();
	void writeEndArray();
	void writePropertyName(const Char* str, int length = -1);
	void writeNull();
	void writeBool(bool value);
	void writeInt32(int32_t value);
	void writeInt64(int64_t value);
	void writeFloat(float value);
	void writeDouble(double value);
	void writeString(const Char* str, int length = -1);
	bool isComplete() const;

protected:
	enum PrefixType
	{
		PrefixType_Array = 0,
		PrefixType_Object,
		PrefixType_Key,
	};

	void onPrefix(PrefixType type, int valueCount);
	void onStartObject();
	void onEndObject();
	void onStartArray();
	void onEndArray();
	void onKey(const Char* str, int length);
	void onNull();
	void onBool(bool value);
	void onInt32(int32_t value);
	void onInt64(int64_t value);
	void onFloat(float value);
	void onDouble(double value);
	void onString(const Char* str, int length);
	//void OnIndent(int level);

private:
	void autoComplete(JsonToken token);

private:
	struct Level
	{
		Level(bool inArray) : valueCount(0), inArray(inArray), justSawKey(false), justSawContainerEnd(false){}
		int		valueCount;				// 現在のネストレベルに含まれている値の数。=配列の要素数またはオブジェクトのメンバ数
		bool	inArray;				// true なら配列、false ならオブジェクト
		bool	justSawKey;
		bool	justSawContainerEnd;	// for indent
	};

	JsonFormatting	m_formatting;
	TextWriter*		m_textWriter;
	std::stack<Level>	m_levelStack;
	//bool			m_isComplete;
};

} // namespace tr
} // namespace ln
