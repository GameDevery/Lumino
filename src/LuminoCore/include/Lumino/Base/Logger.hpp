﻿// Copyright (c) 2018 lriki. Distributed under the MIT license.

#pragma once

#include <sstream>
#ifdef _WIN32
#include <sys/timeb.h>
#else
#include <time.h>
#endif

//#ifdef _MSC_VER
//#define LN_FUNC_MACRO		__FUNCTION__
//#else
//#define LN_FUNC_MACRO		__PRETTY_FUNCTION__
//#endif

#define LN_LOG(level)		!(::ln::detail::LoggerInterface::getInstance() && ::ln::detail::LoggerInterface::getInstance()->CheckLevel(level)) ? (void)0 : (*::ln::detail::LoggerInterface::getInstance()) += ::ln::detail::LogRecord(level, __FILE__, LN_FUNC_MACRO, __LINE__)
#define LN_LOG_FATAL		LN_LOG(::ln::LogLevel::Fatal)
#define LN_LOG_ERROR		LN_LOG(::ln::LogLevel::Error)
#define LN_LOG_WARNING		LN_LOG(::ln::LogLevel::Warning)
#define LN_LOG_INFO			LN_LOG(::ln::LogLevel::Info)
#define LN_LOG_DEBUG		LN_LOG(::ln::LogLevel::Debug)
#define LN_LOG_VERBOSE		LN_LOG(::ln::LogLevel::Verbose)

namespace ln {
class StringRef;

/** ログの通知レベル */
enum class LogLevel
{
	Fatal,
	Error,
	Warning,
	Info,
	Debug,
	Verbose,
};

/** グローバルなロギング機能をコントロールするためのクラスです。 */
class GlobalLogger
{
public:

	/** ログをファイルへ出力するためのアダプタを追加します。
		@param[in]	filePath	: ログファイルのパス
		@return		true=成功 / false=失敗
	*/
	static bool addFileAdapter(const StringRef& filePath);
	
	/** ログを標準エラーへ出力するためのアダプタを追加します。
	*/
	static void addStdErrAdapter();
};


namespace detail {

#ifdef _WIN32
typedef timeb LogTime;
#else
struct LogTime
{
	time_t time;
	unsigned short millitm;
};
#endif

class LogRecord
{
public:
	LogRecord(LogLevel level, const char* file, const char* func, int line);
	const LogTime& getTime() const { return m_time; }
	LogLevel GetLevel() const { return m_level; }
	const char* getMessage() const;
	const char* GetFile() const { return m_file; }
	const char* GetFunc() const { return m_func; }
	int GetLine() const { return m_line; }
	unsigned int getThreadId() const { return m_threadId; }

	//LogRecord& operator<<(const wchar_t* str);

	template<typename T>
	LogRecord& operator<<(const T& data)
	{
		m_message << data;
		return *this;
	}

private:
	LogTime				m_time;
	LogLevel			m_level;
	const char*			m_file;
	const char*			m_func;
	int					m_line;
	unsigned int		m_threadId;
	std::stringstream	m_message;
	mutable std::string m_messageStr;
};

class ILoggerAdapter
{
public:
	virtual ~ILoggerAdapter();
	virtual void write(const char* str, size_t len) = 0;
};

class LoggerInterface
{
public:
	static LoggerInterface* getInstance();

	LoggerInterface();
	~LoggerInterface();
	bool CheckLevel(LogLevel level);
	void operator+=(const LogRecord& record);

	class Impl;
	Impl* m_impl;
};

} // namespace detail
} // namespace ln
