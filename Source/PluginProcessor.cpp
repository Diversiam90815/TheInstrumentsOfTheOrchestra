

//==============================================================================
//				INCLUDES
//==============================================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
//				AUDIO PROCESSOR
//==============================================================================


OrchestraProcessor::OrchestraProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
						 .withInput("Input", AudioChannelSet::stereo(), true)
#endif
						 .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	  )
#endif
{
	init();
}

OrchestraProcessor::~OrchestraProcessor()
{
}


void OrchestraProcessor::init()
{
	mOrchestraSampler.init();
	// mOrchestraSampler.mSampler.getSound()
}



//==============================================================================
//				JUCE Overrides
//==============================================================================

const String OrchestraProcessor::getName() const
{
	return JucePlugin_Name;
}


bool OrchestraProcessor::acceptsMidi() const
{
	return true;
}


bool OrchestraProcessor::producesMidi() const
{
	return false;
}


bool OrchestraProcessor::isMidiEffect() const
{
	return false;
}


double OrchestraProcessor::getTailLengthSeconds() const
{
	return 0.0;
}


int OrchestraProcessor::getNumPrograms()
{
	return 1;
}


int OrchestraProcessor::getCurrentProgram()
{
	return 0;
}


void OrchestraProcessor::setCurrentProgram(int index)
{
}


const String OrchestraProcessor::getProgramName(int index)
{
	return {};
}


void OrchestraProcessor::changeProgramName(int index, const String &newName)
{
}


void OrchestraProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	mOrchestraSampler.mSampler.setCurrentPlaybackSampleRate(sampleRate);
}


void OrchestraProcessor::releaseResources()
{
}


#ifndef JucePlugin_PreferredChannelConfigurations
bool OrchestraProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

	return true;
}
#endif


void OrchestraProcessor::processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages)
{
	ScopedNoDenormals noDenormals;
	auto			  totalNumInputChannels	 = getTotalNumInputChannels();
	auto			  totalNumOutputChannels = getTotalNumOutputChannels();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	if (!mOrchestraSampler.getSamplesAreReady())
		return;

	mOrchestraSampler.mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}



bool OrchestraProcessor::hasEditor() const
{
	return true;
}


AudioProcessorEditor *OrchestraProcessor::createEditor()
{
	return new OrchestraEditor(*this);
}


void OrchestraProcessor::getStateInformation(MemoryBlock &destData)
{
}


void OrchestraProcessor::setStateInformation(const void *data, int sizeInBytes)
{
}


AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
	return new OrchestraProcessor();
}
