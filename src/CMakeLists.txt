cmake_minimum_required(VERSION 3.0)
project(QAcTime)
 
set(CMAKE_CXX_STANDARD 11)

# Эта настройка для моей машины с линуксом
# set(CMAKE_PREFIX_PATH "/home/iscander/Qt/5.5/gcc/lib/cmake")
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
set(SOFT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
 
# включаем поиск библиотек с использованием find_package()
# через REQUIRED указываем, что библиотеки обязательны
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)

qt5_add_resources(RCC_RESOURCES
    qactime.qrc
    )
 
set(SOURCE_FILES
    common.cpp
    common.h
    database.cpp
    database.h
    dateselector.cpp
    dateselector.h
    dateselector.ui
    deleteactivitydialog.cpp
    deleteactivitydialog.h
    deleteactivitydialog.ui
    filtermodel.cpp
    filtermodel.h
    main.cpp
    neweditactivitydialog.cpp
    neweditactivitydialog.h
    neweditactivitydialog.ui
    qactime.cpp
    qactime.h
    QAcTime.rc
    qactime.ui
    )
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${RCC_RESOURCES})

# указываем, какие библиотеки необходимо подключить
target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Sql)
