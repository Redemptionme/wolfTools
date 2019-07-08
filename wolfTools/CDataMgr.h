#pragma once

#include "CData.h"
#include <map>
#include <vector>
#include "SingletonTemplate.h"
#include <set>


class CDataMgr
{
public:
    CDataMgr();
    ~CDataMgr();

public:
    void clearData();
    void addData(const PlayerData &data);
    void addPlayData(const PlayerData &pData);
    void print();
    void calAllData();

    const std::map<int, std::vector<RoundData> > &getAllData();
    const std::set<int> &getAllJobNum();
    const std::map<int, PlayVecData> &getPlayVecData();

private:
    std::map<int, std::vector<RoundData> > m_mapRoundData;
    std::set<int>  m_setPlayerJobNum;
    std::map<int, PlayVecData> m_mapPlayerData;
};

typedef ChaSingleton<CDataMgr>  CDataMgrInst;