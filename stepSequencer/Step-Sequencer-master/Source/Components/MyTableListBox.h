/*
 *  MyTableListBox.h
 *  syzygrd_sequencer1
 *
 *  Created by Matt Sonic on 6/15/10.
 *  Copyright 2010 SonicTransfer. All rights reserved.
 *
 */

#ifndef MyTableListBox_H
#define MyTableListBox_H

#include "JuceHeader.h"

class MyTableListBox : public TableListBox
{
public:
    MyTableListBox (const String& componentName,
					TableListBoxModel* model);
	~MyTableListBox();
	
	// ListBox methods
	bool keyPressed (const KeyPress& key);
};

#endif
