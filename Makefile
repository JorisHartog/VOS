deps:
	rustup component add rust-src
	rustup component add llvm-tools-preview
	cargo install bootimage

run:
	cargo run

.PHONY: clean
clean:
	rm -rf target
