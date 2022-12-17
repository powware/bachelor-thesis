switch (GetImageType(File))
{
case IMAGE_FROM_FV:
    Policy = ALWAYS_EXECUTE;
    break;

case IMAGE_FROM_OPTION_ROM:
    Policy = PcdGet32(PcdOptionRomImageVerificationPolicy);
    break;

case IMAGE_FROM_REMOVABLE_MEDIA:
    Policy = PcdGet32(PcdRemovableMediaImageVerificationPolicy);
    break;

case IMAGE_FROM_FIXED_MEDIA:
    Policy = PcdGet32(PcdFixedMediaImageVerificationPolicy);
    break;

default:
    Policy = DENY_EXECUTE_ON_SECURITY_VIOLATION;
    break;
}