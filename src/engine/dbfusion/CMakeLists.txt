
include_directories(${SDL_INCLUDE_DIR})

add_subdirectory(src)

FILE(GLOB INCLUDES ./include/*.h)

if(DBFUSION)
    add_definitions(-DDBFUSION)
    add_subdirectory(menu)
endif()

add_library(dbfusion OBJECT config.h dbFusionNgine.h dbFusionNgine.cpp ${INCLUDES})

set_property(GLOBAL APPEND PROPERTY CG_OBJ_LIBS $<TARGET_OBJECTS:dbfusion>)
