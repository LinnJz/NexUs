project "NexUsExample"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"
	staticruntime "off"

	targetdir ("%{wks.location}/Bin/%{wks.name}_%{cfg.buildcfg}_%{cfg.platform}")
	objdir ("%{wks.location}/Bin/Intermediate/%{prj.name}_%{cfg.buildcfg}_%{cfg.platform}")

	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"Source/ExamplePage/**.h",
		"Source/ExamplePage/**.cpp",
		"Source/ModelView/**.h",
		"Source/ModelView/**.cpp",
		"main.cpp",
		"NexUsExample.qrc"
	}

	-- Qt MOC files (headers that contain Q_OBJECT)
	qtmoc
	{
		-- ExamplePage headers
		"Source/ExamplePage/T_ListView.h",
		"Source/ExamplePage/T_Graphics.h",
		"Source/ExamplePage/T_RecvScreen.h",
		"Source/ExamplePage/T_BaseComponents.h",
		"Source/ExamplePage/T_TableView.h",
		"Source/ExamplePage/T_About.h",
		"Source/ExamplePage/T_Card.h",
		"Source/ExamplePage/T_NXScreen.h",
		"Source/ExamplePage/T_Icon.h",
		"Source/ExamplePage/T_BasePage.h",
		"Source/ExamplePage/T_Navigation.h",
		"Source/ExamplePage/T_UpdateWidget.h",
		"Source/ExamplePage/T_TreeView.h",
		"Source/ExamplePage/T_LogWidget.h",
		"Source/ExamplePage/T_NXPacketIO.h",
		"Source/ExamplePage/T_Setting.h",
		"Source/ExamplePage/T_Popup.h",
		"Source/ExamplePage/T_Home.h",
		
		-- ModelView headers
		"Source/ModelView/T_TreeItem.h",
		"Source/ModelView/T_LogModel.h",
		"Source/ModelView/T_IconModel.h",
		"Source/ModelView/T_ListViewModel.h",
		"Source/ModelView/T_TableViewModel.h",
		"Source/ModelView/T_TreeViewModel.h",
		"Source/ModelView/T_IconDelegate.h",
		
		-- Main window
		"Source/mainwindow.h"
	}

	-- Qt RCC files (resource files)
	qtrcc
	{
		"NexUsExample.qrc"
	}

	includedirs
	{
		"Source",
		"%{IncludeDir.NexUs}",
		"%{IncludeDir.NexUsInclude}"
	}

	defines
	{
		"NXPACKETIO_LIBRARY_ALREADY_BUILT"
	}

	links
	{
		"NexUs"
	}

	-- <Dependencies.lua> Configure Qt
	ConfigureQt()

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"Q_OS_WIN"
		}
		links
		{
			"NXPacketIO"
		}

	filter "system:linux"
		defines
		{
			"Q_OS_LINUX"
		}

	filter "system:macosx"
		defines
		{
			"Q_OS_MACOS"
		}

	filter "configurations:Debug"
		defines "NX_DEBUG"
		runtime "Debug"
		symbols "on"
		links
		{
			"%{Library.QtCore_Debug}",
			"%{Library.QtGui_Debug}",
			"%{Library.QtWidgets_Debug}"
		}

	filter "configurations:Release"
		defines "NX_RELEASE"
		runtime "Release"
		optimize "on"
		links
		{
			"%{Library.QtCore_Release}",
			"%{Library.QtGui_Release}",
			"%{Library.QtWidgets_Release}"
		}

	filter "configurations:RelWithDebInfo"
		defines "NX_RELEASE"
		runtime "Release"
		optimize "on"
		symbols "on"
		links
		{
			"%{Library.QtCore_Release}",
			"%{Library.QtGui_Release}",
			"%{Library.QtWidgets_Release}"
		}

	filter {}

	-- <Dependencies.lua> Configure Qt-specific file handling
	ConfigureQtFiles("NexUsExample")