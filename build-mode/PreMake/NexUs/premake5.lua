project "NexUs"
	kind "SharedLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "off"

	targetdir ("%{wks.location}/Bin/%{wks.name}_%{cfg.buildcfg}_%{cfg.architecture}")
	objdir ("%{wks.location}/Bin/Intermediate/%{prj.name}_%{cfg.buildcfg}_%{cfg.architecture}")

	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"Source/DeveloperComponents/**.h",
		"Source/DeveloperComponents/**.cpp",
		"Source/include/**.h",
		"Source/include/**.cpp",
		"Source/include/magic_enum/**.hpp",
		"Source/include/aesni/**.h",
		"Source/private/**.h",
		"Source/private/**.cpp",
		"NexUs.qrc"
	}
	
	
	-- Qt MOC files (headers that contain Q_OBJECT)
	qtmoc =
	{
		-- Include headers
		"Source/include/NXScrollPage.h",
		"Source/include/NXSlider.h",
		"Source/include/NXMenuBar.h",
		"Source/include/NXCalendar.h",
		"Source/include/NXToggleButton.h",
		"Source/include/NXComboBox.h",
		"Source/include/NXEventBus.h",
		"Source/include/NXContentDialog.h",
		"Source/include/NXImageCard.h",
		"Source/include/NXProgressBar.h",
		"Source/include/NXToolBar.h",
		"Source/include/NXToolButton.h",
		"Source/include/NXPivot.h",
		"Source/include/NXLCDNumber.h",
		"Source/include/NXSpinBox.h",
		"Source/include/NXScrollBar.h",
		"Source/include/NXMultiSelectComboBox.h",
		"Source/include/NXBreadcrumbBar.h",
		"Source/include/NXInteractiveCard.h",
		"Source/include/NXStatusBar.h",
		"Source/include/NXMenu.h",
		"Source/include/NXScrollPageArea.h",
		"Source/include/NXRadioButton.h",
		"Source/include/NXAcrylicUrlCard.h",
		"Source/include/NXTabBar.h",
		"Source/include/NXCentralStackedWidget.h",
		"Source/include/NXGraphicsScene.h",
		"Source/include/NXLineEdit.h",
		"Source/include/NXColorDialog.h",
		"Source/include/NXDrawerArea.h",
		"Source/include/NXNavigationBar.h",
		"Source/include/NXSuggestBox.h",
		"Source/include/NXExponentialBlur.h",
		"Source/include/NXProgressRing.h",
		"Source/include/NXDoubleSpinBox.h",
		"Source/include/NXTabWidget.h",
		"Source/include/NXSharedMemoryManager.h",
		"Source/include/NXTheme.h",
		"Source/include/NXShadowGraphicsEffect.h",
		"Source/include/NXDockWidget.h",
		"Source/include/NXToolTip.h",
		"Source/include/NXMessageBar.h",
		"Source/include/NXMessageButton.h",
		"Source/include/NXIconButton.h",
		"Source/include/NXPushButton.h",
		"Source/include/NXWindow.h",
		"Source/include/NXShadowWidget.h",
		"Source/include/NXCheckBox.h",
		"Source/include/NXToggleSwitch.h",
		"Source/include/NXCalendarPicker.h",
		"Source/include/NXPlainTextEdit.h",
		"Source/include/NXPopularCard.h",
		"Source/include/NXWidget.h",
		"Source/include/NXDxgiManager.h",
		"Source/include/NXDialog.h",
		"Source/include/NXLog.h",
		"Source/include/NXText.h",
		"Source/include/NXNavigationRouter.h",
		"Source/include/NXPromotionView.h",
		"Source/include/NXListView.h",
		"Source/include/NXKeyBinder.h",
		"Source/include/NXApplication.h",
		"Source/include/NXPromotionCard.h",
		"Source/include/NXGraphicsView.h",
		"Source/include/NXAESEncryption.h",
		"Source/include/NXTableView.h",
		"Source/include/NXGraphicsItem.h",
		"Source/include/NXAppBar.h",
		"Source/include/NXScrollArea.h",
		"Source/include/NXTreeView.h",
		"Source/include/NXRoller.h",
		"Source/include/NXReminderCard.h",
		"Source/include/NXFlowLayout.h",
		
		-- Private headers
		"Source/private/NXTabBarPrivate.h",
		"Source/private/NXScrollAreaPrivate.h",
		"Source/private/NXCalendarPrivate.h",
		"Source/private/NXDoubleSpinBoxPrivate.h",
		"Source/private/NXSliderPrivate.h",
		"Source/private/NXDxgiManagerPrivate.h",
		"Source/private/NXInteractiveCardPrivate.h",
		"Source/private/NXContentDialogPrivate.h",
		"Source/private/NXScrollPageAreaPrivate.h",
		"Source/private/NXToggleSwitchPrivate.h",
		"Source/private/NXGraphicsViewPrivate.h",
		"Source/private/NXGraphicsItemPrivate.h",
		"Source/private/NXPromotionViewPrivate.h",
		"Source/private/NXToolBarPrivate.h",
		"Source/private/NXScrollPagePrivate.h",
		"Source/private/NXPlainTextEditPrivate.h",
		"Source/private/NXApplicationPrivate.h",
		"Source/private/NXWindowPrivate.h",
		"Source/private/NXDockWidgetPrivate.h",
		"Source/private/NXAppBarPrivate.h",
		"Source/private/NXNavigationBarPrivate.h",
		"Source/private/NXScrollBarPrivate.h",
		"Source/private/NXPushButtonPrivate.h",
		"Source/private/NXTableViewPrivate.h",
		"Source/private/NXRadioButtonPrivate.h",
		"Source/private/NXTabWidgetPrivate.h",
		"Source/private/NXFlowLayoutPrivate.h",
		"Source/private/NXShadowGraphicsEffectPrivate.h",
		"Source/private/NXBreadcrumbBarPrivate.h",
		"Source/private/NXProgressRingPrivate.h",
		"Source/private/NXMessageButtonPrivate.h",
		"Source/private/NXRollerPrivate.h",
		"Source/private/NXLineEditPrivate.h",
		"Source/private/NXSuggestBoxPrivate.h",
		"Source/private/NXColorDialogPrivate.h",
		"Source/private/NXPromotionCardPrivate.h",
		"Source/private/NXCalendarPickerPrivate.h",
		"Source/private/NXThemePrivate.h",
		"Source/private/NXExponentialBlurPrivate.h",
		"Source/private/NXEventBusPrivate.h",
		"Source/private/NXNavigationRouterPrivate.h",
		"Source/private/NXDrawerAreaPrivate.h",
		"Source/private/NXGraphicsScenePrivate.h",
		"Source/private/NXToolTipPrivate.h",
		"Source/private/NXToolButtonPrivate.h",
		"Source/private/NXImageCardPrivate.h",
		"Source/private/NXPopularCardPrivate.h",
		"Source/private/NXReminderCardPrivate.h",
		"Source/private/NXPivotPrivate.h",
		"Source/private/NXToggleButtonPrivate.h",
		"Source/private/NXLogPrivate.h",
		"Source/private/NXMessageBarPrivate.h",
		"Source/private/NXWidgetPrivate.h",
		"Source/private/NXProgressBarPrivate.h",
		"Source/private/NXTextPrivate.h",
		"Source/private/NXSpinBoxPrivate.h",
		"Source/private/NXComboBoxPrivate.h",
		"Source/private/NXAcrylicUrlCardPrivate.h",
		"Source/private/NXMenuPrivate.h",
		"Source/private/NXCheckBoxPrivate.h",
		"Source/private/NXLCDNumberPrivate.h",
		"Source/private/NXListViewPrivate.h",
		"Source/private/NXTreeViewPrivate.h",
		"Source/private/NXMultiSelectComboBoxPrivate.h",
		"Source/private/NXKeyBinderPrivate.h",
		"Source/private/NXIconButtonPrivate.h",
		"Source/private/NXDialogPrivate.h",
		
		-- DeveloperComponents headers
		"Source/DeveloperComponents/NXDrawerContainer.h",
		"Source/DeveloperComponents/NXColorDisplayDelegate.h",
		"Source/DeveloperComponents/NXDrawerHeader.h",
		"Source/DeveloperComponents/NXPivotView.h",
		"Source/DeveloperComponents/NXNavigationNode.h",
		"Source/DeveloperComponents/NXSliderStyle.h",
		"Source/DeveloperComponents/NXMicaBaseInitObject.h",
		"Source/DeveloperComponents/NXTreeViewStyle.h",
		"Source/DeveloperComponents/NXIntValidator.h",
		"Source/DeveloperComponents/NXLCDNumberStyle.h",
		"Source/DeveloperComponents/NXNavigationView.h",
		"Source/DeveloperComponents/NXDockWidgetTitleBar.h",
		"Source/DeveloperComponents/NXScrollBarStyle.h",
		"Source/DeveloperComponents/NXColorPreview.h",
		"Source/DeveloperComponents/NXStatusBarStyle.h",
		"Source/DeveloperComponents/NXCheckBoxStyle.h",
		"Source/DeveloperComponents/NXMenuBarStyle.h",
		"Source/DeveloperComponents/NXRadioButtonStyle.h",
		"Source/DeveloperComponents/NXSpinBoxStyle.h",
		"Source/DeveloperComponents/NXComboBoxStyle.h",
		"Source/DeveloperComponents/NXCalendarModel.h",
		"Source/DeveloperComponents/NXPivotStyle.h",
		"Source/DeveloperComponents/NXThemeAnimationWidget.h",
		"Source/DeveloperComponents/NXSuggestDelegate.h",
		"Source/DeveloperComponents/NXToolBarStyle.h",
		"Source/DeveloperComponents/NXTabBarStyle.h",
		"Source/DeveloperComponents/NXWindowStyle.h",
		"Source/DeveloperComponents/NXProgressBarStyle.h",
		"Source/DeveloperComponents/NXBaseListView.h",
		"Source/DeveloperComponents/NXTableViewStyle.h",
		"Source/DeveloperComponents/NXLineEditStyle.h",
		"Source/DeveloperComponents/NXMaskWidget.h",
		"Source/DeveloperComponents/NXKeyBinderContainer.h",
		"Source/DeveloperComponents/NXCustomTabWidget.h",
		"Source/DeveloperComponents/NXColorPicker.h",
		"Source/DeveloperComponents/NXCalendarTitleDelegate.h",
		"Source/DeveloperComponents/NXWinShadowHelper.h",
		"Source/DeveloperComponents/NXBreadcrumbBarDelegate.h",
		"Source/DeveloperComponents/NXPopularCardFloater.h",
		"Source/DeveloperComponents/NXNavigationStyle.h",
		"Source/DeveloperComponents/NXComboBoxView.h",
		"Source/DeveloperComponents/NXCustomWidget.h",
		"Source/DeveloperComponents/NXCalendarTitleModel.h",
		"Source/DeveloperComponents/NXFooterModel.h",
		"Source/DeveloperComponents/NXMenuStyle.h",
		"Source/DeveloperComponents/NXCalendarPickerContainer.h",
		"Source/DeveloperComponents/NXSuggestBoxSearchViewContainer.h",
		"Source/DeveloperComponents/NXColorDisplayModel.h",
		"Source/DeveloperComponents/NXToolButtonStyle.h",
		"Source/DeveloperComponents/NXPlainTextEditStyle.h",
		"Source/DeveloperComponents/NXListViewStyle.h",
		"Source/DeveloperComponents/NXNavigationModel.h",
		"Source/DeveloperComponents/NXColorValueSliderStyle.h",
		"Source/DeveloperComponents/NXDxgi.h",
		"Source/DeveloperComponents/NXCalendarDelegate.h",
		"Source/DeveloperComponents/NXBreadcrumbBarModel.h",
		"Source/DeveloperComponents/NXSuggestModel.h",
		"Source/DeveloperComponents/NXPivotModel.h",
		"Source/DeveloperComponents/NXFooterDelegate.h"
	}

	-- Qt RCC files (resource files)
	qtrcc =
	{
		"NexUs.qrc"
	}


	includedirs
	{
		"Source",
		"%{IncludeDir.NexUs}",
		"%{IncludeDir.NexUsInclude}"
	}

	defines
	{
		"NEXUS_LIBRARY"
	}

	-- <Dependencies.lua> Configure Qt
	ConfigureQt()
	
	-- <Dependencies.lua> Configure DirectX (Windows only)
	ConfigureDirectX()

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"Q_OS_WIN"
		}
		links
		{
			"%{Library.WinSock}",
			"%{Library.WinMM}",
			"%{Library.WinVersion}",
			"%{Library.BCrypt}",
			"%{Library.OpenGL32}"
		}

	filter "system:linux"
		defines
		{
			"Q_OS_LINUX"
		}
		links
		{
			"pthread",
			"dl",
			"GL"
		}

	filter "system:macosx"
		defines
		{
			"Q_OS_MACOS"
		}
		links
		{
			"pthread",
			"OpenGL.framework",
			"Cocoa.framework"
		}

	filter "configurations:Debug"
		defines "NX_DEBUG"
		runtime "Debug"
		symbols "on"
		targetname "nexusd"
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
		targetname "nexus"
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
		targetname "nexus"
		links
		{
			"%{Library.QtCore_Release}",
			"%{Library.QtGui_Release}",
			"%{Library.QtWidgets_Release}"
		}

	filter {}

	-- <Dependencies.lua> Configure Qt-specific file handling
	-- ConfigureQtFiles("NexUs")