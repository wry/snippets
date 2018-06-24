ifndef ANDROID_NDK_HOME
$(error ANDROID_NDK_HOME is not set)
endif

SYSROOT := $(ANDROID_NDK_HOME)/platforms/android-9/arch-arm/usr

CROSS_COMPILE := $(ANDROID_NDK_HOME)/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi
CXX := $(CROSS_COMPILE)-g++
CC := $(CROSS_COMPILE)-gcc
AS := $(CROSS_COMPILE)-as
AR := $(CROSS_COMPILE)-ar
NM := $(CROSS_COMPILE)-nm
LD := $(CROSS_COMPILE)-ld
STRIP := $(CROSS_COMPILE)-strip

STL_PATH := $(ANDROID_NDK_HOME)/sources/cxx-stl/gnu-libstdc++/4.9

CFLAGS   += --sysroot=$(SYSROOT) -I$(SYSROOT)/include -fPIE -pie -fPIC
CXXFLAGS += --sysroot=$(SYSROOT) -I$(SYSROOT)/include -fPIE -pie -fPIC -std=c++11 -I$(STL_PATH)/libs/armeabi-v7a/include -I$(STL_PATH)/include
LDFLAGS  += -L${SYSROOT}/lib -L $(STL_PATH)/libs/armeabi-v7a/ -lgnustl_static

