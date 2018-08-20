﻿
#include "Internal.hpp"
#include <Lumino/Audio/AudioNode.hpp>
#include <Lumino/Audio/AudioContext.hpp>
#include "CoreAudioNode.hpp"
#include "ALAudioDevice.hpp"
#include "AudioManager.hpp"

namespace ln {

//==============================================================================
// AudioContext

AudioContext* AudioContext::primary()
{
	return detail::EngineDomain::audioManager()->primaryContext();
}

AudioContext::AudioContext()
{
}

void AudioContext::initialize()
{
	m_manager = detail::EngineDomain::audioManager();

	auto device = makeRef<detail::ALAudioDevice>();
	device->initialize();
	m_device = device;

	m_audioContextHandler = makeRef<detail::AudioContextCore>();

	m_coreDestinationNode = makeRef<detail::CoreAudioDestinationNode>(m_audioContextHandler);
	m_coreDestinationNode->initialize();

	m_device->setRenderCallback(m_coreDestinationNode);

	m_destinationNode = newObject<AudioDestinationNode>(m_coreDestinationNode);

	// TODO: test
	//ssss = newObject<CoreAudioSourceNode>(u"D:\\tmp\\8_MapBGM2.wav");
	//CoreAudioNode::connect(ssss, m_destinationNode);
}

void AudioContext::dispose()
{
	if (m_device)
	{
		m_device->dispose();
		m_device.reset();
	}
}

void AudioContext::process()
{
	m_device->updateProcess();
}

AudioDestinationNode* AudioContext::destination() const
{
	return m_destinationNode;
}

detail::AudioContextCore* AudioContext::coreObject()
{
	return m_audioContextHandler;
}

} // namespace ln

