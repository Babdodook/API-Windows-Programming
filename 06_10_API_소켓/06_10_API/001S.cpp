#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	WSADATA wsadata;   // 윈도우 소켓 초기화 정보를 가지고 있는 구조체
	SOCKET s;
	SOCKADDR_IN addr = { 0 };  // 소켓을 연결할 대상의 주소를 쓰는 구조체
	WSAStartup(MAKEWORD(2, 2), &wsadata);  // 윈도우 소켓 버전을 지정하고 정보를 얻어온다. MAKEWORD(2,2) 는 WORD 타입을 만들 때 사용되는 매크로
	s = socket(AF_INET, SOCK_STREAM, 0);   // IPv4 주소체계를 사용하는 인터넷 망을 쓰고, 입출력 작동의 스트림방식으로 데이터 교환하는 소켓 생성
	addr.sin_family = AF_INET;
	addr.sin_port = 20;
	addr.sin_addr.s_addr = inet_addr("192.168.0.35");  // 소켓에 부여할 IP주소와 포트 번호 설정. IP 주소인 "127.0.0.1"을 32비트 숫자 타입으로 반환.
	bind(s, (LPSOCKADDR)&addr, sizeof(addr));  // 만들어진 주소를 소켓에 부여함.
	if (listen(s, 5) == -1)    // 클라이언트의 접속을 기다림.
		return 0;
	do
	{
		SOCKADDR_IN c_addr;
		int size = sizeof(c_addr);
		accept(s, (LPSOCKADDR)&c_addr, &size);    // accept 함수로 클라이언트 접속을 기다림. accept 함수는 클라이언트와 연결할 소켓을 반환.
	} while (MessageBox(NULL, "클라이언트 접속을 확인했습니다. 서버를 종료하시겠습니까?", "Server 메시지", MB_YESNO) == IDNO);
	closesocket(s);
	WSACleanup();
	return 1;
}

//+SOCKET
//SOCKET sock(int af, int type, int protocol);
//
//  AF_UNSPEC : 기타 네트워크에 접속합니다.
//	AF_INET : IPv4 주소체계를 사용하는 인터넷 망에 접속
//	AF_INET6 : IPv6 주소체계를 사용하는 인터넷 망에 접속
//	AF_IRDA : 적외선 통신(IrDA) 으로 상대편에게 접속
//	AF_BTH : 블루투스로 상대편에게 접속