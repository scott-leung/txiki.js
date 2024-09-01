# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/code/txiki.js/deps/libffi"
  "E:/code/txiki.js/build/libffi-prefix/src/libffi-build"
  "E:/code/txiki.js/build/ffi_root"
  "E:/code/txiki.js/build/libffi-prefix/tmp"
  "E:/code/txiki.js/build/libffi-prefix/src/libffi-stamp"
  "E:/code/txiki.js/build/libffi-prefix/src"
  "E:/code/txiki.js/build/libffi-prefix/src/libffi-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/code/txiki.js/build/libffi-prefix/src/libffi-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/code/txiki.js/build/libffi-prefix/src/libffi-stamp${cfgdir}") # cfgdir has leading slash
endif()
