#Generated by VisualGDB (http://visualgdb.com)
#DO NOT EDIT THIS FILE MANUALLY UNLESS YOU ABSOLUTELY NEED TO
#USE VISUALGDB PROJECT PROPERTIES DIALOG INSTEAD

BINARYDIR := Debug

#Toolchain
CC := D:/cpplib/arm-linux-gnueabihf-gcc49/bin/arm-linux-gnueabihf-gcc.exe
CXX := D:/cpplib/arm-linux-gnueabihf-gcc49/bin/arm-linux-gnueabihf-g++.exe
LD := $(CXX)
AR := D:/cpplib/arm-linux-gnueabihf-gcc49/bin/arm-linux-gnueabihf-ar.exe
OBJCOPY := D:/cpplib/arm-linux-gnueabihf-gcc49/bin/arm-linux-gnueabihf-objcopy.exe

#Additional flags
PREPROCESSOR_MACROS := DEBUG _ARM32 _DEBUG ENABLE_NEXT_TICK ENABLE_CHECK_LOST DISABLE_BOOST_TIMER
INCLUDE_DIRS := D:\cpplib\boost
LIBRARY_DIRS := D:\cpplib\boost\lib_armhf-linux-gcc492 ../MyActor/actor/lib
LIBRARY_NAMES := pthread sigsegv_armhf32 fcontext_armhf32 boost_thread boost_system boost_chrono
ADDITIONAL_LINKER_INPUTS := 
MACOS_FRAMEWORKS := 
LINUX_PACKAGES := 

CFLAGS := -ggdb -ffunction-sections -O0
CXXFLAGS := -ggdb -ffunction-sections -O0 -std=c++11
ASFLAGS := 
LDFLAGS := -Wl,-gc-sections -static
COMMONFLAGS := 

START_GROUP := -Wl,--start-group
END_GROUP := -Wl,--end-group

#Additional options detected from testing the toolchain
USE_DEL_TO_CLEAN := 1
CP_NOT_AVAILABLE := 1
IS_LINUX_PROJECT := 1
