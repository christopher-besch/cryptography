# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  decryption_config = debug
  encryption_config = debug
  xor_config = debug
  utils_config = debug

else ifeq ($(config),release)
  decryption_config = release
  encryption_config = release
  xor_config = release
  utils_config = release

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := decryption encryption xor utils

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

decryption: xor utils
ifneq (,$(decryption_config))
	@echo "==== Building decryption ($(decryption_config)) ===="
	@${MAKE} --no-print-directory -C decryption -f Makefile config=$(decryption_config)
endif

encryption:
ifneq (,$(encryption_config))
	@echo "==== Building encryption ($(encryption_config)) ===="
	@${MAKE} --no-print-directory -C encryption -f Makefile config=$(encryption_config)
endif

xor: utils
ifneq (,$(xor_config))
	@echo "==== Building xor ($(xor_config)) ===="
	@${MAKE} --no-print-directory -C xor -f Makefile config=$(xor_config)
endif

utils:
ifneq (,$(utils_config))
	@echo "==== Building utils ($(utils_config)) ===="
	@${MAKE} --no-print-directory -C utils -f Makefile config=$(utils_config)
endif

clean:
	@${MAKE} --no-print-directory -C decryption -f Makefile clean
	@${MAKE} --no-print-directory -C encryption -f Makefile clean
	@${MAKE} --no-print-directory -C xor -f Makefile clean
	@${MAKE} --no-print-directory -C utils -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   decryption"
	@echo "   encryption"
	@echo "   xor"
	@echo "   utils"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"