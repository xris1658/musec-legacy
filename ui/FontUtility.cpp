#include "FontUtility.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QFontDatabase>

namespace Musec::UI
{
const char* ttfSuffix = "ttf";
const char* otfSuffix = "otf";
const char* ttcSuffix = "ttc";

bool FontUtility::suffixIsFont(const QString& suffix)
{
    return suffix == ttfSuffix
        || suffix == otfSuffix
        || suffix == ttcSuffix;
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
