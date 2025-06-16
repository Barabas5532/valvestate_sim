/*
    Copyright 2020 Barabas Raffai

    This file is part of Valvestate Sim.

    Valvestate Sim is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    Valvestate Sim is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License along
    with Valvestate Sim.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <juce_dsp/juce_dsp.h>

#include "PluginProcessor.h"
#include "ui.h"

using namespace juce;

class ValvestateAudioProcessorEditor final : public AudioProcessorEditor, private Timer
{
public:
    explicit ValvestateAudioProcessorEditor (ValvestateAudioProcessor&);

    void resized() override;

#if _DEBUG
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;
#endif

private:
    void timerCallback() override;

    void show_normal_ui();

    ValvestateAudioProcessor& processor;

    ValvestateLookAndFeel vsLookAndFeel;

    ThreadPool pool;
    std::vector<Image> preCachedImages;
    int totalImageCount;
    std::atomic<int> cachedImageCount{0};
    
    std::unique_ptr<ValvestateUi> ui;

    static constexpr int UI_WIDTH = 1267;
    static constexpr int UI_HEIGHT = 712;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValvestateAudioProcessorEditor)
};
