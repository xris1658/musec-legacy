#ifndef MUSEC_EVENT_OBJECTCREATELISTENER
#define MUSEC_EVENT_OBJECTCREATELISTENER

#include <QObject>

namespace Musec
{
namespace Event
{
class LoadQmlComponentListener: public QObject
{
    Q_OBJECT
private:
    LoadQmlComponentListener(QObject* parent = nullptr);
public:
    LoadQmlComponentListener(const LoadQmlComponentListener&) = delete;
public:
    static LoadQmlComponentListener& instance();
public slots:
    void onObjectCreated(QObject* object, const QUrl& url);
};
}
}

#endif //MUSEC_EVENT_OBJECTCREATELISTENER
