-- NexUs Dependencies

-- Qt version configuration
newoption {
	trigger = "qt-version",
	value = "VERSION",
	description = "Choose Qt version (5 or 6)",
	allowed = {
		{ "5", "Qt 5" },
		{ "6", "Qt 6" }
	},
	default = "6"
}

-- Qt minor version configuration
newoption {
	trigger = "qt-minor-version",
	value = "VERSION",
	description = "Set Qt minor version (default: 6)",
	default = "6"
}

-- Qt patch version configuration
newoption {
	trigger = "qt-patch-version",
	value = "VERSION",
	description = "Set Qt patch version (default: 2)",
	default = "2"
}

-- Qt Version Detection and Configuration
QT_VERSION_MAJOR = _OPTIONS["qt-version"] or "6"
QT_VERSION_MINOR = _OPTIONS["qt-minor-version"] or "6"
QT_VERSION_PATCH = _OPTIONS["qt-patch-version"] or "2"

-- Validate major version
QT_VERSION_MAJOR_NUM = tonumber(QT_VERSION_MAJOR)
if QT_VERSION_MAJOR_NUM ~= 5 and QT_VERSION_MAJOR_NUM ~= 6 then
    error("Invalid Qt major version: " .. QT_VERSION_MAJOR .. ". Must be 5 or 6.")
end

-- Validate minor and patch versions (should be numbers)
if not tonumber(QT_VERSION_MINOR) then
    error("Invalid Qt minor version: " .. QT_VERSION_MINOR .. ". Must be a number.")
end

if not tonumber(QT_VERSION_PATCH) then
    error("Invalid Qt patch version: " .. QT_VERSION_PATCH .. ". Must be a number.")
end

-- Build full version string
QT_VERSION_FULL = QT_VERSION_MAJOR .. "." .. QT_VERSION_MINOR .. "." .. QT_VERSION_PATCH
print("Using Qt version: " .. QT_VERSION_FULL)

-- Qt Installation Path Detection
function DetectQtPath()
    local qt_path = os.getenv("QT_DIR")
    if qt_path and os.isdir(qt_path) then
        return qt_path
    end
	
	local found_path = nil
    local searched_paths = {}
    
    -- Default paths for different platforms and Qt versions
    if os.host() == "windows" then
        local default_paths = {
            "E:/Qt/" .. QT_VERSION_FULL .. "/msvc2019_64",
            "C:/Qt/" .. QT_VERSION_FULL .. "/msvc2019_64",
            "D:/Qt/" .. QT_VERSION_FULL .. "/msvc2019_64"
        }
        
        for _, path in ipairs(default_paths) do
            table.insert(searched_paths, path)
            if os.isdir(path) then
                found_path = path
                break
            end
        end
    elseif os.host() == "linux" then
        local default_paths = {
            "/opt/Qt/" .. QT_VERSION_FULL .. "/gcc_64",
            "/usr/local/Qt/" .. QT_VERSION_FULL .. "/gcc_64",
            os.getenv("HOME") .. "/Qt/" .. QT_VERSION_FULL .. "/gcc_64"
        }
        
        for _, path in ipairs(default_paths) do
            table.insert(searched_paths, path)
            if os.isdir(path) then
                found_path = path
                break
            end
        end
    elseif os.host() == "macosx" then
        local default_paths = {
            "/opt/Qt/" .. QT_VERSION_FULL .. "/macos",
            "/usr/local/Qt/" .. QT_VERSION_FULL .. "/macos",
			"/Applications/Qt/" .. QT_VERSION_FULL .. "/macos",
            os.getenv("HOME") .. "/Qt/" .. QT_VERSION_FULL .. "/macos"
        }
        
        for _, path in ipairs(default_paths) do
            table.insert(searched_paths, path)
            if os.isdir(path) then
                found_path = path
                break
            end
        end
    end
    
    -- If a path is found, return it.
    if found_path then
        print("Found Qt at: " .. found_path)
        return found_path
    end
    
    -- If no paths are found, throw an error and display the searched paths.
    local error_msg = "Error: Could not find Qt " .. QT_VERSION_FULL .. " installation.\n\n"
    error_msg = error_msg .. "Searched paths:\n"
    error_msg = error_msg .. "- QT_DIR environment variable: " .. (os.getenv("QT_DIR") or "not set") .. "\n"
    
    for i, path in ipairs(searched_paths) do
        error_msg = error_msg .. "- " .. path .. "\n"
    end
    
    error_msg = error_msg .. "\nPlease do one of the following:\n"
    error_msg = error_msg .. "1. Set QT_DIR environment variable to your Qt installation\n"
    error_msg = error_msg .. "2. Install Qt " .. QT_VERSION_FULL .. " in one of the expected locations\n"
    error_msg = error_msg .. "3. Modify the premake script to include your Qt path"
    
    error(error_msg)
