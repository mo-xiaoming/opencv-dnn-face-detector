# This function will prevent in-source builds
function(AssureOutOfSourceBuilds)
    file(REAL_PATH "${CMAKE_SOURCE_DIR}" srcdir)
    file(REAL_PATH "${CMAKE_BINARY_DIR}" outdir)

    # disallow in-source builds
    if("${srcdir}" STREQUAL "${bindir}")
        message("######################################################")
        message("Warning: in-source builds are disabled")
        message("Please create a separate build directory and run cmake from there")
        message("######################################################")
        message(FATAL_ERROR "Quitting configuration")
    endif()
endfunction()

assureoutofsourcebuilds()