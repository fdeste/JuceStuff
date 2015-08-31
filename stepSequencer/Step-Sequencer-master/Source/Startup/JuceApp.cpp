/*
 *  JuceApp.cpp
 *  syzygrd_sequencer1
 *
 *  Created by Matt Sonic on 3/27/10.
 *  Copyright 2010 SonicTransfer. All rights reserved.
 *
 */

#include "juce_StandaloneFilterWindow.h"

#include "JuceApp.h"

JuceApp::JuceApp() :
standaloneFilterWindow (0)
{
}

JuceApp::~JuceApp()
{
}

void JuceApp::initialise (const String& commandLine)
{
	const String applicationName = "syzygrd_sequencer1";
	const String fileNameSuffix("app");
	const String folderName("SonicTransfer");
	ApplicationProperties::getInstance()->setStorageParameters(applicationName,
															   fileNameSuffix,
															   folderName,
															   0,
															   PropertiesFile::storeAsXML);
	
	standaloneFilterWindow = new StandaloneFilterWindow ("syzygrd_sequencer1", 
														 Colours::lightgrey);
	standaloneFilterWindow->setVisible (true);
}

void JuceApp::shutdown()
{
	if (standaloneFilterWindow != 0) delete standaloneFilterWindow;
}

const String JuceApp::getApplicationName()
{
	return T("syzygrd_sequencer1");
}

const String JuceApp::getApplicationVersion()
{
	return T("1.0");
}

bool JuceApp::moreThanOneInstanceAllowed()
{
	return true;
}

void JuceApp::anotherInstanceStarted (const String& commandLine)
{
}
