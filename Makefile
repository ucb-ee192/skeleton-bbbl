# Adapted from the old build in https://github.com/ucb-ee192/SkeletonBeagle/blob/master/rc_balance/Makefile
# but to use the 
# NOTE - this makefile may differ from how Eclipse does its build!
# This also assumes compilation on the target, with librobotcontrol pre-installed.

TARGET = main

# compiler and linker binaries
CC		:= gcc
CXX		:= g++
LINKER		:= g++

# compiler and linker flags
WFLAGS		:= -Wall -Wextra -Werror=float-equal -Wuninitialized -Wunused-variable -Wdouble-promotion
CFLAGS		:= -g -c -Wall
LDFLAGS		:= -pthread -lm -lrt -l:librobotcontrol.so

INCLUDES	:= $(wildcard src/*.h)
C_SOURCES		:= $(wildcard src/*.c)
CXX_SOURCES		:= $(wildcard src/*.cpp)
C_OBJECTS		:= $(C_SOURCES:$%.c=$%.o)
CXX_OBJECTS		:= $(CXX_SOURCES:$%.cpp=$%.o)

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
$(C_OBJECTS): %.o : %.c $(INCLUDES)
	@$(CC) $(CFLAGS) $(WFLAGS) $(DEBUGFLAG) $< -o $@
	@echo "Compiled: $@"

$(CXX_OBJECTS): %.o : %.cpp $(INCLUDES)
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
	@$(SYMLINK) $(DESTDIR)$(prefix)/bin/$(TARGET) $(SYMLINKDIR)/$(SYMLINKNAME)
	@echo "$(TARGET) Set to Run on Boot"
