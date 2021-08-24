/*
 * (C) Copyright 2011 - 2012 Samsung Electronics
 * EXT4 filesystem implementation in Uboot by
 * Uma Shankar <uma.shankar@samsung.com>
 * Manjunatha C Achar <a.manjunatha@samsung.com>
 *
 * Data structures and headers for ext4 support have been taken from
 * ext2 ls load support in Uboot
 *
 * (C) Copyright 2004
 * esd gmbh <www.esd-electronics.com>
 * Reinhard Arlt <reinhard.arlt@esd-electronics.com>
 *
 * based on code from grub2 fs/ext2.c and fs/fshelp.c by
 * GRUB  --  GRand Unified Bootloader
 * Copyright (C) 2003, 2004  Free Software Foundation, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __EXT_COMMON__
#define __EXT_COMMON__
#include <command.h>
#define SECTOR_SIZE		0x200

/* Magic value used to identify an ext2 filesystem.  */
#define	EXT2_MAGIC			0xEF53
/* Amount of indirect blocks in an inode.  */
#define INDIRECT_BLOCKS			12
/* Maximum lenght of a pathname.  */
#define EXT2_PATH_MAX				4096
/* Maximum nesting of symlinks, used to prevent a loop.  */
#define	EXT2_MAX_SYMLINKCNT		8

/* Filetype used in directory entry.  */
#define	FILETYPE_UNKNOWN		0
#define	FILETYPE_REG			1
#define	FILETYPE_DIRECTORY		2
#define	FILETYPE_SYMLINK		7

/* Filetype information as used in inodes.  */
#define FILETYPE_INO_MASK		0170000
#define FILETYPE_INO_REG		0100000
#define FILETYPE_INO_DIRECTORY		0040000
#define FILETYPE_INO_SYMLINK		0120000
#define EXT2_ROOT_INO			2 /* Root inode */

/* The size of an ext2 block in bytes.  */
#define EXT2_BLOCK_SIZE(data)	   (1 << LOG2_BLOCK_SIZE(data))

/* Log2 size of ext2 block in bytes.  */
#define LOG2_BLOCK_SIZE(data)	   (__le32_to_cpu		   \
				    (data->sblock.log2_block_size) \
				    + EXT2_MIN_BLOCK_LOG_SIZE)
#define INODE_SIZE_FILESYSTEM(data)	(__le32_to_cpu \
			(data->sblock.inode_size))

#define EXT2_FT_DIR	2
#define SUCCESS	1

/* Macro-instructions used to manage several block sizes  */
#define EXT2_MIN_BLOCK_LOG_SIZE	10 /* 1024 */
#define EXT2_MAX_BLOCK_LOG_SIZE	16 /* 65536 */
#define EXT2_MIN_BLOCK_SIZE		(1 << EXT2_MIN_BLOCK_LOG_SIZE)
#define EXT2_MAX_BLOCK_SIZE		(1 << EXT2_MAX_BLOCK_LOG_SIZE)

