if(MSVC)
	cmake_minimum_required (VERSION 3.21) # 3.21 -> has vs 2022
else()
	cmake_minimum_required (VERSION 3.0.0)
endif()

set(CMAKE_CXX_STANDARD 17)

project("ChrTrcProfiler" VERSION "0.1.0" LANGUAGES CXX)

# place all predefined target projects into a filter
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PREDEFINED_TARGETS_FOLDER "Targets")

set (SRC
	src/ChrTrcProfiler.cpp
)

set (INCLUDE
	include/ChrTrcProfiler.h
	include/ChrTrcMacros.h
	include/ChrTrcCompileTimeString.h
	ipp_src/ChrTrcProfiler.ipp
)

# Add source to this project's executable.
add_library ( ${PROJECT_NAME} 
	${INCLUDE} ${SRC}
)

if(MSVC)
  target_compile_options(${PROJECT_NAME}  PUBLIC "/ZI")
  target_link_options(${PROJECT_NAME}  PUBLIC "/INCREMENTAL")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR} "include/" "ipp_src/")

# place files into the correct filters
source_group("src" FILES ${SRC})
source_group("include" FILES ${INCLUDE})

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-installer")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CMAKE_PROJECT_NAME}")
include(CPack)
