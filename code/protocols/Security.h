#include <Uefi.h>
#include <Protocol/Security.h>

typedef EFI_STATUS(EFIAPI *EFI_SECURITY_FILE_AUTHENTICATION_STATE)(
    IN CONST EFI_SECURITY_ARCH_PROTOCOL *This,
    IN UINT32 AuthenticationStatus,
    IN CONST EFI_DEVICE_PATH_PROTOCOL *File);

struct _EFI_SECURITY_ARCH_PROTOCOL
{
  EFI_SECURITY_FILE_AUTHENTICATION_STATE FileAuthenticationState;
};

extern EFI_GUID gEfiSecurityArchProtocolGuid;
