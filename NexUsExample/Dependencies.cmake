set(BASE_URL "https://raw.githubusercontent.com/Liniyous/ElaWidgetTools/main/ElaWidgetToolsExample/Resource/Image")

# 创建所有目标目录
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image/WindowBase)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image/Card)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image/IARC)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image/Model)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image/control)

# ==================== 根目录图片 ====================
set(ROOT_IMAGES
    "Cirno.jpg"
    "Home_Background.png"
    "Moon.jpg"
    "github.png"
)

foreach(IMG_FILE IN LISTS ROOT_IMAGES)
    set(DEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image/${IMG_FILE}")
    if(NOT EXISTS ${DEST_PATH})
        message(STATUS "Downloading ${IMG_FILE}...")
        file(DOWNLOAD
            "${BASE_URL}/${IMG_FILE}"
            "${DEST_PATH}"
            SHOW_PROGRESS
            TLS_VERIFY OFF
        )
    else()
        message(STATUS "${IMG_FILE} already exists, skipping download.")
    endif()
endforeach()

# ==================== WindowBase 目录 ====================
set(WINDOWBASE_IMAGES
    "Miku.gif"
    "Miku.png"
    "WorldTree.gif"
    "WorldTree.jpg"
)

foreach(IMG_FILE IN LISTS WINDOWBASE_IMAGES)
    set(DEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image/WindowBase/${IMG_FILE}")
    if(NOT EXISTS ${DEST_PATH})
        message(STATUS "Downloading WindowBase/${IMG_FILE}...")
        file(DOWNLOAD
            "${BASE_URL}/WindowBase/${IMG_FILE}"
            "${DEST_PATH}"
            SHOW_PROGRESS
            TLS_VERIFY OFF
        )
    else()
        message(STATUS "WindowBase/${IMG_FILE} already exists, skipping download.")
    endif()
endforeach()

# ==================== Card 目录 ====================
set(CARD_IMAGES
    "beach.png"
    "classroom.png"
    "dream.png"
    "miku.png"
)

foreach(IMG_FILE IN LISTS CARD_IMAGES)
    set(DEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image/Card/${IMG_FILE}")
    if(NOT EXISTS ${DEST_PATH})
        message(STATUS "Downloading Card/${IMG_FILE}...")
        file(DOWNLOAD
            "${BASE_URL}/Card/${IMG_FILE}"
            "${DEST_PATH}"
            SHOW_PROGRESS
            TLS_VERIFY OFF
        )
    else()
        message(STATUS "Card/${IMG_FILE} already exists, skipping download.")
    endif()
endforeach()

# ==================== IARC 目录 ====================
set(IARC_IMAGES
    "IARC_12+.svg.png"
    "IARC_16+.svg.png"
    "IARC_18+.svg.png"
    "IARC_3+.svg.png"
    "IARC_7+.svg.png"
)

foreach(IMG_FILE IN LISTS IARC_IMAGES)
    set(DEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image/IARC/${IMG_FILE}")
    if(NOT EXISTS ${DEST_PATH})
        message(STATUS "Downloading IARC/${IMG_FILE}...")
        file(DOWNLOAD
            "${BASE_URL}/IARC/${IMG_FILE}"
            "${DEST_PATH}"
            SHOW_PROGRESS
            TLS_VERIFY OFF
        )
    else()
        message(STATUS "IARC/${IMG_FILE} already exists, skipping download.")
    endif()
endforeach()

# ==================== Model 目录 ====================
set(MODEL_IMAGES
    "CollapsingWorld.jpg"
    "DarkForest.jpg"
    "LASTSTARDUST.jpg"
    "Light.jpg"
    "MaVieEnRose.jpg"
    "NaightNavigationStar.jpg"
    "RainMan.jpg"
    "RunningInTheDark.jpg"
    "STYXHELIX.jpg"
)

foreach(IMG_FILE IN LISTS MODEL_IMAGES)
    set(DEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image/Model/${IMG_FILE}")
    if(NOT EXISTS ${DEST_PATH})
        message(STATUS "Downloading Model/${IMG_FILE}...")
        file(DOWNLOAD
            "${BASE_URL}/Model/${IMG_FILE}"
            "${DEST_PATH}"
            SHOW_PROGRESS
            TLS_VERIFY OFF
        )
    else()
        message(STATUS "Model/${IMG_FILE} already exists, skipping download.")
    endif()
endforeach()

# ==================== control 目录 ====================
set(CONTROL_IMAGES
    "Acrylic.png"
    "AnimatedIcon.png"
    "AnimatedVisualPlayer.png"
    "AnimationInterop.png"
    "AppBarButton.png"
    "AppBarSeparator.png"
    "AppBarToggleButton.png"
    "AutoSuggestBox.png"
    "AutomationProperties.png"
    "Border.png"
    "BreadcrumbBar.png"
    "Button.png"
    "CalendarDatePicker.png"
    "CalendarView.png"
    "Canvas.png"
    "Checkbox.png"
    "Clipboard.png"
    "ColorPaletteResources.png"
    "ColorPicker.png"
    "ComboBox.png"
    "CommandBar.png"
    "CommandBarFlyout.png"
    "CompactSizing.png"
    "ConnectedAnimation.png"
    "ContentDialog.png"
    "CreateMultipleWindows.png"
    "DataGrid.png"
    "DatePicker.png"
    "DropDownButton.png"
    "EasingFunction.png"
    "Expander.png"
    "FilePicker.png"
    "FlipView.png"
    "Flyout.png"
    "Grid.png"
    "GridView.png"
    "HyperlinkButton.png"
    "IconElement.png"
    "Image.png"
    "ImplicitTransition.png"
    "InfoBadge.png"
    "InfoBar.png"
    "InkCanvas.png"
    "InkToolbar.png"
    "InputValidation.png"
    "ItemsRepeater.png"
    "Line.png"
    "ListBox.png"
    "ListView.png"
    "MediaPlayerElement.png"
    "MenuBar.png"
    "MenuFlyout.png"
    "NavigationView.png"
    "NumberBox.png"
    "PageTransition.png"
    "ParallaxView.png"
    "PasswordBox.png"
    "PersonPicture.png"
    "PipsPager.png"
    "Pivot.png"
    "ProgressBar.png"
    "ProgressRing.png"
    "PullToRefresh.png"
    "RadialGradientBrush.png"
    "RadioButton.png"
    "RadioButtons.png"
    "RatingControl.png"
    "RelativePanel.png"
    "RepeatButton.png"
    "RevealFocus.png"
    "RichEditBox.png"
    "RichTextBlock.png"
    "ScrollViewer.png"
    "SemanticZoom.png"
    "Shape.png"
    "Slider.png"
    "Sound.png"
    "SplitButton.png"
    "SplitView.png"
    "StackPanel.png"
    "StandardUICommand.png"
    "SwipeControl.png"
    "TabView.png"
    "TeachingTip.png"
    "TextBlock.png"
    "TextBox.png"
    "ThemeTransition.png"
    "TimePicker.png"
    "TitleBar.png"
    "ToggleButton.png"
    "ToggleSplitButton.png"
    "ToggleSwitch.png"
    "ToolTip.png"
    "TreeView.png"
    "VariableSizedWrapGrid.png"
    "Viewbox.png"
    "WebView.png"
    "XamlUICommand.png"
)

foreach(IMG_FILE IN LISTS CONTROL_IMAGES)
    set(DEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image/control/${IMG_FILE}")
    if(NOT EXISTS ${DEST_PATH})
        message(STATUS "Downloading control/${IMG_FILE}...")
        file(DOWNLOAD
            "${BASE_URL}/control/${IMG_FILE}"
            "${DEST_PATH}"
            SHOW_PROGRESS
            TLS_VERIFY OFF
        )
    else()
        message(STATUS "control/${IMG_FILE} already exists, skipping download.")
    endif()
endforeach()