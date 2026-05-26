project "NXPacketIO"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"GenIO/**.h",
		"GenIO/**.cpp",
		"PacketIO/**.h",
		"PacketIO/**.cpp",
		"Util/**.h",
		"Util/**.cpp",
		"XIO/**.h",
		"XIO/**.cpp",
		"*.h"
	}

	includedirs
	{
		".",
		"%{IncludeDir.NXPacketIO}",
		"%{IncludeDir.NXPacketIOGenIO}",
		"%{IncludeDir.NXPacketIOPacketIO}",
		"%{IncludeDir.NXPacketIOUtil}",
		"%{IncludeDir.NXPacketIOXIO}"
	}

	defines
	{
		"NXPACKETIO_LIBRARY",
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"WIN32",
			"_WINDOWS"
		}
		links
		{
			"%{Library.WinSock}"
		}

	filter "system:linux"
		defines
		{
			"LINUX"
		}
		links
		{
			"pthread"
		}

	filter "system:macosx"
		defines
		{
			"MACOS"
		}
		links
		{
			"pthread"
		}

	filter "configurations:Debug"
		defines "_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
		optimize "on"

	filter "configurations:RelWithDebInfo"
		defines "NDEBUG"
		runtime "Release"
		optimize "on"
		symbols "on"

	filter {}