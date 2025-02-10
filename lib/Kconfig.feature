# SPDX-License-Identifier: GPL-3.0-or-later

config CC_HAS_TOPLEVEL_REORDER
	def_bool $(cc-option,-ftoplevel-reorder)

menu "Core features"

menuconfig I18N_SUPPORT
	bool "Enable i18n support"

if I18N_SUPPORT

config TEXT_DOMAIN_NAME
	string "Text domain name for gettext"
	default $(pg-name)

config CUSTOM_TEXT_LOCALE
	bool "Enable custom program locale"

config TEXT_LOCALE
	string "Specify program locale"
	default "C.UTF-8"
	depends on CUSTOM_TEXT_LOCALE

endif # I18N_SUPPORT

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
