## ---------------------------------------------------------------------
##
## Copyright (C) 2016 - 2022 by the deal.II Authors
##
## This file is part of the deal.II library.
##
## The deal.II library is free software; you can use it, redistribute
## it, and/or modify it under the terms of the GNU Lesser General
## Public License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
## The full text of the license can be found in the file LICENSE.md at
## the top level directory of deal.II.
##
## ---------------------------------------------------------------------

# Auxiliary functions
function(cat IN_FILE OUT_FILE INDENT)
  file(READ ${IN_FILE} CONTENTS)
  if(${INDENT} MATCHES "TRUE")
    file(STRINGS ${IN_FILE} LINESTMP)
    foreach(LINETMP ${LINESTMP})
      file(APPEND ${OUT_FILE} "  ${LINETMP}\n")
    endforeach()
  else()
    file(APPEND ${OUT_FILE} "${CONTENTS}")
  endif()
endfunction()

function(process IN_DIR OUT_FILE)
  file(APPEND ${OUT_FILE} "<ol>\n")
  file(GLOB ENTRY_LIST ${IN_DIR}/[0-9]*)
  list(SORT ENTRY_LIST)
  list(REVERSE ENTRY_LIST)
  foreach(ENTRY ${ENTRY_LIST})
    file(APPEND ${OUT_FILE} "\n <li>\n")
    CAT(${ENTRY} ${OUT_FILE} "TRUE")
    file(APPEND ${OUT_FILE} " </li>\n")
  endforeach()
  file(APPEND ${OUT_FILE} "\n</ol>\n")
endfunction()

# Generate 'changes.h'.

# First, create a file 'changes.h.in' based on all changelog fragments.
set(OUTPUT_FILE_TEMP "${OUTPUT_FILE}.in")
file(WRITE ${OUTPUT_FILE_TEMP} "")
CAT    (${CMAKE_CURRENT_SOURCE_DIR}/header
        ${OUTPUT_FILE_TEMP} "FALSE")
CAT    (${CMAKE_CURRENT_SOURCE_DIR}/header_incompatibilities
        ${OUTPUT_FILE_TEMP} "FALSE")
PROCESS(${CMAKE_CURRENT_SOURCE_DIR}/incompatibilities
        ${OUTPUT_FILE_TEMP})
CAT    (${CMAKE_CURRENT_SOURCE_DIR}/header_major
        ${OUTPUT_FILE_TEMP} "FALSE")
PROCESS(${CMAKE_CURRENT_SOURCE_DIR}/major
        ${OUTPUT_FILE_TEMP})
CAT    (${CMAKE_CURRENT_SOURCE_DIR}/header_minor
        ${OUTPUT_FILE_TEMP} "FALSE")
PROCESS(${CMAKE_CURRENT_SOURCE_DIR}/minor
        ${OUTPUT_FILE_TEMP})
CAT    (${CMAKE_CURRENT_SOURCE_DIR}/footer
        ${OUTPUT_FILE_TEMP} "FALSE")

# Copy it over to 'changes.h' but only touch the time stamp
# if the file actually changed (this is what CONFIGURE_FILE does).
message(STATUS "Generating changelog file: ${OUTPUT_FILE}")
configure_file(${OUTPUT_FILE_TEMP} ${OUTPUT_FILE} COPYONLY)
