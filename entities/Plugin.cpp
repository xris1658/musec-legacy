#include "Plugin.hpp"

namespace Musec
{
namespace Entities
{
Plugin::Plugin(QObject* parent, const QString& name, bool enabled,
               bool sidechainExist, bool sidechainEnabled):
    QObject(parent),
    name_(name),
    enabled_(enabled),
    sidechainExist_(sidechainExist),
    sidechainEnabled_(sidechainEnabled)
{

}

const QString& Plugin::getName() const
{
    return name_;
}

void Plugin::setName(const QString& name)
{
    name_ = name;
}

bool Plugin::isEnabled() const
{
    return enabled_;
}

void Plugin::setEnabled(bool enabled)
{
    enabled_ = enabled;
}

bool Plugin::isSidechainExist() const
{
    return sidechainExist_;
}

void Plugin::setSidechainExist(bool sidechainExist)
{
    sidechainExist_ = sidechainExist;
}

bool Plugin::isSidechainEnabled() const
{
    return sidechainEnabled_;
}

void Plugin::setSidechainEnabled(bool sidechainEnabled)
{
    sidechainEnabled_ = sidechainEnabled;
}
}
}