
#include "../Internal.h"
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <io.h>
#include <sys/timeb.h>
#endif
#include <time.h>
#include <sstream>
#include <iomanip>
#include <Lumino/Base/Version.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/Environment.h>
#include <Lumino/Base/Logger.h>
#include <Lumino/Threading/Thread.h>
#include <Lumino/Threading/Mutex.h>
LN_NAMESPACE_BEGIN

//==============================================================================
// LogHelper
//==============================================================================
class LogHelper
{
public:
#ifdef _WIN32
	typedef timeb Time;

	static void GetTime(Time* t)
	{
		::ftime(t);
	}
#else
	struct Time
	{
		time_t time;
		unsigned short millitm;
	};

	static void GetTime(Time* t)
	{
		timeval tv;
		::gettimeofday(&tv, NULL);

		t->time = tv.tv_sec;
		t->millitm = static_cast<unsigned short>(tv.tv_usec / 1000);
	}
#endif

	static void GetLocalTime(struct tm* t, const time_t* time)
	{
#ifdef _WIN32
		::localtime_s(t, time);
#else
		::localtime_r(time, t);
#endif
	}
};

//==============================================================================
// LogRecord
//==============================================================================
class LogRecord
{
public:
	LogRecord(LogLevel level, const char* file, const char* func, int line)
		: m_level(level)
		, m_file(file)
		, m_func(func)
		, m_line(line)
		, m_threadId(Thread::GetCurrentThreadId())
	{
		LogHelper::GetTime(&m_time);
	}

	void SetMessage(const StringRefA& message) { m_message = message; }

	LogHelper::Time& GetTime() { return m_time; }

	LogLevel GetLevel() const { return m_level; }

	const StringA& GetMessage() const { return m_message; }

	const char* GetFile() const { return m_file; }

	const char* GetFunc() const { return m_func; }

	int GetLine() const { return m_line; }

	unsigned int GetThreadId() const { return m_threadId; }

private:
	LogHelper::Time	m_time;
	LogLevel		m_level;
	const char*		m_file;
	const char*		m_func;
	int				m_line;
	unsigned int	m_threadId;
	StringA			m_message;
};

//==============================================================================
// LogFile
//==============================================================================
class LogFile
{
public:
	LogFile()
		: m_file(-1)
	{
	}

	~LogFile()
	{
		Close();
	}

	bool IsOpend() const
	{
		return m_file != -1;
	}

#ifdef _WIN32
	void Open(const char* filePath)
	{
		Close();
		::_sopen_s(&m_file, filePath, _O_CREAT | _O_TRUNC | _O_WRONLY | _O_BINARY, _SH_DENYWR, _S_IREAD | _S_IWRITE);
	}

	int Write(const void* buf, size_t count)
	{
		return (m_file != -1) ? ::_write(m_file, buf, static_cast<unsigned int>(count)) : -1;
	}

	off_t Seek(off_t offset, int whence)
	{
		return (m_file != -1) ? ::_lseek(m_file, offset, whence) : -1;
	}

