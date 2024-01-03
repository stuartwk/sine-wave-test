#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

Oscillator osc;
// AdEnv env;
// Switch button1;

// void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
// {
void AudioCallback(AudioHandle::InterleavingInputBuffer in,
									 AudioHandle::InterleavingOutputBuffer out,
									 size_t size)
{
	float osc_out, env_out;

	// Nobody likes a bouncy button
	// button1.Debounce();

	// If you push the button,...
	// if (button1.RisingEdge())
	// {
	// 	env.Trigger(); // Trigger the envelope!
	// }

	// Convert floating point knob to midi (0-127)
	// Then convert midi to freq. in Hz
	osc.SetFreq(mtof(hw.adc.GetFloat(0) * 127));

	// Fill the block with samples
	for (size_t i = 0; i < size; i += 2)
	{
		// Get the next envelope value
		// env_out = env.Process();
		// Set the oscillator volume to the latest env value
		osc.SetAmp(.1f);
		// get the next oscillator sample
		osc_out = osc.Process();

		// Set the left and right outputs
		out[i] = osc_out;
		out[i + 1] = osc_out;
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);

	float samplerate = hw.AudioSampleRate();

	// Create an ADC configuration
	AdcChannelConfig adcConfig;
	// Add pin 21 as an analog input in this config. We'll use this to read the knob
	// adcConfig.InitSingle(hw.GetPin(21));
	adcConfig.InitSingle(hw.GetPin(22));

	// Initialize the button on pin 28
	// button1.Init(hw.GetPin(25), samplerate / 48.f);

	// Set the ADC to use our configuration
	hw.adc.Init(&adcConfig, 1);

	// Set up oscillator
	osc.Init(samplerate);
	osc.SetWaveform(osc.WAVE_SIN);
	osc.SetAmp(.1f);
	osc.SetFreq(1000);

	// Set up volume envelope
	// env.Init(samplerate);
	// // Envelope attack and decay times
	// env.SetTime(ADENV_SEG_ATTACK, .01);
	// env.SetTime(ADENV_SEG_DECAY, .4);
	// // minimum and maximum envelope values
	// env.SetMin(0.0);
	// env.SetMax(1.f);
	// env.SetCurve(0); // linear

	// Start the adc
	hw.adc.Start();

	// Start calling the audio callback
	hw.StartAudio(AudioCallback);

	// Loop forever
	for (;;)
	{
	}
}