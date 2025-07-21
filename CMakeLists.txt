cmake_minimum_required(VERSION 3.14)

project(mice)

file(GLOB_RECURSE MICE_SOURCES "src/**.c" "src/**.h" "include/**.h")
add_executable(mice ${MICE_SOURCES})
target_include_directories(mice PUBLIC "include/" PRIVATE "src/")

if(WIN32)
  target_link_libraries(mice ws2_32)
  target_compile_definitions(mice PRIVATE MICE_WINDOWS)
else()
  message(ERROR Unsupported platform)
endif()

target_compile_options(mice PRIVATE
  $<$<AND:$<CONFIG:Debug>,$<C_COMPILER_ID:GNU,Clang,AppleClang>>:-Wall -Wextra -Werror -Wpedantic>
  $<$<AND:$<CONFIG:Debug>,$<C_COMPILER_ID:MSVC>>:/W4 /WX>
)