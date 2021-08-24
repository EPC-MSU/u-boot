/*
 * File:        fdt.c
 * Description: FDT fixup methods
 * Created:     10 Nov 2016
 * Author:      Sergey Semin <sergey.semin@t-platforms.ru>
 * Copyright 2016 T-platforms
 *
 * U-boot works in a 'program' memory of MIPS architecture. In general it means,
 * there are five memory segments KUSEG, KSEG0 - KSEG3. These segments reflects
 * real physical memory space as: KUSEG - is accessible over MMU only by
 * unpriviliged mode, KSEG0 - mapped to first 512Mb of physical memory in a way
 * so can not be mapped by MMU, but can be cached, KSEG1 - the same as KSEG0
 * but can't be cached, KSEG2 and KSEG3 - each maps 512Mb accessible over MMU
 * in kernel mode only.
 * U-boot uses these segments transparently, so it thinks it works in physical
 * memory, which just starts from KSEG0 (0x80000000). One can notice, that
 * U-boot mostly operates  with memory within KSEG0/KSEG1 (0x80000000 -
 * 0xBFFFFFF). But Linux MIPS CSP is designed the way so KSEG0 and KSEG1 are
 * considered as virtually mapped contigious physical memory of kernel space.
 * In fact Linux approach is only correct, since it reflects the real physical
 * and virtual memory maps. Due to this inconsistency some addresses passed from
 * u-boot to Linux and vice-versa are inapproprietly considered. Particularly
 * it concerns fdt  related addresses like /memreserve/ and /linux,initrd-/
 * nodes, which must have physical memory according to standard.
 * Since It's too painful to make any design refactors to harmonize both
 * approachs, we need to use libfdt fixups to alter passed fdt.
 *
 * WARNING #1! There is method boot_fdt_add_mem_rsv_regions(), which is called
 * twice in the code:
 * 1) just before arch-specific bootm code is executed,
 * 2) in the arch specific code if image_setup_linux() method is called.
 * This method uses data from fdt /memreserve/ node to reserve memory in lmb.
 * Alas it is called before any libfdt initialization, which makes it impossible
 * to fixup
 *
 * WARNING #2! Alas U-boot doesn't analize /reserved-memory/ of passed fdt.
 * In addition it analizes /memreserve/ too late, so initrd and fdt are
 * relocated before memory declared in fdt is reserved. So get ready to see
 * memory overlap hidden errors, which may cause sudden Linux crashs =)
 */
#include <common.h>
#include <fdt_support.h>
#include <errno.h>
#include <libfdt.h>
#include <asm/io.h>
#include <board.h>

/*
 * Reduce maximum high memory size with one byte so the memory limits wouldn't
 * overflow.
 */
#define HIGHMEM_MAX_SIZE32 0xDFFF0000U
#define HIGHMEM_BASE_ADDR  0x20000000U
#define LOWMEM_BASE_SIZE   0x08000000U

/*
 * Convert the passed property to be of physical address
 */
static int fdt_convert_prop(void *data, int len)
{
	int ret = 0;

	/* Copy the value from the data, convert and get it back */
	if (len == 8) {
		uint64_t val = fdt64_to_cpu(*(uint64_t *)data);
		val = cpu_to_fdt64(CPHYSADDR(val));
		memmove(data, &val, len);
	} else if (len == 4) {
		uint32_t val = fdt32_to_cpu(*(uint32_t *)data);
		val = cpu_to_fdt32(CPHYSADDR(val));
		memmove(data, &val, len);
	} else {
		printf("Unsupported linut,initrd-* property len: %d", len);
		ret = -EINVAL;
	}

	return ret;
}

/*
 * Get value of an element from array with passed size flag
 */
static int fdt_getprop_uxx(void *fdt, int nodeoffset, const char *name,
			   int off, uint64_t *val, int is_u64)
{
	void *data;
	int len;

	/* Get the property data pointer */
	data = fdt_getprop_w(fdt, nodeoffset, name, &len);
	if (!data)
		return len;

	/* Sanity check the requested offset */
	if (off + (is_u64 ? 8 : 4) > len)
		return -EINVAL;

	/* Seek to the requested position and get a data placed there */
	data += off;
	if (is_u64)
		*val = fdt64_to_cpu(*(uint64_t *)data);
	else
		*val = fdt32_to_cpu(*(uint32_t *)data);

	return 0;
}

/*
 * Set the passed property in accordance with the passed size flag
 */
static int fdt_setprop_uxx(void *fdt, int nodeoffset, const char *name,
			   uint64_t val, int is_u64)
{
	if (is_u64)
		return fdt_setprop_u64(fdt, nodeoffset, name, val);
	else
		return fdt_setprop_u32(fdt, nodeoffset, name, (uint32_t)val);
}

/*
 * Append the passed property in accordance with the passed size flag
 */
static int fdt_appendprop_uxx(void *fdt, int nodeoffset, const char *name,
			      uint64_t val, int is_u64)
{
	if (is_u64)
		return fdt_appendprop_u64(fdt, nodeoffset, name, val);
	else
		return fdt_appendprop_u32(fdt, nodeoffset, name, (uint32_t)val);
}

/*
 * Translate the /memreserve/ nodes to be within KSEG0 and initialize
 * the memory node by data retrieved from the SPD blob.
 */
