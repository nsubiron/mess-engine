CONFIG = ./build/configure.pyz -f configure.yaml $(FLAGS)

default: build

.PHONY: configure embed sublime codeblocks doxygen

configure:
	@$(CONFIG) --ninja

build: configure
	@ninja

clean:
	@ninja -t clean

embed:
	@$(CONFIG) --embed

sublime:
	@$(CONFIG) --sublime

codeblocks:
	@$(CONFIG) --codeblocks

doxygen:
	@$(CONFIG) --doxyfile
	@doxygen build/Doxyfile

release: configure
	@ninja release

debug: configure
	@ninja debug

all: configure
	@ninja all
