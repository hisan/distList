cmake_minimum_required(VERSION 3.16)
project(DirHold)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

set(ENV{Qt6_DIR} /home/zyb/working/Ddisk/Dev/Qt/6.7.0/gcc_64)
set(CMAKE_PREFIX_PATH $ENV{Qt6_DIR}/lib/cmake)
message("prefix path: ${CMAKE_PREFIX_PATH}")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(RESOURCES resource.qrc)

set(SOURCES
    src/myDirListWidget.cpp
    src/MainWindow.cpp
    src/dirInputWidget.cpp
    src/OperateWidget.cpp
    src/main.cpp
    ${RESOURCES}
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES} )

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Core Qt6::Widgets sqlite3
    pthread
)