make:
	-@mkdir "%ConfigurationBuildDir%\bin%PlatformArchitecture%\CyberKit.resources\WebInspectorUI"
	@perl build-webinspectorui.pl "%ConfigurationBuildDir%" "%CyberKit_Libraries%" windows "%PlatformArchitecture%" "%OFFICIAL_BUILD%"
	if errorlevel 1 exit 1

	@xcopy /y /e "..\Localizations\en.lproj\*" "%ConfigurationBuildDir%\bin%PlatformArchitecture%\CyberKit.resources\WebInspectorUI"
	if errorlevel 1 exit 1

	@xcopy /y /e "..\UserInterface\*" "%ConfigurationBuildDir%\bin%PlatformArchitecture%\CyberKit.resources\WebInspectorUI"
	if errorlevel 1 exit 1

	@xcopy /y /e "..\AppleInternal\include\private\CyberScriptCore\inspector\InspectorBackendCommands.js" "%ConfigurationBuildDir%\bin%PlatformArchitecture%\CyberKit.resources\WebInspectorUI\Protocol"
	if errorlevel 1 exit 1

	if exist "%CONFIGURATIONBUILDDIR%\buildfailed" del "%CONFIGURATIONBUILDDIR%\buildfailed"

clean:
	if exist "%ConfigurationBuildDir%\bin%PlatformArchitecture%\CyberKit.resources\WebInspectorUI" del /s /q "%ConfigurationBuildDir%\bin%PlatformArchitecture%\CyberKit.resources\WebInspectorUI"
