// WormGame1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
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
    if (!InitInstance (hInstance, nCmdShow))
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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WORMGAME1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WORMGAME1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
RECT wall, worm[30], item[2];

//장애물 좌표선언
RECT obstacle[6];

//지렁이 방향 확인
int w_flag = 3;
//아이템 확인
int i_flag = 0; 
// 지렁이 스피드 값 변수
int w_speed; 
//방향키 체크 여부
int testkey = 4;

//게임 시작 여부 체크
BOOL checkStart = FALSE; 

//타이머
int g_timer;  
//점수
int g_score = 0;
//게임모드
int checkMode = 0; 


#define TIMER_1 1 //게임 타이머
#define TIMER_2 2 //지렁이 스피드



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
        //윈도우 창 크기
        SetWindowPos(hWnd, NULL, 200, 100, 1000, 680, 0);

        //타이머 세팅
        srand((unsigned int)time(NULL));

        //버튼 생성
        CreateWindow(TEXT("button"), TEXT("일반모드"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            810, 200, 100, 20, hWnd, (HMENU)0, hInst, NULL);
        CreateWindow(TEXT("button"), TEXT("헬모드"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            810, 250, 100, 20, hWnd, (HMENU)1, hInst, NULL);

        //게임 벽 크기
        wall.left = 20;
        wall.top = 20;
        wall.right = 800;
        wall.bottom = 600;
        
        //지렁이 좌표 구성
        worm[0].left = 400;
        worm[0].top = 400;
        worm[0].right = 420;
        worm[0].bottom = 420;

        //지렁이 초기 몸 생성
        for (int i = 1; i < 3; i++)
        {
            worm[i].left = worm[(i-1)].left;
            worm[i].top = worm[(i-1)].top;
            worm[i].right = worm[(i-1)].left + 20;
            worm[i].bottom = worm[(i-1)].top + 20;
        }

        //아이템 좌표 구성
        item[0].left = rand() % 40 * 20;
        item[0].top = rand() % 40 * 20;
        item[0].right = item[0].left + 20;
        item[0].bottom = item[0].top + 20;

        item[1].left = rand() % 40 * 20;
        item[1].top = rand() % 40 * 20;
        item[1].right = item[1].left + 20;
        item[1].bottom = item[1].top + 20;

        //장애물 좌표 구성
        for (int i = 0; i < 6; i++)
        {
            obstacle[i].left = rand() % 40 * 20;
            obstacle[i].top = rand() % 40 * 20;
            obstacle[i].right = obstacle[i].left + 40;
            obstacle[i].bottom = obstacle[i].top + 40;

        }

        //지렁이 스피드
        w_speed = 1000;
        SetTimer(hWnd, TIMER_2, w_speed, NULL);

        //게임 제한시간 15초
        g_timer = 20;
        SetTimer(hWnd, TIMER_1, 1000, NULL);

        while (true)
        {
            int i = 0;
            int a = 0;
            for ( i; i < 2; i++) // 10 10 40 * 20 600
            {
                if (item[i].left <= 10 || item[i].top <= 10 ||
                    item[i].right >= 40 * 20 || item[i].bottom >= 600)
                {
                    item[i].left = rand() % 40 * 20;
                    item[i].top = rand() % 40 * 20;
                    item[i].right = item[i].left + 20;
                    item[i].bottom = item[i].top + 20;
                }
                else
                    o = 1;
            }
            for (a; a < 6; a++) 
            {
                if (obstacle[i].left <= 10 || obstacle[i].top <= 10 ||
                    obstacle[i].right >= 40 * 20 || obstacle[i].bottom >= 600)
                {
                    obstacle[i].left = rand() % 40 * 20;
                    obstacle[i].top = rand() % 40 * 20;
                    obstacle[i].right = obstacle[i].left + 40;
                    obstacle[i].bottom = obstacle[i].top + 40;
                }
                else
                    o2 = 1;
            }

            if (o == 1 && o2 == 1)
                break;
        }


        

    }
    break;

 if (checkStart == TRUE)
 {
    case WM_TIMER:
    {
        int i = 0;
        RECT dst; //지렁이 머리와 아이템 겹침좌표 설정
        switch (wParam)
        {
            case TIMER_1: //게임 타이머
            {
                g_timer--;
                
                if (0 == g_timer)
                {
                    KillTimer(hWnd, TIMER_1);
                    KillTimer(hWnd, TIMER_2);
                    MessageBox(hWnd, L"타임오버 종료", L"gameover", MB_OK);
                    checkStart = FALSE;
                }
            }
            break;


            case TIMER_2: //지렁이 스피드 설정
            {
                if (w_speed != 200) //지렁이의 스피드 체크
                {   
                    w_speed -= 100;
                    KillTimer(hWnd, TIMER_2);

                    SetTimer(hWnd, TIMER_2, w_speed, NULL);
                }
                for (i; i < 2; i++)
                {
                    if (TRUE == IntersectRect(&dst, &worm[0], &item[i]))
                    {
                        g_timer += 6;
                        KillTimer(hWnd, TIMER_1);
                        SetTimer(hWnd, TIMER_1, g_timer, NULL);
                    }
                }
                i = 0;



                switch (w_flag) //방향키 입력에 따라 머리가 이동한다.
                {
                case 1: // 왼쪽 방향키
                    if (testkey != 2)
                    {
                        worm[0].left -= 20;
                        worm[0].right -= 20;
                    }
                    break;
                case 2: // 오른쪽 방향키
                    if (testkey != 1)
                    {
                        worm[0].left += 20;
                        worm[0].right += 20;
                    }
                    break;
                case 3: // 위 방향키
                    if (testkey != 4)
                    {
                        worm[0].top -= 20;
                        worm[0].bottom -= 20;
                    }
                    break;
                case 4: // 아래 방향키
                    if (testkey != 3)
                    {
                        worm[0].top += 20;
                        worm[0].bottom += 20;
                    }
                    break;
                }





                InvalidateRect(hWnd, NULL, TRUE);

            }
            break;

        }

    }
    break;

    case WM_KEYUP:
    {
        RECT dst;

        switch (wParam) //방향키 버튼 입력으로 지렁이의 방향 설정
        {
        case VK_LEFT:
            if (testkey != 2)
            {
                testkey = w_flag;
                w_flag = 1;
            }
            break;

        case VK_RIGHT:
            if (testkey != 1)
            {
                testkey = w_flag;
                w_flag = 2;
            }
            break;

        case VK_UP:
            if (testkey != 4)
            {
                testkey = w_flag;
                w_flag = 3;
            }
            break;

        case VK_DOWN:
            if (testkey != 3)
            {
                testkey = w_flag;
                w_flag = 4;
            }
            break;
        }
        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;
 }

    
    case WM_PAINT:
    {
        int i = 0;
        PAINTSTRUCT ps;

        WCHAR index[64] = { 0, };
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        Rectangle(hdc, wall.left, wall.top, wall.right, wall.bottom);

        wsprintfW(index, L"점수 : %d 제한시간 : %d", g_score, g_timer);
        TextOut(hdc, 810, 50, index, lstrlenW(index));
        TextOut(hdc, 810, 60, index, lstrlenW(index));

        if (checkStart == TRUE)
        {
            if (checkMode == 1)
            {
                Ellipse(hdc, worm[0].left, worm[0].top, worm[0].right, worm[0].bottom);
                Ellipse(hdc, item[0].left, item[0].top, item[0].right, item[0].bottom);
                Ellipse(hdc, item[1].left, item[1].top, item[1].right, item[1].bottom);
            }
            else if (checkMode == 2)
            {
                Ellipse(hdc, worm[0].left, worm[0].top, worm[0].right, worm[0].bottom);
                Ellipse(hdc, item[0].left, item[0].top, item[0].right, item[0].bottom);
                Ellipse(hdc, item[1].left, item[1].top, item[1].right, item[1].bottom);
                for (i; i < 6; i++)
                {
                    Rectangle(hdc, obstacle[i].left, obstacle[i].top, obstacle[i].right, obstacle[i].bottom);
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
