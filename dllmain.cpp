/*
	Name: dllmain.cpp
    Author: Nikko
    Description: main code for the extension
*/

#include "dllmain.h"

#define ExtensionVersion "0.0.1"

// ("Example" callExtension "example")
void Example::CallExtension(char* output, int outputSize, const char* function) noexcept
{
	//return string <int> only in here so we can use ` parseNumber("Example" callExtension "version") ` within arma
	if (strcmp(function, "armaMoudle") == 0)
	{
		if (Example::moudleLoaded) return;//run once
		Example::CallExtension(output, outputSize, "version");
		Example::moudleLoaded = true;
		return;
	}
	else if (strcmp(function, "version") == 0)
	{  
		Example::Return(output, outputSize, ExtensionVersion);
		return;
	}
	else if (strcmp(function, "test") == 0)
	{
		Example::ExtensionCallback("Hint", function, "[[\"Hello %1 I Excuted This Hint From The Extension FNC test\",name player],\"76561198276956558\"]");
		Example::ExtensionCallback("Rpt", function, "[\"Hello I Excuted This Hint From The Extension FNC test @ %1\",serverTime]");
		Example::Return(output, outputSize, "200");
		return;
	} 

	Example::Return(output, outputSize, "-1");
};

// ("Example" callExtension ["example",[]]
int Example::CallExtension(char* output, int outputSize, const char* function, const char** args, int argsCnt) noexcept
{
	//return don't matter in here as return not really used as we use extensionCallback ` ("Example" callExtension ["test",[]]) ` returns <array> [returnstring, returncode, armareturncode]
	bool hasArgs = (argsCnt > 0);
	
	if (strcmp(function, "start") == 0)
	{ 
		if (Example::startLoaded) {
			Example::ExtensionCallback("Rpt", function, "[\"<Example Extension> Start Function Already Excuted\"]"); 
			//std::exit(EXIT_SUCCESS);// Kill Server already ran -- Evil
			return Example::Return(output, outputSize, "BAD", 403);
		}
		Example::startLoaded = true;

		std::string logMsg = "[\"<Example Extension> Excuted @ (%1) \",systemTimeUTC]";

		Example::ExtensionCallback("Rpt", function, logMsg.c_str());
		return Example::Return(output, outputSize, "OK", 201);
	}
	else if(strcmp(function, "test") == 0)
	{
		std::string argsArray = "[";

		if (hasArgs) {
			int i = 0;
			argsArray += args[i++];
			while (i < argsCnt)
			{
				argsArray += ",";
				argsArray += args[i++];
			}
		}

		argsArray += "]";

		Example::ExtensionCallback("CallFunction", "ExampleTest", argsArray.c_str());
		Example::ExtensionCallback("SpawnFunction", "ExampleTest2", argsArray.c_str());
		Example::ExtensionCallback("RunCallCode", function, "[true,{RunCallCode = _this;}]");
		Example::ExtensionCallback("RunSpawnCode", function, "[true,{RunSpawnCode = _this;}]");
		Example::ExtensionCallback("RunCallCode", function, "[true,{RunCallCode2 = _this;},\"76561198276956558\"]");
		Example::ExtensionCallback("RunSpawnCode", function, "[true,{RunSpawnCode2 = _this;},\"76561198276956558\"]");
		Example::ExtensionCallback("Rpt", function, "[\"Hello I Excuted This Hint From The Extension @ %1\",serverTime]");
		Example::ExtensionCallback("Hint", function, "[[\"Hello %1 I Excuted This Hint From The Extension\",name player],\"76561198276956558\"]");
		return Example::Return(output, outputSize, "OK - test ran", 200);
	}

	Example::ExtensionCallback("Rpt", function, "[\"<Example_Extension> - Function Not Defined And/Or Wrong Arguments Passed\"]");
	return Example::Return(output, outputSize, "BAD - Function Not Defined And/Or Wrong Arguments Passed", -2);
};