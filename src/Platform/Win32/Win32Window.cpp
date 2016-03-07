﻿
/*
	[2015/01/10] メッセージ処理は非同期にするべきか？

		するべきではない。ウィンドウドラッグ中に描画更新などができるが、
		煩雑なメッセージ管理やコールバックが必要で、メリットよりもデメリットの方が大きい。

		デメリット
		・イベントは必ず PostEvent で一度メインスレッドに渡した後、
		  メイン側から各 Window にアタッチされている Listener に送信する必要がある。少し複雑。
		・PostEvent しなければならないということは、キューに入れるイベントデータは
		  new したオブジェクトかサイズ固定の構造体でなければならない。
		  イベント1つごとに new はコストが高いし、サイズ固定構造体は拡張性が無い。
		・Closing イベントでアプリの終了確認メッセージボックスとかが出せない。
		  ツール系ではメインスレッドが動いたままだと都合が悪い。
		  →ゲームならまぁいいけど。
*/

#include "../../Internal.h"
#include <Lumino/Platform/Win32/Win32Window.h>
#include "Win32WindowManager.h"
#include "../MouseCursorVisibility.h"

LN_NAMESPACE_BEGIN
namespace Platform
{
	
//=============================================================================
// Win32Window
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32Window::Win32Window(Win32WindowManager* app)
	: Window(app)
	, mLastMouseX(-1)
	, mLastMouseY(-1)
	, mIsActive(true)	// 初期値 true。WM_ACTIVATE は初回表示で最前面になった時は呼ばれない
	, m_systemMouseShown(true)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
	Win32Window::~Win32Window()
{
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void Win32Window::HideCursor()
//{
//	if (m_cursorShown)
//	{
//		::ShowCursor(FALSE);
//		m_cursorShown = false;
//	}
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void Win32Window::ShowCursor()
//{
//	if (!m_cursorShown)
//	{
//		::ShowCursor(TRUE);
//		m_cursorShown = true;
//	}
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LRESULT Win32Window::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, bool* handled)
{
	*handled = false;

	// WM_SYSKEYDOWN 関係をむやみに postMessage() すると強制終了するので必要なものだけフィルタリング
	//if ( msg != WM_SYSKEYDOWN || (msg == WM_SYSKEYDOWN && wparam == VK_RETURN) )
	{
		// マウスキャプチャの処理
		switch (msg)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			::SetCapture(hwnd);
			break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			::ReleaseCapture();
			break;
		}

		switch (msg)
		{
			/////////////////// ウィンドウが閉じられようとしている
			case WM_CLOSE:
			{
				/*
				ここは DefWindowProc() を呼ばないで終了する。
				こうしないと、×ボタンが押された瞬間にウィンドウが破棄されて、
				終了直前の最後の描画時に DirectX の EndScene() で
				「描画先のウィンドウがありません」ということでエラーが出る。

				代わりに終了フラグだけを立てて、それをメインループで検出、
				その後の finalize() 呼び出しで DestroyWindow() を呼び出す。
				*/

				EventArgs e(EventType_Close, this);
				if (NortifyEvent(e)) {
					*handled = true;
					return 0;
				}

				// TODO
				if (this == m_windowManager->GetMainWindow()) {
					m_windowManager->Exit();
				}

				*handled = true;
				return 0;
			}

			/////////////////// ウィンドウがアクティブ・非アクティブになった場合
			case WM_ACTIVATE:
			{
				bool active = ((wparam & 0xffff) != 0);
				if (active != mIsActive)
				{
					mIsActive = active;

					//EventArgs e;
					//e.Type = (mIsActive) ? LN_EVENT_APP_ACTIVATE : LN_EVENT_APP_DEACTIVATE;
					//e.Sender = this;
					//SendEventToAllListener(e);		// 同期処理の場合はこの場で通知
					//mApplication->PostEvent(&e);	// 非同期処理の場合は一度キューに入れる
				}
				*handled = true;
				return 0;
			}
			/////////////////////////////////////////////// ウィンドウサイズが変更された
			case WM_SIZE:
			{
				mClientSize.Set(lparam & 0xFFFF, (lparam >> 16) & 0xFFFF);



				// ウィンドウサイズを拾っておく
				//RECT rc;
				//::GetClientRect(mWindowHandle, &rc);
				//window->mClientSize.Set(rc.right, rc.bottom);

				//EventArgs e;
				//e.Type = LN_EVENT_WINDOW_SIZE_CHANGED;
				//e.Sender = this;
				//SendEventToAllListener(e);		// 同期処理の場合はこの場で通知
				//mApplication->PostEvent(&e);	// 非同期処理の場合は一度キューに入れる

				//*handled = true;
				return 0;
			}
			/////////////////////////////////////////////// Alt + Enter 確認
			//case WM_SYSKEYDOWN:
			//{
			//	if ( wparam == VK_RETURN )
			//	{
			//		EventArgs e;
			//		e.Type			= LN_EVENT_ALTENTER;
			//		e.Sender		= this;
			//		e.StructSize	= sizeof(EventArgs);
			//		mWindowManager->getManager()->postEventFromWindowThread( &e );

			//		return MAKELONG( -1, MNC_EXECUTE );     // 第一引数はメニューの属性。今回はメニュー使ってないのでとりあえずこのまま
			//	}
			//	// ここは以降に DefWindowProc() を呼ぶ。
			//	// こうしないと Alt + F4 とかが無効になってそれで終了できなくなる。
			//	return 0;
			//}
			/////////////////////////////////////////////// Alt + Enter の警告音カット http://www26.atwiki.jp/gunsyu3/pages/11.html
			case WM_SYSCHAR:
			{
				*handled = true;
				return 0;
			}
			/////////////////////////////////////////////// マウスボタン入力
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
			{
				EventArgs e;
				e.Sender = this;

				switch (msg)
				{
				case WM_LBUTTONDOWN:
					e.Type = EventType_MouseDown;
					e.Mouse.Button = MouseButton::Left;
					break;
				case WM_LBUTTONUP:
					e.Type = EventType_MouseUp;
					e.Mouse.Button = MouseButton::Left;
					break;
				case WM_RBUTTONDOWN:
					e.Type = EventType_MouseDown;
					e.Mouse.Button = MouseButton::Right;
					break;
				case WM_RBUTTONUP:
					e.Type = EventType_MouseUp;
					e.Mouse.Button = MouseButton::Right;
					break;
				case WM_MBUTTONDOWN:
					e.Type = EventType_MouseDown;
					e.Mouse.Button = MouseButton::Middle;
					break;
				case WM_MBUTTONUP:
					e.Type = EventType_MouseUp;
					e.Mouse.Button = MouseButton::Middle;
					break;
				case WM_XBUTTONDOWN:
					e.Type = EventType_MouseDown;
					e.Mouse.Button = (wparam & MK_XBUTTON1) ? MouseButton::X1 : MouseButton::X2;
					break;
				case WM_XBUTTONUP:
					e.Type = EventType_MouseUp;
					e.Mouse.Button = (wparam & MK_XBUTTON1) ? MouseButton::X1 : MouseButton::X2;
					break;
				}

				e.Mouse.X = LOWORD(lparam);
				e.Mouse.Y = HIWORD(lparam);
				e.Mouse.WheelDelta = 0;
				e.Mouse.MoveX = (mLastMouseX >= 0) ? e.Mouse.X - mLastMouseX : 0;
				e.Mouse.MoveY = (mLastMouseY >= 0) ? e.Mouse.Y - mLastMouseY : 0;
				NortifyEvent(e);

				mLastMouseX = e.Mouse.X;
				mLastMouseY = e.Mouse.Y;

				*handled = true;
				return 0;
			}
			/////////////////////////////////////////////// マウス移動
			case WM_MOUSEMOVE:
			{
				EventArgs e;
				e.Type = EventType_MouseMove;
				e.Sender = this;
				e.Mouse.Button = MouseButton::None;
				e.Mouse.WheelDelta = 0;
				e.Mouse.X = static_cast< short >(LOWORD(lparam));     // 一度 short にキャストしないと、
				e.Mouse.Y = static_cast< short >(HIWORD(lparam));     // マイナス値になったとき 65535 とか値が入る
				e.Mouse.MoveX = (mLastMouseX >= 0) ? e.Mouse.X - mLastMouseX : 0;
				e.Mouse.MoveY = (mLastMouseY >= 0) ? e.Mouse.Y - mLastMouseY : 0;
				e.Mouse.InClientArea = true;
				NortifyEvent(e);

				mLastMouseX = e.Mouse.X;
				mLastMouseY = e.Mouse.Y;

				*handled = true;
				return 0;
			}
			/////////////////////////////////////////////// クライアント領域外でマウスが移動した
			case WM_NCMOUSEMOVE:
			{
				// ホントにクライアント領域外かチェック
				if (wparam != HTCLIENT)
				{
					POINT pt;
					pt.x = static_cast<short>(LOWORD(lparam));
					pt.y = static_cast<short>(HIWORD(lparam));
					::ScreenToClient(hwnd, &pt);

					EventArgs e;
					e.Type = EventType_MouseMove;
					e.Sender = this;
					e.Mouse.Button = MouseButton::None;
					e.Mouse.WheelDelta = 0;
					e.Mouse.X = (short)pt.x;
					e.Mouse.Y = (short)pt.y;
					e.Mouse.MoveX = (mLastMouseX >= 0) ? e.Mouse.X - mLastMouseX : 0;
					e.Mouse.MoveY = (mLastMouseY >= 0) ? e.Mouse.Y - mLastMouseY : 0;
					e.Mouse.InClientArea = false;
					NortifyEvent(e);

					mLastMouseX = e.Mouse.X;
					mLastMouseY = e.Mouse.Y;

					*handled = true;
					return 0;
				}
			}
			///////////////////////////////////////////// マウスホイールが操作された
			case WM_MOUSEWHEEL:
			{
				EventArgs e;
				e.Type = EventType_MouseWheel;
				e.Sender = this;
				e.Mouse.Button = MouseButton::None;
				e.Mouse.WheelDelta = GET_WHEEL_DELTA_WPARAM(wparam) / WHEEL_DELTA;
				e.Mouse.X = static_cast< short >(LOWORD(lparam));
				e.Mouse.Y = static_cast< short >(HIWORD(lparam));
				e.Mouse.MoveX = (mLastMouseX >= 0) ? e.Mouse.X - mLastMouseX : 0;
				e.Mouse.MoveY = (mLastMouseY >= 0) ? e.Mouse.Y - mLastMouseY : 0;
				NortifyEvent(e);

				mLastMouseX = e.Mouse.X;
				mLastMouseY = e.Mouse.Y;

				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// キー↓
			case WM_KEYDOWN:
			{
				EventArgs e;
				e.Type = EventType_KeyDown;
				e.Sender = this;
				e.Key.KeyCode = ConvertVirtualKeyCode(wparam);	// 仮想キーコード
				e.Key.IsAlt = ::GetKeyState(VK_MENU) < 0;
				e.Key.IsShift = ::GetKeyState(VK_SHIFT) < 0;
				e.Key.IsControl = ::GetKeyState(VK_CONTROL) < 0;
				NortifyEvent(e);

				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// キー↑
			case WM_KEYUP:
			{
				EventArgs e;
				e.Type = EventType_KeyUp;
				e.Sender = this;
				e.Key.KeyCode = ConvertVirtualKeyCode(wparam);	// 仮想キーコード
				e.Key.IsAlt = ::GetKeyState(VK_MENU) < 0;
				e.Key.IsShift = ::GetKeyState(VK_SHIFT) < 0;
				e.Key.IsControl = ::GetKeyState(VK_CONTROL) < 0;
				NortifyEvent(e);

				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// Alt + KeyDown
			case WM_SYSKEYDOWN:
			{
				EventArgs e;
				e.Type = EventType_KeyDown;
				e.Sender = this;
				e.Key.KeyCode = ConvertVirtualKeyCode(wparam);	// 仮想キーコード
				e.Key.IsAlt = true;								// Alt on
				e.Key.IsShift = ::GetKeyState(VK_SHIFT) < 0;
				e.Key.IsControl = ::GetKeyState(VK_CONTROL) < 0;
				NortifyEvent(e);

				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// Alt + KeyUp
			case WM_SYSKEYUP:
			{
				EventArgs e;
				e.Type = EventType_KeyUp;
				e.Sender = this;
				e.Key.KeyCode = ConvertVirtualKeyCode(wparam);	// 仮想キーコード
				e.Key.IsAlt = true;								// Alt on
				e.Key.IsShift = ::GetKeyState(VK_SHIFT) < 0;
				e.Key.IsControl = ::GetKeyState(VK_CONTROL) < 0;
				NortifyEvent(e);
				break;	// WM_SYSKEYUPを捕まえた場合、必ずDefWindowProcに行くようにする
			}
			///////////////////////////////////////////// 文字入力
			case WM_CHAR:
			{
				// 文字のみ送る
				if (0x20 <= wparam && wparam <= 0x7E)
				{
					EventArgs e;
					e.Type = EventType_KeyChar;
					e.Sender = this;
					e.Key.KeyCode = Key::Unknown;
					e.Key.IsAlt = ::GetKeyState(VK_MENU) < 0;
					e.Key.IsShift = ::GetKeyState(VK_SHIFT) < 0;
					e.Key.IsControl = ::GetKeyState(VK_CONTROL) < 0;
					e.Key.Char = wparam;
					NortifyEvent(e);
					return 0;
				}
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Win32Window::NortifyEvent(const EventArgs& e)
{
	/*	マウス非表示はもっと上のレベルで共通処理できるかと思ったけど、
		割とOSにより変わりそうなので (ウィンドウ上にあるときだけカーソルが変わるのかとか)
		このあたりでやってしまう。
	*/

	// 非アクティブの場合はクライアント領域外で移動したことにして、カーソルを表示する
	if (!mIsActive)
	{
		m_mouseCursorVisibility->OnMoveCursor(false);
	}
	// クライアント上移動によるマウスカーソル非表示処理
	else if (e.Type == EventType_MouseMove)
	{
		if (e.Mouse.InClientArea) {
			m_mouseCursorVisibility->OnMoveCursor(true);
		}
		else {
			m_mouseCursorVisibility->OnMoveCursor(false);
		}
	}

	// 時間経過によるマウスカーソルの非表示処理
	bool mc_visible = m_mouseCursorVisibility->CheckVisible();
	if (mc_visible != m_systemMouseShown)
	{
		if (mc_visible)
		{
			::ShowCursor(TRUE);
		}
		else
		{
			::ShowCursor(FALSE);
		}
		m_systemMouseShown = mc_visible;
	}

	return SendEventToAllListener(e);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Key Win32Window::ConvertVirtualKeyCode(DWORD winVK)
{
	if ('A' <= winVK && winVK <= 'Z') return (Key)((int)Key::A + (winVK - 'A'));
	if ('0' <= winVK && winVK <= '9') return (Key)((int)Key::D0 + (winVK - '0'));
	if (VK_F1 <= winVK && winVK <= VK_F12) return (Key)((int)Key::F1 + (winVK - VK_F1));

	switch (winVK)
	{
	case VK_SPACE:
		return Key::Space;
	case VK_ESCAPE:
		return Key::Escape;
	case VK_UP:
		return Key::Up;
	case VK_DOWN:
		return Key::Down;
	case VK_LEFT:
		return Key::Left;
	case VK_RIGHT:
		return Key::Right;
	case VK_LSHIFT:
		return Key::LShift;
	case VK_RSHIFT:
		return Key::RShift;
	case VK_LCONTROL:
		return Key::LCtrl;
	case VK_RCONTROL:
		return Key::RCtrl;
	case VK_LMENU:
		return Key::LAlt;
	case VK_RMENU:
		return Key::RAlt;
	case VK_TAB:
		return Key::Tab;
	case VK_RETURN:
		return Key::Enter;
	case VK_BACK:
		return Key::BackSpace;
	case VK_INSERT:
		return Key::Insert;
	case VK_DELETE:
		return Key::Delete;
	case VK_PRIOR:
		return Key::PageUp;
	case VK_NEXT:
		return Key::PageDown;
	case VK_HOME:
		return Key::Home;
	case VK_END:
		return Key::End;

	case VK_OEM_1:
		return Key::Colon;
	case VK_OEM_PLUS:
		return Key::SemiColon;
	case VK_OEM_COMMA:
		return Key::Comma;
	case VK_OEM_PERIOD:
		return Key::Period;
	case VK_OEM_2:
		return Key::Slash;
	case VK_OEM_MINUS:
		return Key::Minus;
	case VK_OEM_102:
		return Key::BackSlash;
	case VK_OEM_5:
		return Key::Yen;
	case VK_OEM_7:
		return Key::Caret;
	case VK_OEM_4:
		return Key::LBracket;
	case VK_OEM_6:
		return Key::RBracket;
		/*
		case 'A': return Key::A;
		case 'B': return Key::B;
		case 'C': return Key::C;
		case 'D': return Key::D;
		case 'E': return Key::E;
		case 'F': return Key::F;
		case 'G': return Key::G;
		case 'H': return Key::H;
		case 'I': return Key::I;
		case 'J': return Key::J;
		case 'K': return Key::K;
		case 'L': return Key::L;
		case 'M': return Key::M;
		case 'N': return Key::N;
		case 'O': return Key::O;
		case 'P': return Key::P;
		case 'Q': return Key::Q;
		case 'R': return Key::R;
		case 'S': return Key::S;
		case 'T': return Key::T;
		case 'U': return Key::U;
		case 'V': return Key::V;
		case 'W': return Key::W;
		case 'X': return Key::X;
		case 'Y': return Key::Y;
		case 'Z': return Key::Z;

		Key::0                    = '0',
		Key::1                    = '1',
		Key::2                    = '2',
		Key::3                    = '3',
		Key::4                    = '4',
		Key::5                    = '5',
		Key::6                    = '6',
		Key::7                    = '7',
		Key::8                    = '8',
		Key::9                    = '9',

		Key::F1					= VK_F1,
		Key::F2					= VK_F2,
		Key::F3					= VK_F3,
		Key::F4					= VK_F4,
		Key::F5					= VK_F5,
		Key::F6					= VK_F6,
		Key::F7					= VK_F7,
		Key::F8					= VK_F8,
		Key::F9					= VK_F9,
		Key::F10					= VK_F10,
		Key::F11					= VK_F11,
		Key::F12					= VK_F12,
		*/
	}
	return Key::Unknown;
}

//=============================================================================
// Win32NativeWindow
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32NativeWindow::Win32NativeWindow(Win32WindowManager* windowManager, HWND hWnd, DWORD hWindowedStyle, HACCEL hAccel, const String& title)
	: Win32Window(windowManager)
	, mTitleText(title)
	, mWindowHandle(hWnd)
	, mAccelerators(hAccel)
	, mWindowedStyle(hWindowedStyle)
	, mFullScreen(false)
{
	// 初期化時のサイズ記憶
	RECT rc;
	GetClientRect(mWindowHandle, &rc);
	mClientSize.Width = rc.right - rc.left;
	mClientSize.Height = rc.bottom - rc.top;
	m_originalClientSize = mClientSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32NativeWindow::~Win32NativeWindow()
{
	if (mWindowHandle)
	{
		::DestroyWindow(mWindowHandle);
		mWindowHandle = NULL;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32NativeWindow::SetVisible(bool visible)
{
	::ShowWindow(mWindowHandle, (visible) ? SW_SHOW : SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32NativeWindow::SetFullScreenEnabled(bool flag)
{
	// フルスクリーンにする場合
	if (flag)
	{
		mFullScreen = true;
		::SetWindowLong(mWindowHandle, GWL_STYLE, Win32WindowManager::FULLSCREEN_STYLE);
		::SetWindowLong(mWindowHandle, GWL_EXSTYLE, 0);
		//Resize(true);

		// 画面いっぱい
		mClientSize.Width = ::GetSystemMetrics(SM_CXSCREEN);
		mClientSize.Height = ::GetSystemMetrics(SM_CYSCREEN);

		::SetForegroundWindow(mWindowHandle);

		int x = (::GetSystemMetrics(SM_CXSCREEN) - mClientSize.Width) / 2;
		int y = (::GetSystemMetrics(SM_CYSCREEN) - mClientSize.Height) / 2;

		//res = ::SetWindowPos( mWindowHandle, HWND_TOP, 0, 0, mWidth, mHeight, SWP_SHOWWINDOW );
		::SetWindowPos(mWindowHandle, NULL, x, y, mClientSize.Width, mClientSize.Height, SWP_SHOWWINDOW);
	}
	// ウィンドウモードにする場合
	else
	{
		mFullScreen = false;
		::SetWindowLong(mWindowHandle, GWL_STYLE, mWindowedStyle);
		Win32WindowManager::SetWindowClientSize(mWindowHandle, m_originalClientSize);
		Win32WindowManager::AbjustLocationCentering(mWindowHandle);
		mClientSize = m_originalClientSize;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32NativeWindow::CaptureMouse()
{
	::SetCapture(mWindowHandle);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32NativeWindow::ReleaseMouseCapture()
{
	::ReleaseCapture();
}

//=============================================================================
// Win32UserHostWindow
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32UserHostWindow::Win32UserHostWindow(Win32WindowManager* windowManager, HWND hWnd)
	: Win32Window(windowManager)
	, m_hWnd(hWnd)
	, m_clientSize()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32UserHostWindow::~Win32UserHostWindow()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Size& Win32UserHostWindow::GetSize() const
{
	// コンストラクタで mClientSize に格納しておいてもいいと思ったけど、
	// フルスクリーン化等でウィンドウサイズが変わった時の対応が面倒そうなのでこのまま。
	RECT rc = { 0, 0, 0, 0 };
	::GetClientRect(m_hWnd, &rc);
	m_clientSize.Set(rc.right, rc.bottom);
	return m_clientSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32UserHostWindow::CaptureMouse()
{
	::SetCapture(m_hWnd);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32UserHostWindow::ReleaseMouseCapture()
{
	::ReleaseCapture();
}

} // namespace Platform
LN_NAMESPACE_END
