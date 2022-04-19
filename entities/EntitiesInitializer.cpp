#include "EntitiesInitializer.hpp"

#include "base/QmlBase.hpp"
#include "entities/CompleteTrack.hpp"

namespace Musec::Entities
{
EntitiesInitializer::EntitiesInitializer()
{
    QML_REGISTER_TYPE(CompleteTrack, Musec.Entities, 1, 0);
}

void EntitiesInitializer::initialize()
{
    static EntitiesInitializer init;
}
}
