#include <Uefi.h>

#include <Protocol/DiskIo.h>
#include <Protocol/BlockIo.h>

typedef struct
{
    UINTN Signature;
    EFI_DISK_IO_PROTOCOL DiskIo;
    EFI_BLOCK_IO_PROTOCOL *BlockIo;
} DISK_IO_PRIVATE_DATA;

EFI_STATUS
EFIAPI
DiskIoReadDisk(
    IN EFI_DISK_IO_PROTOCOL *This,
    IN UINT32 MediaId,
    IN UINT64 Offset,
    IN UINTN BufferSize,
    OUT VOID *Buffer)
{
    DISK_IO_PRIVATE_DATA *Private;

    Private = DISK_IO_PRIVATE_DATA_FROM_THIS(This);

    Private->BlockIo->ReadBlocks(...);
}