﻿// Copyright (c) 2018 lriki. Distributed under the MIT license.
#pragma once

namespace ln {
typedef uint32_t FlagsType;

/** ビット演算を簡単に使用できるように enum 値をラップするクラスです。 */
template<typename EnumType, typename TBitsType = FlagsType>
class Flags
{
public:
    using BitsType = TBitsType;

    Flags()
        : m_bits(0)
    {
    }

    Flags(EnumType bit)
        : m_bits(static_cast<BitsType>(bit))
    {
    }

    Flags(const Flags<EnumType>& rhs)
        : m_bits(rhs.m_bits)
    {
    }

    explicit Flags(BitsType flags)
        : m_bits(flags)
    {
    }

	/** 指定されたビット値が設定されているかどうかを確認します。 */
    bool hasFlag(EnumType value) const
    {
        BitsType bitValue = static_cast<BitsType>(value);
        return (m_bits & bitValue) == bitValue && (bitValue != 0 || m_bits == bitValue);
    }

	/** 指定されたビット値を 1 に設定します。 */
    Flags<EnumType>& set(EnumType value)
    {
        m_bits |= static_cast<BitsType>(value);
        return *this;
    }

	/** 指定されたビット値を 0 に設定します。 */
    void unset(EnumType value)
    {
        m_bits &= ~static_cast<BitsType>(value);
    }

    EnumType get() const
    {
        return static_cast<EnumType>(m_bits);
    }

    Flags<EnumType>& operator=(const Flags<EnumType>& rhs)
    {
        m_bits = rhs.m_bits;
        return *this;
    }

    Flags<EnumType>& operator|=(const Flags<EnumType>& rhs)
    {
        m_bits |= rhs.m_bits;
        return *this;
    }

    Flags<EnumType>& operator&=(const Flags<EnumType>& rhs)
    {
        m_bits &= rhs.m_bits;
        return *this;
    }

    Flags<EnumType>& operator^=(const Flags<EnumType>& rhs)
    {
        m_bits ^= rhs.m_bits;
        return *this;
    }

    bool operator!() const
    {
        return !m_bits;
    }

    Flags<EnumType> operator~() const
    {
        Flags<EnumType> result(*this);
        result.m_bits = (BitsType)~m_bits;
        return result;
    }

    explicit operator bool() const
    {
        return !!m_bits;
    }

    explicit operator BitsType() const
    {
        return m_bits;
    }

    operator EnumType() const
    {
        return static_cast<EnumType>(m_bits);
    }

public:
    BitsType m_bits;
};

template<typename TEnumType>
static bool testFlag(TEnumType lhs, TEnumType rhs)
{
	return Flags<TEnumType>(lhs).hasFlag(rhs);
}

} // namespace ln

/** Flags<T> のためのグローバル演算子を定義するマクロ */
#define LN_FLAGS_OPERATORS(EnumType)                                                                                              \
    inline bool operator==(EnumType lhs, EnumType rhs)                                                                            \
    {                                                                                                                             \
        return static_cast<ln::Flags<EnumType>::BitsType>(lhs) == static_cast<ln::Flags<EnumType>::BitsType>(rhs);                \
    }                                                                                                                             \
    inline bool operator!=(EnumType lhs, EnumType rhs)                                                                            \
    {                                                                                                                             \
        return static_cast<ln::Flags<EnumType>::BitsType>(lhs) != static_cast<ln::Flags<EnumType>::BitsType>(rhs);                \
    }                                                                                                                             \
    inline ln::Flags<EnumType> operator|(EnumType lhs, EnumType rhs)                                                              \
    {                                                                                                                             \
        ln::Flags<EnumType> r(static_cast<ln::Flags<EnumType>::BitsType>(lhs) | static_cast<ln::Flags<EnumType>::BitsType>(rhs)); \
        return r;                                                                                                                 \
    }                                                                                                                             \
    inline ln::Flags<EnumType> operator&(EnumType lhs, EnumType rhs)                                                              \
    {                                                                                                                             \
        ln::Flags<EnumType> r(static_cast<ln::Flags<EnumType>::BitsType>(lhs) & static_cast<ln::Flags<EnumType>::BitsType>(rhs)); \
        return r;                                                                                                                 \
    }                                                                                                                             \
    inline ln::Flags<EnumType> operator^(EnumType lhs, EnumType rhs)                                                              \
    {                                                                                                                             \
        ln::Flags<EnumType> r(static_cast<ln::Flags<EnumType>::BitsType>(lhs) ^ static_cast<ln::Flags<EnumType>::BitsType>(rhs)); \
        return r;                                                                                                                 \
    }                                                                                                                             \
    inline ln::Flags<EnumType> operator~(EnumType a)                                                                              \
    {                                                                                                                             \
        return ~ln::Flags<EnumType>(a);                                                                                           \
    }
