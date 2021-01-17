#include "GameplayManager.h"

#include <QTimer>

GameplayManager::GameplayManager(QObject *parent) :
    QObject(parent)
{
    m_settingsManager = new GameSettingsManager(this);
    this->restartGame();
}

void GameplayManager::restartGame()
{
    this->setAttemptsCount(0);

    m_matchesCount = 0;
    m_attemptState = nullptr;
    if (m_fieldModel == nullptr)
    {
        m_fieldModel = new CardsFieldModel(m_totalCardsCount, m_cardUsagesCount, this);
        connect(m_fieldModel, &QAbstractItemModel::dataChanged, this, &GameplayManager::onFieldModelDataChanged);
    }
    else
    {
        m_fieldModel->resetModel(m_totalCardsCount, m_cardUsagesCount);
    }
}

CardsFieldModel* GameplayManager::getCardsFieldModel() const
{
    return m_fieldModel;
}

// Q_PROPERTY
int GameplayManager::attemptsCount() const
{
    return m_attemptsCount;
}

int GameplayManager::bestScore() const
{
    return m_settingsManager->getBestScore();
}
// Q_PROPERTY

void GameplayManager::setAttemptsCount(int attemptsCount)
{
    m_attemptsCount = attemptsCount;
    emit attemptsCountChanged(m_attemptsCount);
}

void GameplayManager::setBestScore(int newBestScore)
{
     m_settingsManager->setBestScore(newBestScore);
     emit bestScoreChanged(this->bestScore());
}

void GameplayManager::onFieldModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if (topLeft != bottomRight)
    {
        qFatal("Different model cells cannot be changed simultaneously! Invalid scenario.");
    }

    if (roles.size() != 1)
    {
        qFatal("Only one model item property can be changed in a moment! Invalid scenario.");
    }

    CardsFieldModel::Roles role = static_cast<CardsFieldModel::Roles>(roles[0]);

    switch (role)
    {
    case CardsFieldModel::ShownRole:
    {
        bool isCardSelected = m_fieldModel->data(topLeft, role).toBool();
        if (isCardSelected)
        {
            int id = m_fieldModel->data(topLeft, CardsFieldModel::IdRole).toInt();
            processCardSelection(topLeft.row(), id);
        }
        break;
    }
    default: return; // process ShownRole change only
    }
}

void GameplayManager::processCardSelection(int modelIdx, int id)
{
    if (m_attemptState == nullptr) // first card selected
    {
        int defaultVal = -1;
        m_attemptState = std::make_unique<AttemptState>(AttemptState{defaultVal, defaultVal, defaultVal, defaultVal});
        m_attemptState->firstCardModelIdx = modelIdx;
        m_attemptState->firstCardId = id;
    }
    else // second card selected
    {
        this->setAttemptsCount(this->attemptsCount() + 1);
        m_attemptState->secondCardModelIdx = modelIdx;
        m_attemptState->secondCardId = id;
        if (m_attemptState->firstCardModelIdx == m_attemptState->secondCardModelIdx)
        {
            qFatal("One field cell selected twice! Invalid scenario");
        }
        if (m_attemptState->firstCardId == m_attemptState->secondCardId) // selected cards match
        {
            m_attemptState = nullptr;
            ++m_matchesCount;
            if (m_matchesCount == m_winMatchesCount)
            {
                int currentBestScore = this->bestScore();
                if (m_attemptsCount < currentBestScore || currentBestScore == 0)
                {
                    this->setBestScore(m_attemptsCount);
                }
                emit gameOver(m_attemptsCount, currentBestScore);
            }
        }
        else // selected cards don't match
        {
            emit changeFieldEnabled(false);
            QTimer::singleShot(m_sceneSuspendTimeMs, [this]()
            {
                m_fieldModel->setData(m_fieldModel->index(m_attemptState->firstCardModelIdx), false, CardsFieldModel::ShownRole);
                m_fieldModel->setData(m_fieldModel->index(m_attemptState->secondCardModelIdx), false, CardsFieldModel::ShownRole);
                m_attemptState = nullptr;
                emit changeFieldEnabled(true);
            });
        }
    }
}
