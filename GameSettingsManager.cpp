#include "GameSettingsManager.h"

GameSettingsManager::GameSettingsManager(QObject *parent) :
    QObject(parent)
{

}

int GameSettingsManager::getBestScore() const
{
    return m_settings.value(m_bestScoreSettingName, m_defaultBestScore).toInt();
}

void GameSettingsManager::setBestScore(int newBestScore)
{
    m_settings.setValue(m_bestScoreSettingName, newBestScore);
}
