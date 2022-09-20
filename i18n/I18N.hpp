#ifndef MUSEC_I18N_I18N
#define MUSEC_I18N_I18N

#include <QString>
#include <QTranslator>

#include <tuple>
#include <vector>

namespace Musec
{
namespace I18N
{
extern QTranslator* translator;

QString readableNameFromLanguageName(const QString& name);

bool loadTranslationFile(const QString& filePath, const QString& language);
}
}

#endif //MUSEC_I18N_I18N
