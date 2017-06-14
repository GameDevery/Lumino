#pragma once

namespace ln {

/**
	@brief	UUID (Universally Unique Identifier) �̃N���X�ł��B
*/
class Uuid
{
public:
	
	/** �V���� UUID �𐶐����܂��B*/
	static Uuid generate();

	/** ��� UUID �𐶐����܂��B"{00000000-0000-0000-0000-000000000000}" */
	Uuid();

	/** UUID ��������w�肵�ăC���X�^���X�𐶐����܂��B"{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}" */
	Uuid(const StringRef& uuidText);
	
	/** 16Byte �̔z����w�肵�ăC���X�^���X�𐶐����܂��B*/
	Uuid(const uint8_t* bytes);

	/** ������ɕϊ����܂��B*/
	String toString() const;

	// copy
	Uuid(const Uuid &other);
	Uuid& operator=(const Uuid& other);

	// compare
	bool operator==(const Uuid& other) const;
	bool operator!=(const Uuid& other) const;

private:
	std::vector<byte_t> m_data;
};

} // namespace ln

