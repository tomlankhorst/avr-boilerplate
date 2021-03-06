cmake_minimum_required(VERSION 3.0)

### TOOLCHAIN #######
# Set any variables used in the toolchain prior project() call (!)
# In that case they are already set and used.
include(cmake/generic-gcc-avr.cmake)

# Tools to be used for programming the AVR
set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER atmelice)
set(AVR_UPLOADTOOL_PORT usb)

# AVR and fuses needs to be set
set(AVR_MCU "atxmega256a3bu")
set(AVR_MCU_SPEED "32000000")
set(AVR_BOARD "XMEGA_A3BU_XPLAINED")
set(AVR_H_FUSE 0xd9)
set(AVR_L_FUSE 0xc3)

### PROJECT ###########
# Set any variables used in the toolchain prior project() call. In that
# case they are already set and used.

project(avr-boilerplate CXX C ASM)

# Device specific definitions
add_definitions("-DIOPORT_XMEGA_COMPAT")
add_definitions("-DGFX_MONO_C12832_A1Z=1")
add_definitions("-DCONFIG_NVM_IGNORE_XMEGA_A3_D3_REVB_ERRATA")

if(AVR_BOARD)
  add_definitions("-DBOARD=${AVR_BOARD}")
endif(AVR_BOARD)

### ATMEL SOFTWARE FRAMEWORK ##########
# Paths, platforms, cpus
set(ASF_PATH /opt/xdk-asf-3.36.0)
set(ASF_PLATFORM xmega)
set(ASF_CPU xmega256a3bu)

### SETUP BUILD ENVIRONMENT ########
# Echo configuration, set build type, set variabels
# Prepare environment

# Status tools
message(STATUS "Current uploadtool is: ${AVR_UPLOADTOOL}")
message(STATUS "Current programmer is: ${AVR_PROGRAMMER}")
message(STATUS "Current upload port is: ${AVR_UPLOADTOOL_PORT}")
message(STATUS "Current uploadtool options are: ${AVR_UPLOADTOOL_OPTIONS}")
message(STATUS "Current MCU is set to: ${AVR_MCU}")
message(STATUS "Current H_FUSE is set to: ${AVR_H_FUSE}")
message(STATUS "Current L_FUSE is set to: ${AVR_L_FUSE}")

# Set build type if not already set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

# Cross-compile necessities
if(DEFINED ENV{AVR_FIND_ROOT_PATH})
    set(CMAKE_FIND_ROOT_PATH $ENV{AVR_FIND_ROOT_PATH})
else(DEFINED ENV{AVR_FIND_ROOT_PATH})
    if(EXISTS "/opt/local/avr")
        set(CMAKE_FIND_ROOT_PATH "/opt/local/avr")
    elseif(EXISTS "/usr/local/opt/avr-libc/avr")
        set(CMAKE_FIND_ROOT_PATH "/usr/local/opt/avr-libc/avr")
    elseif(EXISTS "/usr/avr")
        set(CMAKE_FIND_ROOT_PATH "/usr/avr")
    elseif(EXISTS "/usr/lib/avr")
        set(CMAKE_FIND_ROOT_PATH "/usr/lib/avr")
    elseif(EXISTS "/usr/local/CrossPack-AVR")
        set(CMAKE_FIND_ROOT_PATH "/usr/local/CrossPack-AVR")
    else(EXISTS "/opt/local/avr")
        message(FATAL_ERROR "Please set AVR_FIND_ROOT_PATH in your environment.")
    endif(EXISTS "/opt/local/avr")
endif(DEFINED ENV{AVR_FIND_ROOT_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# not added automatically, since CMAKE_SYSTEM_NAME is "generic"
set(CMAKE_SYSTEM_INCLUDE_PATH "${CMAKE_FIND_ROOT_PATH}/include")
set(CMAKE_SYSTEM_LIBRARY_PATH "${CMAKE_FIND_ROOT_PATH}/lib")

# Status paths
message(STATUS "Set CMAKE_FIND_ROOT_PATH to ${CMAKE_FIND_ROOT_PATH}")
message(STATUS "Set CMAKE_SYSTEM_INCLUDE_PATH to ${CMAKE_SYSTEM_INCLUDE_PATH}")
message(STATUS "Set CMAKE_SYSTEM_LIBRARY_PATH to ${CMAKE_SYSTEM_LIBRARY_PATH}")

# Compiler options for Release build type
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_C_FLAGS_RELEASE "-Os")
    set(CMAKE_CXX_FLAGS_RELEASE "-Os")
endif(CMAKE_BUILD_TYPE MATCHES Release)

# Compiler options for RelWithDebInfo build type
if(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "-Os -save-temps -g -gdwarf-3 -gstrict-dwarf")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -save-temps -g -gdwarf-3 -gstrict-dwarf")
endif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)

# Compiler options for RELEASE build type
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS_DEBUG "-Os -save-temps -g -gdwarf-3 -gstrict-dwarf")
    set(CMAKE_CXX_FLAGS_DEBUG "-Os -save-temps -g -gdwarf-3 -gstrict-dwarf")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

