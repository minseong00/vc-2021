// WORMGAME2.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WormGame1.h"

#include <time.h>
#include <stdlib.h>



#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WORMGAME1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WORMGAME1));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WORMGAME1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WORMGAME1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

//지렁이 아이템 벽 좌표선언
RECT  worm[30], item[2];
//RECT wall[28][38];
RECT wall;

int worm1[30];
//장애물 좌표선언
RECT obstacle[20];

//지렁이 방향 확인
int w_flag = 3;
//아이템 확인
int i_flag = 0;
//방향키 체크 여부
int testkey = 4;
//기본 for문 변수
int i;
//장애물 카운트 변수
int x;
WCHAR p[20] = { 0 , };

//게임 시작 여부 체크
BOOL checkStart = FALSE;
RECT dst;

//타이머
int g_timer;
//점수
int g_score = 0;
//게임모드
int checkMode = 0;


#define TIMER_1 1 //게임 타이머
#define TIMER_2 2 //지렁이 스피드
#define TIMER_3 3

int g_cnt = 1;
int g_item = 0;





LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HBRUSH brush, brush1;

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;

        case 0:
        {
            checkMode = 1;
            checkStart = TRUE;

            MessageBox(hWnd, TEXT("일반모드로 시작합니다"), TEXT("게임시작"), MB_OK);
        }
        break;

        case 1:
        {
            checkMode = 2;
            checkStart = TRUE;

            MessageBox(hWnd, TEXT("헬모드로 시작합니다"), TEXT("게임시작"), MB_OK);
        }
        break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_CREATE:
    {
        int o = 0;
        int o2 = 0;
        int x = 420;
        int y = 420;
        //윈도우 창 크기
        SetWindowPos(hWnd, NULL, 200, 100, 1000, 680, 0);


        //타이머 세팅
        srand((unsigned int)time(NULL));

        //버튼 생성
        CreateWindow(TEXT("button"), TEXT("일반모드"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            810, 200, 100, 20, hWnd, (HMENU)0, hInst, NULL);
        CreateWindow(TEXT("button"), TEXT("어려움모드"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            810, 250, 100, 20, hWnd, (HMENU)1, hInst, NULL);

        //게임 벽 크기
        wall.left = 20;
        wall.top = 20;
        wall.right = 800;
        wall.bottom = 600;
        //for (int q = 0; q < 28; q++)
        //{
        //    for (int e = 0; e < 38; e++)
        //    {
        //        wall[q][e].left = x;
        //        wall[q][e].top = y;
        //        wall[q][e].right = wall[q][e].left + 20;
        //        wall[q][e].bottom = wall[q][e].top + 20;
        //        x += 20;
        //    }
        //    y += 20;
        //    x = 20;
        //}

        //지렁이 좌표 구성
        for (i = 0; i < 3; i++)
        {
            worm[i].left = x;
            worm[i].top = y;
            worm[i].right = worm[i].left + 20;
            worm[i].bottom = worm[i].top + 20;
            x += 20;
            y += 20;
        }


        //아이템 좌표 구성
        for (i = 0; i < 2; i++)
        {
            item[i].left = rand() % 38 * 20 + 20;
            item[i].top = rand() % 27 * 20 + 20;
            item[i].right = item[i].left + 40;
            item[i].bottom = item[i].top + 40;
        }


        //지렁이 스피드
        SetTimer(hWnd, TIMER_2, 200, NULL);

        //게임 제한시간 15초
        g_timer = 20;
        SetTimer(hWnd, TIMER_1, 1000, NULL);

        //화면 무효화용
        SetTimer(hWnd, TIMER_3, 200, NULL);

    }
    break;

    if (checkStart == TRUE)
    {
    case WM_TIMER:
    {
        switch (wParam)
        {
        case TIMER_1: //게임 타이머
        {
            g_timer--;

            /* if (0 == g_timer)
             {
                 KillTimer(hWnd, TIMER_1);
                 KillTimer(hWnd, TIMER_2);
                 MessageBox(hWnd, L"타임오버 종료", L"gameover", MB_OK);
                 checkStart = FALSE;
             }*/




        }
        break;


        case TIMER_2: //지렁이 이동
        {

            switch (w_flag) //방향키 입력에 따라 머리가 이동한다.
            {
            case 1: // 왼쪽 방향키
                if (testkey != 2)
                {
                    // 1번방부터 20번방까지 (g_cnt) 순서대로 방을 올라가며 전 방의 위치를 준다.
                    for (i = 1; i < g_cnt; i++)
                    {
                        worm1[30 - 1] = worm[i - 1].left;

                        /*worm[i].left = worm1[i];*/
                       /* worm[i].top = worm[i - 1].top;
                        worm[i].right = worm[i].left + 20;
                        worm[i].bottom = worm[i].top + 20;*/
                    }
                    // 1번방이 새로운 위치로 떠난다 = 새로운 위치의 정보를 받는다
                    worm[0].left -= 20;
                    worm[0].right -= 20; 
                }
                break;
            case 2: // 오른쪽 방향키
                if (testkey != 1)
                {
                    for (i = 1; i < g_cnt; i++)
                    {
                        //if (g_item != 1)
                        //    break;

                        worm[i].left = worm[i - 1].left;
                        worm[i].top = worm[i - 1].top;
                        worm[i].right = worm[i].left + 20;
                        worm[i].bottom = worm[i].top + 20;
                    }
                    if (g_cnt < 20)
                        g_cnt++;
                    worm[0].left += 20;
                    worm[0].right += 20;
                }
                break;
            case 3: // 위 방향키
                if (testkey != 4)
                {
                    for (i = 1; i < g_cnt; i++)
                    {
                        //if (g_item != 1)
                        //    break;

                        worm[i].left = worm[i - 1].left;
                        worm[i].top = worm[i - 1].top;
                        worm[i].right = worm[i].left + 20;
                        worm[i].bottom = worm[i].top + 20;
                    }
                    if (g_cnt < 20)
                        g_cnt++;
                    worm[0].top -= 20;
                    worm[0].bottom -= 20;
                }
                break;
            case 4: // 아래 방향키
                if (testkey != 3)
                {
                    for (i = 1; i < g_cnt; i++)
                    {
                        //if (g_item != 1)
                        //    break;

                        worm[i].left = worm[i - 1].left;
                        worm[i].top = worm[i - 1].top;
                        worm[i].right = worm[i].left + 20;
                        worm[i].bottom = worm[i].top + 20;
                    }
                    if (g_cnt < 20)
                        g_cnt++;
                    worm[0].top += 20;
                    worm[0].bottom += 20;
                }
                break;
            }

            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

        case TIMER_3: // 체크용
        {
            for (i = 0; i < 2; i++)
            {
                if (TRUE == IntersectRect(&dst, &worm[0], &item[i]))
                {
                    g_cnt++;
                    g_timer += 6;
                    g_score += 100;
                    i_flag++;
                    g_item++;
                    item[i].left = rand() % 38 * 20 + 20;
                    item[i].top = rand() % 27 * 20 + 20;
                    item[i].right = item[i].left + 40;
                    item[i].bottom = item[i].top + 40;

                    obstacle[i_flag].left = rand() % 37 * 20 + 20;
                    obstacle[i_flag].top = rand() % 26 * 20 + 20;
                    obstacle[i_flag].right = obstacle[i_flag].left + 60;
                    obstacle[i_flag].bottom = obstacle[i_flag].top + 60;
                }
            }
            
            /*if (worm[0].left <= 20 || worm[0].right >= 800 || worm[0].top <= 20 || worm[0].bottom >= 600)
            {
                KillTimer(hWnd, TIMER_1);
                KillTimer(hWnd, TIMER_2);
                MessageBox(hWnd, L"벽에 닿았습니다", L"gameover", MB_OK);
                checkStart = FALSE;
                // 모든 생성 초기화코드 넣기
            }
            for (i = 0; i < (sizeof(obstacle) / sizeof(obstacle[0])); i++)
            {
                if (TRUE == IntersectRect(&dst, &worm[0], &obstacle[i]))
                {
                    KillTimer(hWnd, TIMER_1);
                    KillTimer(hWnd, TIMER_2);
                    MessageBox(hWnd, L"장애물에 닿았습니다", L"gameover", MB_OK);
                    checkStart = FALSE;
                    // 모든 생성 초기화코드 넣기

                }
            }*/


            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;



        }
    }
    break;

    

    case WM_KEYDOWN:
    {
        RECT dst;
        switch (wParam) //방향키 버튼 입력으로 지렁이의 방향 설정
        {
        case VK_LEFT:
            /*  if (testkey != 2)
              {

              }*/
            testkey = w_flag;
            w_flag = 1;
            g_cnt++;
            break;

        case VK_RIGHT:
            /* if (testkey != 1)
             {

             }*/
            testkey = w_flag;
            w_flag = 2;
            g_cnt++;
            break;

        case VK_UP:
            /* if (testkey != 4)
             {

             }*/
            testkey = w_flag;
            w_flag = 3;
            g_cnt++;
            break;

        case VK_DOWN:
            /*if (testkey != 3)
            {

            }*/
            testkey = w_flag;
            w_flag = 4;
            g_cnt++;
            break;


        }

        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;

    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        RECT txt;

        WCHAR index[64] = { 0, };
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        //for(int a = 0; a < 28; a++)                
        //{
        //    for (int b = 0; b < 38; b++)
        //    {
        //        Rectangle(hdc, wall[a][b].left, wall[a][b].top, wall[a][b].right, wall[a][b].bottom);
        //    }
        //}
        Rectangle(hdc, wall.left, wall.top, wall.right, wall.bottom);

        txt.left = 830;
        txt.top = 50;
        txt.right = 1000;
        txt.bottom = 200;
        wsprintfW(index, L"점수 : %d\n제한시간 : %d", g_score, g_timer);
        DrawText(hdc, index, -1, &txt, DT_LEFT | DT_WORDBREAK);
            //wsprintfW(p, L"%d", worm[3].left);
            //MessageBox(hWnd, p,p, MB_OK);

        if (checkStart == TRUE)
        {
            if (checkMode == 1)
            {
                for (int c = 0; c < g_cnt; c++)
                {
                    Ellipse(hdc, worm[c].left, worm[c].top, worm[c].right, worm[c].bottom);
                }
                for (int c = 0; c < (sizeof(item) / sizeof(item[0])); c++)
                {
                    Ellipse(hdc, item[c].left, item[c].top, item[c].right, item[c].bottom);
                }

            }
            else if (checkMode == 2)
            {
                for (int c = 0; c < g_cnt; c++)
                {
                    Ellipse(hdc, worm[c].left, worm[c].top, worm[c].right, worm[c].bottom);
                }
                for (int c = 0; c < (sizeof(item) / sizeof(item[0])); c++)
                {
                    Ellipse(hdc, item[c].left, item[c].top, item[c].right, item[c].bottom);
                }
                for (x = 0; x < (sizeof(obstacle) / sizeof(obstacle[0])); x++)
                {
                    Rectangle(hdc, obstacle[x].left, obstacle[x].top, obstacle[x].right, obstacle[x].bottom);
                }
            }

        }

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}




// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
