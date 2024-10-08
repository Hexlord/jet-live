
cmake_minimum_required(VERSION 3.6)
project(jet-live)
cmake_policy(SET CMP0077 NEW)

option(JET_LIVE_SHARED "Compile as shared library. ON - shared, OFF - static." OFF)
option(JET_LIVE_BUILD_EXAMPLE "Compile example application." ON)
option(JET_LIVE_BUILD_TESTS "Compile test applications." OFF)

if (NOT JET_LIVE_CONFIGURED)
  include(cmake/jet_live_setup.cmake)
endif()
# For debugging JetLive
if(0)
	set(CMAKE_CXX_FLAGS_DEBUG 		"-O0 -g            -DSE_DEBUG=1 -DSE_VERBOSE_DEBUG=1 -DDEBUG=1          -DSE_SHIPPING=0 -DSE_BUILD_CONFIG=\"Debug\"")
endif()

########## lib_subhook
add_library(lib_subhook STATIC "")
target_sources(lib_subhook
  PRIVATE
  libs/subhook/subhook.c
  )
target_include_directories(lib_subhook
  PUBLIC
  libs/subhook
  )

########## lib_whereami
add_library(lib_whereami STATIC "")
target_sources(lib_whereami
  PRIVATE
  libs/whereami/src/whereami.c
  )
target_include_directories(lib_whereami
  PUBLIC
  libs/whereami/src
  )

########## lib_efsw
add_library(lib_efsw STATIC "")
target_sources(lib_efsw
  PRIVATE
  libs/efsw/src/efsw/Debug.cpp
  libs/efsw/src/efsw/DirectorySnapshot.cpp
  libs/efsw/src/efsw/DirectorySnapshotDiff.cpp
  libs/efsw/src/efsw/DirWatcherGeneric.cpp
  libs/efsw/src/efsw/FileInfo.cpp
  libs/efsw/src/efsw/FileSystem.cpp
  libs/efsw/src/efsw/FileWatcher.cpp
  libs/efsw/src/efsw/FileWatcherCWrapper.cpp
  libs/efsw/src/efsw/FileWatcherGeneric.cpp
  libs/efsw/src/efsw/FileWatcherImpl.cpp
  libs/efsw/src/efsw/Log.cpp
  libs/efsw/src/efsw/Mutex.cpp
  libs/efsw/src/efsw/String.cpp
  libs/efsw/src/efsw/System.cpp
  libs/efsw/src/efsw/Thread.cpp
  libs/efsw/src/efsw/Watcher.cpp
  libs/efsw/src/efsw/WatcherGeneric.cpp
  libs/efsw/src/efsw/platform/posix/FileSystemImpl.cpp
  libs/efsw/src/efsw/platform/posix/MutexImpl.cpp
  libs/efsw/src/efsw/platform/posix/SystemImpl.cpp
  libs/efsw/src/efsw/platform/posix/ThreadImpl.cpp
  )
if (UNIX AND NOT APPLE)
  target_sources(lib_efsw
    PRIVATE
    libs/efsw/src/efsw/FileWatcherInotify.cpp
    libs/efsw/src/efsw/WatcherInotify.cpp
    )
elseif(UNIX AND APPLE)
  target_sources(lib_efsw
    PRIVATE
    libs/efsw/src/efsw/FileWatcherFSEvents.cpp
    libs/efsw/src/efsw/WatcherFSEvents.cpp
    )
  target_link_libraries(lib_efsw "-framework CoreFoundation" "-framework CoreServices")
endif()
target_include_directories(lib_efsw
  PUBLIC
  libs/efsw/include
  libs/efsw/src
  )

########## lib_tiny-process-library
add_library(lib_tiny-process-library STATIC "")
target_sources(lib_tiny-process-library
  PRIVATE
  libs/tiny-process-library/process.cpp
  libs/tiny-process-library/process_unix.cpp
  )
target_include_directories(lib_tiny-process-library
  PUBLIC
  libs/tiny-process-library
  )
set_target_properties(lib_tiny-process-library
  PROPERTIES
  CXX_STANDARD 11
  )

########## lib_teenypath
add_library(lib_teenypath STATIC "")
target_sources(lib_teenypath
  PRIVATE
  libs/teenypath/src/teenypath.cpp
  )
target_include_directories(lib_teenypath
  PUBLIC
  libs/teenypath/include
  )
set_target_properties(lib_teenypath
  PROPERTIES
  CXX_STANDARD 11
  )

########## lib_json
add_library(lib_json INTERFACE)
target_include_directories(lib_json
  INTERFACE
  libs/json
  )

########## lib_argh
add_library(lib_argh INTERFACE)
target_include_directories(lib_argh
  INTERFACE
  libs/argh/include
  )

########## lib_xxhash
add_library(lib_xxhash STATIC "")
target_sources(lib_xxhash
  PRIVATE
  libs/xxHash/xxhash.c
  )
target_include_directories(lib_xxhash
  PUBLIC
  libs/xxHash
  )
set_target_properties(lib_xxhash
  PROPERTIES
  CXX_STANDARD 11
  )

