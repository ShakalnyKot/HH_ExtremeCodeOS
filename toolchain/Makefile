# Get from https://codeberg.org/neonOS/BuildEnv, file Toolchain/Makefile

include src/build_env/env_cross.mk

cross-compiler:
	@mkdir -v $(PWD)/toolchain/cross -p
	@bash toolchain/cross-compiler.bash $(TARGET) $(PWD)/toolchain/cross