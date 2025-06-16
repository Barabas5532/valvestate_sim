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

#include "PluginEditor.h"
#include "PluginProcessor.h"

const char *get_gain_image_resource(int &dataSizeInBytes) {
  dataSizeInBytes = gain_binary_data::gain_pngSize;
  return gain_binary_data::gain_png;
}

const char *get_bass_image_resource(int &dataSizeInBytes) {
  dataSizeInBytes = bass_binary_data::bass_pngSize;
  return bass_binary_data::bass_png;
}

const char *get_middle_image_resource(int &dataSizeInBytes) {
  dataSizeInBytes = middle_binary_data::middle_pngSize;
  return middle_binary_data::middle_png;
}

const char *get_treble_image_resource(int &dataSizeInBytes) {
  dataSizeInBytes = treble_binary_data::treble_pngSize;
  return treble_binary_data::treble_png;
}

const char *get_contour_image_resource(int &dataSizeInBytes) {
  dataSizeInBytes = contour_binary_data::contour_pngSize;
  return contour_binary_data::contour_png;
}

const char *get_volume_image_resource(int &dataSizeInBytes) {
  dataSizeInBytes = volume_binary_data::volume_pngSize;
  return volume_binary_data::volume_png;
}

const char *get_led_image_resource(int &dataSizeInBytes) {
  dataSizeInBytes = BackgroundBinaryData::led_pngSize;
  return BackgroundBinaryData::led_png;
}

ValvestateAudioProcessorEditor::ValvestateAudioProcessorEditor(
    ValvestateAudioProcessor &p)
    : AudioProcessorEditor(&p), processor(p) {
  startTimer(100);
      
  setResizable(false, false);

  auto assets = {
      get_gain_image_resource,    get_bass_image_resource,
      get_middle_image_resource,  get_treble_image_resource,
      get_contour_image_resource, get_volume_image_resource,
      get_led_image_resource,
  };
  
  totalImageCount = assets.size();

  // Pre cache the image while showing a loading screen
  for (const auto &asset : assets) {
    pool.addJob([this, asset]() -> void {
      int size;
      auto data = asset(size);
      this->preCachedImages.push_back(ImageCache::getFromMemory(data, size));
      cachedImageCount++;
    });
  }

  // A UI timer will wake up this object later to poll the cachedImageCount to
  // switch from the loading screen to the real UI.

  setSize(UI_WIDTH, UI_HEIGHT);
}

void ValvestateAudioProcessorEditor::resized() {
  if (ui)
    ui->setBounds(getLocalBounds());
}

void ValvestateAudioProcessorEditor::timerCallback() {
  if (cachedImageCount == totalImageCount) {
    show_normal_ui();
    stopTimer();
  }
}

void ValvestateAudioProcessorEditor::show_normal_ui(){
  ui = std::make_unique<ValvestateUi>(processor.parameters);

  ui->setBounds(getLocalBounds());
  addAndMakeVisible(*ui);
}

#if _DEBUG
void ValvestateAudioProcessorEditor::mouseDown(const MouseEvent &event) {
  Component::mouseDown(event);

  Logger::outputDebugString(
      juce::String::formatted("mouse down %d %d", event.x, event.y));
}

void ValvestateAudioProcessorEditor::mouseUp(const MouseEvent &event) {
  Component::mouseUp(event);

  Logger::outputDebugString(
      juce::String::formatted("mouse up %d %d", event.x, event.y));
}
#endif