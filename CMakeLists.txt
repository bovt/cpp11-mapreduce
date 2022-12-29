cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(hw11_mr VERSION ${PROJECT_VESRION})

set(CMAKE_CXX_STANDARD 17)

include_directories("core/")
include_directories("common/")

add_executable(mr main.cpp)

find_package(Boost REQUIRED COMPONENTS program_options system REQUIRED)
if(NOT Boost_FOUND)
    message(SEND_ERROR "Failed to find boost")
    return()
else()
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(mr pthread ${Boost_LIBRARIES})
endif()


install(TARGETS mr RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)


