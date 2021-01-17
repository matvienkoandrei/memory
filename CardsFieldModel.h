#ifndef CARDSFIELDMODEL_H
#define CARDSFIELDMODEL_H

#include <QObject>
#include <QAbstractListModel>

class CardsFieldModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        IdRole = Qt::UserRole + 1,
        ShownRole
    };

    struct CardItem
    {
        int id;
        bool shown;
    };

    CardsFieldModel(int fieldSize, int cardUsagesCount, QObject *parent = nullptr);

    void resetModel(int fieldSize, int cardUsagesCount);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    QList<CardItem> m_cardItems;
};

#endif // CARDSFIELDMODEL_H
