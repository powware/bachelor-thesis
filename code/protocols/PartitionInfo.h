#include <Uefi.h>
#include <Protocol/PartitionInfo.h>

typedef struct
{
  UINT32 Revision;
  UINT32 Type;
  UINT8 System;
  UINT8 Reserved[7];
  union
  {
    MBR_PARTITION_RECORD Mbr;
    EFI_PARTITION_ENTRY Gpt;
  } Info;
} EFI_PARTITION_INFO_PROTOCOL;

extern EFI_GUID gEfiPartitionInfoProtocolGuid;