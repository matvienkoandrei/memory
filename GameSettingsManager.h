#ifndef GAMESETTINGSMANAGER_H
#define GAMESETTINGSMANAGER_H

#include <QObject>
#include <QSettings>

class GameSettingsManager : public QObject
{
    Q_OBJECT

public:
    GameSettingsManager(QObject *parent = nullptr);

    int getBestScore() const;
    void setBestScore(int newBestScore);

private:
    QSettings m_settings;

    const QString m_bestScoreSettingName = "best_score";
    const int m_defaultBestScore = 0;
};

#endif // GAMESETTINGSMANAGER_H
