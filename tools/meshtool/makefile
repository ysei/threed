TARGET := meshtool
BUILDDIR := build

include ../../macros.mk

#PROFILE := -pg

OBJDUMP := objdump
CFLAGS := -O2 -g -I. -Wall -W -Wno-unused-parameter -Wno-multichar -fno-exceptions -finline $(PROFILE)
LDFLAGS := -g $(PROFILE)
LDLIBS := -lstdc++

UNAME := $(shell uname -s)
ARCH := $(shell uname -m)

BINEXT := 

# cygwin has some special needs
ifeq ($(findstring CYGWIN,$(UNAME)),CYGWIN)
CFLAGS += -DASM_LEADING_UNDERSCORES=1
LDLIBS += -L/usr/local/lib
BINEXT := .exe
endif

# Darwin (Mac OS X) too
ifeq ($(UNAME),Darwin)
CFLAGS += -DASM_LEADING_UNDERSCORES=1 -mdynamic-no-pic -I/sw/include
LDLIBS += 
endif

# FreeBSD
ifeq ($(UNAME),FreeBSD)
CFLAGS += -I/usr/local/include
LDLIBS += -L/usr/local/lib -lpthread
endif

ifeq ($(UNAME),Linux)
ifeq ($(ARCH),ppc)
CFLAGS += -fno-pic -mregnames
CFLAGS += -mcpu=7450
endif
ifeq ($(ARCH),ppc64)
CFLAGS += -DASM_LEADING_DOTS=1
CFLAGS += -mregnames -fno-pic
CFLAGS += -mcpu=970
endif
endif

all:: $(BUILDDIR)/$(TARGET)$(BINEXT) $(BUILDDIR)/$(TARGET).lst

OBJS := \
	$(subst cpp,o,$(wildcard *.cpp))

# some lib includes
include ../../lib/math/rules.mk
include ../../lib/tinyxml/rules.mk

OBJS := $(subst ..,dotdot,$(addprefix $(BUILDDIR)/,$(OBJS)))

DEPS := $(OBJS:.o=.d)

# The main target
$(BUILDDIR)/$(TARGET)$(BINEXT): $(OBJS)
	@$(MKDIR)
	@echo linking $@
	@$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)
	cp $@ ../..

# A dissassembly of it
$(BUILDDIR)/$(TARGET).lst: $(BUILDDIR)/$(TARGET)$(BINEXT)
ifeq ($(UNAME),Darwin)
	otool -Vt $< | c++filt > $(BUILDDIR)/$(TARGET).lst
else
	$(OBJDUMP) -d $< | c++filt > $(BUILDDIR)/$(TARGET).lst
	$(OBJDUMP) -S $< | c++filt > $(BUILDDIR)/$(TARGET).g.lst
endif

clean:
	rm -f $(OBJS) $(DEPS) $(BUILDDIR)/$(TARGET)$(BINEXT) $(BUILDDIR)/$(TARGET).lst

spotless:
	rm -rf build

$(BUILDDIR)/%.o: %.c
	@$(MKDIR)
	@echo compiling $<
	@$(CC) $(CFLAGS) -c $< -MD -MT $@ -MF $(@:%o=%d) -o $@

$(BUILDDIR)/%.o: %.cpp
	@$(MKDIR)
	@echo compiling $<
	@$(CC) $(CFLAGS) -c $< -MD -MT $@ -MF $(@:%o=%d) -o $@

$(BUILDDIR)/%.o: %.S
	@$(MKDIR)
	@echo compiling $<
	@$(CC) $(CFLAGS) -c $< -MD -MT $@ -MF $(@:%o=%d) -o $@

$(BUILDDIR)/dotdot/dotdot/%.o: ../../%.c
	@$(MKDIR)
	@echo compiling $<
	@$(CC) $(CFLAGS) -c $< -MD -MT $@ -MF $(@:%o=%d) -o $@

$(BUILDDIR)/dotdot/dotdot/%.o: ../../%.cpp
	@$(MKDIR)
	@echo compiling $<
	@$(CC) $(CFLAGS) -c $< -MD -MT $@ -MF $(@:%o=%d) -o $@

$(BUILDDIR)/dotdot/dotdot/%.o: ../../%.S
	@$(MKDIR)
	@echo compiling $<
	@$(CC) $(CFLAGS) -c $< -MD -MT $@ -MF $(@:%o=%d) -o $@

# Empty rule for the .d files. The above rules will build .d files as a side
# effect. Only works on gcc 3.x and above, however.
%.d:

ifeq ($(filter $(MAKECMDGOALS), clean), )
-include $(DEPS)
endif
