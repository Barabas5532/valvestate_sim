#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Valvestate.h"

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new ValvestateAudioProcessor(std::make_unique<ValvestateProcessor>());
}
