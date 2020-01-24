# Adapted from the old build in https://github.com/ucb-ee192/SkeletonBeagle/blob/master/rc_balance/Makefile
# but to use the 
# NOTE - this makefile may differ from how Eclipse does its build!
# This also assumes compilation on the target, with librobotcontrol pre-installed.

# What the heck is this syntax? Why are spaces and tabs distinct?
# SCons, CMake, and the line would be a more elegant tool for a more civilized age.
# But alas, make and its perculiar language lives on by the power of inertia, despite its extreme user unfriendliness.

# compiler and linker binaries
CC		:= gcc
CXX		:= g++
LINKER		:= g++

# compiler and linker flags
WFLAGS		:= -Wall -Wextra -Werror=float-equal -Wuninitialized -Wunused-variable -Wdouble-promotion
CFLAGS		:= -g -c -Wall
LDFLAGS		:= -pthread -lm -lrt -l:librobotcontrol.so

INCLUDES	:= $(wildcard src/*.h)
SRC_DIR   := src/
BUILD_DIR := build/
TARGET_NAME = main
TARGET = $(BUILD_DIR)/$(TARGET_NAME)
C_SOURCES		:= $(wildcard $(SRC_DIR)/*.c)
CXX_SOURCES		:= $(wildcard $(SRC_DIR)/*.cpp)
C_OBJECTS		:= $(subst $(SRC_DIR)/, $(BUILD_DIR)/, $(C_SOURCES:$%.c=$%.o))
CXX_OBJECTS		:= $(subst $(SRC_DIR)/, $(BUILD_DIR)/, $(CXX_SOURCES:$%.cpp=$%.o))

prefix		:= /usr/local
RM		:= rm -f
INSTALL		:= install -m 4755
INSTALLDIR	:= install -d -m 755

SYMLINK		:= ln -s -f
SYMLINKDIR	:= /etc/robotcontrol
SYMLINKNAME	:= link_to_startup_program


# linking Objects
$(TARGET): $(C_OBJECTS) $(CXX_OBJECTS)
	@$(LINKER) -o $@ $(C_OBJECTS) $(CXX_OBJECTS) $(LDFLAGS)
	@echo "Made: $@"


# compiling command
$(C_OBJECTS): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.c $(INCLUDES)
	mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $(WFLAGS) $(DEBUGFLAG) $< -o $@
	@echo "Compiled: $@"

$(CXX_OBJECTS): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp $(INCLUDES)
	mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $(WFLAGS) $(DEBUGFLAG) $< -o $@
	@echo "Compiled: $@"

all:	$(TARGET)

debug:
	$(MAKE) $(MAKEFILE) DEBUGFLAG="-g -D DEBUG"
	@echo " "
	@echo "$(TARGET) Make Debug Complete"
	@echo " "

install:
	@$(MAKE) --no-print-directory
	@$(INSTALLDIR) $(DESTDIR)$(prefix)/bin
	@$(INSTALL) $(TARGET) $(DESTDIR)$(prefix)/bin
	@echo "$(TARGET) Install Complete"

clean:
	@$(RM) $(OBJECTS)
	@$(RM) $(TARGET)
	@echo "$(TARGET) Clean Complete"

uninstall:
	@$(RM) $(DESTDIR)$(prefix)/bin/$(TARGET)
	@echo "$(TARGET) Uninstall Complete"

runonboot:
	@$(MAKE) install --no-print-directory
	@$(SYMLINK) $(DESTDIR)$(prefix)/bin/$(TARGET_NAME) $(SYMLINKDIR)/$(SYMLINKNAME)
	@echo "$(TARGET) Set to Run on Boot"
