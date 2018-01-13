#include "SocketSvc.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include "ev.h"
#include "Client.h"
#include "LogUtil.h"
#include <unistd.h>
#include "ClientMgr.h"
#include "ConfigMgr.h"

int SocketCreateAndBind(int iPort);
int SocketSetNonblock(int iFd);
void SocketAccept(struct ev_loop *pstMainLoop, ev_io *pstSockEvIo, int iEvents);
void SocketRead(struct ev_loop *pstMainLoop, struct ev_io *pstClientEvIo, int iEvents);

int SocketSetNonblock(int iFd)
{
	long lFlags = fcntl(iFd, F_GETFL);
	if (lFlags < 0) {
		LOG(ERROR) << "fcntl F_GETFL error";
		return -1;
	}

	lFlags |= O_NONBLOCK;
	if (fcntl(iFd, F_SETFL, lFlags) < 0) {
		LOG(ERROR) << "fcntl F_SETFL error";
		return -1;
	}

	return 0;
}

int SocketCreateAndBind(int iPort)
{
	//创建套接字
	int iSvrSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (iSvrSock < 0)
	{
		LOG(ERROR) << "socket error";
		return -1;
	}

	//将套接字和IP、端口绑定
	struct sockaddr_in stSvrAddr;
	memset(&stSvrAddr, 0, sizeof(stSvrAddr));  //每个字节都用0填充
	stSvrAddr.sin_family = AF_INET;  //使用IPv4地址
	stSvrAddr.sin_addr.s_addr = INADDR_ANY;  //具体的IP地址
	stSvrAddr.sin_port = htons(iPort);  //端口
	int iOnFlag = 1; 
	setsockopt(iSvrSock, SOL_SOCKET, SO_REUSEADDR, &iOnFlag, sizeof(iOnFlag) ); //地址重用

	if (bind(iSvrSock, (struct sockaddr*)&stSvrAddr, sizeof(stSvrAddr)) != 0)
	{
		LOG(ERROR) << "bind error";
		return -1;
	}

	return iSvrSock;
}

void SocketAccept(struct ev_loop *pstMainLoop, ev_io *pstSockEvIo, int iEvents)
{
	struct sockaddr_in stAddrIn;
	socklen_t iAddrInLen = sizeof(stAddrIn);

	int iFd = 0;
	if ((iFd = accept(pstSockEvIo->fd, (struct sockaddr *)&stAddrIn, &iAddrInLen)) == -1) {
		if (errno != EAGAIN && errno != EINTR) {
			LOG(ERROR) << "bad accept";
		}
		return;
	}

	do {
		LOG(ERROR) << "new connection " << inet_ntoa(stAddrIn.sin_addr) << ":" << ntohs(stAddrIn.sin_port);

		SocketSetNonblock(iFd);

		CClient *pClient = CClientMgr::Inst().FindClient(iFd);
		if (NULL != pClient)
		{
			OSSLOG << "client is not release, iFd=" << iFd << ", client=" << pClient->GetLogInfo();
			ev_io_stop(pstMainLoop, pClient->GetEvIo());
			CClientMgr::Inst().DestoryClient(pClient);
		}

		// new client
		pClient = CClientMgr::Inst().CreateNewClient(iFd, inet_ntoa(stAddrIn.sin_addr), ntohs(stAddrIn.sin_port));
		if (!pClient)
		{
			LOG(ERROR) << "cannot create new client";
			close(iFd);
			return;
		}

		ev_io* pstEvIo = pClient->GetEvIo();
		ev_io_init(pstEvIo, SocketRead, iFd, EV_READ);
		ev_io_start(pstMainLoop, pstEvIo);
		
	} while (0);
}

void SocketRead(struct ev_loop *pstMainLoop, struct ev_io *pstClientEvIo, int iEvents)
{
	LOG(DEBUG) << "socket read: evnets=" << iEvents;

	if (EV_ERROR & iEvents) {
		LOG(ERROR) << "error events, iEvents=" << iEvents;
		return;
	}

	int iClientFd = pstClientEvIo->fd;
	CClient *pClient = CClientMgr::Inst().FindClient(iClientFd);
	if (!pClient) {
		LOG(ERROR) << "client is not found, fd=" << iClientFd;
		return;
	}

	char szBuff[1024];
	memset(&szBuff, 0, sizeof(szBuff));
	ssize_t iReadSize = recv(iClientFd, szBuff, 1024, 0);
	if (iReadSize < 0) {
		LOG(ERROR) << "read error";
		return;
	}

	if (iReadSize == 0) {
		LOG(INFO) << "client is disconnect";
		CSocketSvc::Inst().DisConnectClient(pClient);
		return;
	}

	LOG(DEBUG) << "recv data: " << szBuff;

	if (CConfigMgr::Inst().IsDisconnectAfterProcCommand())
	{
		LOG(DEBUG) << "disconnect client after processing command";
		CSocketSvc::Inst().DisConnectClient(pClient);
	}
}

CSocketSvc::CSocketSvc()
{
}

CSocketSvc::~CSocketSvc()
{
	close(m_iSvrFD);
	ev_io_stop(m_pstMainLoop, &m_stSvrEvIo);
}

int CSocketSvc::Initialize(int iPort, struct ev_loop *pstMainLoop)
{
	m_iSvrFD = 0;
	m_pstMainLoop = pstMainLoop;
	m_iSvrPort = iPort;
	m_iSvrFD = SocketCreateAndBind(iPort);
	if (m_iSvrFD == -1) {
		LOG(ERROR) << "create and bind socket error, port=" << iPort;
		return -1;
	}

	int iRet = SocketSetNonblock(m_iSvrFD);
	if (0 != iRet) {
		LOG(ERROR) << "SocketSetNonblock error";
		return -1;
	}

	iRet = listen(m_iSvrFD, SOMAXCONN);
	if (0 != iRet) {
		LOG(ERROR) << "listen error";
		return -1;
	}

	ev_init(&m_stSvrEvIo, SocketAccept);
	ev_io_set(&m_stSvrEvIo, m_iSvrFD, EV_READ);
	ev_io_start(m_pstMainLoop, &m_stSvrEvIo);

	return 0;
}

void CSocketSvc::DisConnectClient(CClient * pClient)
{
	APP_CHECK_NULL(pClient, );
	close(pClient->GetFd());
	ev_io_stop(m_pstMainLoop, pClient->GetEvIo());
	CClientMgr::Inst().DestoryClient(pClient);
}
