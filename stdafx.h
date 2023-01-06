// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//
#include <queue>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.


///����
#include <io.h>
#include "fmod.h"
#pragma comment(lib, "fmodex_vc.lib")

#include "Headers.h"

#pragma comment(lib, "d3d9.lib")

#pragma comment(lib, "d3dx9.lib")

#include "Scroll_Manager.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"
#include "Scene_Manager.h"
#include "Key_Manager.h"
#include "ClientFunction.h"
#include "Combat_Manager.h"
#include "AStar_Manager.h"
#include "Spawn_Manager.h"
#include "UI_Manager.h"
#include "Dialog_Manager.h"
#include "Effect_Manager.h"
#include "Game_Manager.h"
#include "AnimClip_Manager.h"
#include "SoundMgr.h"


#include "resource.h"
