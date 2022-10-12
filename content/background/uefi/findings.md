% secure boot authorization process

1. Reset. This is when the platform begins initialization during boot.
2. Key Store Initialization. During the firmware initialization and before any signed UEFI images
are initialized, the platform firmware must validate the signature database.
3. UEFI Image Validation Succeeded? During initialization of an UEFI image, the UEFI Boot
Manager decides whether or not the UEFI image should be initialized. By comparing the
calculated UEFI image signature against that in one of the signature databases, the firmware
can determine if there is a match.
The security database db must either contain an entry with a hash value of the image (with a
supported hash type), or it must contain an entry with a certificate against which an entry in
the image’s certificate table can be verified. In either case verification must not succeed if the
security database dbx contains any record with:
A. Any entry with SignatureListType of EFI_CERT_SHA256_GUID with any
SignatureData containing the SHA-256 hash of the binary.
B. Any entry with SignatureListType of EFI_CERT_X509_SHA256,
EFI_CERT_X509_SHA384, or EFI_CERT_X509_SHA512, with any SignatureData
which reflects the To-Be-Signed hash included in any certificate in the signing chain of
the signature being verified.
C. Any entry with SignatureListType of EFI_CERT_X509_GUID, with SignatureData
which contains a certificate with the same Issuer, Serial Number, and To-Be-Signed hash
included in any certificate in the signing chain of the signature being verified.
Multiple signatures are allowed to exist in the binary’s certificate table (as per PE/COFF Section
“Attribute Certificate Table”). Only one hash or signature is required to be present in db
in order to pass validation, so long as neither the SHA-256 hash of the binary nor any
present signature is reflected in dbx.
Then, based on this match or its own policy, the firmware can decide whether or not to launch
the UEFI image.
4. Start UEFI Image. If the UEFI Image is approved, then it is launched normally.
5. UEFI Image Not Approved. If the UEFI image was not approved the platform firmware may use
other methods to discover if the UEFI image is authorized, such as consult a disk-based catalog
or ask an authorized user. The result can be one of three responses: Yes, No or Defer.
6. UEFI Image Signature Added To Signature Database. If the user approves of the UEFI image,
then the UEFI image’s signature is saved in the firmware’s signature database. If user approval
is supported, then the firmware be able to update of the Signature Database. For more
information, see Signature Database Update.
7. Go To Next Boot Option. If an UEFI image is rejected, then the next boot option is selected
normally and go to step 3. This is in the case where the image is listed as a boot option.
8. UEFI Image Signature Passed In System Configuration Table. If user defers, then the UEFI image
signature is copied into the Image Execution Information Table in the EFI System Configuration
Table which is available to the operating system.
9. OS Application Validates UEFI Image. An OS application determines whether the image is valid.
10. UEFI Image Signature Added To Signature Database. For more information, see Signature
Database Update.
11. End.


% implementation details in form of protcols

two protocols exist:
- Security Architecture Protocol
- Security2 Architecture Protocol

Security Architecture Protocol
Abstracts security-specific functions from the DXE Foundation for purposes of handling GUIDed
section encapsulations. This protocol must be produced by a boot service or runtime DXE driver
and may only be consumed by the DXE Foundation and any other DXE drivers that need to validate
the authentication of files.

FileAuthenticationState()
checks authentication status of a file
system executes platform specific policy in response to different status values
locking flash upon failure to authenticate
attestation logging

% EFI_SECURITY_VIOLATION, platform specific policy to execute untrusted code
% EFI_ACCESS_DENIED, image unloaded from memory





Security2 Architecture Protocol
Abstracts security-specific functions from the DXE Foundation of UEFI Image Verification,
Trusted Computing Group (TCG) measured boot, and User Identity policy for image loading and
consoles. This protocol must be produced by a boot service or runtime DXE driver.
% This protocol is optional and must be published prior to the EFI_SECURITY_ARCH_PROTOCOL.
As a result, the same driver must publish both of these interfaces.
When both Security and Security2 Architectural Protocols are published, LoadImage must use
them in accordance with the following rules:
- The Security2 protocol must be used on every image being loaded.
- The Security protocol must be used after the Securiy2 protocol and only on images that have been read using Firmware Volume protocol.
When only Security architectural protocol is published, LoadImage must use it on every image
being loaded.

measuring the PE/COFF image prior to invoking, comparing the
image against a policy (whether a white-list/black-list of public image verification keys or registered hashes)

FileAuthentication()
This service abstracts the invocation of Trusted Computing Group (TCG) measured boot, UEFI
Secure boot, and UEFI User Identity infrastructure. For the former two, the DXE Foundation
invokes the FileAuthentication() with a DevicePath and corresponding image in
FileBuffer memory. The TCG measurement code will record the FileBuffer contents into the
appropriate PCR. The image verification logic will confirm the integrity and provenance of the
image


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