cmake_minimum_required(VERSION 3.19)

project(snif VERSION 0.0.1)

find_library(PCAP_LIBRARY NAMES pcap REQUIRED)
find_path(PCAP_INCLUDE NAMES pcap.h REQUIRED)

add_executable(snif)

set_target_properties(snif
  PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)
target_sources(snif
  PRIVATE
    src/main.c
    src/Dumper.c
    src/Sniffer.c
)
target_include_directories(snif
  PRIVATE
    ${PCAP_INCLUDE}
)
target_compile_options(snif
    PRIVATE
        $<$<C_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
target_link_libraries(snif
  PRIVATE
    ${PCAP_LIBRARY}
)
install(
  TARGETS snif
)