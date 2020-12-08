# VOS - My DIY micro-kernel

## Install

You could build VOS locally by executing the following command:

```bash
make install
```

## Development

Create a bootable ISO using vagrant:

```bash
vagrant up
```

This will create a `vos.iso` file which you could use as live ISO for a
VirtualBox VM.

## Design

VOS is an Operating System containing a custom micro-kernel. This means that
most processes of the OS itself live in user space.

The micro-kernel itself contains the scheduler and provides a way for processes
to communicate with each other using the following kernel calls:


