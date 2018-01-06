#-------------------------------------------------------------------------------
#	  _____             __ _                       _   _                 
#	 / ____|           / _(_)                     | | (_)                
#	| |     ___  _ __ | |_ _  __ _ _   _ _ __ __ _| |_ _  ___  _ __  ___ 
#	| |    / _ \| '_ \|  _| |/ _` | | | | '__/ _` | __| |/ _ \| '_ \/ __|
#	| |___| (_) | | | | | | | (_| | |_| | | | (_| | |_| | (_) | | | \__ \
#	 \_____\___/|_| |_|_| |_|\__, |\__,_|_|  \__,_|\__|_|\___/|_| |_|___/
#	                          __/ |                                      
#	                         |___/                                       
#
#-------------------------------------------------------------------------------
#
#	Custom project Makefile configurations.
#	This file is auto generated.
#
#-------------------------------------------------------------------------------

EXEC =						hsm

PLATFORM =					host

CUSTOM_C_DEFS =				-DSTM32F072xB

TARGET_LDSCRIPT =			ld/STM32F072RBTx_FLASH.ld

TARGET_TC_PATH =			~/opt/gcc-arm-none-eabi-6-2017-q2-update/bin/

CPPUTEST_DIR =				~/opt/cpputest/

#-------------------------------------------------------------------------------
#						Custom tools check if exist

JLINK_EXISTS := $(shell command -v JLinkExe 2> /dev/null)
ifndef JLINK_EXISTS
  $(warning "Please install Segger J-Link drivers!")
endif

SALEAE_EXISTS := $(shell command -v Saleae 2> /dev/null)
ifndef SALEAE_EXISTS
  $(warning "Please install Saleae logic analyser!")
endif

PICOCOM_EXISTS := $(shell command -v picocom 2> /dev/null)
ifndef PICOCOM_EXISTS
  $(warning "Please install 'picocom' terminal emulation!")
endif
