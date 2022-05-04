#ifndef MUSEC_MODEL_PLUGINSEQUENCEMODEL
#define MUSEC_MODEL_PLUGINSEQUENCEMODEL

#include "entities/Plugin.hpp"
#include "model/ModelBase.hpp"

#include <QAbstractListModel>

#include <memory>
#include <vector>

namespace Musec
{
namespace Model
{
class PluginSequenceModel: public QAbstractListModel
{
    Q_OBJECT
public:
    using Item = Musec::Entities::Plugin;
    using List = std::vector<Item*>;
    enum RoleNames
    {
        ValidRole = Qt::UserRole,
        ActivatedRole,
        NameRole,
        SidechainExistRole,
        SidechainEnabledRole,
        RoleNamesEnd
    };
public:
    PluginSequenceModel(const List& list);
    PluginSequenceModel(List&& list);
    explicit PluginSequenceModel(QObject* parent = nullptr);
    virtual ~PluginSequenceModel();
public:
    const List& getList() const;
    void setList(const List& list);
    void setList(List&& list);
public:
    Q_INVOKABLE int itemCount() const;
    static constexpr int columnSize();
public:
    void insert(Item* plugin, int index);
    void move(int from, int to);
    void remove(int index);
public:
    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
protected:
    RoleNamesType roleNames() const override;
private:
    List list_;
    RoleNamesType roleNames_;
};
}
}

#endif //MUSEC_MODEL_PLUGINSEQUENCEMODEL
