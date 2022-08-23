#ifndef MUSEC_BASE_QMLBASE
#define MUSEC_BASE_QMLBASE

// If we include <QtQml>, then a lot of errors are emitted by the compiler (why?)
#include <qqml.h>

#define QML_REGISTER_TYPE(class_name, module_name, major_version, minor_version) \
    qmlRegisterType<class_name>(#module_name, major_version, minor_version, #class_name)

#endif //MUSEC_BASE_QMLBASE
