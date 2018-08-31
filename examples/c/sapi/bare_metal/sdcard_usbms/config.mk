# Compile options

VERBOSE=n
OPT=0
USE_NANO=y
SEMIHOST=n
USE_FPU=y

# Libraries

USE_LPCOPEN=y
USE_SAPI=y
USE_LPCUSBLIB=y
USE_FATFS=y

USE_FREERTOS=n
FREERTOS_HEAP_TYPE=5

# Let FSUSB print their own status messages by DEBUGSTR()
DEFINES+=DEBUG_ENABLE

