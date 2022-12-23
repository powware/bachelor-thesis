#include <Uefi.h>
#include <Protocol/Security2.h>

typedef EFI_STATUS(EFIAPI *EFI_SECURITY2_FILE_AUTHENTICATION)(
    IN CONST EFI_SECURITY2_ARCH_PROTOCOL *This,
    IN CONST EFI_DEVICE_PATH_PROTOCOL *File OPTIONAL,
    IN VOID *FileBuffer,
    IN UINTN FileSize,
    IN BOOLEAN BootPolicy);

struct _EFI_SECURITY2_ARCH_PROTOCOL
{
    EFI_SECURITY2_FILE_AUTHENTICATION FileAuthentication;
};

extern EFI_GUID gEfiSecurity2ArchProtocolGuid;