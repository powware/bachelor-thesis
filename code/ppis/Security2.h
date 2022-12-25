#include <Uefi.h>
#include <Ppi/Security2.h>

typedef EFI_STATUS(EFIAPI *EFI_PEI_SECURITY_AUTHENTICATION_STATE)(
    IN CONST EFI_PEI_SERVICES **PeiServices,
    IN CONST EFI_PEI_SECURITY2_PPI *This,
    IN UINT32 AuthenticationStatus,
    IN EFI_PEI_FV_HANDLE FvHandle,
    IN EFI_PEI_FILE_HANDLE FileHandle,
    IN OUT BOOLEAN *DeferExecution);

struct _EFI_PEI_SECURITY2_PPI
{
  EFI_PEI_SECURITY_AUTHENTICATION_STATE AuthenticationState;
};

extern EFI_GUID gEfiPeiSecurity2PpiGuid;
