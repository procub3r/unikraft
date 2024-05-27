#!/usr/bin/sh

ARCH=X64
UK_IMG=build/unikraft_qemu-x86_64
OUTPUT_IMG=${UK_IMG}.img
OVMF_CODE=/usr/share/edk2/x64/OVMF_CODE.fd
OVMF_VARS=/usr/share/edk2/x64/OVMF_VARS.fd

# build the kernel
make

# build the output image
./support/scripts/mkukimg -k ${UK_IMG} -f disk -b ukefi -a ${ARCH} -o ${OUTPUT_IMG}

# run the image in qemu
sudo qemu-system-x86_64 \
	-drive file=${OUTPUT_IMG},if=none,id=drive0,format=raw \
	-device virtio-blk,drive=drive0 \
	-drive if=pflash,format=raw,readonly=on,file=${OVMF_CODE} \
	-drive if=pflash,format=raw,file=${OVMF_VARS} \
	-m 8G -enable-kvm -cpu host -smp 4 --no-shutdown
