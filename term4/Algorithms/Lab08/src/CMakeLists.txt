set(EXECUTABLE_NAME ${PROJECT_NAME})

file(GLOB HEADER_FILES ./*.hpp ./*.inl)
file(GLOB SOURCE_FILES ./*.cpp)

add_executable(${EXECUTABLE_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} PRIVATE NgRandom)