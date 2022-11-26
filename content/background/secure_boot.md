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

PEI

PEI Guided Section Extraction PPI

EFI_PEI_SECURITY2_PPI

optional and vendor specific

This PPI is installed by some platform PEIM that abstracts the security policy to the PEI Foundation,
namely the case of a PEIM’s authentication state being returned during the PEI section extraction
process

This PPI is a means by which the platform builder can indicate a response to a PEIM's authentication
state. This can be in the form of a requirement for the PEI Foundation to skip a module using the
DeferExecution Boolean output in the AuthenticationState() member function.
Alternately, the Security PPI can invoke something like a cryptographic PPI that hashes the PEIM
contents to log attestations, for which the FileHandle parameter in
AuthenticationState() will be useful. If this PPI does not exist, PEIMs will be considered
trusted.

AuthenticationState()

This service is published by some platform PEIM. The purpose of this service is to expose a given platform's policy-based response to the PEI Foundation. For example, if there is a PEIM in a GUIDed encapsulation section and the extraction of the PEI file section yields an authentication failure, there is no a priori policy in the PEI Foundation. Specifically, this situation leads to the question whether PEIMs that are either not in GUIDed sections or are in sections whose authentication fails should still be executed. In fact, it is the responsibility of the platform builder to make this decision. This platform-scoped policy is a result that a desktop system might not be able to skip or not execute PEIMs because the skipped PEIM could be the agent that initializes main memory. Alternately, a system may require that unsigned PEIMs not be executed under any circumstances. In either case, the PEI Foundation simply multiplexes access to the Section Extraction PPI and the Security PPI. The Section Extraction PPI determines the contents of a section, and the Security PPI tells the PEI Foundation whether or not to invoke the PEIM. The PEIM that publishes the AuthenticationState() service uses its parameters in the following ways:
- AuthenticationStatus conveys the source information upon which the PEIM acts.
- The DeferExecution value tells the PEI Foundation whether or not to dispatch the PEIM. In addition, between receiving the AuthenticationState() from the PEI Foundation and returning with the DeferExecution value, the PEIM that publishes AuthenticationState() can do the following:
- Log the file state.
- Lock the firmware hubs in response to an unsigned PEIM being discovered.
These latter behaviors are platform- and market-specific and thus outside the scope of the PEI CIS.

DXE implementation details in form of protcols

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