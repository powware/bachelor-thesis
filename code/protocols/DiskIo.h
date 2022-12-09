#include <Uefi.h>
#include <Protocol/DiskIo.h>

typedef EFI_STATUS(EFIAPI *EFI_DISK_READ)(
    IN EFI_DISK_IO_PROTOCOL *This,
    IN UINT32 MediaId,
    IN UINT64 Offset,
    IN UINTN BufferSize,
    OUT VOID *Buffer);

typedef EFI_STATUS(EFIAPI *EFI_DISK_WRITE)(
    IN EFI_DISK_IO_PROTOCOL *This,
    IN UINT32 MediaId,
    IN UINT64 Offset,
    IN UINTN BufferSize,
    IN VOID *Buffer);

struct _EFI_DISK_IO_PROTOCOL
{
  UINT64 Revision;
  EFI_DISK_READ ReadDisk;
  EFI_DISK_WRITE WriteDisk;
};

extern EFI_GUID gEfiDiskIoProtocolGuid;