# Переменная PWD разворачивается в путь к файлу *.pro/*.pri, в котором используется
BUILD_DIR = $${PWD}/build

# В зависимости от типа билда, debug или release, устанавливаем соответствующее
# значение переменной BUILD_TYPE
CONFIG(release, debug|release){
    message(Release)
    BUILD_TYPE = release
}else{
    message(Debug)
    BUILD_TYPE = debug
}

DEST_BINS = $${BUILD_DIR}/$${BUILD_TYPE}

# В зависимости от платформы, на которой собираем проект, устанавливаем
# расположение исполняемых файлов.
# Регулярное выражение "s,/,\\,g" в SOME_VAR ~= s,/,\\,g означает
# "заменить в переменной SOME_VAR прямой слэш ("/") на обратный слэш ("\")".
win32 {
    DEST_BINS ~= s,/,\\,g
    DESTDIR = $${DEST_BINS}
}

linux {
    DESTDIR = $${DEST_BINS}
}

# Остальные файлы, которые генерируются по ходу сборки, выносим за пределы репозитория.
# Переменная OUT_PWD разворачивается в путь построения билда,
# заданный в Qt Creator в настройках проекта.
MOC_DIR        = $${OUT_PWD}/$${BUILD_TYPE}/moc
UI_DIR         = $${OUT_PWD}/$${BUILD_TYPE}/ui
UI_HEADERS_DIR = $${OUT_PWD}/$${BUILD_TYPE}/ui
UI_SOURCES_DIR = $${OUT_PWD}/$${BUILD_TYPE}/ui
OBJECTS_DIR    = $${OUT_PWD}/$${BUILD_TYPE}/obj
RCC_DIR        = $${OUT_PWD}/$${BUILD_TYPE}/rcc
