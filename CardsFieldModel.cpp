#include "CardsFieldModel.h"

#include <algorithm>
#include <random>
#include <chrono>

CardsFieldModel::CardsFieldModel(int fieldSize, int cardUsagesCount, QObject *parent) :
    QAbstractListModel(parent)
{
    this->resetModel(fieldSize, cardUsagesCount);
}

void CardsFieldModel::resetModel(int fieldSize, int cardUsagesCount)
{
    int differentCardsCount = fieldSize / cardUsagesCount;

    beginResetModel();

    m_cardItems.clear();

    for (int i = 0; i < differentCardsCount; i++)
    {
        for (int j = 0; j < cardUsagesCount; j++)
        {
            m_cardItems.append({i, false});
        }
    }
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(m_cardItems.begin(), m_cardItems.end(), std::default_random_engine(seed));

    endResetModel();
}


int CardsFieldModel::rowCount(const QModelIndex &) const
{
    return m_cardItems.size();
}

QVariant CardsFieldModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    int rowIdx = index.row();
    if (rowIdx < 0 || rowIdx >= m_cardItems.size())
    {
        return QVariant();
    }

    switch (role)
    {
        case IdRole: return m_cardItems[rowIdx].id;
        case ShownRole: return m_cardItems[rowIdx].shown;
        default: return QVariant();
    }
}

bool CardsFieldModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
    {
        return false;
    }

    int rowIdx = index.row();
    if (rowIdx < 0 || rowIdx >= m_cardItems.size())
    {
        return false;
    }

    switch (role)
    {
    case ShownRole: m_cardItems[rowIdx].shown = value.toBool(); break;
    default: return false;
    }

    emit dataChanged(index, index, {role});
    return true;
}

QHash<int, QByteArray> CardsFieldModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[IdRole] = "id";
    roles[ShownRole] = "shown";

    return roles;
}
