cmake_minimum_required(VERSION 3.4.1)

set(JNI_LIBS ${CMAKE_SOURCE_DIR}/../jnilibs)

add_library(speex-lib SHARED IMPORTED)
set_target_properties(speex-lib  PROPERTIES IMPORTED_LOCATION ${JNI_LIBS}/${ANDROID_ABI}/libspeex.so)


add_library(speexdsp-lib SHARED IMPORTED)
set_target_properties(speexdsp-lib  PROPERTIES IMPORTED_LOCATION ${JNI_LIBS}/${ANDROID_ABI}/libspeexdsp.so)

include_directories( ${JNI_LIBS}/speex_include )

add_library(native SHARED
            native.cpp
            opensl_io.c
            opensl_io.h
            log.h)

target_link_libraries(
                       native
                       log
                       OpenSLES
                       speex-lib
                       speexdsp-lib
                       )
