// XhairOverlay.cpp

#include "framework.h"
#include "XhairOverlay.h"

// 最大文字列長の定義
#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス（アプリケーションのインスタンス）
WCHAR szTitle[MAX_LOADSTRING];                  // アプリケーションのタイトルバーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メインウィンドウのクラス名

// クロスヘアの色
COLORREF crosshairColor = RGB(0, 180, 0);       // 緑色に設定

// このコードモジュールに含まれる関数の宣言
ATOM                MyRegisterClass(HINSTANCE hInstance);  // ウィンドウクラスを登録する関数
BOOL                InitInstance(HINSTANCE, int);           // ウィンドウインスタンスを初期化する関数
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);   // メッセージ処理のコールバック関数
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);    // ヘルプダイアログ用のコールバック関数

// アプリケーションのエントリーポイント
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);  // 古いインスタンスは使わない
    UNREFERENCED_PARAMETER(lpCmdLine);      // コマンドライン引数も使わない

    // タイトルとウィンドウクラス名をリソースから読み込む
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_XHAIROVERLAY, szWindowClass, MAX_LOADSTRING);

    // ウィンドウクラスの登録
    MyRegisterClass(hInstance);

    // ウィンドウの初期化
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    // メッセージループ開始
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))  // メッセージを取得
    {
        TranslateMessage(&msg);  // メッセージを変換
        DispatchMessage(&msg);   // メッセージをウィンドウプロシージャに送る
    }

    return (int)msg.wParam;  // メッセージの結果を返す
}

// ウィンドウクラスの登録
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);  // 構造体のサイズを設定
    wcex.style = CS_HREDRAW | CS_VREDRAW;  // 水平・垂直のリサイズで再描画
    wcex.lpfnWndProc = WndProc;  // ウィンドウプロシージャ（メッセージ処理関数）を設定
    wcex.cbClsExtra = 0;  // クラスごとの追加メモリなし
    wcex.cbWndExtra = 0;  // ウィンドウごとの追加メモリなし
    wcex.hInstance = hInstance;  // アプリケーションのインスタンス
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_XHAIROVERLAY));  // アイコンを読み込む
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);  // デフォルトの矢印カーソル
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));  // 背景色は黒
    wcex.lpszMenuName = nullptr;  // メニューは使用しない
    wcex.lpszClassName = szWindowClass;  // ウィンドウクラス名
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));  // 小さなアイコン

    // ウィンドウクラスを登録
    return RegisterClassExW(&wcex);
}

// ウィンドウインスタンスの初期化
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;  // グローバル変数にアプリケーションのインスタンスを保存

    // ウィンドウを作成
    HWND hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        szWindowClass, szTitle, WS_POPUP,  // ウィンドウをポップアップスタイルで作成
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),  // 画面全体に表示
        nullptr, nullptr, hInstance, nullptr);  // 親ウィンドウなし、メニューなし

    if (!hWnd)
    {
        return FALSE;  // ウィンドウ作成失敗
    }

    // 透過ウィンドウの設定
    SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);  // 黒を透過色に設定
    ShowWindow(hWnd, SW_SHOW);  // ウィンドウを表示
    UpdateWindow(hWnd);  // ウィンドウを更新

    return TRUE;  // 成功
}

// メッセージ処理（ウィンドウプロシージャ）
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:  // 描画メッセージ
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);  // 描画用のコンテキスト取得

        // 画面の中央の座標を計算
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2;

        // ブラシを作成
        HBRUSH hBrush = CreateSolidBrush(crosshairColor);
        HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);  // 描画用にブラシを選択

        // ドット（小さな円）を描画
        // Ellipse(描画コンテキスト, 左上X, 左上Y, 右下X, 右下Y)
        // 小さな円を描くことでドットを表現
        Ellipse(hdc, centerX - 3, centerY - 3, centerX + 3, centerY + 3);

        SelectObject(hdc, hOldBrush);  // 元のブラシを戻す
        DeleteObject(hBrush);  // 作成したブラシを削除

        EndPaint(hWnd, &ps);  // 描画処理終了
    }
    break;
    case WM_DESTROY:  // ウィンドウが閉じられたとき
        PostQuitMessage(0);  // アプリケーションを終了するメッセージを送る
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);  // デフォルトのメッセージ処理
    }
    return 0;
}
