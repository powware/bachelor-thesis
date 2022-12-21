#include <Uefi.h>
#include <Protocol/DriverBinding.h>

typedef EFI_STATUS(EFIAPI *EFI_DRIVER_BINDING_SUPPORTED)(
    IN EFI_DRIVER_BINDING_PROTOCOL *This,
    IN EFI_HANDLE ControllerHandle,
    IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL);

typedef EFI_STATUS(EFIAPI *EFI_DRIVER_BINDING_START)(
    IN EFI_DRIVER_BINDING_PROTOCOL *This,
    IN EFI_HANDLE ControllerHandle,
    IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL);

typedef EFI_STATUS(EFIAPI *EFI_DRIVER_BINDING_STOP)(
    IN EFI_DRIVER_BINDING_PROTOCOL *This,
    IN EFI_HANDLE ControllerHandle,
    IN UINTN NumberOfChildren,
    IN EFI_HANDLE *ChildHandleBuffer OPTIONAL);

struct _EFI_DRIVER_BINDING_PROTOCOL
{
    EFI_DRIVER_BINDING_SUPPORTED Supported;
    EFI_DRIVER_BINDING_START Start;
    EFI_DRIVER_BINDING_STOP Stop;
    UINT32 Version;
    EFI_HANDLE ImageHandle;
    EFI_HANDLE DriverBindingHandle;
};

extern EFI_GUID gEfiDriverBindingProtocolGuid;