/* The ext2 superblock.  */
struct ext2_sblock {
	uint32_t total_inodes;
	uint32_t total_blocks;
	uint32_t reserved_blocks;
	uint32_t free_blocks;
	uint32_t free_inodes;
	uint32_t first_data_block;
	uint32_t log2_block_size;
	uint32_t log2_fragment_size;
	uint32_t blocks_per_group;
	uint32_t fragments_per_group;
	uint32_t inodes_per_group;
	uint32_t mtime;
	uint32_t utime;
	uint16_t mnt_count;
	uint16_t max_mnt_count;
	uint16_t magic;
	uint16_t fs_state;
	uint16_t error_handling;
	uint16_t minor_revision_level;
	uint32_t lastcheck;
	uint32_t checkinterval;
	uint32_t creator_os;
	uint32_t revision_level;
	uint16_t uid_reserved;
	uint16_t gid_reserved;
	uint32_t first_inode;
	uint16_t inode_size;
	uint16_t block_group_number;
	uint32_t feature_compatibility;
	uint32_t feature_incompat;
	uint32_t feature_ro_compat;
	uint32_t unique_id[4];
	char volume_name[16];
	char last_mounted_on[64];
	uint32_t compression_info;
	uint8_t prealloc_blocks;
	uint8_t prealloc_dir_blocks;
	__le16 reserved_gdt_blocks;
	uint8_t journal_uuid[16];
	__le32 journal_inode;
	__le32 journal_dev;
	__le32 last_orphan;
	__le32 hash_seed[4];
	uint8_t default_hash_version;
	uint8_t journal_backup_type;
	__le16 descriptor_size;
	__le32 default_mount_options;
	__le32 first_meta_block_group;
	__le32 mkfs_time;
	__le32 journal_blocks[17];
	__le32 total_blocks_high;
	__le32 reserved_blocks_high;
	__le32 free_blocks_high;
	__le16 min_extra_inode_size;
	__le16 want_extra_inode_size;
	__le32 flags;
	__le16 raid_stride;
	__le16 mmp_interval;
	__le64 mmp_block;
	__le32 raid_stripe_width;
	uint8_t log2_groups_per_flex;
	uint8_t checksum_type;
};

struct ext2_block_group {
	__u32 block_id;	/* Blocks bitmap block */
	__u32 inode_id;	/* Inodes bitmap block */
	__u32 inode_table_id;	/* Inodes table block */
	__u16 free_blocks;	/* Free blocks count */
	__u16 free_inodes;	/* Free inodes count */
	__u16 used_dir_cnt;	/* Directories count */
	__u16 bg_flags;
	__le32 bg_exclude_bitmap;
	__le16 bg_block_id_csum;
	__le16 bg_inode_id_csum;
	__u16 bg_itable_unused; /* Unused inodes count */
	__le16 bg_checksum;	/* crc16(s_uuid+group_num+group_desc)*/
	/* following fields only exist if descriptor size is 64 */
	__le32 block_id_high;
	__le32 inode_id_high;
	__le32 inode_table_id_high;
	__le16 free_blocks_high;
	__le16 free_inodes_high;
	__le16 used_dir_cnt_high;
	__le16 bg_itable_unused_high;
	__le32 bg_exclude_bitmap_high;
	__le16 bg_block_id_csum_high;
	__le16 bg_inode_id_csum_high;
	__le32 bg_reserved;
};

/* The ext2 inode. */
struct ext2_inode {
	uint16_t mode;
	uint16_t uid;
	uint32_t size;
	uint32_t atime;
	uint32_t ctime;
	uint32_t mtime;
	uint32_t dtime;
	uint16_t gid;
	uint16_t nlinks;
	uint32_t blockcnt;	/* Blocks of either 512 or block_size bytes */
	uint32_t flags;
	uint32_t osd1;
	union {
		struct datablocks {
			uint32_t dir_blocks[INDIRECT_BLOCKS];
			uint32_t indir_block;
			uint32_t double_indir_block;
			uint32_t triple_indir_block;
		} blocks;
		char symlink[60];
		char inline_data[60];
	} b;
	uint32_t version;
	uint32_t acl;
	__le32 size_high;	/* previously dir_acl, but never used */
	uint32_t fragment_addr;
	uint32_t osd2[3];
};

/* The header of an ext2 directory entry. */
struct ext2_dirent {
	uint32_t inode;
	uint16_t direntlen;
	uint8_t namelen;
	uint8_t filetype;
};

struct ext2fs_node {
	struct ext2_data *data;
	struct ext2_inode inode;
	int ino;
	int inode_read;
};

/* Information about a "mounted" ext2 filesystem. */
struct ext2_data {
	struct ext2_sblock sblock;
	struct ext2_inode *inode;
	struct ext2fs_node diropen;
};

extern lbaint_t part_offset;

int do_ext2ls(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ext2load(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ext4_load(cmd_tbl_t *cmdtp, int flag, int argc,
					char *const argv[]);
int do_ext4_ls(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[]);
int do_ext4_write(cmd_tbl_t *cmdtp, int flag, int argc,
				char *const argv[]);
#endif
