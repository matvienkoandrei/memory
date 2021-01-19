#ifndef GAMEPLAYMANAGER_H
#define GAMEPLAYMANAGER_H

#include <QObject>
#include <QQmlContext>

#include <memory>

#include "CardsFieldModel.h"
#include "GameSettingsManager.h"

class GameplayManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int attemptsCount READ   attemptsCount
                                 NOTIFY attemptsCountChanged)

    Q_PROPERTY(int bestScore     READ   bestScore
                                 NOTIFY bestScoreChanged)

public:

    struct AttemptState
    {
        int firstCardModelIdx;
        int firstCardId;
        int secondCardModelIdx;
        int secondCardId;
    };

    GameplayManager(QObject *parent = nullptr);

    Q_INVOKABLE void restartGame();

    bool registerInQml(QQmlContext* context);

    // Q_PROPERTY
    int attemptsCount() const;
    int bestScore() const;
    // Q_PROPERTY

signals:
    void gameOver(int attemptsCount, int currentBestScore);
    void changeFieldEnabled(bool enabled);

    // Q_PROPERTY
    void attemptsCountChanged(int);
    void bestScoreChanged(int);
    // Q_PROPERTY

private slots:
    void onFieldModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);

private:
    void processCardSelection(int modelIdx, int id);
    void setAttemptsCount(int attemptsCount);
    void setBestScore(int newBestScore);

private:
    const int m_sideCardsCount = 10;
    const int m_totalCardsCount = m_sideCardsCount * m_sideCardsCount;
    const int m_cardUsagesCount = 2;
    const int m_sceneSuspendTimeMs = 500; // timeout value used to suspend scene after failed attempt, so player could better remember cards
    const int m_winMatchesCount = m_totalCardsCount / m_cardUsagesCount;

    CardsFieldModel *m_fieldModel = nullptr;
    GameSettingsManager* m_settingsManager = nullptr;

    std::unique_ptr<AttemptState> m_attemptState = nullptr;

    int m_attemptsCount = 0;
    int m_matchesCount = 0;
};

#endif // GAMEPLAYMANAGER_H
