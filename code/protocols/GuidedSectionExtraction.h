#include <Uefi.h>
#include <Protocol/GuidedSectionExtraction.h>

typedef EFI_STATUS(EFIAPI *EFI_EXTRACT_GUIDED_SECTION)(
    IN CONST EFI_GUIDED_SECTION_EXTRACTION_PROTOCOL *This,
    IN CONST VOID *InputSection,
    OUT VOID **OutputBuffer,
    OUT UINTN *OutputSize,
    OUT UINT32 *AuthenticationStatus);

struct _EFI_GUIDED_SECTION_EXTRACTION_PROTOCOL
{
  EFI_EXTRACT_GUIDED_SECTION ExtractSection;
};