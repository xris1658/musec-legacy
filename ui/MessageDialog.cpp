#include "MessageDialog.hpp"

#include "event/EventBase.hpp"

namespace Musec::UI::MessageDialog
{
void messageDialog(const QString& message, const QString& title, int icon)
{
    Musec::Event::eventHandler->messageDialog(message, title, icon);
}
}
