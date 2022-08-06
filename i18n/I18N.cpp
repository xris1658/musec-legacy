#include "I18N.hpp"

#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>

namespace Musec::I18N
{
QTranslator* translator = nullptr;

QString readableNameFromLanguageName(const QString& name)
{
    QLocale locale(name);
    // Didn't take RTL languages into consideration
    return locale.nativeLanguageName().append(" (").append(locale.nativeCountryName()).append(")");
}

bool loadTranslationFile(const QString& filePath)
{
    return translator->load(filePath) && QCoreApplication::installTranslator(translator);
}
}