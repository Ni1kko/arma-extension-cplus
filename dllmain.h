/*
	Name: dllmain.h
    Author: Nikko
    Description: main header for the extension

	Info about arma extensions:  					   https://community.bistudio.com/wiki/callExtension
	Info on how to install compiler for linux builds:  https://www.instructables.com/How-to-Install-MinGW-GCCG-Compiler-in-Windows-XP78/
	Info about above comipler predfined macros: 	   https://blog.kowalczyk.info/article/j/guide-to-predefined-macros-in-c-compilers-gcc-clang-msvc-etc..html

	<USAGE>
	Right click in folder next to dllmain.h and select `new gitbash here` In gitbash run these 2 commands below
	
	cd linux
	sh compile_linux.sh
*/
#define _CRT_SECURE_NO_WARNINGS

#pragma region Includes
	#include <stdio.h>
	#include <iostream>
	#include <array>

	#pragma region Linux Only includes
		#if defined __GNUC__  
			#include <cstring> //Added for strncpy()
			#include <algorithm> //Added for std::generate_n()
			using namespace std;
		#endif
	#pragma endregion

	#pragma region Windows Only includes
		#if defined _MSC_VER
			#define WIN32_LEAN_AND_MEAN
			#include <windows.h>
			#include <shellapi.h>
		#endif
	#pragma endregion

#pragma endregion

#pragma region Example
	class Example {
		public:
			static bool moudleLoaded;
			static bool startLoaded;
			static int(*ExtensionCallback)(char const* name, char const* function, char const* data);
			static int CallExtension(char* output, int outputSize, const char* function, const char** args, int argsCnt) noexcept;
			static void CallExtension(char* output, int outputSize, const char* function) noexcept;  
			static void CallExtension(int(*callbackPointer)(char const* name, char const* function, char const* data)) noexcept { Example::ExtensionCallback = callbackPointer; }
			static void CallExtension(char* output, int outputSize) noexcept { Example::CallExtension(output, outputSize, "armaMoudle"); };
		private:
			static std::string AuthKey;
			static void Return(char* output, int outputSize, std::string returnMessage) noexcept { strncpy(output, returnMessage.c_str(), outputSize); }
			static int Return(char* output, int outputSize, std::string returnMessage, int returncode) noexcept { Example::Return(output, outputSize, returnMessage); return returncode; }
	};
	int(*Example::ExtensionCallback)(char const* name, char const* function, char const* data) = nullptr;//Initatlize Callback Pointer
	bool Example::moudleLoaded = false;
	bool Example::startLoaded = false;
#pragma endregion

#pragma region Externals
	extern "C"
	{
		#pragma region Extension entry points
			#pragma region Linux entry points
				#if defined __GNUC__
					//--- Engine called on extension load
					void RVExtensionRegisterCallback(int(*callbackPointer)(char const* name, char const* function, char const* data)) { Example::CallExtension(callbackPointer); }
					//--- Engine called on extension load
					void RVExtensionVersion(char* output, int outputSize) { Example::CallExtension(output, outputSize); }
					//--- STRING callExtension STRING
					void RVExtension(char* output, int outputSize, const char* function) { Example::CallExtension(output, outputSize, function); }
					//--- STRING callExtension ARRAY
					int RVExtensionArgs(char* output, int outputSize, const char* function, const char** args, int argsCnt) { return Example::CallExtension(output, outputSize, function, args, argsCnt); }
				#endif
			#pragma endregion 
	
			#pragma region Windows entry points
				#if defined _MSC_VER
					//--- Engine called on extension load
					_declspec(dllexport) void __stdcall RVExtensionRegisterCallback(int(*callbackPointer)(char const* name, char const* function, char const* data)) { Example::CallExtension(callbackPointer); }
					//--- Engine called on extension load
					_declspec(dllexport) void __stdcall RVExtensionVersion(char* output, int outputSize) { Example::CallExtension(output, outputSize); }
					//--- STRING callExtension STRING
					_declspec(dllexport) void __stdcall RVExtension(char* output, int outputSize, const char* function) { Example::CallExtension(output, outputSize, function); }
					//--- STRING callExtension ARRAY
					_declspec(dllexport) int __stdcall RVExtensionArgs(char* output, int outputSize, const char* function, const char** args, int argsCnt) { return Example::CallExtension(output, outputSize, function, args, argsCnt); }
				#endif
			#pragma endregion
		#pragma endregion 
	};
#pragma endregion