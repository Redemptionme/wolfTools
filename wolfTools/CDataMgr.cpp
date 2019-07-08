#include "pch.h"
#include "CDataMgr.h"
#include <utility>



CDataMgr::CDataMgr()
{
}

CDataMgr::~CDataMgr()
{
}

void CDataMgr::clearData()
{
    m_mapRoundData.clear();
    m_setPlayerJobNum.clear();
    m_mapPlayerData.clear();
}

void CDataMgr::addData(const PlayerData &data)
{
    m_setPlayerJobNum.insert(data.jobNum);
    this->addPlayData(data);

    int day = data.day;

    std::map<int, std::vector<RoundData> >::iterator iter = m_mapRoundData.find(day);
    if (iter == m_mapRoundData.end()) {
        std::vector<RoundData> vec;
        m_mapRoundData[day] = vec;
    }
    std::vector<RoundData> &vec = m_mapRoundData[day];
    bool bIn = false;
    for (unsigned int i = 0; i < vec.size(); i++) {
        if (data.round == vec[i].round) {            
            vec[i].vecPlayerData.push_back(data);            
            bIn = true;
        }
    }
    // Èç¹û¿Õ
    if (!bIn) {
        RoundData rData;
        rData.day = day;
        rData.round = data.round;
        rData.vecPlayerData.push_back(data);        
        m_mapRoundData[day].push_back(rData);
    }
}

void CDataMgr::addPlayData(const PlayerData &data)
{
    int jobNum = data.jobNum;
    if (m_mapPlayerData.find(jobNum) == m_mapPlayerData.end()) {
        PlayVecData vData;        
        m_mapPlayerData[jobNum].vecData.clear();
    }

    m_mapPlayerData[jobNum].vecData.push_back(data);
}

void CDataMgr::print()
{
    for (std::set<int>::iterator iter = m_setPlayerJobNum.begin(); iter != m_setPlayerJobNum.end(); iter++) {
        int jobNum = *iter;

        std::map<int, PlayVecData>::iterator iter2 = m_mapPlayerData.find(jobNum);
        /*if (iter2 != m_mapPlayerData.end()) {
            const std::vector<PlayerData> & vecData = iter2->second;
            


        }*/
    }


}

void CDataMgr::calAllData()
{
    for (std::set<int>::iterator iter = m_setPlayerJobNum.begin(); iter != m_setPlayerJobNum.end(); iter++) {
        int jobNum = *iter;
        std::map<int, PlayVecData>::iterator iter2 = m_mapPlayerData.find(jobNum);
        iter2->second.CalAllData();
    }

}

const std::map<int, std::vector<RoundData> > & CDataMgr::getAllData()
{
    return m_mapRoundData;
}

const std::set<int> & CDataMgr::getAllJobNum()
{
    return m_setPlayerJobNum;
}

const std::map<int, PlayVecData> & CDataMgr::getPlayVecData()
{
    return m_mapPlayerData;
}
