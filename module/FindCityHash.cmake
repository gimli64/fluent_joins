FIND_PATH(CityHash_INCLUDE_DIR
  /usr/local/lib
)

IF(CityHash_INCLUDE_DIR)
  IF(CityHash_LIBRARY)
    SET(CityHash_FOUND "YES")
    SET(CityHash_LIBRARIES ${CityHash_LIBRARY})
  ENDIF(CityHash_LIBRARY)
ENDIF(CityHash_INCLUDE_DIR)