set(CMAKE_INCLUDE_CURRENT_DIR 1)

protobuf_generate_cpp(
    GENERATED_SOURCES GENERATED_HEADERS
    ${CMAKE_SOURCE_DIR}/messages/Temperature.proto
    ${CMAKE_SOURCE_DIR}/messages/Status.proto
)

add_executable(publisher publisher.cpp ${GENERATED_SOURCES})
target_include_directories(publisher PUBLIC ${INCLUDES})
target_link_libraries(publisher PUBLIC ${LIBS})

add_executable(subscriber subscriber.cpp ${GENERATED_SOURCES})
target_include_directories(subscriber PUBLIC ${INCLUDES})
target_link_libraries(subscriber PUBLIC ${LIBS})

