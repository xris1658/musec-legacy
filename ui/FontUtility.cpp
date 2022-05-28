#include "FontUtility.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QFontDatabase>

namespace Musec::UI
{
QString& FontUtility::ttfSuffix()
{
    static QString ret = "ttf";
    return ret;
}
QString& FontUtility::otfSuffix()
{
    static QString ret = "otf";
    return ret;
}
QString& FontUtility::ttcSuffix()
{
    static QString ret = "ttc";
    return ret;
}
bool FontUtility::suffixIsFont(const QString& suffix)
{
    return suffix == Musec::UI::FontUtility::ttfSuffix()
        || suffix == Musec::UI::FontUtility::otfSuffix()
        || suffix == Musec::UI::FontUtility::ttcSuffix();
}
void FontUtility::loadFonts()
{
    QDir fontDir(QCoreApplication::applicationDirPath().append("/Resources/fonts"));
    auto entryList = fontDir.entryInfoList(QDir::Filter::NoFilter, QDir::SortFlag::NoSort);
    for(auto& i: entryList)
    {
        if(!i.isDir() && suffixIsFont(i.suffix()))
        {
            QFontDatabase::addApplicationFont(i.absoluteFilePath());
        }
    }
}
}
