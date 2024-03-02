# Function to generate a cmake file that sets a variable with a list of files.
# Takes four arguments:
# - base_directories: a list of directories to search for files.
# - file_extensions: a list of file extensions to include in the search.
# - variable_name: the name of the variable to set with the found files.
# - relative_path: the base path to make file paths relative to.
function(generate_sources base_directories file_extensions variable_name relative_path)
    # Initialize an empty string to hold the list of source files.
    set(SOURCES_LIST "")
    # Iterate over each directory in the list of base directories.
    foreach(DIRECTORY IN LISTS base_directories)
        # Iterate over each file extension in the list of file extensions.
        foreach(EXTENSION IN LISTS file_extensions)
            # Glob all files matching the current extension in the current directory,
            # making paths relative to the specified relative_path.
            file(GLOB_RECURSE FILES RELATIVE "${relative_path}" "${DIRECTORY}/*.${EXTENSION}")
            # Iterate over each file found.
            foreach(FILE IN LISTS FILES)
                # Append the relative file path to SOURCES_LIST, ensuring each path starts on a new line.
                set(SOURCES_LIST "${SOURCES_LIST}\"${FILE}\"\n")
            endforeach()
        endforeach()
    endforeach()
    # Write the constructed list of sources to Sources.cmake, formatting the set command
    # so each file path appears on a new line for clarity.
    file(WRITE "${PROJECT_BINARY_DIR}/Sources.cmake" "set(${variable_name}\n${SOURCES_LIST})\n")
endfunction()