cmake_minimum_required(VERSION 3.6)
project(mymiddleware)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
find_package(ZMQ REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Boost COMPONENTS system regex thread timer filesystem serialization REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(INCLUDES
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${ZMQ_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIR}
)
set(LIBS
    ${ZMQ_LIBRARY}
    ${PROTOBUF_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_TIMER_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/messages.pc.cc ${CMAKE_BINARY_DIR}/messages.pb.h
    DEPENDS messages.proto
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND protoc -I=. --cpp_out=${CMAKE_BINARY_DIR} messages.proto
)

foreach(X client server)
add_executable(${X} client-server/${X}.cpp ${CMAKE_BINARY_DIR}/messages.pb.cc)
target_include_directories(${X} PUBLIC ${INCLUDES})
target_link_libraries(${X} PUBLIC ${LIBS})
endforeach()

foreach(X publisher subscriber)
add_executable(${X} publisher-subscriber/${X}.cpp ${CMAKE_BINARY_DIR}/messages.pb.cc)
target_include_directories(${X} PUBLIC ${INCLUDES})
target_link_libraries(${X} PUBLIC ${LIBS})
endforeach()

