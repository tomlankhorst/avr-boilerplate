# Status ASF
message(STATUS "ASF: ${ASF_PATH} (${ASF_PLATFORM}, ${ASF_CPU})")
message(STATUS "ASF drivers: ${ASF_DRIVERS}")
message(STATUS "ASF services: ${ASF_SERVICES}")


IF(NOT DEFINED ASF_PLATFORM OR NOT DEFINED ASF_PATH)
  message( FATAL_ERROR "ASF_PLATFORM and ASF_PATH need to be defined to use Atmel Software Framework.")
ENDIF(NOT DEFINED ASF_PLATFORM OR NOT DEFINED ASF_PATH)
IF(NOT DEFINED ASF_CPU AND ASF_SERVICES MATCHES ".*sysclk.*")
  message( FATAL_ERROR "You should define your ASF_CPU for sysclk support such as uc3b0_b1 or sam4s")
ENDIF(NOT DEFINED ASF_CPU AND ASF_SERVICES MATCHES ".*sysclk.*")


SET(ASF_COMMON_INCLUDES
  "${ASF_PATH}/common"
  "${ASF_PATH}/common/boards"
  "${ASF_PATH}/common/utils"
  )
SET(ASF_PLATFORM_INCLUDES
  "${ASF_PATH}/${ASF_PLATFORM}"
  "${ASF_PATH}/${ASF_PLATFORM}/boards"
  "${ASF_PATH}/${ASF_PLATFORM}/utils"
  "${ASF_PATH}/${ASF_PLATFORM}/utils/preprocessor"
  )

# Special cases which don't follow the usual platform/category/module/module.c pattern are listed below
#~ SET(ASF_

#~ include_directories(${ASF_PATH}/avr32)
#~ include_directories(${ASF_PATH}/common)
#~ include_directories(${ASF_PATH}/avr32/boards)
#~ include_directories(${ASF_PATH}/common/boards)
#~ include_directories(${ASF_PATH}/avr32/utils)
#~ include_directories(${ASF_PATH}/common/utils)
#~ include_directories(${ASF_PATH}/avr32/utils/preprocessor)

set(ASF_DRIVER_cycle_counter_EXTRA_SEARCHPATH "${ASF_PATH}/${ASF_PLATFORM}/drivers/cpu/cycle_counter")
set(ASF_DRIVER_power_clocks_lib_EXTRA_SEARCHPATH "${ASF_PATH}/${ASF_PLATFORM}/drivers/pm")
set(ASF_SERVICE_usart_serial_EXTRA_SEARCHPATH "${ASF_PATH}/common/services/serial")
set(ASF_SERVICE_sysclk_EXTRA_SEARCHPATH
  "${ASF_PATH}/common/services/clock"
  "${ASF_PATH}/common/services/clock/${ASF_CPU}")
set(ASF_SERVICE_twi_master_EXTRA_SEARCHPATH
  "${ASF_PATH}/common/services/twi"
  "${ASF_PATH}/common/services/twi/${ASF_ARCH}")
set(ASF_SERVICE_twi_slave_EXTRA_SEARCHPATH
  "${ASF_PATH}/common/services/twi"
  "${ASF_PATH}/common/services/twi/${ASF_ARCH}")

set(ASF_SOURCE_FILES)
set(ASF_INCLUDE_DIRS)

### CUSTOM FUNCTIONS FOR NON-STANDARD FILES ########
function(asf_add_custom_source_file TYPE PLATFORM GROUP FILE)
  SET(ASF_FILETMP "${ASF_PATH}/${PLATFORM}/${TYPE}s/${GROUP}/${FILE}")
  if(EXISTS "${ASF_FILETMP}")
    set(ASF_${TYPE}_${GROUP}_SOURCE_FILE "${ASF_FILETMP}")
    message(STATUS "Found ASF custom ${TYPE} source ${ASF_${TYPE}_${GROUP}_SOURCE_FILE}")
    list(APPEND ASF_SOURCE_FILES "${ASF_${TYPE}_${GROUP}_SOURCE_FILE}")
    # Propagate the new list to parent scope
    set(ASF_SOURCE_FILES ${ASF_SOURCE_FILES} PARENT_SCOPE)
  endif(EXISTS "${ASF_FILETMP}")
endfunction(asf_add_custom_source_file)

