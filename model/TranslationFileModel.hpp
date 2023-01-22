#ifndef MUSEC_MODEL_TRANSLATIONFILEMODEL
#define MUSEC_MODEL_TRANSLATIONFILEMODEL

#include "model/ModelBase.hpp"

#include <QAbstractListModel>

#include <tuple>
#include <vector>

namespace Musec
{
namespace Model
{
class TranslationFileModel: public QAbstractListModel
{
    Q_OBJECT
public:
    using Item = std::tuple<QString, QString, QString>;
    using List = std::vector<Item>;
    enum RoleNames
    {
        PathRole = Qt::UserRole,
        LanguageNameRole,
        ReadableLanguageNameRole,
        RoleEnd
    };
public:
    explicit TranslationFileModel(QObject* parent = nullptr);
    ~TranslationFileModel();
public:
    const Item& operator[](int index) const;
    int itemCount() const;
    static constexpr int columnSize() noexcept;
public:
    virtual int rowCount(const QModelIndex&) const override;
    virtual int columnCount(const QModelIndex&) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
protected:
    virtual RoleNamesType roleNames() const override;
private:
    List list_;
};
}
}

#endif //MUSEC_MODEL_TRANSLATIONFILEMODEL
