cmake_minimum_required(VERSION 2.8)

### TOOLCHAIN ##################################################################
# Set any variables used in the toolchain prior project() call. In that
# case they are already set and used.

# Tools to be used for programming the AVR
set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER jtag2)
set(AVR_UPLOADTOOL_PORT usb)

# AVR and fuses needs to be set
set(AVR_MCU atxmega256a3bu)
set(AVR_MCU_SPEED "32000000UL")
set(AVR_BOARD "XMEGA_A3BU_XPLAINED")
set(AVR_H_FUSE 0xd9)
set(AVR_L_FUSE 0xc3)

# External libraries
set(ASF /opt/xdk-asf-3.36.0)

### PROJECT ##################################################################
# Set any variables used in the toolchain prior project() call. In that
# case they are already set and used.

project(avr-boilerplate)

# Device specific definitions
add_definitions("-DIOPORT_XMEGA_COMPAT")
add_definitions("-DGFX_MONO_C12832_A1Z=1")


### SETUP BUILD ENVIRONMENT ####################################################
# Echo configuration, set build type, set variabels
# Prepare environment

include(cmake/generic-gcc-avr.cmake)

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
add_definitions("-Werror")
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

### REGISTER TARGETS ##############################################
set(AVR_BP_SRCS src/main.c)

add_avr_executable(
    avr_boilerplate
    ${AVR_BP_SRCS}
)

#target_link_libraries(
#    <library-name>.elf
#    <library-name>
#)

#add_avr_library(
#   <library-name>
#   <sources> <headers>
#)