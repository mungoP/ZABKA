# Pure-C control core: FOC math, encoder decoding, estimators, protocol codecs.
#
# Rule: nothing in here may depend on the STM32 HAL or any hardware header.
# This target deliberately does NOT link stm32cubemx, so a HAL #include fails
# to compile. That keeps the core buildable on the host for unit tests.

add_library(control STATIC
    src/control.c
)

target_include_directories(control PUBLIC include)

target_compile_features(control PUBLIC c_std_11)

target_compile_options(control PRIVATE -Wall -Wextra -Werror)
