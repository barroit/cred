# SPDX-License-Identifier: GPL-3.0-or-later or MIT

menu "Core features"

menuconfig ENABLE_I18N
	bool "Enable i18n support"

if ENABLE_I18N

config TEXT_DOMAIN_NAME
	string "Text domain name for gettext"
	default $(pg-name)

menuconfig ENABLE_CUSTOM_TEXT_LOCALE
	bool "Customize program locale"

config TEXT_LOCALE
	string "Specify program locale"
	default "C.UTF-8"
	depends on ENABLE_CUSTOM_TEXT_LOCALE

endif # ENABLE_I18N

menu "Formatting options"

menuconfig SPEC_ALT_CNTRL
	bool "Customize control character replacement"
	default y

if SPEC_ALT_CNTRL

config ALT_CNTRL
	string "Control character replacement"
	default "�"

endif

config LINE_WRAP
	int "Line wrap threshold"
	default 80

config HELP_OPT_INDENT
	int "Help message indentation"
	default 2

config HELP_OPT_WRAP
	int "Help message wrap threshold"
	default 16

endmenu # Character settings

config NULL_DEVICE
	string "Null device path"
	default "NUL:" if $(WIN32)
	default "/dev/null"

endmenu # Program features
