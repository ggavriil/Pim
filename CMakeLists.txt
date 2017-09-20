cmake_minimum_required( VERSION 2.8 )

# Mark the language as C so that CMake doesn't try to test the C++
# cross-compiler's ability to compile a simple program because that will fail
project( rpi C ASM )

# We had to adjust the CMAKE_C_FLAGS variable in the toolchain file to make sure
# the compiler would work with CMake's simple program compilation test. So unset
# it explicitly before re-setting it correctly for our system
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostartfiles" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99" )

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-T,${PROJECT_SOURCE_DIR}/rpi.x")

add_executable( rpi
    cstartup.c
    cstubs.c
    timer.c
    main.c
    uart.c
    graphics.c
    engine.c
    mailbox.c
    gamehelpers.c
    physics.c
    game.c
    start.s )

target_link_libraries(rpi m)

add_custom_command(
    TARGET rpi POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ./rpi${CMAKE_EXECUTABLE_SUFFIX} -O binary ./kernel.img
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Converting the ELF output file to a binary image" )
