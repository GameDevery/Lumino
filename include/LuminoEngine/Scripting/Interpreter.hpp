#pragma once
#include "../Base/Delegate.hpp"
#include "../Base/Serializer.hpp"


namespace ln {


class InterpreterCommand : public Object
{
public:
	String code;
	List<String> params;

	void serialize2(Serializer2& ar) override
	{
		ar & makeNVP(u"code", code);
		ar & makeNVP(u"params", params);
	}
};

class InterpreterCommandList : public Object
{
public:
	List<Ref<InterpreterCommand>> commands;

	void serialize2(Serializer2& ar) override
	{
		ar & makeNVP(u"commands", commands);
	}
};

using InterpreterCommandHandler = Delegate<bool(InterpreterCommandList* cmd)>;

/**
 * �R���[�`���x�[�X�̃X�N���v�g�C���^�v���^�B
 *
 * �c�N�[���̂Ɠ����d�g�݁B�����X�N���v�g���s����Ȃ� Lua �Ƃ��g���Ă��悢�̂����A����̏d�v�ȗv���́u���s��Ԃ�ۑ��ł���悤�ɂ��邱�Ɓv
 */
class Interpreter : public Object
{
public:
	Interpreter();
	virtual ~Interpreter() = default;

	/** �R�}���h���X�g��ݒ肵�A���s���J�n���� */
	void run(const Ref<InterpreterCommandList>& commandList);

	/** �R�}���h���X�g�̎��s���ł��邩 */
	bool isRunning() const;

	void update();

	/* �R�}���h���X�g�̎��s���I������ */
	void terminate();

	void registerCommandHandler(const StringRef& name, Ref<InterpreterCommandHandler> handler);

protected:

	void setWaitMode(const String& mode) { m_waitMode = mode; }
	void setWaitCount(int count) { m_waitCount = count; }

	/**  */
	const Ref<InterpreterCommand>& getCurrentCommand() const;

	/** Wait ���Ȃ� true ��Ԃ� */
	virtual bool onUpdateWait(const String& waitMode);


private:

	bool updateChild();
	bool updateWait();
	bool checkFreeze();

	/** �R�}���h��1���s���� */
	bool executeCommand();

	/** �R�}���h��1���s���� */
	virtual bool onExecuteCommand(InterpreterCommand* cmd);

	List<Ref<InterpreterCommand>> m_commandList;
	int m_index;
	String m_waitMode;
	int m_waitCount;
	std::unordered_map<String, Ref<InterpreterCommandHandler>> m_commandDelegateMap;
};


//struct TkMVSoundData
//{
//	String		name;
//	int				pitch;
//	int				volume;
//
//	void serialize(tr::Archive& ar, int version)
//	{
//		ar & LN_NVP(name);
//		ar & LN_NVP(pitch);
//		ar & LN_NVP(volume);
//	}
//};
//
//struct TkMVMapData
//{
//	bool			autoplayBgm;
//	bool			autoplayBgs;
//	TkMVSoundData	bgm;
//	TkMVSoundData	bgs;
//	String		displayName;
//
//	void serialize(tr::Archive& ar, int version)
//	{
//		ar & LN_NVP(autoplayBgm);
//		ar & LN_NVP(autoplayBgs);
//		ar & LN_NVP(bgm);
//		ar & LN_NVP(bgs);
//		ar & LN_NVP(displayName);
//	}
//};
//

} // namespace ln
