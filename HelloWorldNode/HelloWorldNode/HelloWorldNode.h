#pragma once

#include <maya/MPxLocatorNode.h>

class HelloWorldNode : public MPxLocatorNode
{
public:
	HelloWorldNode();
	virtual ~HelloWorldNode() override;

	// static methods
	static void* Creator();
	static MStatus Initialize();

	static MTypeId GetTypeId();
	static MString GetTypeName();

	static MString GetDrawDbClassification();
	static MString GetDrawRegistrationId();
};