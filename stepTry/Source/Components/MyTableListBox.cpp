/*
 *  MyTableListBox.cpp
 *  syzygrd_sequencer1
 *
 *  Created by Matt Sonic on 6/15/10.
 *  Copyright 2010 SonicTransfer. All rights reserved.
 *
 */

#include "MyTableListBox.h"

MyTableListBox::MyTableListBox(const String& componentName,
							   TableListBoxModel* model) :
TableListBox (componentName, model)
{
}

MyTableListBox::~MyTableListBox()
{
}

// ListBox methods
bool MyTableListBox::keyPressed (const KeyPress& key)
{
	if (key.isKeyCode (KeyPress::upKey)
		|| key.isKeyCode (KeyPress::downKey)
		|| key.isKeyCode (KeyPress::pageUpKey)
		|| key.isKeyCode (KeyPress::pageDownKey)
		|| key.isKeyCode (KeyPress::homeKey)
		|| key.isKeyCode (KeyPress::endKey)
		|| key.isKeyCode (KeyPress::deleteKey)
		|| key.isKeyCode (KeyPress::tabKey)
		|| key.isKeyCode (KeyPress::spaceKey))
	{
		return false;
	}
	
	return TableListBox::keyPressed (key);
}

