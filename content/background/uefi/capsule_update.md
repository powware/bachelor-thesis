
DXE MM Ready to Lock Protocol

This protocol in tandem with the End of DXE Event facilitates transition of the platform from the
environment where all of the components are under the authority of the platform manufacturer to the
environment where third party extensible modules such as UEFI drivers and UEFI applications are
executed.
The protocol is published immediately after signaling of the End of DXE Event.
PI modules that need to lock or protect their resources in anticipation of the invocation of 3rd party
extensible modules should register for notification on installation of this protocol and effect the
appropriate protections in their notification handlers.

End of DXE Event

From SEC through the signaling of this event, all of the components should be under the authority of
the platform manufacturer and not have to worry about interaction or corruption by 3rd party
extensible modules such as UEFI drivers and UEFI applications.
Platform may choose to lock certain resources or disable certain interfaces prior to executing third
party extensible modules. Transition from the environment where all of the components are under
the authority of the platform manufacturer to the environment where third party modules are
executed is a two-step process:

Firmware Update and Reporting
- Get the attributes of the current firmware image. Attributes include revision level.
- Get a copy of the current firmware image. As an example, this service could be used by a management application to facilitate a firmware roll-back.
- Program the device with a firmware image supplied by the user.
- Label all the firmware images within a device with a single version.

Firmware Management Protocol

GetImageInfo() is the only required function. GetImage(), SetImage(), CheckImage(),
GetPackageInfo(), and SetPackageInfo() shall return EFI_UNSUPPORTED if not supported by the
driver.

The authentication support leverages the authentication scheme employed in variable authentication.
Please reference EFI_VARIABLE_AUTHENTICATION in the “Variable Services” section of “Services –
Runtime Services” chapter.

Compatibilities are used to ensure the targeted firmware image supports the current hardware
configuration. Compatibilities are set based on the current hardware configuration and firmware update
policy should match the current settings to those supported by the new firmware image, and only permits update
to proceed if the new firmware image settings are equal or greater than the current hardware configuration
settings. For example, if this function returns Compatibilities= 0x0000000000070001 and the new firmware
image supports settings=0x0000000000030001, then the update policy should block the firmware update and
notify the user that updating the hardware with the new firmware image may render the hardware inoperable. This
situation usually occurs when updating the hardware with an older version of firmware.

If IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED is supported and set, then authentication is
required to perform the firmware image operations. In firmware image operations, the image pointer
points to the start of the authentication data and the image size is the size of the authentication data and
the size of the firmware image.

SetImage()

- Validate the image authentication if image has attribut IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED. The function return EFI_SECURITY_VIOLATION if the validation fails.
- Validate the image is a supported image for this device. The function returns EFI_ABORTED i the image is unsupported. The function can optionally provide more detailed information o why the image is not a supported image.
- Validate the data from VendorCode if not null. Image validation must be performed before VendorCode data validation. VendorCode data is ignored or considered invalid if image validation failed. The function returns EFI_ABORTED if the data is invalid.

VendorCode enables vendor to implement vendor-specific firmware image update policy. Null if the
caller did not specify the policy or use the default policy. As an example, vendor can implement a policy
to allow an option to force a firmware image update when the abort reason is due to the new firmware
image version is older than the current firmware image version or bad image checksum. Sensitive
operations such as those wiping the entire firmware image and render the device to be non-functional
should be encoded in the image itself rather than passed with the VendorCode.

CheckImage()

IMAGE_UPDATABLE_VALID indicates SetImage() will accept the new image and update the device with the
new image.The version of the new image could be higher or lower than the current image. SetImage
VendorCode is optional but can be used for vendor specific action.
IMAGE_UPDATABLE_INVALID indicates SetImage() will reject the new image. No additional information is
provided for the rejection.
IMAGE_UPDATABLE_INVALID_TYPE indicates SetImage() will reject the new image. The rejection is due to
the new image is not a firmware image recognized for this device.
IMAGE_UPDATABLE_INVALID_OLD indicates SetImage() will reject the new image. The rejection is due to
the new image version is older than the current firmware image version in the device. The device firmware update
policy does not support firmware version downgrade.
IMAGE_UPDATABLE_VALID_WITH_VENDOR_CODE indicates SetImage() will accept and update the
new image only if a correct VendorCode is provided or else image would be rejected and SetImage will
return appropriate error.

EFI_FIRMWARE_MANAGEMENT_CAPSULE_ID_GUID

typedef struct {
UINT32
Version;
EmbeddedDriverCount;
UINT16
PayloadItemCount;
UINT16
ItemOffsetList[];
// UINT64
} EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER;