end

QT_PATH = DetectQtPath()
print("Using Qt Path: " .. QT_PATH)

-- Include Directories
IncludeDir = {}
-- Library Directories
LibraryDir = {}
-- Libraries
Library = {}

-- NexUs 包含目录
IncludeDir["NexUs"] = "%{wks.location}/NexUs/Source"
IncludeDir["NexUsInclude"] = "%{wks.location}/NexUs/Source/include"

-- NXPacketIO 包含目录
IncludeDir["NXPacketIO"] = "%{wks.location}/NXPacketIO"
IncludeDir["NXPacketIOGenIO"] = "%{wks.location}/NXPacketIO/GenIO"
IncludeDir["NXPacketIOPacketIO"] = "%{wks.location}/NXPacketIO/PacketIO"
IncludeDir["NXPacketIOUtil"] = "%{wks.location}/NXPacketIO/Util"
IncludeDir["NXPacketIOXIO"] = "%{wks.location}/NXPacketIO/XIO"

-- Windows-specific libraries
Library["D3D11"] = "d3d11.lib"
Library["D3DCompiler"] = "d3dcompiler.lib"
Library["DXGI"] = "dxgi.lib"
Library["DXGuid"] = "dxguid.lib"
Library["OpenGL32"] = "opengl32.lib"

-- Cross-platform system libraries
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"

function InitializeQt()
	-- Qt Include Directories
	IncludeDir["QtCore"] = QT_PATH .. "/include/QtCore"
	IncludeDir["QtGui"] = QT_PATH .. "/include/QtGui"
	IncludeDir["QtWidgets"] = QT_PATH .. "/include/QtWidgets"
	IncludeDir["Qt"] = QT_PATH .. "/include"

	-- Qt Libraries Directories
	LibraryDir["Qt"] = QT_PATH .. "/lib"

	-- Qt Libraries (Debug and Release versions for Qt5 and Qt6)
	if QT_VERSION_MAJOR == 6 then
		Library["QtCore_Debug"] = "%{LibraryDir.Qt}/Qt6Cored.lib"
		Library["QtGui_Debug"] = "%{LibraryDir.Qt}/Qt6Guid.lib"
		Library["QtWidgets_Debug"] = "%{LibraryDir.Qt}/Qt6Widgetsd.lib"
		
		Library["QtCore_Release"] = "%{LibraryDir.Qt}/Qt6Core.lib"
		Library["QtGui_Release"] = "%{LibraryDir.Qt}/Qt6Gui.lib"
		Library["QtWidgets_Release"] = "%{LibraryDir.Qt}/Qt6Widgets.lib"
	else
		Library["QtCore_Debug"] = "%{LibraryDir.Qt}/Qt5Cored.lib"
		Library["QtGui_Debug"] = "%{LibraryDir.Qt}/Qt5Guid.lib"
		Library["QtWidgets_Debug"] = "%{LibraryDir.Qt}/Qt5Widgetsd.lib"
		
		Library["QtCore_Release"] = "%{LibraryDir.Qt}/Qt5Core.lib"
		Library["QtGui_Release"] = "%{LibraryDir.Qt}/Qt5Gui.lib"
		Library["QtWidgets_Release"] = "%{LibraryDir.Qt}/Qt5Widgets.lib"
	end
	print("Qt " .. QT_VERSION_FULL .. " configuration initialized successfully")
end

