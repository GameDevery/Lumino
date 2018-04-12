﻿#include "Common.hpp"
#include <Lumino/Text/EncodingConverter.hpp>

class Test_Text_EncodingConverter : public ::testing::Test {};

TEST_F(Test_Text_EncodingConverter, AsciiCode)
{
	EncodingConversionOptions options;
	options.NullTerminated = true;
	EncodingConverter convAsciiToUtf16;
	convAsciiToUtf16.getSourceEncoding(TextEncoding::systemMultiByteEncoding());
	convAsciiToUtf16.setDestinationEncoding(TextEncoding::utf16Encoding());
	convAsciiToUtf16.setConversionOptions(options);
	ByteBuffer buf = convAsciiToUtf16.convert("ab", 2);
	const UTF16* utf16 = (const UTF16*)buf.getConstData();
	ASSERT_EQ('a', utf16[0]);
	ASSERT_EQ('b', utf16[1]);
	ASSERT_EQ(0x00, utf16[2]);
}
