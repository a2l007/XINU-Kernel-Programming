#include <xinu.h>
#include <kernel.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <fs.h>


#if FS
#include <fs.h>

static struct fsystem fsd;
struct dirent dr;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2

#define NUM_FD 16
struct filetable oft[NUM_FD];
int next_open_fd = 0;
int next_inode=0;

#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);

/* YOUR CODE GOES HERE */

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock) {
  int diskblock;

  if (fileblock >= INODEBLOCKS - 2) {
    printf("No indirect block support\n");
    return SYSERR;
  }

  diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address

  return diskblock;
}


int fs_create(char *filename,int mode){
 int n,i,j,fd;
 for(n=0;n<next_open_fd;n++){
	if(strcmp(oft[n].de->name,filename)==0){
		printf("\nFile already exists");
		return SYSERR;
	}
 }
 if(next_open_fd==DIRECTORY_SIZE -1){
	printf("\nMaximum file limit reached!");
	return SYSERR;
 }
 oft[next_open_fd].de=getmem(sizeof(dr));
 strcpy(oft[next_open_fd].de->name,filename);
 oft[next_open_fd].de->inode_num=next_inode;
 oft[next_open_fd].in.id=next_inode;
 oft[next_open_fd].state=mode;
 oft[next_open_fd].in.type=INODE_TYPE_FILE;
 oft[next_open_fd].in.device=0;
 oft[next_open_fd].in.size=64;
 if((next_inode+1)%INODES_PER_BLOCK==0)
	fs_setmaskbit(next_inode/INODES_PER_BLOCK);
 next_inode++;
 fd=next_open_fd;
 next_open_fd++;
 fs_put_inode_by_num(0,oft[fd].in.id,&oft[fd].in);
 oft[fd].fileptr=0;
 return fd;
}

/* Opens a file in Read,Write or Read/Write mode */
int fs_open(char *filename, int flags){
 int i;
 for(i=0;i<next_open_fd;i++){
 	if(strcmp(oft[i].de->name,filename)==0){
		oft[next_open_fd].state=FSTATE_OPEN;
 		oft[i].fflag=flags;
		return i;
	}
 }
 printf("\n File not found");
 return SYSERR;
}

/* Writes nbytes of data into a file */
int fs_write(int fd, void *buf, int nbytes){
 int i=0,blockno,offset=0,j,k=0,l=0,space;
 if(oft[fd].fflag==O_RDONLY){
	printf("\nThis is a read only file");
	return SYSERR;
 }
 int num_blocks=(nbytes/MDEV_BLOCK_SIZE)+1;
// Fetch the required number of disk blocks
 while(i<num_blocks){
	for(j=18;j<512;j++){
		 if(fs_getmaskbit(j)==0){
      			printf("\nData block %d is free",j);
			oft[fd].in.blocks[i]=j;
			fs_setmaskbit(j);
			break;
  		}
	}
	i++;
}
 fs_put_inode_by_num(0,oft[fd].in.id,&oft[fd].in);
 int i_num=oft[fd].de->inode_num;
 struct inode *in=getmem(sizeof(struct inode));
 int start_block=oft[fd].fileptr/MDEV_BLOCK_SIZE;
 //printf("\n Start block %d",start_block);
 int start_block_addr=start_block*MDEV_BLOCK_SIZE;
 //printf("\n Start block address %d",start_block_addr);
 //printf("\n Bytes to read %d",nbytes);
 k=start_block;
 fs_get_inode_by_num(0,i_num,in);
 //printf("Inode number is %d",in->id);
 int nb=nbytes;
 offset=oft[fd].fileptr-start_block_addr;
 while(nbytes > 0){
        blockno=in->blocks[k];
        if(l==0){  //First block to be read
                if(MDEV_BLOCK_SIZE-(oft[fd].fileptr%MDEV_BLOCK_SIZE) < nbytes){
                        space=MDEV_BLOCK_SIZE-(oft[fd].fileptr%MDEV_BLOCK_SIZE);
                        strncpy(block_cache,buf,space);
                        nbytes-=space;
			bs_bwrite(dev0, blockno, offset,block_cache, space);
			offset+=space;
                }
                else{
                        strncpy(block_cache,buf,nbytes);
                	bs_bwrite(dev0, blockno, offset,block_cache, nbytes);
			offset+=nbytes;
                        nbytes=0;
}
        }
        else{
                if(nbytes>MDEV_BLOCK_SIZE){
                        strncpy(block_cache,buf+offset,MDEV_BLOCK_SIZE);
                        nbytes-=MDEV_BLOCK_SIZE;
			offset+=MDEV_BLOCK_SIZE;
			bs_bwrite(dev0, blockno, 0,block_cache, MDEV_BLOCK_SIZE);
                }
                else{
		        strncpy(block_cache,buf+offset,nbytes);
			bs_bwrite(dev0, blockno, 0,block_cache, nbytes);
			offset+=nbytes;
                        nbytes=0;


                }
        }
	k++;l++;
}
 oft[fd].fileptr+=nb-1;
 freemem(in,sizeof(in));
 return nbytes;
}

