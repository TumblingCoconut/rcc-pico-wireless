add_library(rcc_stdlib rcc_stdlib.h)
set_target_properties(rcc_stdlib PROPERTIES LINKER_LANGUAGE CXX)
# add_library(rcc_wireless rcc_wireless_msg_interface.h)
# target_link_libraries(rcc_wireless serial-packets)