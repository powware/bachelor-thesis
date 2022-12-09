#include <Uefi.h>
#include <Protocol/Tcg2Protocol.h>

typedef EFI_STATUS(EFIAPI *EFI_TCG2_HASH_LOG_EXTEND_EVENT)(
    IN EFI_TCG2_PROTOCOL *This,
    IN UINT64 Flags,
    IN EFI_PHYSICAL_ADDRESS DataToHash,
    IN UINT64 DataToHashLen,
    IN EFI_TCG2_EVENT *EfiTcgEvent);

typedef EFI_STATUS(EFIAPI *EFI_TCG2_SUBMIT_COMMAND)(
    IN EFI_TCG2_PROTOCOL *This,
    IN UINT32 InputParameterBlockSize,
    IN UINT8 *InputParameterBlock,
    IN UINT32 OutputParameterBlockSize,
    IN UINT8 *OutputParameterBlock);

typedef EFI_STATUS(EFIAPI *EFI_TCG2_GET_ACTIVE_PCR_BANKS)(
    IN EFI_TCG2_PROTOCOL *This,
    OUT UINT32 *ActivePcrBanks);

typedef EFI_STATUS(EFIAPI *EFI_TCG2_SET_ACTIVE_PCR_BANKS)(
    IN EFI_TCG2_PROTOCOL *This,
    IN UINT32 ActivePcrBanks);

struct tdEFI_TCG2_PROTOCOL
{
  EFI_TCG2_GET_CAPABILITY GetCapability;
  EFI_TCG2_GET_EVENT_LOG GetEventLog;
  EFI_TCG2_HASH_LOG_EXTEND_EVENT HashLogExtendEvent;
  EFI_TCG2_SUBMIT_COMMAND SubmitCommand;
  EFI_TCG2_GET_ACTIVE_PCR_BANKS GetActivePcrBanks;
  EFI_TCG2_SET_ACTIVE_PCR_BANKS SetActivePcrBanks;
  EFI_TCG2_GET_RESULT_OF_SET_ACTIVE_PCR_BANKS GetResultOfSetActivePcrBanks;
};

extern EFI_GUID gEfiTcg2ProtocolGuid;