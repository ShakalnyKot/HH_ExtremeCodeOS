rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

include src/build_env/env.mk

HHDIR=src/hyperhal
ARCHSPEC_HHDIR=src/arch/$(ARCH)/hyperhal
OUTFILES := $(wildcard $(OUTDIR)/hh/*.o)
OUTDIR=src/output

CFILES = $(call rwildcard,,$(HHDIR)/*.c) $(call rwildcard,,$(ARCHSPEC_HHDIR)/*.c)
CPPFILES = $(call rwildcard,,$(HHDIR)/*.cpp) $(call rwildcard,,$(ARCHSPEC_HHDIR)/*.cpp)
ASMFILES = $(call rwildcard,,$(ARCHSPEC_HHDIR)/*.S)
SUMSFILES = $(CFILES) $(CPPFILES) $(ASMFILES)

OCFILES = $(patsubst %.c, %.o, $(CFILES))
OCPPFILES = $(patsubst %.cpp, %.o, $(CPPFILES))
OASMFILES = $(patsubst %.S, %.o, $(ASMFILES))
SUMOFILES = $(OCFILES) $(OCPPFILES) $(OASMFILES)

OUTFILE = $(OUTDIR)/HyperHAL.elf

all: $(SUMSFILES) $(OUTFILE)

$(OUTFILE): $(SUMOFILES)
	$(LINK_HH) $(OUTFILE) $(SUMOFILES)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
%.o: %.S
	$(ASM) $(AFLAGS) $< -o $@
%.o: %.cpp
	$(CPP) $(CPPFLAGS) $< -o $@
