#include <Uefi.h>

#include <Protocol/SimpleTextInEx.h>

EFI_BOOT_SERVICES *gBS;

EFI_STATUS
EFIAPI
EntryPoint(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
    gBS = SystemTable->BootServices;

    EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleTextInEx;

    gBS->HandleProtocol(SystemTable->ConsoleInHandle,
                        &gEfiSimpleTextInputExProtocolGuid,
                        (VOID **)&SimpleTextInEx);

    UINTN EventIndex;
    gBS->WaitForEvent(1, &SimpleTextInEx->WaitForKeyEx, &EventIndex);

    EFI_KEY_DATA KeyData;
    SimpleTextInEx->ReadKeyStrokeEx(SimpleTextInEx, &KeyData);

    // do something with key press

    return EFI_SUCCESS;
}