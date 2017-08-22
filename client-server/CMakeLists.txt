set(CMAKE_INCLUDE_CURRENT_DIR 1)

protobuf_generate_cpp(
    GENERATED_SOURCES GENERATED_HEADERS
    ${CMAKE_SOURCE_DIR}/messages/Sum.proto
)

add_executable(client client.cpp ${GENERATED_SOURCES})
target_include_directories(client PUBLIC ${INCLUDES})
target_link_libraries(client PUBLIC ${LIBS})

add_executable(server server.cpp ${GENERATED_SOURCES})
target_include_directories(server PUBLIC ${INCLUDES})
target_link_libraries(server PUBLIC ${LIBS})

