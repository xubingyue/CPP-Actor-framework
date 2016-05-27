#Generated by VisualGDB (http://visualgdb.com)
#DO NOT EDIT THIS FILE MANUALLY UNLESS YOU ABSOLUTELY NEED TO
#USE VISUALGDB PROJECT PROPERTIES DIALOG INSTEAD

BINARYDIR := Release

#Toolchain
CC := /usr/gcc-4.9.3/bin/gcc
CXX := /usr/gcc-4.9.3/bin/g++
LD := $(CXX)
AR := ar
OBJCOPY := objcopy

#Additional flags
PREPROCESSOR_MACROS := NDEBUG RELEASE ENABLE_NEXT_TICK ENABLE_CHECK_LOST DISABLE_BOOST_TIMER
INCLUDE_DIRS := /home/ham/cpplib/boost
LIBRARY_DIRS := /home/ham/cpplib/boost/stage/lib ./actor/lib
LIBRARY_NAMES := pthread rt sigsegv_x64 fcontext_x64 boost_thread boost_system boost_chrono
ADDITIONAL_LINKER_INPUTS := 
MACOS_FRAMEWORKS := 
LINUX_PACKAGES := 

CFLAGS := -ffunction-sections -O3
CXXFLAGS := -ffunction-sections -O3 -std=c++11
ASFLAGS := 
LDFLAGS := -Wl,-gc-sections
COMMONFLAGS := 

START_GROUP := -Wl,--start-group
END_GROUP := -Wl,--end-group

#Additional options detected from testing the toolchain
IS_LINUX_PROJECT := 1
