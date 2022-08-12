#include "ObjectCreateListener.hpp"

#include <QUrl>

#include <stdexcept>

namespace Musec::Event
{
LoadQmlComponentListener::LoadQmlComponentListener(QObject* parent):
    QObject(parent)
{

}

LoadQmlComponentListener& LoadQmlComponentListener::instance()
{
    static LoadQmlComponentListener ret;
    return ret;
}

void LoadQmlComponentListener::onObjectCreated(QObject* object, const QUrl& url)
{
    auto string = url.toString().toStdString();
    auto displayString = url.toDisplayString().toStdString();
    auto localFile = url.toLocalFile().toStdString();
    // auto isLocal = url.isLocalFile();
    // auto isValid = url.isValid();
    auto query = url.query().toStdString();
    if(!object)
    {
        throw std::runtime_error("Error: QML object is not created as expected.");
    }
}
}
