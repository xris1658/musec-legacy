#ifndef MUSEC_BASE_QMLBASE
#define MUSEC_BASE_QMLBASE

// 包含 <QtQml> 则编译器会报告一系列不相关的错误（为什么？）
#include <qqml.h>

#define QML_REGISTER_TYPE(class_name, module_name, major_version, minor_version) \
    qmlRegisterType<class_name>(#module_name, major_version, minor_version, #class_name)

#endif //MUSEC_BASE_QMLBASE