if (UNIX AND NOT APPLE)
  ########## lib_elfio
  add_library(lib_elfio INTERFACE)
  target_include_directories(lib_elfio
    INTERFACE
    libs/ELFIO
    )
endif()

########## jet-live
if (JET_LIVE_SHARED)
  add_library(jet-live SHARED "")
  target_compile_options(jet-live PRIVATE $<$<COMPILE_LANGUAGE:C>:-fPIC> $<$<COMPILE_LANGUAGE:CXX>:-fPIC>)
  target_compile_options(lib_subhook PRIVATE $<$<COMPILE_LANGUAGE:C>:-fPIC> $<$<COMPILE_LANGUAGE:CXX>:-fPIC>)
  target_compile_options(lib_whereami PRIVATE $<$<COMPILE_LANGUAGE:C>:-fPIC> $<$<COMPILE_LANGUAGE:CXX>:-fPIC>)
  target_compile_options(lib_efsw PRIVATE $<$<COMPILE_LANGUAGE:C>:-fPIC> $<$<COMPILE_LANGUAGE:CXX>:-fPIC> -Wno-implicit-fallthrough)
  target_compile_options(lib_tiny-process-library PRIVATE $<$<COMPILE_LANGUAGE:C>:-fPIC> $<$<COMPILE_LANGUAGE:CXX>:-fPIC>)
  target_compile_options(lib_teenypath PRIVATE $<$<COMPILE_LANGUAGE:C>:-fPIC> $<$<COMPILE_LANGUAGE:CXX>:-fPIC>)
  target_compile_options(lib_xxhash PRIVATE $<$<COMPILE_LANGUAGE:C>:-fPIC> $<$<COMPILE_LANGUAGE:CXX>:-fPIC>)
else()
  add_library(jet-live STATIC "")
endif()
target_sources(jet-live
  PRIVATE
  src/jet/live/Utility.cpp
  src/jet/live/DefaultSymbolsFilter.cpp
  src/jet/live/FileWatcher.cpp
  src/jet/live/CompileCommandsCompilationUnitsParser.cpp
  src/jet/live/Compiler.cpp
  src/jet/live/Live.cpp
  src/jet/live/DepfileDependenciesHandler.cpp
  src/jet/live/CodeReloadPipeline.cpp
  src/jet/live/LinkTimeRelocationsStep.cpp
  src/jet/live/FunctionsHookingStep.cpp
  src/jet/live/StaticsCopyStep.cpp
  src/jet/live/SignalReloader.cpp
  src/jet/live/AsyncEventQueue.cpp
  src/jet/live/BuildConfig.cpp
  )
if (UNIX AND NOT APPLE)
  target_sources(jet-live
    PRIVATE
    src/jet/live/_linux/ElfProgramInfoLoader.cpp
    src/jet/live/_linux/Utility.cpp
    )
elseif (UNIX AND APPLE)
  target_sources(jet-live
    PRIVATE
    src/jet/live/_macos/MachoProgramInfoLoader.cpp
    src/jet/live/_macos/Utility.cpp
    )
endif()
target_include_directories(jet-live
  PUBLIC
  src/
  libs/utils/
  )
find_package(Threads REQUIRED)
target_link_libraries(jet-live
  PRIVATE
  lib_subhook
  lib_whereami
  lib_teenypath
  lib_json
  lib_argh
  lib_xxhash
  dl
  Threads::Threads
  )
target_link_libraries(jet-live
  PUBLIC
  lib_efsw
  lib_tiny-process-library
  )
if (UNIX AND NOT APPLE)
  target_link_libraries(jet-live
    PRIVATE
    lib_elfio
    )
endif()
set_target_properties(jet-live
  PROPERTIES
  CXX_STANDARD 11
  )
target_compile_options(jet-live
  PRIVATE
  $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
  $<$<COMPILE_LANGUAGE:C>:-Wall>
  $<$<COMPILE_LANGUAGE:CXX>:-Wall>
  $<$<COMPILE_LANGUAGE:C>:-Wextra>
  $<$<COMPILE_LANGUAGE:CXX>:-Wextra>
  $<$<COMPILE_LANGUAGE:C>:-Wpedantic>
  $<$<COMPILE_LANGUAGE:CXX>:-Wpedantic>
  $<$<COMPILE_LANGUAGE:C>:-Wshadow>
  $<$<COMPILE_LANGUAGE:CXX>:-Wshadow>
  $<$<COMPILE_LANGUAGE:C>:-Wconversion>
  $<$<COMPILE_LANGUAGE:CXX>:-Wconversion>
  -Wno-dangling-reference -Wno-shadow -Wno-deprecated-declarations
  )
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(jet-live
    PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-Wvexing-parse>
    $<$<COMPILE_LANGUAGE:CXX>:-Wvexing-parse>
    )
endif()
set_property(
  SOURCE src/jet/live/BuildConfig.cpp
  APPEND
  PROPERTY COMPILE_DEFINITIONS
  JET_LIVE_CMAKE_GENERATOR="${CMAKE_GENERATOR}"
  JET_LIVE_CMAKE_BUILD_DIR="${CMAKE_BINARY_DIR}"
  )

if (JET_LIVE_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()

if (JET_LIVE_BUILD_TESTS)
  add_subdirectory(tests)
endif()

