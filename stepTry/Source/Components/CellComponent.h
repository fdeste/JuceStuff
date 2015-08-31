/*
 *  CellComponent.h
 *  syzygrd_sequencer1
 *
 *  Created by Matt Sonic on 6/17/10.
 *  Copyright 2010 SonicTransfer. All rights reserved.
 *
 */

#ifndef CellComponent_H
#define CellComponent_H

#include "JuceHeader.h"

class Cell;

class CellComponent : public Component
{
public:
	CellComponent (Cell* cell_);
	~CellComponent();
	
	// Component methods
	virtual void paint (Graphics& g);
	virtual void resized();
	
private:
	Cell* cell;	
};

#endif