// File read method
int fs_read(int fd, void *buf, int nbytes){
 int i=0,offset=0,blockno,space,j=0;
 int bytes_to_read=nbytes;
 if(oft[fd].fflag==O_WRONLY){
        printf("\nThis is a write only file");
        return SYSERR;
 }

 //printf("\n Bytes to read %d",bytes_to_read);
 int num_blocks=(bytes_to_read/MDEV_BLOCK_SIZE)+1;
 //printf("\n Num blocks %d",num_blocks);
 int start_block=oft[fd].fileptr/MDEV_BLOCK_SIZE;
 i=start_block;
 //printf("\n Start block %d",start_block);
 int start_block_addr=start_block*MDEV_BLOCK_SIZE;
 //printf("\n Start block address %d",start_block_addr);
 int i_num=oft[fd].de->inode_num;
 //printf("\n\n FILEPTR IS %d",oft[fd].fileptr);
 struct inode *in=getmem(sizeof(struct inode));
 fs_get_inode_by_num(0,i_num,in);
 int nb=nbytes;
 //printf("\n~~File pointer is %d",oft[fd].fileptr);
 offset=oft[fd].fileptr-start_block_addr;
 while(nbytes > 0){
        blockno=in->blocks[i];
        printf("Block number is %d",blockno);
	if(j==0){  //First block to be read
		if(MDEV_BLOCK_SIZE-(oft[fd].fileptr%MDEV_BLOCK_SIZE) < nbytes){
			space=MDEV_BLOCK_SIZE-(oft[fd].fileptr%MDEV_BLOCK_SIZE);
			bs_bread(dev0, blockno, offset,block_cache, space);
			strcpy(buf,block_cache); //sceeen
			nbytes-=space;
			offset+=space;
		}
		else{
			bs_bread(dev0, blockno, offset,block_cache, nbytes);
			strcpy(buf,block_cache);
			offset+=nbytes;
			nbytes=0;
		}
		j++;
	}
	else{
		if(nbytes>MDEV_BLOCK_SIZE){
			bs_bread(dev0, blockno, 0,block_cache, MDEV_BLOCK_SIZE);
			strncat(buf,block_cache,MDEV_BLOCK_SIZE);
			nbytes-=MDEV_BLOCK_SIZE;
			offset+=MDEV_BLOCK_SIZE;
		}
		else{
			bs_bread(dev0, blockno, 0,block_cache, nbytes);
			strncat(buf,block_cache,nbytes);
			nbytes=0;
		}
		j++;
	}
	i++;
}
//bs_bread(dev0, blockno, 0,buf, nbytes);
//bs_bread(dev0, blockno, 0,block_cache, nbytes-1);
 oft[fd].fileptr+=nb-1;
 return nbytes;
}
/* Seek to a specific position based on offset */
int fs_seek(int fd, int offset){
 if(oft[fd].fileptr+offset+1 <0){
	printf("\nInvalid offset");
	return SYSERR;
 }
 else{
	 oft[fd].fileptr+=offset+1;
 	printf("\n Seek successful. File PTR IS NOW %d",oft[fd].fileptr);
 	return OK;
 }
}

