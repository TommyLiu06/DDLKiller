cmake_minimum_required(VERSION 3.10)
project(ws_server)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system thread)

add_executable(ws_server
    src/main.cpp
    src/websocket/WebSocketServer.cpp
    src/websocket/WebSocketSession.cpp
)

target_include_directories(ws_server PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(ws_server PRIVATE Boost::system Boost::thread pthread)

target_include_directories(ws_server PRIVATE ${SQLITECPP_INCLUDE_DIRS})
target_link_libraries(ws_server PRIVATE
    SQLiteCpp
    sqlite3
)