	void Close()
	{
		if (m_file != -1)
		{
			::_close(m_file);
			m_file = -1;
		}
	}
#else
	void Open(const char* filePath)
	{
		Close();
		m_file = ::open(fileName, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	}

	int Write(const void* buf, size_t count)
	{
		return (m_file != -1) ? static_cast<int>(::write(m_file, buf, count)) : -1;
	}

	off_t Seek(off_t offset, int whence)
	{
		return (m_file != -1) ? ::lseek(m_file, offset, whence) : -1;
	}

	void Close()
	{
		if (m_file != -1)
		{
			::close(m_file);
			m_file = -1;
		}
	}
#endif

private:
	int m_file;
};

//==============================================================================
// Logger
//==============================================================================

static LogFile				g_logFile;
static Mutex				g_logMutex;
static std::stringstream	g_logSS;

//------------------------------------------------------------------------------
void Logger2::Initialize(const StringRef& filePath)
{
	StringA path(filePath);
	g_logFile.Open(path.c_str());
}

//------------------------------------------------------------------------------
void Logger2::WriteLineInternal(LogLevel level, const char* file, const char* func, int line, const StringA& message)
{
	if (g_logFile.IsOpend())
	{
		LogRecord record(level, file, func, line);
		record.SetMessage(message);

		tm t;
		char date[64];
		LogHelper::GetLocalTime(&t, &record.GetTime().time);
		//strftime(date, sizeof(date), "%Y/%m/%d %a %H:%M:%S", &t);
		strftime(date, sizeof(date), "%Y/%m/%d %H:%M:%S", &t);

		g_logSS.str("");							// �o�b�t�@���N���A����B
		g_logSS.clear(std::stringstream::goodbit);	// �X�g���[���̏�Ԃ��N���A����B���̍s���Ȃ��ƈӐ}�ʂ�ɓ��삵�Ȃ�
		g_logSS << date << " ";
		g_logSS << std::setw(5) << std::left << GetLogLevelString(level) << " ";
		g_logSS << "[" << record.GetThreadId() << "]";
		g_logSS << "[" << record.GetFunc() << "(" << record.GetLine() << ")] ";
		g_logSS << record.GetMessage().c_str() << std::endl;

		auto str = g_logSS.str();
		g_logFile.Write(str.c_str(), str.length());
	}
}

//------------------------------------------------------------------------------
void Logger2::WriteLineInternal(LogLevel level, const char* file, const char* func, int line, const StringW& message)
{
	WriteLineInternal(level, file, func, line, message.ToStringA());
}

//------------------------------------------------------------------------------
const char* Logger2::GetLogLevelString(LogLevel level)
{
	switch (level)
	{
	case LogLevel::Fatal:
		return "Fatal";
	case LogLevel::Error:
		return "Error";
	case LogLevel::Warning:
		return "Warning";
	case LogLevel::Info:
		return "Info";
	case LogLevel::Debug:
		return "Debug";
	case LogLevel::Verbose:
		return "Verbose";
	default:
		return "";
	}
}






//==============================================================================
// Logger
//==============================================================================

static const int	TEMP_BUFFER_SIZE = 2048;
static uint64_t		g_logStartTime;

static TCHAR		g_logFilePath[LN_MAX_PATH] = { 0 };

class FileClose
{
public:
	FileClose() {}
	~FileClose()
	{
		FILE* stream;
		if (_tfopen_s(&stream, g_logFilePath, _T("a+")) == 0)
		{
			_ftprintf(
				stream,
				_T("============== Application Exit ============== \n"));
			fclose(stream);
		}
	}
};
static FileClose g_fileClose;

//------------------------------------------------------------------------------
bool Logger::Initialize(const TCHAR* filePath) throw()
{
	//if (log_dir)
	//{
	//	_tcscpy(gLogFilePath, log_dir);
	//	size_t len = _tcslen(log_dir);
	//	gLogFilePath[len] = '/';//'\\';
	//	_tcscpy(&gLogFilePath[len + 1], log_name);
	//}
	//else
	//{
	//	System::Environment::getCurrentDirectory(gLogFilePath);
	//	size_t len = _tcslen(gLogFilePath);
	//	gLogFilePath[len] = '/';//'\\';
	//	_tcscpy(&gLogFilePath[len + 1], log_name);
	//}
	_tcscpy_s(g_logFilePath, LN_MAX_PATH, filePath);

	FILE* stream;
	if (_tfopen_s(&stream, g_logFilePath, _T("w+")) != 0) {
		g_logFilePath[0] = _T('\0');
		return false;
	}

	//time_t timer;
	//time(&timer);
	//char* ts = asctime(localtime(&timer));

	fprintf(stream, "===============================================================\n");
	fprintf(stream, " %s Log    Version:%s\n", LUMINO_NAME, LUMINO_CORE_VERSION_STRING);//    %s\n", LUMINO_CORE_VERSION_STRING, ts);
	fprintf(stream, "---------------------------------------------------------------\n");

#ifdef LN_DEBUG
	fprintf(stream, "BuildConfig : Debug\n");
#else
	fprintf(stream, "BuildConfig : Release\n");
#endif

#ifdef LN_DEBUG
	fprintf(stream, "CharSet     : Unicode\n");
#else
	fprintf(stream, "CharSet     : MultiByte\n");
#endif

	fprintf(stream, "===============================================================\n");

	fclose(stream);

	g_logStartTime = Environment::GetTickCount();
	return true;
}

//------------------------------------------------------------------------------
static const char* GetInfoString(Logger::Level level)
{
	switch (level)
	{
	default:
	case Logger::Level::Info:    return "Info    ";
	case Logger::Level::Warning: return "Warning ";
	case Logger::Level::Error:   return "Error   ";
	}
}

//------------------------------------------------------------------------------
void Logger::WriteLine(Level level, const char* format, ...) throw()
{
	if (g_logFilePath[0] == '\0') {
		return;
	}
	FILE* stream;
	if (_tfopen_s(&stream, g_logFilePath, _T("a+")) == 0)
	{
		char buf[TEMP_BUFFER_SIZE];

		va_list args;
		va_start(args, format);
		StringTraits::VSPrintf(buf, TEMP_BUFFER_SIZE, format, args);
		va_end(args);

		fprintf(stream, "%llu %s: ", Environment::GetTickCount() - g_logStartTime, GetInfoString(level));
		fprintf(stream, "%s\n", buf);

		fclose(stream);
	}
}

//------------------------------------------------------------------------------
void Logger::WriteLine(Level level, const wchar_t* format, ...) throw()
{
	if (g_logFilePath[0] == '\0') {
		return;
	}
	FILE* stream;
	if (_tfopen_s(&stream, g_logFilePath, _T("a+")) == 0)
	{
		wchar_t buf[TEMP_BUFFER_SIZE];

		va_list args;
		va_start(args, format);
		StringTraits::VSPrintf(buf, TEMP_BUFFER_SIZE, format, args);
		va_end(args);

		fprintf(stream, "%llu %s: ", Environment::GetTickCount() - g_logStartTime, GetInfoString(level));
		fwprintf(stream, L"%s\n", buf);

		fclose(stream);
	}

}
//------------------------------------------------------------------------------
void Logger::WriteLine(const char* format, ...) throw()
{
	if (g_logFilePath[0] == '\0') {
		return;
	}
	FILE* stream;
	if (_tfopen_s(&stream, g_logFilePath, _T("a+")) == 0)
	{
		char buf[TEMP_BUFFER_SIZE];

		va_list args;
		va_start(args, format);
		StringTraits::VSPrintf(buf, TEMP_BUFFER_SIZE, format, args);
		va_end(args);

		fprintf(stream, "%llu %s: ", Environment::GetTickCount() - g_logStartTime, GetInfoString(Level::Info));
		fprintf(stream, "%s\n", buf);

		fclose(stream);
	}
}

//------------------------------------------------------------------------------
void Logger::WriteLine(const wchar_t* format, ...) throw()
{
	if (g_logFilePath[0] == '\0') {
		return;
	}
	FILE* stream;
	if (_tfopen_s(&stream, g_logFilePath, _T("a+")) == 0)
	{
		wchar_t buf[TEMP_BUFFER_SIZE];

		va_list args;
		va_start(args, format);
		StringTraits::VSPrintf(buf, TEMP_BUFFER_SIZE, format, args);
		va_end(args);

		fprintf(stream, "%llu %s: ", Environment::GetTickCount() - g_logStartTime, GetInfoString(Level::Info));
		fwprintf(stream, L"%s\n", buf);

		fclose(stream);
	}

}

LN_NAMESPACE_END