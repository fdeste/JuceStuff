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

#include "browser_look_and_feel.h"


void BrowserLookAndFeel::drawButtonBackground(Graphics& g, Button& button,
                                              const Colour &backgroundColour,
                                              bool isMouseOverButton, bool isButtonDown) {
  g.setColour(button.findColour(TextButton::buttonColourId));
  g.fillAll();

  if (isButtonDown)
    g.fillAll(Colour(0x11000000));
  else if (isMouseOverButton)
    g.fillAll(Colour(0x11ffffff));
}