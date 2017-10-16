# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Generic)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER /opt/cross-tools/bin/sdcc)

# here is the target environment is located
SET(CMAKE_FIND_ROOT_PATH  /opt/cross-tools/share/sdcc )

set(CMAKE_C_FLAGS_INIT  "-mstm8 --std-sdcc99")
# Flags used by the compiler during all build types.
set(CMAKE_C_FLAGS       "-mstm8 --std-sdcc99")
# Flags used by the compiler during debug builds.
set(CMAKE_C_FLAGS_DEBUG "--debug --allow-unsafe-read")
# Flags used by the compiler during release builds for minimum
# size.
set(CMAKE_C_FLAGS_MINSIZEREL "--model-small --opt-code-size")
# Flags used by the compiler during release builds.
set(CMAKE_C_FLAGS_RELEASE   "--model-small")
#Flags used by the compiler during release builds with debug info.
set(CMAKE_C_FLAGS_RELWITHDEBINFO "--debug")

#set(CMAKE_EXE_LINKER_FLAGS_INIT "")
#Flags used by the linker.
#set(CMAKE_EXE_LINKER_FLAGS  "")
#Flags used by the linker during debug builds.
#set(CMAKE_EXE_LINKER_FLAGS_DEBUG "")
# Flags used by the linker during release minsize builds.
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "")
# Flags used by the linker during release builds.
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "")
# Flags used by the linker during Release with Debug Info builds.
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "")

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)