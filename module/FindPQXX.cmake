SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/module)
include (FindPackageHandleStandardArgs)

find_path (Pqxx_INCLUDE_DIRS
    NAME
        pqxx
    PATHS
        /usr/include
        /usr/local/include
    PATH_SUFFIXES
        pqxx
    DOC
        "Directory for pqxx headers"
)

find_library (Pqxx_LIBRARIES
    NAMES
        pqxx
)

FIND_PACKAGE_HANDLE_STANDARD_ARGS("Pqxx"
    "libpqxx couldn't be found"
    Pqxx_LIBRARIES
    Pqxx_INCLUDE_DIRS
)

mark_as_advanced (Pqxx_INCLUDE_DIR Pqxx_LIBRARY)