/*Close the file */
int fs_close(int fd){
 if(fd>next_open_fd||oft[fd].state==FSTATE_CLOSED)
	return SYSERR;
 else{
	oft[fd].state=FSTATE_CLOSED;
	return OK;
 }
}

/* Update superblock with latest file allocation details*/
int fs_mount(int dev){
 int i;
 struct fsystem mountfsd;
 bs_bread(dev0, SB_BLK, 0, &mountfsd, sizeof(struct fsystem));
 mountfsd.root_dir.numentries=next_open_fd;
 for(i=0;i<next_open_fd;i++){
	 mountfsd.root_dir.entry[i].inode_num=oft[i].de->inode_num;
	 strcpy(mountfsd.root_dir.entry[i].name,oft[i].de->name);
 }
 bs_bwrite(dev0, SB_BLK, 0, &mountfsd, sizeof(struct fsystem));
}
/* read in an inode and fill in the pointer */
int
fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;
  int inode_off;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_get_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  inode_off = inn * sizeof(struct inode);

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  printf("inn*sizeof(struct inode): %d\n", inode_off);
  */

  bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
  memcpy(in, &block_cache[inode_off], sizeof(struct inode));

  return OK;

}

int
fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_put_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  */

  bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
  memcpy(&block_cache[(inn*sizeof(struct inode))], in, sizeof(struct inode));
  bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

  return OK;
}
     
int fs_mkfs(int dev, int num_inodes) {
  int i;

  if (dev == 0) {
    fsd.nblocks = dev0_numblocks;
    fsd.blocksz = dev0_blocksize;
  }
  else {
    printf("Unsupported device\n");
    return SYSERR;
  }

  if (num_inodes < 1) {
    fsd.ninodes = DEFAULT_NUM_INODES;
  }
  else {
    fsd.ninodes = num_inodes;
  }

  i = fsd.nblocks;
  while ( (i % 8) != 0) {i++;}
  fsd.freemaskbytes = i / 8; 
  
  if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
    printf("fs_mkfs memget failed.\n");
    return SYSERR;
  }
  
  /* zero the free mask */
  for(i=0;i<fsd.freemaskbytes;i++) {
    fsd.freemask[i] = '\0';
  }
  
  fsd.inodes_used = 0;
  
  /* write the fsystem block to SB_BLK, mark block used */
  fs_setmaskbit(SB_BLK);
  bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
  
  /* write the free block bitmask in BM_BLK, mark block used */
  fs_setmaskbit(BM_BLK);
  bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

  return 1;
}

void
fs_print_fsd(void) {

  printf("fsd.ninodes: %d\n", fsd.ninodes);
  printf("sizeof(struct inode): %d\n", sizeof(struct inode));
  printf("INODES_PER_BLOCK: %d\n", INODES_PER_BLOCK);
  printf("NUM_INODE_BLOCKS: %d\n", NUM_INODE_BLOCKS);
}

/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  fsd.freemask[mbyte] |= (0x80 >> mbit);
  return OK;
}

/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
  return OK;

}

/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
  int mbyte, mbit, invb;
  mbyte = b / 8;
  mbit = b % 8;

  invb = ~(0x80 >> mbit);
  invb &= 0xFF;

  fsd.freemask[mbyte] &= invb;
  return OK;
}

/* This is maybe a little overcomplicated since the lowest-numbered
   block is indicated in the high-order bit.  Shift the byte by j
   positions to make the match in bit7 (the 8th bit) and then shift
   that value 7 times to the low-order bit to print.  Yes, it could be
   the other way...  */
void fs_printfreemask(void) {
  int i,j;

  for (i=0; i < fsd.freemaskbytes; i++) {
    for (j=0; j < 8; j++) {
      printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
    }
    if ( (i % 8) == 7) {
      printf("\n");
    }
  }
  printf("\n");
}

#endif /* FS */

