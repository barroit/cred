# SPDX-License-Identifier: GPL-3.0-or-later or MIT

if [[ ! -d .git ]]; then
	>&2 echo you need to be on the source tree
	exit 1
fi

source scripts/libutil

license=$(jvalue license .vscode/settings.json)
year=$(jvalue year .vscode/settings.json)

if [[ ! $1 ]]; then
	die 'missing program name'
elif [[ -f .program ]]; then
	die "you've already set $(cvalue name .program) up"
elif [[ $license = 'GPL-3.0-or-later or MIT' ]]; then
	die 'you probably forgot to update license'
fi

cat <<EOF > .program
name	$1
version	0.0
EOF

ln -s ../../.hooks/pre-commit .git/hooks/pre-commit

mkdir commands

cat <<EOF > unix/main.c
// SPDX-License-Identifier: $license
/*
 * Copyright $year Jiamu Sun <barroit@linux.com>
 */

int main(int argc, const char **argv)
{
	return 0;
}
EOF

cp unix/main.c win32/main.tui.c

cat <<EOF > win32/main.gui.c
// SPDX-License-Identifier: $license
/*
 * Copyright $year Jiamu Sun <barroit@linux.com>
 */

int WinMain(HINSTANCE app, HINSTANCE _, char *cmdline, int __)
{
	return 0;
}
EOF

cat <<EOF > $1.manifest.in
<?xml version="1.0" encoding="UTF-8"?>
<!-- SPDX-License-Identifier: $license -->
<assembly
  xmlns="urn:schemas-microsoft-com:asm.v1"
  xmlns:asmv3="urn:schemas-microsoft-com:asm.v3"
  manifestVersion="1.0">
  <assemblyIdentity
    type="win32"
    name="@name@"
    version="@version_manifest@"/>
  <compatibility xmlns="urn:schemas-microsoft-com:compatibility.v1">
    <maxversiontested Id="10.0.22621.1778"/>
    <!-- Windows 10 and Windows 11 -->
    <supportedOS Id="{8e0f7a12-bfb3-4fe8-b9a5-48fd50a15a9a}"/>
    <!-- Windows 8.1 -->
    <supportedOS Id="{1f676c76-80e1-4239-95bb-83d0f6d0da78}"/>
    <!-- Windows 8 -->
    <supportedOS Id="{4a2f28e3-53b9-4441-ba9c-d69d4a4a6e38}"/>
    <!-- Windows 7 -->
    <supportedOS Id="{35138b9a-5d96-4fbd-8e2d-a2440225f93a}"/>
    <!-- windows Vista -->
    <supportedOS Id="{e2011457-1546-43c5-a5fe-008deee3d3f0}" />
  </compatibility>
  <dependency>
    <dependentAssembly>
      <assemblyIdentity
        type="win32"
        name="Microsoft.Windows.Common-Controls"
        version="6.0.0.0"
        processorArchitecture="*"
        publicKeyToken="6595b64144ccf1df"
        language="*"/>
    </dependentAssembly>
  </dependency>
  <asmv3:application>
    <asmv3:windowsSettings
      xmlns="http://schemas.microsoft.com/SMI/2005/WindowsSettings">
      <dpiAware>true</dpiAware>
    </asmv3:windowsSettings>
    <asmv3:windowsSettings
      xmlns:ws2="http://schemas.microsoft.com/SMI/2016/WindowsSettings">
      <ws2:longPathAware>true</ws2:longPathAware>
    </asmv3:windowsSettings>
    <asmv3:windowsSettings
      xmlns="http://schemas.microsoft.com/SMI/2019/WindowsSettings"> 
      <activeCodePage>UTF-8</activeCodePage> 
    </asmv3:windowsSettings>
    <asmv3:windowsSettings
      xmlns="http://schemas.microsoft.com/SMI/2020/WindowsSettings">
      <heapType>SegmentHeap</heapType>
    </asmv3:windowsSettings>
  </asmv3:application>
</assembly>
EOF
