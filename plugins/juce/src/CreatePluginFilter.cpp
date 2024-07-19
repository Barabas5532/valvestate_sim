#include "PluginProcessor.h"
#include "Valvestate.h"

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new ValvestateAudioProcessor(JucePlugin_Name, std::make_unique<ValvestateProcessor>());
}
