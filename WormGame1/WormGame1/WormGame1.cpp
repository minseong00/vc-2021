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
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(120, 60, 0));
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

//지렁이 아이템 좌표 변수
RECT  worm[100], item[2];
//벽 좌표 변수
RECT wall;
//지렁이와 장애물 좌표 체크 변수
RECT wormt;
//순서대로 이동하기 위한 지렁이 앞 몸체 좌표 변수
int worm1[100];
//장애물 좌표선언
RECT obstacle[31];

//지렁이 방향 전환 변수
int w_flag = 3;
//지렁이 방향 체크 변수
int testkey = 3;
//아이템 확인
int i_flag = 0;
int i_flag1 = 0;
//기본 for문 전용 변수
int i;
int a;
int b = 0;
//지렁이 초기좌표 변수
int x = 420;
int y = 420;

//게임 시작 여부 체크
BOOL checkStart = FALSE;
//좌표 체크용 변수
RECT dst;

//타이머
int g_timer;
//점수
int g_score = 0;
//게임모드
int checkMode = 0;


#define TIMER_1 1 //게임 타이머
#define TIMER_2 2 //지렁이 스피드
#define TIMER_3 3 //화면 무효화용

//지렁이 몸 크기
int g_cnt = 3;





LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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

        case 0: //버튼 종류에 따라 모든 초기화 후 시작
        {
            KillTimer(hWnd, TIMER_1);
            KillTimer(hWnd, TIMER_2);
            KillTimer(hWnd, TIMER_3);
            w_flag = 3;
            i_flag = 0;
            i_flag1 = 0;
            testkey = 3;
            g_score = 0;
            checkMode = 0;
            g_timer = 20;
            g_cnt = 3;
            x = 420;
            y = 420;
            b = 0;
            for (i = 0; i < 3; i++)
            {
                worm[i].left = x;
                worm[i].top = y;
                worm[i].right = worm[i].left + 20;
                worm[i].bottom = worm[i].top + 20;
                y += 20;
            } y = 420;

            while (true)
            {
                for (i = 0; i < 2; i++)
                {
                    item[i].left = rand() % 38 * 20 + 20;
                    item[i].top = rand() % 27 * 20 + 20;
                    item[i].right = item[i].left + 40;
                    item[i].bottom = item[i].top + 40;
                }
                if (FALSE == IntersectRect(&dst, &item[0], &item[1]))
                    break;
            }
            for (i = 0; i < (sizeof(obstacle) / sizeof(obstacle[0])); i++)
            {
                obstacle[i].left = 0;
                obstacle[i].top = 0;
                obstacle[i].right = 0;
                obstacle[i].bottom = 0;
            }

            checkMode = 1;
            checkStart = TRUE;

            MessageBox(hWnd, TEXT("일반모드로 시작합니다"), TEXT("게임시작"), MB_OK);
            SetTimer(hWnd, TIMER_2, 100, NULL);
            SetTimer(hWnd, TIMER_1, 1000, NULL);
            SetTimer(hWnd, TIMER_3, 1, NULL);
        }
        break;

        case 1: //버튼 종류에 따라 모든 초기화 후 시작
        {
            KillTimer(hWnd, TIMER_1);
            KillTimer(hWnd, TIMER_2);
            KillTimer(hWnd, TIMER_3);
            w_flag = 3;
            i_flag = 0;
            i_flag1 = 0;
            testkey = 3;
            g_score = 0;
            checkMode = 0;
            g_timer = 20;
            g_cnt = 3;
            x = 420;
            y = 420;
            b = 0;
            for (i = 0; i < 3; i++)
            {
                worm[i].left = x;
                worm[i].top = y;
                worm[i].right = worm[i].left + 20;
                worm[i].bottom = worm[i].top + 20;
                y += 20;
            }
            while (true)
            {
                for (i = 0; i < 2; i++)
                {
                    item[i].left = rand() % 38 * 20 + 20;
                    item[i].top = rand() % 27 * 20 + 20;
                    item[i].right = item[i].left + 40;
                    item[i].bottom = item[i].top + 40;
                }
                if (FALSE == IntersectRect(&dst, &item[0], &item[1]))
                    break;
            }
            
            for (i = 0; i < (sizeof(obstacle) / sizeof(obstacle[0])); i++)
            {
                obstacle[i].left = 0;
                obstacle[i].top = 0;
                obstacle[i].right = 0;
                obstacle[i].bottom = 0;
            }
            checkMode = 2;
            checkStart = TRUE;

            MessageBox(hWnd, TEXT("헬모드로 시작합니다"), TEXT("게임시작"), MB_OK);
            SetTimer(hWnd, TIMER_2, 100, NULL);
            SetTimer(hWnd, TIMER_1, 1000, NULL);
            SetTimer(hWnd, TIMER_3, 1, NULL);
        }
        break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_CREATE:
    {
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

        //지렁이 좌표 구성
        for (i = 0; i < 3; i++)
        {
            worm[i].left = x;
            worm[i].top = y;
            worm[i].right = worm[i].left + 20;
            worm[i].bottom = worm[i].top + 20;
            y += 20;
        } y = 420;



        //아이템 좌표 구성
        while (true)
        {
            for (i = 0; i < 2; i++)
            {
                item[i].left = rand() % 38 * 20 + 20;
                item[i].top = rand() % 27 * 20 + 20;
                item[i].right = item[i].left + 40;
                item[i].bottom = item[i].top + 40;
            }
            if (FALSE == IntersectRect(&dst, &item[0], &item[1]))
                break;
        }


        MessageBox(hWnd, L"뒤를 돌때 방향키를 너무 빠르게 연속으로 누르지 마세요!!", L"TIP", NULL);

        //지렁이 스피드
        SetTimer(hWnd, TIMER_2, 100, NULL);

        //게임 제한시간 15초
        g_timer = 20;
        SetTimer(hWnd, TIMER_1, 1000, NULL);

        //화면 무효화용
        SetTimer(hWnd, TIMER_3, 1, NULL);

    }
    break;

    
    if (checkStart == TRUE) // 게임 시작 여부 체크
    {
    case WM_TIMER:
    {   
        if (checkStart == TRUE)
        {
        switch (wParam)
        {
        case TIMER_1: //게임 타이머
        {
            
                g_timer--;

                 if (-1 == g_timer)
                 {
                     KillTimer(hWnd, TIMER_1);
                     KillTimer(hWnd, TIMER_2);
                     KillTimer(hWnd, TIMER_3);
                     checkStart = FALSE;
                     MessageBox(hWnd, L"타임오버 종료", L"gameover", MB_OK);
                 }
        }
        break;


        case TIMER_2: //지렁이 스피드 이동 타이머 ( 벽과 아이템, 장애물 겹침 여부도 확인 )
        {
            wormt.left = worm[0].left - 40;
            wormt.right = worm[0].right + 40;
            wormt.top = worm[0].top - 40;
            wormt.bottom = worm[0].bottom + 40;
            //지렁이 머리와 아이템 좌표 겹침 확인
            for (i = 0; i < 2; i++)
            {
                if (TRUE == IntersectRect(&dst, &worm[0], &item[i]))
                {
                    if(g_cnt < 100)
                    g_cnt++;
                    b++;
                    g_timer += 6;
                    g_score += 100;
                    i_flag++;
                    while (true)
                    {
                        i_flag1 = 0; 
                        if (i_flag < 30 && checkMode == 2)
                        {
                            obstacle[i_flag - 1].left = rand() % 37 * 20 + 20;
                            obstacle[i_flag - 1].top = rand() % 26 * 20 + 20;
                            obstacle[i_flag - 1].right = obstacle[i_flag - 1].left + 60;
                            obstacle[i_flag - 1].bottom = obstacle[i_flag - 1].top + 60;
                        }
                            if (FALSE == IntersectRect(&dst, &wormt, &obstacle[i_flag - 1]))
                                i_flag1 = 1;
                        if (i_flag1 == 1)
                            break;
                    }
                    while (true)
                    {
                        i_flag1 = 0;
                        item[i].left = rand() % 38 * 20 + 20;
                        item[i].top = rand() % 27 * 20 + 20;
                        item[i].right = item[i].left + 40;
                        item[i].bottom = item[i].top + 40;
                        for (a = 0; a < i_flag; a++)
                        {
                            if (FALSE == IntersectRect(&dst, &item[0], &item[1]))
                            {
                                if (FALSE == IntersectRect(&dst, &item[i], &obstacle[a]))
                                    i_flag1++;
                            }
                        }
                        if (i_flag1 == b)
                            break;
                    }
                }
            }
            switch (w_flag) //방향키 입력에 따라 머리가 이동한다.
            {
            case 1: // 왼쪽 방향키
                if (testkey != 2)
                {
                    for (i = 0; i < g_cnt; i++)
                    {
                        worm1[i] = worm[g_cnt - i - 1].left;
                        worm[g_cnt - i].left = worm1[i];
                        worm[g_cnt - i].top = worm[g_cnt - i - 1].top;
                        worm[g_cnt - i].right = worm[g_cnt - i - 1].right;
                        worm[g_cnt - i].bottom = worm[g_cnt - i - 1].bottom;
                    }
                    worm[0].left -= 20;
                    worm[0].right -= 20; 
                }
                break;
            case 2: // 오른쪽 방향키
                if (testkey != 1)
                {
                    for (i = 0; i < g_cnt; i++)
                    {
                        worm1[i] = worm[g_cnt - i - 1].left;
                        worm[g_cnt - i].left = worm1[i];
                        worm[g_cnt - i].top = worm[g_cnt - i - 1].top;
                        worm[g_cnt - i].right = worm[g_cnt - i - 1].right;
                        worm[g_cnt - i].bottom = worm[g_cnt - i - 1].bottom;
                    }
                    worm[0].left += 20;
                    worm[0].right += 20;
                }
                break;
            case 3: // 위 방향키
                if (testkey != 4)
                {
                    for (i = 0; i < g_cnt; i++)
                    {
                        worm1[i] = worm[g_cnt - i - 1].top;
                        worm[g_cnt - i].left = worm[g_cnt - i - 1].left;
                        worm[g_cnt - i].top = worm1[i];
                        worm[g_cnt - i].right = worm[g_cnt - i - 1].right;
                        worm[g_cnt - i].bottom = worm[g_cnt - i - 1].bottom;
                    }
                    worm[0].top -= 20;
                    worm[0].bottom -= 20;
                }
                break;
            case 4: // 아래 방향키
                if (testkey != 3)
                {
                    for (i = 0; i < g_cnt; i++)
                    {
                        worm1[i] = worm[g_cnt - i - 1].top;
                        worm[g_cnt - i].left = worm[g_cnt - i - 1].left;
                        worm[g_cnt - i].top = worm1[i];
                        worm[g_cnt - i].right = worm[g_cnt - i - 1].right;
                        worm[g_cnt - i].bottom = worm[g_cnt - i - 1].bottom;
                    }
                    worm[0].top += 20;
                    worm[0].bottom += 20;
                }
                break;
            }

            //지렁이 벽 좌표 겹침 여부
            if (worm[0].left <= 0 || worm[0].right >= 820 || worm[0].top <= 0 || worm[0].bottom >= 620)
            {
                KillTimer(hWnd, TIMER_1);
                KillTimer(hWnd, TIMER_2);
                KillTimer(hWnd, TIMER_3);
                checkStart = FALSE;
                MessageBox(hWnd, L"벽에 돌진해 죽었습니다", L"gameover", MB_OK);
            }
            
            //지렁이 장애물 좌표 겹침 여부
            for (i = 0; i < (sizeof(obstacle) / sizeof(obstacle[0])); i++)
            {
                if ((TRUE == IntersectRect(&dst, &worm[0], &obstacle[i])) && checkMode == 2)
                {
                    KillTimer(hWnd, TIMER_1);
                    KillTimer(hWnd, TIMER_2);
                    KillTimer(hWnd, TIMER_3);
                    checkStart = FALSE;
                    MessageBox(hWnd, L"장애물에 돌진해 죽었습니다", L"gameover", MB_OK);
                }
            }
            //지렁이 머리와 몸체 좌표 겹침 여부
            for (i = 1; i < g_cnt; i++)
            {
                if (TRUE == IntersectRect(&dst, &worm[0], &worm[i]))
                {
                    KillTimer(hWnd, TIMER_1);
                    KillTimer(hWnd, TIMER_2);
                    KillTimer(hWnd, TIMER_3);
                    checkStart = FALSE;
                    MessageBox(hWnd, L"자기 몸에 돌진해 죽었습니다", L"gameover", MB_OK);
                }
            }
        }
        break;

        case TIMER_3: // 체크용
        {



            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
        }
        }
    }
    break;


    case WM_KEYDOWN:
    {
        RECT dst;
        switch (wParam) //방향키 버튼 입력으로 지렁이의 방향 설정
        {
        case VK_LEFT:
            if (w_flag != 2)
            {
                testkey = w_flag;
                w_flag = 1;
            }

            break;

        case VK_RIGHT:
            if (w_flag != 1)
            {
                testkey = w_flag;
                w_flag = 2;
            }

            break;

        case VK_UP:
            if (w_flag != 4)
            {
                testkey = w_flag;
                w_flag = 3;
            }

            break;

        case VK_DOWN:
            if (w_flag != 3)
            {
                testkey = w_flag;
                w_flag = 4;
            }

            break;


        }

        InvalidateRect(hWnd, NULL, FALSE);
    }
    break;

    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        //점수 시간현황판 변수
        RECT txt, b_txt, background;
        WCHAR index[64] = { 0, };        
        
        //색상 설정
        HBRUSH back_brush, wall_brush, ob_brush , worm_brush, worm1_brush, item_brush, brushold;
        HPEN sco_pen, wall_pen, ob_pen, worm_pen, worm1_pen, item_pen, penold;

        worm1_pen = CreatePen(PS_SOLID, 2, RGB(205, 194, 147));
        worm_pen = CreatePen(PS_SOLID, 2, RGB(205, 194, 147));
        sco_pen = CreatePen(PS_DASHDOT, 1, RGB(142, 133, 16));
        wall_pen = CreatePen(PS_SOLID, 3, RGB(75, 75, 75));
        ob_pen = CreatePen(PS_SOLID, 3, RGB(48, 48, 48));
        item_pen = CreatePen(PS_DASH, 3, RGB(255, 177, 47));

        worm1_brush = CreateSolidBrush(RGB(128, 0, 0));
        worm_brush = CreateSolidBrush(RGB(255, 255, 208));
        back_brush = CreateSolidBrush(RGB(82, 46, 20));
        wall_brush = CreateSolidBrush(RGB(159, 89, 39));
        ob_brush = CreateSolidBrush(RGB(75, 75, 75));
        item_brush = CreateSolidBrush(RGB(255, 205, 124));

        //더블 버퍼링
        static HDC hdc, memDC, tmpDC;
        static HBITMAP backbit, backbit1;
        static RECT buffer;
        hdc = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &buffer);
        memDC = CreateCompatibleDC(hdc);
        backbit = CreateCompatibleBitmap(hdc, buffer.right, buffer.bottom);
        backbit1 = (HBITMAP)SelectObject(memDC, backbit);
        PatBlt(memDC, 0, 0, buffer.right, buffer.bottom, WHITENESS);
        tmpDC = hdc;
        hdc = memDC;
        memDC = tmpDC;
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        //배경화면
        background.left = -100;
        background.top = -100;
        background.right = 3000;
        background.bottom = 2000;

        //스코어판배경
        b_txt.left = 815;
        b_txt.top = 45;
        b_txt.right = 950;
        b_txt.bottom = 100;
        //스코어판
        txt.left = 820;
        txt.top = 50;
        txt.right = 950;
        txt.bottom = 100;

        //윈도우 배경색
        brushold = (HBRUSH)SelectObject(hdc, back_brush);
        Rectangle(hdc, background.left, background.top, background.right, background.bottom);
        back_brush = (HBRUSH)SelectObject(hdc, brushold);

        //벽 색상
        brushold = (HBRUSH)SelectObject(hdc, wall_brush);
        penold = (HPEN)SelectObject(hdc, (HGDIOBJ)wall_pen);
        Rectangle(hdc, wall.left, wall.top, wall.right, wall.bottom);
        wall_pen = (HPEN)SelectObject(hdc, penold);
        wall_brush = (HBRUSH)SelectObject(hdc, brushold);

        //스코어판 배경과 텍스트 출력
        wsprintfW(index, L"점수 : %d\n제한시간 : %d", g_score, g_timer);
        penold = (HPEN)SelectObject(hdc, (HGDIOBJ)sco_pen);
        Rectangle(hdc, b_txt.left, b_txt.top, b_txt.right, b_txt.bottom);
        sco_pen = (HPEN)SelectObject(hdc, penold);
        DrawText(hdc, index, -1, &txt, DT_LEFT | DT_WORDBREAK);


        if (checkStart == TRUE)
        {
            if (checkMode == 1)
            {

                brushold = (HBRUSH)SelectObject(hdc, worm1_brush);
                penold = (HPEN)SelectObject(hdc, (HGDIOBJ)worm1_pen);
                Ellipse(hdc, worm[0].left, worm[0].top, worm[0].right, worm[0].bottom);
                worm1_pen = (HPEN)SelectObject(hdc, penold);
                worm1_brush = (HBRUSH)SelectObject(hdc, brushold);


                brushold = (HBRUSH)SelectObject(hdc, worm_brush);
                penold = (HPEN)SelectObject(hdc, (HGDIOBJ)worm_pen);
                for (int c = 1; c < g_cnt; c++)
                {
                    Ellipse(hdc, worm[c].left, worm[c].top, worm[c].right, worm[c].bottom);
                }
                worm_pen = (HPEN)SelectObject(hdc, penold);
                worm_brush = (HBRUSH)SelectObject(hdc, brushold);

                brushold = (HBRUSH)SelectObject(hdc, item_brush);
                penold = (HPEN)SelectObject(hdc, (HGDIOBJ)item_pen);
                for (int c = 0; c < (sizeof(item) / sizeof(item[0])); c++)
                {
                    Ellipse(hdc, item[c].left, item[c].top, item[c].right, item[c].bottom);
                }
                item_pen = (HPEN)SelectObject(hdc, penold);
                item_brush = (HBRUSH)SelectObject(hdc, brushold);
            }
            else if (checkMode == 2)
            {
                for (int c = 1; c < g_cnt; c++)
                {
                    Ellipse(hdc, worm[c].left, worm[c].top, worm[c].right, worm[c].bottom);
                }
                brushold = (HBRUSH)SelectObject(hdc, item_brush);
                penold = (HPEN)SelectObject(hdc, (HGDIOBJ)item_pen);
                for (int c = 0; c < (sizeof(item) / sizeof(item[0])); c++)
                {
                    Ellipse(hdc, item[c].left, item[c].top, item[c].right, item[c].bottom);
                }
                item_pen = (HPEN)SelectObject(hdc, penold);
                item_brush = (HBRUSH)SelectObject(hdc, brushold);

                brushold = (HBRUSH)SelectObject(hdc, ob_brush);
                penold = (HPEN)SelectObject(hdc, (HGDIOBJ)ob_pen);
                for (x = 0; x < (sizeof(obstacle) / sizeof(obstacle[0])); x++)
                {
                    Rectangle(hdc, obstacle[x].left, obstacle[x].top, obstacle[x].right, obstacle[x].bottom);
                }
                ob_pen = (HPEN)SelectObject(hdc, penold);
                ob_brush = (HBRUSH)SelectObject(hdc, brushold);
            }

        }

        tmpDC = hdc;
        hdc = memDC;
        memDC = tmpDC;
        GetClientRect(hWnd, &buffer);
        BitBlt(hdc, 0, 0, buffer.right, buffer.bottom, memDC, 0, 0, SRCCOPY);
        SelectObject(memDC, backbit1);
        DeleteObject(backbit);
        DeleteDC(memDC);

        DeleteObject(wall_pen);
        DeleteObject(ob_pen);
        DeleteObject(item_pen);
        DeleteObject(worm_pen);
        DeleteObject(sco_pen);
        DeleteObject(back_brush);
        DeleteObject(wall_brush);
        DeleteObject(ob_brush);
        DeleteObject(item_brush);
        DeleteObject(worm_brush);
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