function(asf_add_custom_asm_file TYPE PLATFORM GROUP FILE)
  SET(ASF_FILETMP "${ASF_PATH}/${PLATFORM}/${TYPE}s/${GROUP}/${FILE}")
  if(EXISTS "${ASF_FILETMP}")
    asf_add_custom_source_file(${TYPE} ${PLATFORM} ${GROUP} ${FILE})
    set_property(SOURCE ${ASF_FILETMP} PROPERTY LANGUAGE ASM)
    set(ASF_SOURCE_FILES ${ASF_SOURCE_FILES} PARENT_SCOPE)
  endif(EXISTS "${ASF_FILETMP}")
endfunction(asf_add_custom_asm_file)

function(asf_add_custom_header_file TYPE PLATFORM GROUP FILE)
  SET(ASF_FILETMP "${ASF_PATH}/${PLATFORM}/${TYPE}s/${GROUP}/${FILE}")
  if(EXISTS "${ASF_FILETMP}")
    set(ASF_${TYPE}_${GROUP}_HEADER_FILE "${ASF_FILETMP}")
    message(STATUS "Found custom ASF ${TYPE} header ${ASF_${TYPE}_${GROUP}_HEADER_FILE}")
    get_filename_component(ASF_${TYPE}_${GROUP}_RELPATH "${ASF_${TYPE}_${GROUP}_HEADER_FILE}" PATH)
    get_filename_component(ASF_${TYPE}_${GROUP}_PATH "${ASF_${TYPE}_${GROUP}_RELPATH}" ABSOLUTE)
    list(APPEND ASF_INCLUDE_DIRS "${ASF_${TYPE}_${GROUP}_PATH}")
    list(APPEND ASF_HEADER_FILES "${ASF_${TYPE}_${GROUP}_HEADER_FILE}")
    # Propagate the new list to parent scope
    set(ASF_HEADER_FILES ${ASF_HEADER_FILES} PARENT_SCOPE)
    set(ASF_INCLUDE_DIRS ${ASF_INCLUDE_DIRS} PARENT_SCOPE)
  endif(EXISTS "${ASF_FILETMP}")
endfunction(asf_add_custom_header_file)

function(asf_set_board PLATFORM BOARD)
  SET(ASF_FILETMP "${ASF_PATH}/${PLATFORM}/boards/${BOARD}/${BOARD}.h")
  if(EXISTS "${ASF_FILETMP}")
    set(ASF_BOARD_${BOARD}_HEADER_FILE "${ASF_FILETMP}")
    message(STATUS "Found ASF board header ${ASF_BOARD_${BOARD}_HEADER_FILE}")
    get_filename_component(ASF_BOARD_${BOARD}_RELPATH "${ASF_BOARD_${BOARD}_HEADER_FILE}" PATH)
    get_filename_component(ASF_BOARD_${BOARD}_PATH "${ASF_BOARD_${BOARD}_RELPATH}" ABSOLUTE)
    list(APPEND ASF_INCLUDE_DIRS "${ASF_BOARD_${BOARD}_PATH}")
    list(APPEND ASF_HEADER_FILES "${ASF_BOARD_${BOARD}_HEADER_FILE}")
    # Propagate the new list to parent scope
    set(ASF_HEADER_FILES ${ASF_HEADER_FILES} PARENT_SCOPE)
    set(ASF_INCLUDE_DIRS ${ASF_INCLUDE_DIRS} PARENT_SCOPE)
  endif(EXISTS "${ASF_FILETMP}")
endfunction(asf_set_board)

