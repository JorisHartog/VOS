GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

define GRUB_CFG
set timeout=0
set default=0

menuentry 'VOS' {
  multiboot /boot/vos.bin
  boot
}
endef
export GRUB_CFG

objects = loader.o gdt.o port.o keyboard.o interruptstubs.o interrupts.o \
					procs.o kernel.o utils.o

%.o: src/%.cpp
	g++ $(GPPPARAMS) -o $@ -c $<

%.o: src/%.s
	as $(ASPARAMS) -o $@ $<

vos.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

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
	rm -f $(objects) vos.bin vos.iso
