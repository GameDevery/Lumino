
#pragma once
#include "../Base/EnumFlags.h"
#include "../Base/List.h"
#include "../Base/String.h"

LN_NAMESPACE_BEGIN
namespace tr {
class JsonMember;

/** Json �̒l�̌^�������܂��B*/
enum class JsonType
{
	Null = 0,
	Bool,
	Double,
	String,
	Array,
	Object,
};

/**
	@brief	JSON�̒l��\���܂��B
*/
class JsonValue
{
public:
	JsonValue();
	explicit JsonValue(bool value);
	JsonValue(double value);
	explicit JsonValue(const Char* value);
	explicit JsonValue(const String& value);
	explicit JsonValue(const JsonValue& value);
	explicit JsonValue(JsonType type);
	virtual ~JsonValue();
	JsonValue& operator=(const JsonValue& obj);

public:
	JsonType getType()  const { return m_type; }
	bool isNull()   const { return m_type == JsonType::Null; }
	bool IsBool()   const { return m_type == JsonType::Bool; }
	bool IsDouble() const { return m_type == JsonType::Double; }
	bool IsString() const { return m_type == JsonType::String; }
	bool IsArray()  const { return m_type == JsonType::Array; }
	bool IsObject() const { return m_type == JsonType::Object; }

	/*-----------------------------------------------------------------------*/
	/** @name Null */
	/** @{ */

	/** ���̒l�� Null ��ݒ肵�܂��B*/
	void setNull();

	/** @} */
	/*-----------------------------------------------------------------------*/
	/** @name Bool */
	/** @{ */

	/** ���̒l�� Bool �l��ݒ肵�܂��B*/
	void setBool(bool value);

	/** ���̒l�� Bool �l���擾���܂��B*/
	bool getBool() const;

	/** @} */
	/*-----------------------------------------------------------------------*/
	/** @name Double */
	/** @{ */

	/** ���̒l�� Double �l��ݒ肵�܂��B*/
	void setDouble(double value);

	/** ���̒l�� Double �l���擾���܂��B*/
	double getDouble() const;

	/** @} */
	/*-----------------------------------------------------------------------*/
	/** @name String */
	/** @{ */

	/** ���̒l�ɕ������ݒ肵�܂��B*/
	void setString(const String& str);

	/** ���̒l�̕�������擾���܂��B*/
	const String& getString() const;

	/** @} */
	/*-----------------------------------------------------------------------*/
	/** @name Array */
	/** @{ */

	/** ���̒l�ɋ�̔z���ݒ肵�܂��B*/
	void setArray();

	/** ���̔z��Ɋ܂܂�Ă���l�̐����擾���܂��B*/
	int getItemCount() const;

	/** ���̔z��̖����ɗv�f��ǉ����܂��B*/
	void addItem(const JsonValue& value);

	/** �w�肵���C���f�b�N�X�̗v�f���擾���܂��B*/
	JsonValue& operator[](int index);
	const JsonValue& operator[](int index) const;		/**< @copydoc operator[] */

	/** @} */
	/*-----------------------------------------------------------------------*/
	/** @name Object */
	/** @{ */

	/** ���̒l�ɋ�̃I�u�W�F�N�g��ݒ肵�܂��B*/
	void setObject();

	/** ���̃I�u�W�F�N�g�Ɋ܂܂�Ă��郁���o�̐����擾���܂��B*/
	int getMemberCount() const;

	/** ���̃I�u�W�F�N�g�ɐV���������o��ǉ����܂��B*/
	void addMember(const String& name, const JsonValue& value);

	/** @} */

private:
	void copy(const JsonValue& obj);
	void detach();

	friend class JsonDOMHandler;
	void resizeValueList(int count) { m_valueList->resize(count); }
	JsonValue& getAtValue(int index) { return m_valueList->getAt(index); }
	void resizeMemberList(int count) { m_memberList->resize(count); }
	JsonMember& getAtMember(int index) { return m_memberList->getAt(index); }

private:
	typedef List<JsonValue>	ValueList;
	typedef List<JsonMember>	MemberList;

	JsonType	m_type;
	union
	{
		uint64_t	m_uint;
		bool		m_bool;
		double		m_double;
		String*		m_string;
		ValueList*	m_valueList;	// on array
		MemberList*	m_memberList;	// on object
	};
};

/**
	@brief	JSON �� Object �^�̒l���������o��\���܂��B
*/
class JsonMember
{
public:
	String		Name;
	JsonValue	Value;
};

} // namespace tr
LN_NAMESPACE_END
