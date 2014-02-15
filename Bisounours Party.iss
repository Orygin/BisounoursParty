[Setup]
AppId={{B538E52B-1CE5-497A-92FE-734BCFA271CA}
AppName=Bisounours Party
AppVerName=Bisounours Party 5.0.5102
AppVersion=5.0.5102
AppPublisher=Daft Robot
AppPublisherURL=http://www.bisounoursparty.com/
AppSupportURL=http://www.bisounoursparty.com/
AppUpdatesURL=http://www.bisounoursparty.com/
DefaultDirName={reg:HKCU\Software\Valve\Steam,SourceModInstallPath\bpr|{pf}\Steam\SteamApps\Sourcemods\bpr}
DisableDirPage=no
DefaultGroupName=Bisounours Party
AllowNoIcons=yes
OutputDir=.
OutputBaseFilename=bisounours-party_5.0.5102_windows
SetupIconFile=..\bpr\resource\game.ico
Compression=lzma/ultra
SolidCompression=yes
WizardImageFile=.\setup_large.bmp
WizardSmallImageFile=.\setup_small.bmp
WizardImageBackColor=clWhite
WizardImageStretch=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"

[Files]
Source: "..\bpr\*"; Excludes: "*.git*" ; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ".\Bisounours Party.ico"; DestDir: "{reg:HKLM\Software\Valve\Steam,InstallPath\steam\games|{pf}\Steam\steam\games}"; Flags : ignoreversion
Source: "..\bpr\CREDITS.TXT"; DestDir: "{app}"; Flags : ignoreversion isreadme
Source: "..\bpr\CHANGELOG.TXT"; DestDir: "{app}"; Flags : ignoreversion isreadme

[Icons]
Name: "{group}\{cm:UninstallProgram,Bisounours Party}"; Filename: "{uninstallexe}"
