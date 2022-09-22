#ifndef MUSEC_UI_MESSAGEDIALOG
#define MUSEC_UI_MESSAGEDIALOG

#include <QString>

namespace Musec
{
namespace UI
{
namespace MessageDialog
{
// qmlproject/imports/Musec/Dialogs/MessageDialog.qml
enum IconType
{
    None,
    Question,
    Warning,
    Error,
    Check,
    Info
};

void messageDialog(const QString& message, const QString& title, int icon);
void messageDialogNonModal(const QString& message, const QString& title, int icon);
}
}
}

#endif //MUSEC_UI_MESSAGEDIALOG
