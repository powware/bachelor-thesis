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