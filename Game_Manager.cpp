#include "stdafx.h"
#include "Item.h"
#include "Game_Manager.h"

IMPLEMENT_SINGLETON(CGame_Manager)

CGame_Manager::CGame_Manager():
    m_iGold(0)
{

}

CGame_Manager::~CGame_Manager()
{
    while (!m_mapItemVector.empty())
        m_mapItemVector.clear();
}

void CGame_Manager::ADD_InvenItemKey(wstring wstrKey, CGameObject* pitem)
{
    if (m_mapItemVector[wstrKey].size() > 5)
        return;

    m_mapItemVector[wstrKey].push_back(pitem);
}

void CGame_Manager::Delete_InvenItemKey(wstring wstrKey, CGameObject* pitem)
{
    auto iterfind = find(m_mapItemVector[wstrKey].begin(), m_mapItemVector[wstrKey].end(), pitem);
    if (iterfind != m_mapItemVector[wstrKey].end())
    {
        m_mapItemVector[wstrKey].erase(iterfind);
    }
}

vector<CGameObject*> CGame_Manager::Get_InventoryID(wstring &wstrKey)
{
    auto iterFind = m_mapItemVector.find(wstrKey);
    if (iterFind == m_mapItemVector.end())
    {
       // m_mapItemList[wstrKey].emplace_back();
        return m_mapItemVector[wstrKey];
    }
    return iterFind->second;;
}

HRESULT CGame_Manager::Ready_Game_Manager()
{
    m_iGold = 0;

    ITEM_DESC* pitem_DESC = new ITEM_DESC;
    pitem_DESC->eType = CONSUMABLE;
    pitem_DESC->dwDrawId = 0;
    pitem_DESC->iTemCount1 = 10;
    pitem_DESC->iTemCount2 = 0;
    pitem_DESC->iTemCount3 = 0;

    CGameObject* pObject = CItem::Create((LPVOID*)pitem_DESC);

    m_mapItemVector[L"Eirik"].push_back(pObject);
    m_mapItemVector[L"Eirik"].push_back(pObject);

    m_mapItemVector[L"Thief"].push_back(pObject);

    m_mapItemVector[L"Paladin"].push_back(pObject);

    pitem_DESC->iTemCount1 = 0;
    pitem_DESC->iTemCount2 = 50;
    pitem_DESC->dwDrawId = 1;

    CGameObject* pObjectTwo = CItem::Create((LPVOID*)pitem_DESC);

    m_mapItemVector[L"Paladin"].push_back(pObjectTwo);
    m_mapItemVector[L"Eirik"].push_back(pObjectTwo);
    return S_OK;
}