# Compiler options for all build types
add_definitions("-DF_CPU=${AVR_MCU_SPEED}")
add_definitions("-DBOARD=${AVR_BOARD}")
add_definitions("-fpack-struct")
add_definitions("-fshort-enums")
add_definitions("-Wall")
# add_definitions("-Werror")
# http://gcc.gnu.org/onlinedocs/gcc-4.8.2/gcc/Alternate-Keywords.html#Alternate-Keywords
# [...]-pedantic and other options cause warnings for many GNU C extensions. You can prevent such warnings within
# one expression by writing __extension__ before the expression. __extension__ has no effect aside from this.[...]
#add_definitions("-pedantic")
#add_definitions("-pedantic-errors")
add_definitions("-funsigned-char")
add_definitions("-funsigned-bitfields")
add_definitions("-ffunction-sections")
add_definitions("-c")
# add_definitions("-std=gnu99")

### INCLUDE ASF ####
# Which drivers and services to register
set(ASF_DRIVERS
  cpu
  dma
  nvm
  usart
  sleep
  pmic
  qdec
  tc
  # etc...
  )
set(ASF_SERVICES
  clock
  delay
  gpio
  gfx_mono
  ioport
  sleepmgr
  spi
  hugemem
  # etc...
  )

# Process the ASF dependencies
include(cmake/atmel-software-framework.cmake)

# Extras

# Board LED
asf_add_custom_header_file(board xmega xmega_a3bu_xplained led.h)

# IOPort compat
asf_add_custom_source_file(service common ioport xmega/ioport_compat.c)

# Sleepmgr for XMEGA
asf_add_custom_source_file(service common sleepmgr xmega/sleepmgr.c)

# NVM
asf_add_custom_asm_file(driver xmega nvm nvm_asm.S)

# CPU
asf_add_custom_asm_file(driver xmega cpu ccp.S)
asf_add_custom_header_file(driver xmega cpu ccp.h)

# Clock
asf_add_custom_source_file(service common clock xmega/sysclk.c)
asf_add_custom_header_file(service common clock sysclk.h)

# Display
asf_add_custom_source_file(component common display st7565r/st7565r.c)
asf_add_custom_header_file(component common display st7565r/st7565r.h)

asf_add_custom_source_file(service common gfx_mono gfx_mono_c12832_a1z.c)
asf_add_custom_source_file(service common gfx_mono gfx_mono_framebuffer.c)
asf_add_custom_source_file(service common gfx_mono gfx_mono_generic.c)
asf_add_custom_source_file(service common gfx_mono gfx_mono_text.c)
asf_add_custom_source_file(service common gfx_mono sysfont.c)

# USART SPI
asf_add_custom_source_file(service common spi usart_spi.c)
asf_add_custom_source_file(service common spi xmega_usart_spi/usart_spi.c)
asf_add_custom_header_file(service common spi usart_spi.h)

# TWI Slave
asf_add_custom_source_file(service common twi twi_slave.c)
asf_add_custom_header_file(service common twi twi_slave.h)
asf_add_custom_header_file(driver xmega twi twi_common.h)
asf_add_custom_source_file(driver xmega twi twis.c)
asf_add_custom_header_file(driver xmega twi twis.h)

# USB
asf_add_custom_source_file(service common usb class/cdc/device/udi_cdc.c)
asf_add_custom_source_file(service common usb class/cdc/device/udi_cdc_desc.c)
asf_add_custom_source_file(service common usb udc/udc.c)
asf_add_custom_source_file(driver xmega usb usb_device.c)
asf_add_custom_header_file(service common usb usb_protocol.h)
asf_add_custom_header_file(service common usb usb_atmel.h)
asf_add_custom_header_file(service common usb udc/udc.h)
asf_add_custom_header_file(service common usb class/cdc/usb_protocol_cdc.h)
asf_add_custom_header_file(service common usb class/cdc/device/udi_cdc.h)
asf_add_custom_header_file(driver xmega usb usb_device.h)

# stdio
asf_add_custom_source_file(util common stdio read.c)
asf_add_custom_source_file(util common stdio write.c)
asf_add_custom_source_file(util common stdio stdio_usb/stdio_usb.c)
asf_add_custom_header_file(util common stdio stdio_usb/stdio_usb.h)

#
#asf_add_custom_source_file(service common hugemem avr8/avr8_hugemem.c)
#asf_add_custom_header_file(service common hugemem generic/hugemem.h)
#asf_add_custom_header_file(service common hugemem avr8/hugemem.h)

# Finally, include the directories
include_directories(${ASF_INCLUDE_DIRS})

### CONFIGURE HEADER ####
string(TIMESTAMP BUILD_DATE "%Y-%m-%d %H:%M")
message(STATUS "Build Date: ${BUILD_DATE}")
include(cmake/git-status.cmake)

configure_file(conf/conf_build.h.in conf/conf_build.h @ONLY)
include_directories(${CMAKE_SOURCE_DIR}/conf)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/conf)

### REGISTER TARGETS ####
set(avr_boilerplate_srcs
  src/init.c
  src/avr_boilerplate.c
  ${ASF_SOURCE_FILES}
  )

add_avr_executable(
    avr_boilerplate
    ${avr_boilerplate_srcs}
)

include_directories(include)

#target_link_libraries(
#    <library-name>.elf
#    <library-name>
#)

#add_avr_library(
#   <library-name>
#   <sources> <headers>
#)

set(CMAKE_C_LINK_FLAGS "${CMAKE_C_FLAGS} -Wl,-u,vfprintf -lprintf_flt")
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_FLAGS})