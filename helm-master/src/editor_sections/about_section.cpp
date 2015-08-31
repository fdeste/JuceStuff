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

#include "about_section.h"
#include "helm_common.h"
#include "synth_gui_interface.h"
#include "text_look_and_feel.h"

#define LOGO_WIDTH 128
#define INFO_WIDTH 450
#define STANDALONE_INFO_HEIGHT 500
#define PLUGIN_INFO_HEIGHT 160
#define PADDING_X 25
#define PADDING_Y 15

AboutSection::AboutSection(String name) : Component(name) {
  static Font roboto_light(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                             BinaryData::RobotoLight_ttfSize));

  developer_link_ = new HyperlinkButton("Matt Tytel", URL("http://tytel.org"));
  developer_link_->setFont(roboto_light.withPointHeight(16.0f), false, Justification::right);
  developer_link_->setColour(HyperlinkButton::textColourId, Colour(0xffffd740));
  addAndMakeVisible(developer_link_);

  free_software_link_ = new HyperlinkButton(TRANS("Read more about free software."),
                                            URL("http://www.gnu.org/philosophy/free-sw.html"));
  free_software_link_->setFont(roboto_light.withPointHeight(12.0f), false, Justification::right);
  free_software_link_->setColour(HyperlinkButton::textColourId, Colour(0xffffd740));
  addAndMakeVisible(free_software_link_);
}

void AboutSection::paint(Graphics& g) {
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  static Font roboto_light(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                             BinaryData::RobotoLight_ttfSize));
  static const DropShadow shadow(Colour(0xff000000), 5, Point<int>(0, 0));

  g.setColour(Colour(0xbb212121));
  g.fillAll();

  Rectangle<int> info_rect = getInfoRect();
  shadow.drawForRectangle(g, info_rect);
  g.setColour(Colour(0xff303030));
  g.fillRect(info_rect);

  g.saveState();
  g.setOrigin(info_rect.getX() + PADDING_X, info_rect.getY() + PADDING_Y);
  Image helm_small = ImageCache::getFromMemory(BinaryData::helm_icon_128_1x_png,
                                               BinaryData::helm_icon_128_1x_pngSize);
  shadow.drawForImage(g, helm_small);

  const Desktop::Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  if (display.scale > 1.5) {
    Image helm = ImageCache::getFromMemory(BinaryData::helm_icon_128_2x_png,
                                           BinaryData::helm_icon_128_2x_pngSize);
    g.drawImage(helm, 0, 0, 128, 128, 0, 0, 256, 256);
  }
  else
    g.drawImage(helm_small, 0, 0, 128, 128, 0, 0, 128, 128);

  g.setFont(roboto_reg.withPointHeight(32.0));
  g.setColour(Colour(0xff2196f3));
  g.drawText(TRANS("HELM"),
             0.0f, 0.0f,
             info_rect.getWidth() - 2 * PADDING_X, 32.0f, Justification::centredTop);

  g.setFont(roboto_light.withPointHeight(12.0));
  g.setColour(Colour(0xff666666));
  g.drawText(TRANS("v") + " " + ProjectInfo::versionString,
             0.0f, 36.0f,
             info_rect.getWidth() - 2 * PADDING_X, 32.0f, Justification::centredTop);

  g.setFont(roboto_light.withPointHeight(12.0));
  g.drawText(TRANS("Developed by"),
             0.0f, 4.0f,
             info_rect.getWidth() - 2 * PADDING_X, 20.0f, Justification::right);

  g.setColour(Colour(0xffaaaaaa));
  g.drawText(TRANS("Helm is free software and"),
             0.0f, 62.0,
             info_rect.getWidth() - 2 * PADDING_X, 20.0f, Justification::topRight);

  g.drawText(TRANS("comes with no warranty."),
             0.0f, 76.0f,
             info_rect.getWidth() - 2 * PADDING_X, 20.0f, Justification::topRight);

  g.restoreState();
}

void AboutSection::resized() {
  static const float software_link_width = 200.0f;
  static const float developer_link_width = 120.0f;

  Rectangle<int> info_rect = getInfoRect();
  developer_link_->setBounds(info_rect.getRight() - PADDING_X - developer_link_width,
                             info_rect.getY() + PADDING_Y + 24.0f, developer_link_width, 20.0f);
  free_software_link_->setBounds(info_rect.getRight() - PADDING_X - software_link_width,
                                 info_rect.getY() + PADDING_Y + 105.0f, software_link_width, 20.0f);
  if (device_selector_) {
    int y = info_rect.getY() + LOGO_WIDTH + 2 * PADDING_Y;
    device_selector_->setBounds(info_rect.getX(), y,
                                info_rect.getWidth(), info_rect.getBottom() - y);
  }
}

void AboutSection::mouseUp(const MouseEvent &e) {
  if (!getInfoRect().contains(e.getPosition()))
    setVisible(false);
}

void AboutSection::setVisible(bool should_be_visible) {
  if (should_be_visible && device_selector_.get() == nullptr) {
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    AudioDeviceManager* device_manager = parent->getAudioDeviceManager();
    if (device_manager) {
      device_selector_ = new AudioDeviceSelectorComponent(*device_manager, 0, 0,
                                                          mopo::NUM_CHANNELS, mopo::NUM_CHANNELS,
                                                          true, false, true, false);
      device_selector_->setLookAndFeel(TextLookAndFeel::instance());
      addAndMakeVisible(device_selector_);
      Rectangle<int> info_rect = getInfoRect();
      int y = info_rect.getY() + LOGO_WIDTH + 2 * PADDING_Y;
      device_selector_->setBounds(info_rect.getX(), y,
                                  info_rect.getWidth(), info_rect.getBottom() - y);
      resized();
    }
  }

  Component::setVisible(should_be_visible);
}

Rectangle<int> AboutSection::getInfoRect() {
  int info_height = device_selector_ ? STANDALONE_INFO_HEIGHT : PLUGIN_INFO_HEIGHT;
  int x = (getWidth() - INFO_WIDTH) / 2;
  int y = (getHeight() - info_height) / 2;
  return Rectangle<int>(x, y, INFO_WIDTH, info_height);
}
