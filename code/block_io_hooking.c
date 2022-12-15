#include <Uefi.h>
#include <Protocol/SimpleTextInEx.h>

EFI_STATUS EFIAPI ReadBlocksHook(IN EFI_BLOCK_IO_PROTOCOL *This,
                                 IN UINT32 MediaId,
                                 IN EFI_LBA Lba,
                                 IN UINTN BufferSize, OUT VOID *Buffer)
{
    BlockIoHook *Hook = GetHookFromProtocol(This);
    int Read = dislock(Hook->dis_ctx, Buffer, Lba * This->Media->BlockSize, BufferSize);
    return Read == BufferSize ? EFI_SUCCESS : RETURN_PROTOCOL_ERROR;
}

EFI_STATUS EFIAPI WriteBlocksHook(IN EFI_BLOCK_IO_PROTOCOL *This,
                                  IN UINT32 MediaId,
                                  IN EFI_LBA Lba,
                                  IN UINTN BufferSize, IN VOID *Buffer)
{
    BlockIoHook *Hook = GetHookFromProtocol(This);
    int Read = enlock(Hook->dis_ctx, Buffer, Lba * This->Media->BlockSize, BufferSize);
    return Read == BufferSize ? EFI_SUCCESS : RETURN_PROTOCOL_ERROR;
}

ssize_t pread(int fd, void *buf, size_t nbytes, off_t offset)
{
    BlockIoHook *Hook = GetHookDataFromIndex(fd);
    Hook->ReadBlocksOriginal(hook->protocol,
                             BlockIo->Media->MediaId,
                             offset / Hook->BlockIo->Media->BlockSize,
                             nbytes, buf);
    return nbytes;
}

ssize_t pwrite(int fd, const void *buf, size_t nbytes, off_t offset)
{
    BlockIoHook *Hook = GetHookDataFromIndex(fd);
    Hook->WriteBlocksOriginal(hook->protocol,
                              BlockIo->Media->MediaId,
                              offset / Hook->BlockIo->Media->BlockSize,
                              nbytes, (void *)buf);
    return nbytes;
}