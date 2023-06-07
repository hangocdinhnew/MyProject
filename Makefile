all:
	@./scripts/all

debug:
	@./scripts/debug

cmake: 
	@./scripts/cmake

ninja:
	@./scripts/ninja

install:
	@./scripts/install

clangd:
	@./scripts/clangd
	
.PHONY: all
