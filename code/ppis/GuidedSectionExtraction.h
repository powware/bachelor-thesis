#include <Uefi.h>
#include <Ppi/GuidedSectionExtraction.h>

typedef EFI_STATUS(EFIAPI *EFI_PEI_EXTRACT_GUIDED_SECTION)(
    IN CONST EFI_PEI_GUIDED_SECTION_EXTRACTION_PPI *This,
    IN CONST VOID *InputSection,
    OUT VOID **OutputBuffer,
    OUT UINTN *OutputSize,
    OUT UINT32 *AuthenticationStatus);

struct _EFI_PEI_GUIDED_SECTION_EXTRACTION_PPI
{
  EFI_PEI_EXTRACT_GUIDED_SECTION ExtractSection;
};