int arch_fixup_fdt(void *fdt_blob)
{
	int n, total, ret, offset, ac, sc;
	uint64_t addr, size, lowmem;
	bool ac_64, sc_64;

	/*
	 * Walk through all the memreserve nodes deleting a first one and
	 * adding a translated one to tail
	 */
	total = fdt_num_mem_rsv(fdt_blob);
	for (n = 0; n < total; n++) {
		/* FDT header check may fail */
		ret = fdt_get_mem_rsv(fdt_blob, 0, &addr, &size);
		if (ret != 0) {
			puts("Failed to get memreserve from fdt");
			return ret;
		}
		/* Do nothing if the address is already virtual */
		if (KSEGX(addr) != 0)
			continue;
		/* Delete the very first memreserve node */
		ret = fdt_del_mem_rsv(fdt_blob, 0);
		if (ret != 0) {
			puts("Failed to clear first memreserve node");
			return ret;
		}
		/* Add translated /memreserve/ node at the tail */
		addr = (uintptr_t)phys_to_virt(addr);
		ret = fdt_add_mem_rsv(fdt_blob, addr, size);
		if (ret != 0) {
			puts("Failed to get memreserve back");
			return ret;
		}
	}

	/*
	 * Find memory node and try to reinitialize it reg property with
	 * default lowmem range and with highmem range read from SPD
	 */
	offset = fdt_path_offset(fdt_blob, "/");
	if (offset < 0) {
		puts("Couldn't find root fdt node\n");
		return offset;
	}
	ac = fdt_address_cells(fdt_blob, offset);
	sc = fdt_size_cells(fdt_blob, offset);
	if (ac < 0 || sc < 0) {
		puts("Couldn't get address or size cells\n");
		return -EINVAL;
	}
	ac_64 = (ac == 2);
	sc_64 = (sc == 2);
	offset = fdt_path_offset(fdt_blob, "/memory");
	if (offset < 0) {
		puts("Couldn't find memory fdt node\n");
		return offset;
	}
	/*
	 * Try to read the low memory size property and reuse it then for compatibility
	 * with older kernels. If the property doesn't exist, the default constant value
	 * is utilized
	 */
	lowmem = LOWMEM_BASE_SIZE;
	(void)fdt_getprop_uxx(fdt_blob, offset, "reg", (ac_64 ? 8 : 4), &lowmem, sc_64);
	/* First setup the lowmem base address and default size */
	ret = fdt_setprop_uxx(fdt_blob, offset, "reg", 0, ac_64);
	if (!ret)
		ret = fdt_appendprop_uxx(fdt_blob, offset, "reg", lowmem, sc_64);
	if (ret < 0) {
		puts("Couldn't set lowmem fdt property\n");
		return ret;
	}
	/* Then retrieve the highmem size and push its range to the fdt blob */
	ret = fdt_appendprop_uxx(fdt_blob, offset, "reg", HIGHMEM_BASE_ADDR, ac_64);
	if (!ret) {
		size = get_ddr_highmem_size();
		/* Manually clamp the size if there is no room for 64-bit value */
		size = (!sc_64 && size > HIGHMEM_MAX_SIZE32) ? HIGHMEM_MAX_SIZE32 : size;
		ret = fdt_appendprop_uxx(fdt_blob, offset, "reg", size, sc_64);
	}
	if (ret < 0) {
		puts("Couldn't set highmem fdt property\n");
		return ret;
	}

	return 0;
}

/*
 * Translate the /memreserve/ nodes back to have physical addresses
 */
int ft_verify_fdt(void *fdt_blob)
{
	uint64_t addr, size;
	int n, total, ret, len;
	int chosen_node;
	void *data;

	/*
	 * Walk through all the memreserve nodes deleting a first one and
	 * adding a translated one to tail
	 */
	total = fdt_num_mem_rsv(fdt_blob);
	for (n = 0; n < total; n++) {
		/* FDT header check may fail */
		ret = fdt_get_mem_rsv(fdt_blob, 0, &addr, &size);
		if (ret != 0) {
			puts("Failed to get memreserve from fdt");
			return false;
		}
		/* Do nothing if the address is already physical */
		if (KSEGX(addr) == 0)
			continue;
		/* Delete the very first memreserve node */
		ret = fdt_del_mem_rsv(fdt_blob, 0);
		if (ret != 0) {
			puts("Failed to clear first memreserve node");
			return false;
		}
		/* Add translated /memreserve/ node at the tail */
		addr = virt_to_phys((void *)(uintptr_t)addr);
		ret = fdt_add_mem_rsv(fdt_blob, addr, size);
		if (ret != 0) {
			puts("Failed to get memreserve back");
			return false;
		}
	}

	/*
	 * Replace virtual address set to linux,initrd-start and
	 * linux,initrd-end with proper physical addresses
	 */
        chosen_node = fdt_subnode_offset(fdt_blob, 0, "chosen");
        if (chosen_node < 0) {
		puts("Failed to get chosen node to alter initrd address");
		return false;
	}

	/* Retrieve the linux,initrd-start property */
	data = fdt_getprop_w(fdt_blob, chosen_node, "linux,initrd-start", &len);
	if (!data) {
		puts("Failed to get linux,initrd-start property");
		return false;
	}
	/* Convert the retrieved property */
	ret = fdt_convert_prop(data, len);
	if (ret < 0)
		return false;

	/* Retrieve the linux,initrd-end property */
	data = fdt_getprop_w(fdt_blob, chosen_node, "linux,initrd-end", &len);
	if (!data) {
		puts("Failed to get linux,initrd-end property");
		return false;
	}
	/* Convert the retrieved property */
	ret = fdt_convert_prop(data, len);
	if (ret < 0)
		return false;

	return true;
}

/* End of file fdt.c */
