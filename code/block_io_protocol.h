typedef struct _EFI_BLOCK_IO_PROTOCOL
{
    UINT64 Revision;

    EFI_BLOCK_IO_MEDIA *Media;
    EFI_BLOCK_RESET Reset;
    EFI_BLOCK_READ ReadBlocks;
    EFI_BLOCK_WRITE WriteBlocks;
    EFI_BLOCK_FLUSH FlushBlocks;
} EFI_BLOCK_IO_PROTOCOL;

typedef EFI_STATUS(EFIAPI *EFI_BLOCK_READ)(
    IN EFI_BLOCK_IO_PROTOCOL *This,
    IN UINT32 MediaId,
    IN EFI_LBA Lba,
    IN UINTN BufferSize,
    OUT VOID *Buffer);

typedef EFI_STATUS(EFIAPI *EFI_BLOCK_WRITE)(
    IN EFI_BLOCK_IO_PROTOCOL *This,
    IN UINT32 MediaId,
    IN EFI_LBA Lba,
    IN UINTN BufferSize,
    IN VOID *Buffer);