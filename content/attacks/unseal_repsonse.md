Response: "80 02 <size of buf 32-bit big-end> < return code okay: 00 00 00 00> <magic: 00 00> <outer size: 00 2e> <inner size: 00 2c> <0xc bit-locker header bytes starting with 2c 00> <bek (0x20 bytes> ..."

typedef struct {
  TPM_ST    tag;
  UINT32    paramSize;
  TPM_CC    commandCode;
} TPM2_COMMAND_HEADER;

typedef struct
{
        typedef struct {
                TPM_ST    tag;
                UINT32    paramSize;
                TPM_RC    responseCode;
        } TPM2_RESPONSE_HEADER;
        TPM2B_SENSITIVE_DATA data;
} TPM2_UNSEAL_RESPONSE;




TPMU_SENSITIVE_CREATE

Response: "80 02 <size of buf 32-bit big-end> < return code okay: 00 00 00 00> <magic: 00 00> <outer size: 00 2e> <inner size: 00 2c> <0xc bit-locker header bytes starting with 2c 00> <bek (0x20 bytes> ..."


80 02           UINT16 tag
00 00 00 81     UINT32 paramSize
00 00 00 00     UINT32 responseCode
00 00           UINT16 unknown
00 2E           UINT16 outerSize
00 2C           UINT16 innerSize

VMK metadata 0xC 12 byte https://github.com/libyal/libbde/blob/main/documentation/BitLocker%20Drive%20Encryption%20(BDE)%20format.asciidoc#53-fve-metadata-entry

2C 00 entry size
05 00 entry type
01 00 value type Key
00 00 version
03 20 00 00 encryption method

VMK 0x20 32 byte

DD F0 83 CD C7 C5 57 70 55 E9 34 53 9F 42 8F 85 12 4B 3E B7 23 5A D6 FA CF 72 05 C5 8C A5 CF 6C

00 20 size of VMK
A4 BB 23 13 FB F8 DE 22 37 A6 B8 74 E8 29 F8 37 21 09 
B4 52 5D 25 9C 9B 1A 8F 4B E1 A8 0C 66 4E 00 00 
20 DD 62 BE 01 11 32 A6 BD 15 C0 13 8B 8E 9E 36 
5F 51 F5 76 41 BC 28 6F A7 CB 34 D5 D5 E0 EA 9A 
3E 

https://blog.elcomsoft.com/2021/01/understanding-bitlocker-tpm-protection/
https://pulsesecurity.co.nz/articles/TPM-sniffing