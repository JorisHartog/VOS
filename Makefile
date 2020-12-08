RUSTCPARAMS = --target=thumbv7em-none-eabihf

define GRUB_CFG
set timeout=0
set default=0

menuentry 'VOS' {
  multiboot /boot/vos.bin
  boot
}
endef
export GRUB_CFG

vos.bin: src/main.rs
	rustc $(RUSTCPARAMS) $< -o $@

install: vos.bin
	sudo cp $< /boot/vos.bin

vos.iso: vos.bin
	mkdir -p iso/boot/grub
	cp $< iso/boot
	echo "$$GRUB_CFG" > iso/boot/grub/grub.cfg
	grub2-mkrescue --output=$@ iso
	rm -rf iso

.PHONY: clean
clean:
	rm -f vos.bin vos.iso
