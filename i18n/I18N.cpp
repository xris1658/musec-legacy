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

bool loadTranslationFile(const QString& filePath, const QString& language)
{
    translator->load(QCoreApplication::applicationDirPath()
        .append(QString("/translations/qt_%1.qm").arg(language)));
    translator->load(QCoreApplication::applicationDirPath()
        .append(QString("/translations/qtbase_%1.qm").arg(language)));
    translator->load(QCoreApplication::applicationDirPath()
        .append(QString("/translations/qtquickcontrols_%1.qm").arg(language)));
    translator->load(QCoreApplication::applicationDirPath()
        .append(QString("/translations/qtquickcontrols2_%1.qm").arg(language)));
    return translator->load(filePath) && QCoreApplication::installTranslator(translator);
}
}