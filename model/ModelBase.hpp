#ifndef MUSEC_MODEL_MODELBASE
#define MUSEC_MODEL_MODELBASE

#include <QHash>
#include <QByteArray>

#define QML_REGISTER_TYPE(class_name, module_name, major_version, minor_version) \
    qmlRegisterType<class_name>(#module_name, major_version, minor_version, #class_name)

namespace Musec
{
namespace Model
{
using RoleNamesType = QHash<int, QByteArray>;

template<typename... Types>
using ItemType = std::tuple<Types...>;
}
}

#endif // MUSEC_MODEL_MODELBASE
