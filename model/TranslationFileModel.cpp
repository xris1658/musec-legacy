#include "TranslationFileModel.hpp"

#include "i18n/I18N.hpp"

#include <QCoreApplication>
#include <QDir>

namespace Musec::Model
{
TranslationFileModel::TranslationFileModel(QObject* parent): QAbstractListModel(parent)
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::PathRole] = "path";
    roleNames_[RoleNames::LanguageNameRole] = "language_name";
    roleNames_[RoleNames::ReadableLanguageNameRole] = "readable_language_name";
    QTranslator translator;
    static QStringList nameFilter {"Musec_*.qm"};
    static QDir appDir(QCoreApplication::applicationDirPath());
    auto entryInfoList = appDir.entryInfoList(nameFilter, QDir::Filter::Files, QDir::SortFlag::Name);
    auto& ret = list_;
    ret.reserve(entryInfoList.size() + 1);
    for(auto& entryInfo: entryInfoList)
    {
        const auto& path = entryInfo.absoluteFilePath();
        bool loadPathResult = translator.load(path);
        if(loadPathResult)
        {
            const auto& language = translator.language();
            ret.emplace_back(
                std::make_tuple(
                    path,
                    language,
                    Musec::I18N::readableNameFromLanguageName(language)
                )
            );
        }
    }
    ret.emplace_back("", "en_US", "English (US)");
}

TranslationFileModel::~TranslationFileModel()
{

}

const TranslationFileModel::Item& TranslationFileModel::operator[](int index) const
{
    return list_[index];
}

int TranslationFileModel::itemCount() const
{
    return list_.size();
}

constexpr int TranslationFileModel::columnSize()
{
    return RoleNames::RoleEnd - Qt::UserRole;
}

int TranslationFileModel::rowCount(const QModelIndex&) const
{
    return itemCount();
}

int TranslationFileModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant TranslationFileModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= rowCount(index))
    {
        return QVariant();
    }
    switch (role)
    {
    case RoleNames::PathRole:
        return QVariant::fromValue(std::get<RoleNames::PathRole - Qt::UserRole>(list_[row]));
    case RoleNames::LanguageNameRole:
        return QVariant::fromValue(std::get<RoleNames::LanguageNameRole - Qt::UserRole>(list_[row]));
    case RoleNames::ReadableLanguageNameRole:
        return QVariant::fromValue(std::get<RoleNames::ReadableLanguageNameRole - Qt::UserRole>(list_[row]));
    default:
        return QVariant();
    }
}

RoleNamesType TranslationFileModel::roleNames() const
{
    return roleNames_;
}
}