#include <Uefi.h>
#include <Protocol/Bds.h>

typedef VOID(EFIAPI *EFI_BDS_ENTRY)(
    IN EFI_BDS_ARCH_PROTOCOL *This);

struct _EFI_BDS_ARCH_PROTOCOL
{
    EFI_BDS_ENTRY Entry;
};

extern EFI_GUID gEfiBdsArchProtocolGuid;