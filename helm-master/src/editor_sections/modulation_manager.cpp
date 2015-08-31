/* Copyright 2013-2015 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "modulation_manager.h"

#include "full_interface.h"
#include "modulation_look_and_feel.h"
#include "modulation_slider.h"
#include "synth_gui_interface.h"
#include "helm_common.h"

#define FRAMES_PER_SECOND 60

ModulationManager::ModulationManager(
    mopo::output_map modulation_sources,
    std::map<std::string, ModulationButton*> modulation_buttons,
    std::map<std::string, SynthSlider*> sliders,
    mopo::output_map mono_modulations,
    mopo::output_map poly_modulations) : SynthSection("modulation") {
  modulation_buttons_ = modulation_buttons;
  modulation_sources_ = modulation_sources;
  setInterceptsMouseClicks(false, true);
  startTimerHz(FRAMES_PER_SECOND);

  current_modulator_ = "";
  last_value_ = -1.0;

  polyphonic_destinations_ = new Component();
  polyphonic_destinations_->setInterceptsMouseClicks(false, true);

  monophonic_destinations_ = new Component();
  monophonic_destinations_->setInterceptsMouseClicks(false, true);

  for (auto mod_button : modulation_buttons_)
    mod_button.second->addListener(this);

  slider_model_lookup_ = sliders;
  for (auto slider : slider_model_lookup_) {
    std::string name = slider.first;
    const mopo::Processor::Output* mono_total = mono_modulations[name];
    const mopo::Processor::Output* poly_total = poly_modulations[name];

    // Create modulation meter.
    if (mono_total) {
      std::string name = slider.second->getName().toStdString();
      ModulationMeter* meter = new ModulationMeter(mono_total, poly_total, slider.second);
      addAndMakeVisible(meter);
      meter_lookup_[name] = meter;
      meter->setName(name);
      Rectangle<int> local_bounds = slider.second->getBoundsInParent();
      meter->setBounds(slider.second->getParentComponent()->localAreaToGlobal(local_bounds));
    }

    // Create modulation slider.
    ModulationSlider* mod_slider = new ModulationSlider(slider.second);
    mod_slider->setLookAndFeel(ModulationLookAndFeel::instance());
    mod_slider->addListener(this);
    if (poly_total)
      polyphonic_destinations_->addAndMakeVisible(mod_slider);
    else
      monophonic_destinations_->addAndMakeVisible(mod_slider);

    slider_lookup_[name] = mod_slider;
    owned_sliders_.push_back(mod_slider);
  }

  addAndMakeVisible(polyphonic_destinations_);
  addAndMakeVisible(monophonic_destinations_);

  forgetModulator();
}

ModulationManager::~ModulationManager() {
  for (auto meter : meter_lookup_)
    delete meter.second;
  for (Slider* slider : owned_sliders_)
    delete slider;
}

void ModulationManager::paint(Graphics& g) {
}

void ModulationManager::resized() {
  polyphonic_destinations_->setBounds(getBounds());
  monophonic_destinations_->setBounds(getBounds());
  for (auto slider : slider_lookup_) {
    SynthSlider* model = slider_model_lookup_[slider.first];
    Point<float> local_top_left = getLocalPoint(model, Point<float>(0.0f, 0.0f));
    slider.second->setVisible(model->isVisible());
    slider.second->setBounds(local_top_left.x, local_top_left.y,
                             model->getWidth(), model->getHeight());
  }

  for (auto meter : meter_lookup_) {
    Slider* model = slider_model_lookup_[meter.first];
    Point<float> local_top_left = getLocalPoint(model, Point<float>(0.0f, 0.0f));
    meter.second->setBounds(local_top_left.x, local_top_left.y,
                            model->getWidth(), model->getHeight());
    meter.second->setVisible(model->isVisible());
  }

  SynthSection::resized();
}

void ModulationManager::buttonClicked(juce::Button *clicked_button) {
  std::string name = clicked_button->getName().toStdString();
  if (clicked_button->getToggleState()) {
    if (current_modulator_ != "") {
      Button* modulator = modulation_buttons_[current_modulator_];
      modulator->setToggleState(false, NotificationType::dontSendNotification);
    }
    changeModulator(name);
  }
  else
    forgetModulator();
}

void ModulationManager::sliderValueChanged(juce::Slider *moved_slider) {
  std::string destination_name = moved_slider->getName().toStdString();
  setModulationAmount(current_modulator_, destination_name, moved_slider->getValue());

  if ((bool)last_value_ != (bool)moved_slider->getValue())
    modulation_buttons_[current_modulator_]->repaint();
  last_value_ = moved_slider->getValue();
}

void ModulationManager::timerCallback() {
  for (auto slider : slider_lookup_) {
    SynthSlider* model = slider_model_lookup_[slider.first];
    slider.second->setVisible(model->isVisible());
  }

  updateModulationValues();

  for (auto meter : meter_lookup_) {
    meter.second->setVisible(slider_model_lookup_[meter.first]->isVisible());
    meter.second->updateDrawing();
  }
}

void ModulationManager::updateModulationValues() {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent == nullptr)
    return;

  int num_voices = parent->getNumActiveVoices();
  if (num_voices < 0)
    return;

  parent->lockSynth();
  for (auto meter : meter_lookup_)
    meter.second->updateValue(num_voices);
  parent->unlockSynth();
}

void ModulationManager::setModulationAmount(std::string source, std::string destination,
                                            mopo::mopo_float amount) {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent)
    parent->changeModulationAmount(source, destination, amount);
}

void ModulationManager::forgetModulator() {
  polyphonic_destinations_->setVisible(false);
  monophonic_destinations_->setVisible(false);
  current_modulator_ = "";
}

void ModulationManager::setSliderValues() {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent == nullptr)
    return;
  
  std::vector<mopo::ModulationConnection*> connections =
  parent->getSourceConnections(current_modulator_);
  for (auto slider : slider_lookup_) {
    std::string destination_name = slider.second->getName().toStdString();
    float value = 0.0f;

    for (mopo::ModulationConnection* connection : connections) {
      if (connection->destination == destination_name) {
        value = connection->amount.value();
        break;
      }
    }
    slider.second->setValue(value);
    slider.second->repaint();
  }
}

void ModulationManager::changeModulator(std::string new_modulator) {
  current_modulator_ = new_modulator;
  setSliderValues();

  polyphonic_destinations_->setVisible(true);
  polyphonic_destinations_->repaint();
  bool source_is_poly = modulation_sources_[current_modulator_]->owner->isPolyphonic();
  monophonic_destinations_->setVisible(!source_is_poly);
  monophonic_destinations_->repaint();
}

void ModulationManager::showMeter(std::string name, bool show) {
  meter_lookup_[name]->setVisible(show);
}