foreach(DRIVER ${ASF_DRIVERS})
  #~ set(ASF_DRIVER_${DRIVER}_SOURCE_FILE)
  #~ set(ASF_DRIVER_${DRIVER}_HEADER_FILE)

  #~ message(STATUS "Looking for ${DRIVER}.c")
  foreach(ASF_PATHTMP ${ASF_DRIVER_${DRIVER}_EXTRA_SEARCHPATH} "${ASF_PATH}/${ASF_PLATFORM}/drivers/${DRIVER}" )
    #~ message(STATUS "in: ${ASF_PATHTMP}")
    SET(ASF_FILETMP "${ASF_PATHTMP}/${DRIVER}.c")
    if(EXISTS "${ASF_FILETMP}")
      set(ASF_DRIVER_${DRIVER}_SOURCE_FILE "${ASF_FILETMP}")
      message(STATUS "Found ASF driver source ${ASF_DRIVER_${DRIVER}_SOURCE_FILE}")
      list(APPEND ASF_SOURCE_FILES "${ASF_DRIVER_${DRIVER}_SOURCE_FILE}")
      break()
    endif(EXISTS "${ASF_FILETMP}")
  endforeach(ASF_PATHTMP)

  #~ message(STATUS "Looking for ${DRIVER}.h")
  foreach(ASF_PATHTMP ${ASF_DRIVER_${DRIVER}_EXTRA_SEARCHPATH} "${ASF_PATH}/${ASF_PLATFORM}/drivers/${DRIVER}" )
    #~ message(STATUS "in: ${ASF_PATHTMP}")
    SET(ASF_FILETMP "${ASF_PATHTMP}/${DRIVER}.h")
    if(EXISTS "${ASF_FILETMP}")
      set(ASF_DRIVER_${DRIVER}_HEADER_FILE "${ASF_FILETMP}")
      message(STATUS "Found ASF driver header ${ASF_DRIVER_${DRIVER}_HEADER_FILE}")
      get_filename_component(ASF_DRIVER_${DRIVER}_RELPATH "${ASF_DRIVER_${DRIVER}_HEADER_FILE}" PATH)
      get_filename_component(ASF_DRIVER_${DRIVER}_PATH "${ASF_DRIVER_${DRIVER}_RELPATH}" ABSOLUTE)
      list(APPEND ASF_INCLUDE_DIRS "${ASF_DRIVER_${DRIVER}_PATH}")
      list(APPEND ASF_HEADER_FILES "${ASF_DRIVER_${DRIVER}_HEADER_FILE}")
      break()
    endif(EXISTS "${ASF_FILETMP}")
  endforeach(ASF_PATHTMP)
endforeach(DRIVER)

foreach(SERVICE ${ASF_SERVICES})
  #~ message(STATUS "Looking for ${SERVICE}.c")
  foreach(ASF_PATHTMP ${ASF_SERVICE_${SERVICE}_EXTRA_SEARCHPATH} "${ASF_PATH}/${ASF_PLATFORM}/services/${SERVICE}" "${ASF_PATH}/common/services/${SERVICE}")
    #~ message(STATUS "in: ${ASF_PATHTMP}")
    SET(ASF_FILETMP "${ASF_PATHTMP}/${SERVICE}.c")
    if(EXISTS "${ASF_FILETMP}")
      set(ASF_SERVICE_${SERVICE}_SOURCE_FILE "${ASF_FILETMP}")
      message(STATUS "Found ASF service source ${ASF_SERVICE_${SERVICE}_SOURCE_FILE}")
      list(APPEND ASF_SOURCE_FILES "${ASF_SERVICE_${SERVICE}_SOURCE_FILE}")
      break()
    endif(EXISTS "${ASF_FILETMP}")
  endforeach(ASF_PATHTMP)

  #~ message(STATUS "Looking for ${SERVICE}.h")
  foreach(ASF_PATHTMP ${ASF_SERVICE_${SERVICE}_EXTRA_SEARCHPATH} "${ASF_PATH}/${ASF_PLATFORM}/services/${SERVICE}" "${ASF_PATH}/common/services/${SERVICE}")
    #~ message(STATUS "in: ${ASF_PATHTMP}")
    SET(ASF_FILETMP "${ASF_PATHTMP}/${SERVICE}.h")
    if(EXISTS "${ASF_FILETMP}")
      set(ASF_SERVICE_${SERVICE}_HEADER_FILE "${ASF_FILETMP}")
      message(STATUS "Found ASF service header ${ASF_SERVICE_${SERVICE}_HEADER_FILE}")
      get_filename_component(ASF_SERVICE_${SERVICE}_RELPATH "${ASF_SERVICE_${SERVICE}_HEADER_FILE}" PATH)
      get_filename_component(ASF_SERVICE_${SERVICE}_PATH "${ASF_SERVICE_${SERVICE}_RELPATH}" ABSOLUTE)
      list(APPEND ASF_INCLUDE_DIRS "${ASF_SERVICE_${SERVICE}_PATH}")
      list(APPEND ASF_HEADER_FILES "${ASF_SERVICE_${SERVICE}_HEADER_FILE}")
      break()
    endif(EXISTS "${ASF_FILETMP}")
  endforeach(ASF_PATHTMP)
endforeach(SERVICE)

LIST(APPEND ASF_INCLUDE_DIRS ${ASF_COMMON_INCLUDES} ${ASF_PLATFORM_INCLUDES})