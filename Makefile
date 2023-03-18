all:
	@./scripts/all

cmake: 
	@./scripts/cmake

ninja:
	@./scripts/ninja

install:
	@./scripts/install

clangd:
	@./scripts/clangd
	
.PHONY: all