It is expected that
drivers whose presence is indicated by non-zero EmbeddedDriverCount will be used to supply an
implementation of EFI_FIRMWARE_MANAGEMENT_PROTOCOL for devices that lack said protocol within
the image to be updated.

Each payload item contained within the capsule body is preceded by a
EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER struct used to provide information required
to prepare the payload item as an image for delivery to a instance of
EFI_FIRMWARE_MANAGEMENT_PROTOCOL.SetImage()function.

Capsule is presented to system firmware via call to UpdateCapsule()or using mass storage
delivery procedure of Section 8.5.5. The capsule must be constructed to consist of a single
EFI_FIRMWARE_MANAGEMENT_CAPSULE_HEADER structure with the 0 or more drivers and 0
or more binary payload items. However a capsule in which driver count and payload count are
both zero is not processed.

CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE flag must be 0.

If device requires hardware reset to unlock flash write protection,
CAPSULE_FLAGS_PERSIST_ACROSS_RESET and optionally
CAPSULE_FLAGS_INITIATE_RESET should be set to 1 in the EFI_CAPSULE_HEADER.

Each extracted driver is placed into a buffer and passed to LoadImage(). The driver image
passed to LoadImage() must successfully pass all image format, platform type, and security
checks including those related to UEFI secure boot, if enabled on the platform

FmpAuthenticationLib

FmpDxe.c
SetTheImage
CheckTheImageInternal
AuthenticateFmpImage


PcdFmpDeviceLockEventGuid

 ## An event GUID that locks the firmware device when the event is signaled.
  #  If this PCD is not a valid GUID value, then the firmware device is locked
  #  when gEfiEndOfDxeEventGroupGuid (End of DXE Phase) is signaled.  The
  #  default value is empty, so by default the firmware device is locked at the
  #  end of the DXE phase.
  # @Prompt Firmware Device Lock Event GUID.
  gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceLockEventGuid|{0}|VOID*|0x4000000F



/**
  Determines if the FMP device should be locked when the event specified by
  PcdFmpDeviceLockEventGuid is signaled. The expected result from this function
  is TRUE so the FMP device is always locked.  A platform can choose to return
  FALSE (e.g. during manufacturing) to allow FMP devices to remain unlocked.

  @param[in]  This  A pointer to the EDKII_CAPSULE_UPDATE_POLICY_PROTOCOL instance.

  @retval TRUE   The FMP device lock action is required at lock event guid.
  @retval FALSE  Do not perform FMP device lock at lock event guid.

**/
BOOLEAN
EFIAPI
CapsuleUpdatePolicyIsLockFmpDeviceAtLockEventGuidRequired (
  IN  EDKII_CAPSULE_UPDATE_POLICY_PROTOCOL  *This
  )
{
  return IsLockFmpDeviceAtLockEventGuidRequired ();
}

5.1.2.1 End of DXE Event
Prior to invoking any UEFI drivers, or applications that are not from the platform manufacturer, or
connecting consoles, the platform should signals the event EFI_END_OF_DXE_EVENT_GUID
End of DXE Event and immediately after that the platform installs DXE SMM Ready to Lock
Protocol (defined in volume 4)..
#define EFI_END_OF_DXE_EVENT_GROUP_GUID \
{ 0x2ce967a, 0xdd7e, 0x4ffc, { 0x9e, 0xe7, 0x81, 0xc, \
0xf0, 0x47, 0x8, 0x80 } }
From SEC through the signaling of this event, all of the components should be under the authority of
the platform manufacturer and not have to worry about interaction or corruption by 3rd party
extensible modules such as UEFI drivers and UEFI applications.
Platform may choose to lock certain resources or disable certain interfaces prior to executing third
party extensible modules. Transition from the environment where all of the components are under
the authority of the platform manufacturer to the environment where third party modules are
executed is a two-step process:

1. End of DXE Event is signaled. This event presents the last opportunity to use resources or
interfaces that are going to be locked or disabled in anticipation of the invocation of 3rd party
extensible modules.
2. DXE SMM Ready to Lock Protocol is installed. PI modules that need to lock or protect their
resources in anticipation of the invocation of 3rd party extensible modules should register for
notification on installation of this protocol and effect the appropriate protections in their
notification handlers

// The lock event GUID is retrieved from PcdFmpDeviceLockEventGuid.
// If PcdFmpDeviceLockEventGuid is not the size of an EFI_GUID, then
// gEfiEndOfDxeEventGroupGuid is used.