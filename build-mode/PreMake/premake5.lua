include "Dependencies.lua"

workspace "NexUs"
	startproject "NexUsExample"
	
	configurations
	{
		"Debug",
		"Release",
		"RelWithDebInfo"
	}
	
	platforms { "Static32", "Shared32", "Static64", "Shared64" }
	
	flags
	{
		"MultiProcessorCompile"
	}

	filter "platforms:Static32"
		kind "StaticLib"
		architecture "x32"
	
	filter "platforms:Static64"
		kind "StaticLib"
		architecture "x64"
	
	filter "platforms:Shared32"
		kind "SharedLib"
		architecture "x32"
	
	filter "platforms:Shared64"
		kind "SharedLib"
		architecture "x64"

	-- Global Qt configuration
	filter "system:windows"
		systemversion "latest"
		
	filter "system:linux"
		systemversion "latest"
		
	filter "system:macosx"
		systemversion "latest"

	filter {}

-- <Dependencies.lua> Initialize Qt paths and configuration
InitializeQt()

--group "Dependencies"
--	include "NXPacketIO"
--group ""

group "Core"
	include "NexUs"
group ""

--group "Applications"
--	include "NexUsExample"
--group ""