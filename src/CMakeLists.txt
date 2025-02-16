protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS gtfs-realtime.proto)

add_executable(gtfs_realtime
    main.cpp
    ${PROTO_SRCS}
    ${PROTO_HDRS}
)

target_include_directories(gtfs_realtime PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Protobuf_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
)

target_link_libraries(gtfs_realtime PRIVATE
    ${Protobuf_LIBRARIES}
    ${CURL_LIBRARIES}
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(gtfs_realtime PRIVATE -Wall -Wextra)
endif()

