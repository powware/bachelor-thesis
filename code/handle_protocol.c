EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *simple_text_input_ex;
SystemTable->BootServices->HandleProtocol(SystemTable->ConsoleInHandle,
                                          &gEfiSimpleTextInputExProtocolGuid,
                                          (VOID **)&simple_text_input_ex);