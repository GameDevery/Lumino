﻿#pragma once
#include "AudioNode.hpp"

namespace ln {

class AudioSourceNode
	: public AudioNode
{
public:
    enum class PlayingState
    {
        NoChanged,
        Stop,
        Play,
        Pause,
    };

	void setPlaybackRate(float rate);

    void setLoop(bool value);
    bool loop() const;

	void start();
	void stop();
	void pause();
	void resume();
    PlayingState playingState() const;

LN_CONSTRUCT_ACCESS:
	AudioSourceNode();
	virtual ~AudioSourceNode() = default;
	void init(detail::AudioDecoder* decoder);
	virtual detail::AudioNodeCore* coreNode() override;
	virtual void commit() override;

private:

	struct CommitState
	{
		float playbackRate;
		PlayingState requestedState;
        bool loop;
		bool resetRequire;
	};

	Ref<detail::AudioSourceNodeCore> m_coreObject;
	CommitState m_commitState;
};

} // namespace ln

