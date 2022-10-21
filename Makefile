include src/build_env/env.mk

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
HHDIR=src/hyperhal
ARCHSPEC_HHDIR=src/arch/$(ARCH)/hyperhal
OUTDIR=src/output
OUTFILES := $(wildcard $(OUTDIR)/hh/*.o)
MKFILES := $(wildcard src/*.mk)
MSCRIPTS := $(wildcard scripts/*.mk)
QEMU_SERIAL_OUT = file:serial.log

include $(MSCRIPTS)
include src/ver.mk

OUTISOIMG="HyperHAL-master_v$(GVER).$(LVER).$(PATCH).$(BUILD)_$(ARCH).iso"

isoimg: all;
	@mkdir -p compiled/iso;
	@cp -r data/* compiled/iso;
	@cp src/output/HyperHAL.elf compiled/iso/HyperHAL/HyperHAL.elf;
	@cp src/output/hh_app.bin compiled/iso/HyperHAL/hh_app.bin;
	@grub-mkrescue -o compiled/isoimage.iso compiled/iso;
	@cp compiled/isoimage.iso $(OUTISOIMG);

all: binaries;

binaries: dir VerHeader Compile
	@$(ASM) -f bin src/examples/hh_app.S -o src/output/hh_app.bin
#	@$(LINK_NK) $(OUTDIR)/nK.elf $(OUTDIR)/nk/*.o;

Compile:
	for mkfile in $(MKFILES); do \
		make -f $$mkfile all; \
	done

VerHeader:
	@$(PY) toolchain/genverh.py
	#@printf "Here should be version header generation, but i disabled it\n"

dir:
	@mkdir -p $(OUTDIR)/;

clean:
	@rm -rf $(call rwildcard,,*.o)
	@rm -rf $(call rwildcard,,*.elf)
	@rm -rf compiled/iso;
	@rm compiled/isoimage.iso;
	@rm *.log
.PHONY: all binaries dir Compile clean isoimg mkdockimg rundockimg runiso

%.py:
	@$(PY) "scripts/$@"
