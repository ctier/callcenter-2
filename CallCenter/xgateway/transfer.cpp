//
// Transfer.cpp
//

#include "stdafx.h"
#include "Xgateway.h"
#include "Transfer.h"

#include "Network.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CTransfer construction

CTransfer::CTransfer(SOCKET hSocket, SOCKADDR_IN* pHost) : CConnection( hSocket, pHost )
{
	m_bConnected= FALSE;
}

CTransfer::CTransfer()
{
	m_bConnected= FALSE;
}

CTransfer::~CTransfer()
{
	ASSERT( m_hSocket == INVALID_SOCKET );
}

//////////////////////////////////////////////////////////////////////
// CTransfer connect

BOOL CTransfer::ConnectTo(LPCTSTR pAddress, int nPort)
{
	DWORD nAddress = inet_addr( pAddress );
	CConnection::ConnectTo( (IN_ADDR*)&nAddress, nPort );
	
	WSAEventSelect( m_hSocket, Network.m_pWakeup, FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE );
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CTransfer disconnect

void CTransfer::Disconnect()
{
	if ( IsConnected() ) SendPacket( new CByePacket );
	
	CConnection::Close();
}

//////////////////////////////////////////////////////////////////////
// CTransfer run function

BOOL CTransfer::OnRun()
{
	if ( ! CConnection::OnRun() ) return FALSE;
	
	DWORD tNow = GetTickCount();
	
	if ( ! IsConnected() && tNow - m_tConnected >= 15* 1000 )
	{
		OnDropped( WSA_WAIT_TIMEOUT ); return FALSE;
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CTransfer socket event handlers

BOOL CTransfer::OnConnected()
{
	//m_bConnected	= TRUE;
	m_tConnected	= GetTickCount();
	
	return TRUE;
}

void CTransfer::OnDropped(int nError)
{
#ifdef _DEBUG
	CString strHost = inet_ntoa( m_pHost.sin_addr );
	TRACE2( _T("WARNING: Dropped Connection From %s, Error %i\n"), strHost, nError );
#endif
	
	Disconnect();
}

void CTransfer::OnHandshake()
{
	m_bConnected	= TRUE;
	m_tConnected	= GetTickCount();
}

BOOL CTransfer::OnRead()
{
	if ( ! CConnection::OnRead() ) return FALSE;
	
	CPacket* pPacket = ReadBuffer( m_pInput );
	if ( pPacket == NULL ) return TRUE;
	
	BOOL bResult = OnPacket( pPacket );
	pPacket->Release();
	
	return bResult;
}

void CTransfer::DoPrint(BYTE* pBuffer, DWORD nLength, LPCTSTR lpszHeader)
{
	CConnection::DoPrint( pBuffer, nLength, lpszHeader );
}

//////////////////////////////////////////////////////////////////////
// CTransfer packet event handlers

BOOL CTransfer::OnPacket(CPacket* pPacket)
{
	if ( pPacket->IsCommandId( "OK" ) )
	{
		return OnOkPacket( (COkPacket*)pPacket );
	}
	else
	if ( pPacket->IsCommandId( "ERR" ) )
	{
		return OnErrPacket( (CErrPacket*)pPacket );
	}
	else
	if ( pPacket->IsCommandId( "BYE" ) )
	{
		return OnByePacket( (CByePacket*)pPacket );
	}
	
	return TRUE;
}

BOOL CTransfer::OnOkPacket(COkPacket* pPacket)
{
	return TRUE;
}

BOOL CTransfer::OnErrPacket(CErrPacket* pPacket)
{
	return TRUE;
}

BOOL CTransfer::OnByePacket(CByePacket* pPacket)
{
	Disconnect(); return FALSE;
}

//////////////////////////////////////////////////////////////////////
// CTransfer send packet

BOOL CTransfer::SendPacket(const void* pData, DWORD nLength)
{
	m_pOutput->Add( pData, nLength );
	CConnection::OnWrite();

	return TRUE;
}

BOOL CTransfer::SendPacket(CPacket* pPacket, BOOL bPeek)
{
	pPacket->EncodeBody( m_pOutput );
	CConnection::OnWrite();
	
	if ( ! bPeek ) pPacket->Release();

	return TRUE;
}