-- Qt MOC/RCC/UIC Configuration
function ConfigureQtTools()
    filter "system:windows"
        local qt_bin = QT_PATH .. "/bin"
        local moc_exe = qt_bin .. "/moc.exe"
        local rcc_exe = qt_bin .. "/rcc.exe"
        local uic_exe = qt_bin .. "/uic.exe"
        
        -- Set Qt tools paths
        qtmoc(moc_exe)
        qtrcc(rcc_exe)
        qtuic(uic_exe)
        
        -- Set Qt MOC/RCC/UIC output directories
        qtmocdir("GeneratedFiles/$(Configuration)")
        qtrccdir("GeneratedFiles")
        qtuicdir("GeneratedFiles")
        
    filter "system:linux or system:macosx"
        local qt_bin = QT_PATH .. "/bin"
        local moc_exe = qt_bin .. "/moc"
        local rcc_exe = qt_bin .. "/rcc"
        local uic_exe = qt_bin .. "/uic"
        
        -- Set Qt tools paths
        qtmoc(moc_exe)
        qtrcc(rcc_exe)
        qtuic(uic_exe)
        
        -- Set Qt MOC/RCC/UIC output directories
        qtmocdir("GeneratedFiles/$(config)")
        qtrccdir("GeneratedFiles")
        qtuicdir("GeneratedFiles")
    
    filter {}
end

-- Platform-specific Qt MOC/RCC/UIC Configurations
function ConfigureQt()
    filter "system:windows"
        includedirs {
            "%{IncludeDir.Qt}",
            "%{IncludeDir.QtCore}",
            "%{IncludeDir.QtGui}",
            "%{IncludeDir.QtWidgets}"
        }
        
        libdirs {
            "%{LibraryDir.Qt}"
        }
        
        -- Qt-specific defines
        defines {
            "QT_WIDGETS_LIB",
            "QT_GUI_LIB",
            "QT_CORE_LIB"
        }
        
        -- Link Qt Natvis files for debugging
        if QT_VERSION_MAJOR == 6 then
            natvis { "%{wks.location}/qt6.natvis" }
        else
            natvis { "%{wks.location}/qt5.natvis" }
        end
    
    filter "system:linux or system:macosx"
        local qt_pkg = "Qt" .. QT_VERSION_MAJOR .. "Core Qt" .. QT_VERSION_MAJOR .. "Gui Qt" .. QT_VERSION_MAJOR .. "Widgets"
        buildoptions { "`pkg-config --cflags " .. qt_pkg .. "`" }
        linkoptions { "`pkg-config --libs " .. qt_pkg .. "`" }
        
        defines {
            "QT_WIDGETS_LIB",
            "QT_GUI_LIB",
            "QT_CORE_LIB"
        }
    
    filter {}
end

-- Qt-specific file handling
function ConfigureQtFiles(project_name)
    -- Configure .qrc files
    filter "files:**.qrc"
        buildmessage "Compiling Qt Resource %{file.relpath}"
        buildcommands {
            '"%{QT_PATH}/bin/rcc" -name %{file.basename} -o "%{cfg.objdir}/%{file.basename}.cpp" "%{file.relpath}"'
        }
        buildoutputs { "%{cfg.objdir}/%{file.basename}.cpp" }
    
    -- Configure .ui files
    filter "files:**.ui"
        buildmessage "Compiling Qt UI %{file.relpath}"
        buildcommands {
            '"%{QT_PATH}/bin/uic" -o "%{cfg.objdir}/ui_%{file.basename}.h" "%{file.relpath}"'
        }
        buildoutputs { "%{cfg.objdir}/ui_%{file.basename}.h" }
    
    filter {}
end

function ConfigureDirectX()
    filter "system:windows"
        links {
            "%{Library.D3D11}",
            "%{Library.D3DCompiler}",
            "%{Library.DXGI}",
            "%{Library.DXGuid}"
        }
        
        defines {
            "NEXUS_DIRECTX_ENABLED"
        }
    
    filter "system:not windows"
        defines {
            "NEXUS_DIRECTX_DISABLED"
        }
    
    filter {}
end