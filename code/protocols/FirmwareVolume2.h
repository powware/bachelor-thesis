#include <Uefi.h>
#include <Protocols/FimrwareVolume2.h>

typedef EFI_STATUS(EFIAPI *EFI_FV_READ_FILE)(
    IN CONST EFI_FIRMWARE_VOLUME2_PROTOCOL *This,
    IN CONST EFI_GUID *NameGuid,
    IN OUT VOID **Buffer,
    IN OUT UINTN *BufferSize,
    OUT EFI_FV_FILETYPE *FoundType,
    OUT EFI_FV_FILE_ATTRIBUTES *FileAttributes,
    OUT UINT32 *AuthenticationStatus);

typedef EFI_STATUS(EFIAPI *EFI_FV_READ_SECTION)(
    IN CONST EFI_FIRMWARE_VOLUME2_PROTOCOL *This,
    IN CONST EFI_GUID *NameGuid,
    IN EFI_SECTION_TYPE SectionType,
    IN UINTN SectionInstance,
    IN OUT VOID **Buffer,
    IN OUT UINTN *BufferSize,
    OUT UINT32 *AuthenticationStatus);

struct _EFI_FIRMWARE_VOLUME2_PROTOCOL
{
    EFI_FV_GET_ATTRIBUTES GetVolumeAttributes;
    EFI_FV_SET_ATTRIBUTES SetVolumeAttributes;
    EFI_FV_READ_FILE ReadFile;
    EFI_FV_READ_SECTION ReadSection;
    EFI_FV_WRITE_FILE WriteFile;
    EFI_FV_GET_NEXT_FILE GetNextFile;
    UINT32 KeySize;
    EFI_HANDLE ParentHandle;
    EFI_FV_GET_INFO GetInfo;
    EFI_FV_SET_INFO SetInfo;
};

extern EFI_GUID gEfiFirmwareVolume2ProtocolGuid;