#include "pch.h"
#include "CData.h"
#include "CAnalysisTools.h"
#include <utility>


void PlayerData::AnalysisData(int nday)
{
    day = nday;
    round = atoi(sdata[eProPData_Round].c_str());
    num = atoi(sdata[eProPData_Num].c_str());
    name = sdata[eProPData_Name].c_str();
    jobNum = atoi(sdata[eProPData_JobNum].c_str());
    name = sdata[eProPData_Name].c_str();
    gameCard = CAnalysisToolsInst::singleton()->getGameCard(sdata[eProPData_GameCard].c_str());
    luckGameCard = CAnalysisToolsInst::singleton()->getGameCard(sdata[eProPData_LuckGameCard].c_str());
    // 5day 省略
    winScore = atoi(sdata[eProPData_WinScore].c_str());
    exScore = atoi(sdata[eProPData_exScore].c_str());
    totalScore = atoi(sdata[eProPData_TotalScore].c_str());
}

int PlayVecData::getRounds(eGameCard card)
{
    std::map<eGameCard, WinRate>::iterator iter = mapWinRate.find(card);
    if (iter != mapWinRate.end()) {
        return iter->second.allRound;
    }

    return 0;
}

void PlayVecData::CalAllData()
{
    this->CalWinRateData();
    this->CalScoreData();
}

void PlayVecData::CalWinRateData()
{
    mapWinRate.clear();
    const WinRate nullRate;
    mapWinRate.insert(std::make_pair(eGameCard_Good, nullRate));
    mapWinRate.insert(std::make_pair(eGameCard_Bad, nullRate));
    mapWinRate.insert(std::make_pair(eGameCard_All, nullRate));


    for (int i = 0; i < vecData.size(); i++) {
        const PlayerData &data = vecData[i];

        // 先不管幸运儿吧
        std::map<eGameCard, WinRate>::iterator iter = mapWinRate.find(data.gameCard);
        if (iter == mapWinRate.end()) {
            mapWinRate.insert(std::make_pair(data.gameCard, nullRate));
        }
        WinRate &rate = mapWinRate[data.gameCard];
        rate.allRound++;
        bool bWin = false;
        if (data.winScore > 0) {
            bWin = true;
        }
        if (bWin) {
            rate.winRound++;
        }

        // 如果是狼
        if (data.gameCard < eGameCard_Cm ) {
            if (bWin) {
                mapWinRate[eGameCard_Bad].winRound++;
            }
            mapWinRate[eGameCard_Bad].allRound++;            
        } else if (data.gameCard < eGameCard_Count ) {            
            if (bWin) {
                mapWinRate[eGameCard_Good].winRound++;
            }
            mapWinRate[eGameCard_Good].allRound++;
        }

        mapWinRate[eGameCard_All].allRound++;
        if (bWin) {
            mapWinRate[eGameCard_All].winRound++;
        }
    }

    for (std::map<eGameCard, WinRate>::iterator iterCal = mapWinRate.begin();
        iterCal != mapWinRate.end();
        iterCal++) {
        iterCal->second.Cal();
    }
}

void PlayVecData::CalScoreData()
{
    mapScore.clear();
    const ScoreData nullData;
    mapScore.insert(std::make_pair(eGameCard_Good, nullData));
    mapScore.insert(std::make_pair(eGameCard_Bad, nullData));
    mapScore.insert(std::make_pair(eGameCard_All, nullData));

    for (int i = 0; i < vecData.size(); i++) {
        const PlayerData &data = vecData[i];

        std::map<eGameCard, ScoreData>::iterator iter = mapScore.find(data.gameCard);
        if (iter == mapScore.end()) {            
            mapScore.insert(std::make_pair(data.gameCard, nullData));
        }

        ScoreData &scoreData = mapScore[data.gameCard];
        scoreData.totalRound++;
        scoreData.totalScore += data.totalScore;

        // 幸运儿有分数，且主身份副身份不相同，因为相同就没必要重复计算
        if (data.luckGameCard != eGameCard_None && data.luckGameCard != data.gameCard) {
            mapScore[data.luckGameCard].totalRound++;
            mapScore[data.luckGameCard].totalScore += data.totalScore;
        }

        // 如果是狼
        if (data.gameCard < eGameCard_Cm) {            
            mapScore[eGameCard_Bad].totalRound++;
            mapScore[eGameCard_Bad].totalScore += data.totalScore;
        }
        else if (data.gameCard < eGameCard_Count) {
            mapScore[eGameCard_Good].totalRound++;
            mapScore[eGameCard_Good].totalScore += data.totalScore;
        }
        mapScore[eGameCard_All].totalRound++;
        mapScore[eGameCard_All].totalScore += data.totalScore;
    }


    for (std::map<eGameCard, ScoreData>::iterator iterCal = mapScore.begin();
        iterCal != mapScore.end();
        iterCal++) {
        iterCal->second.Cal();
    }
}

