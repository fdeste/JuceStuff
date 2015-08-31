/*
 *  SequencerComponent.h
 *  syzygrd_sequencer1
 *
 *  Created by Matt Sonic on 6/15/10.
 *  Copyright 2010 SonicTransfer. All rights reserved.
 *
 */

#ifndef SequencerComponent_H
#define SequencerComponent_H

#include "JuceHeader.h"

class PluginAudioProcessor;
class Cell;
class MyTableListBox;
class Sequencer;

class SequencerComponent : 
public Component,
public TableListBoxModel,
public Timer
{
public:
	SequencerComponent(PluginAudioProcessor* pluginAudioProcessor_);
	~SequencerComponent();
	
	void addTableColumn (const String& title, int ID);
	
	// Component methods
	virtual void paint (Graphics& g);
	virtual void resized();
	virtual bool keyPressed (const KeyPress& key);
	
	// TableListBoxModel methods
    virtual int getNumRows();
    virtual void paintRowBackground (Graphics& g,
                                     int rowNumber,
                                     int width, int height,
                                     bool rowIsSelected);
    virtual void paintCell (Graphics& g,
							int rowNumber,
							int columnId,
							int width, int height,
							bool rowIsSelected);	
    virtual void cellClicked (int rowNumber, int columnId, const MouseEvent& e);
	virtual void selectedRowsChanged (int lastRowSelected);
	
	// Timer methods
	virtual void timerCallback();

private:
	PluginAudioProcessor* pluginAudioProcessor;
	Sequencer* sequencer;
	
	MyTableListBox* tableListBox;
	Cell* selectedCell; // owned by sequencer
	int selectedRowIndex;
	
	int lastPlayheadRow;
};

#endif
