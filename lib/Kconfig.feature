# SPDX-License-Identifier: GPL-3.0-or-later

menu "Core features"

menuconfig ENABLE_I18N
	bool "Enable i18n support"

if ENABLE_I18N

config TEXT_DOMAIN_NAME
	string "Text domain name for gettext"
	default $(pg-name)

menuconfig ENABLE_CUSTOM_TEXT_LOCALE
	bool "Customize program locale"

config CUSTOM_TEXT_LOCALE
	string "Specify program locale"
	default "C.UTF-8"
	depends on ENABLE_CUSTOM_TEXT_LOCALE

endif # ENABLE_I18N

menu "Formatting options"

menuconfig ENABLE_UTF8_CNTRL_CHAR_REPL
	bool "Customize control character replacement (utf-8)"
	default y

config UTF8_CNTRL_CHAR_REPL
	string "Control character replacement"
	default "�"
	depends on ENABLE_UTF8_CNTRL_CHAR_REPL

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
