//
// DlgSettingsManager.cpp
//

#include "stdafx.h"
#include "Xuser32.h"

#include "DlgSettingsManager.h"
#include "PageSettingsAudio.h"
#include "PageSettingsVideo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSettingsManagerDlg, CSettingsSheet)

BEGIN_MESSAGE_MAP(CSettingsManagerDlg, CSettingsSheet)
	//{{AFX_MSG_MAP(CSettingsManagerDlg)
	ON_COMMAND(IDRETRY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsManagerDlg construction

CSettingsManagerDlg::CSettingsManagerDlg() : CSettingsSheet( 0, IDS_SETTINGS )
{
	m_nTopMargin	= 52;
}

CSettingsManagerDlg::~CSettingsManagerDlg()
{
}

/////////////////////////////////////////////////////////////////////////////
// CSettingsManagerDlg operations

BOOL CSettingsManagerDlg::Run(LPCTSTR pszWindow)
{
	CSettingsManagerDlg pSheet;
	BOOL bResult = ( pSheet.DoModal( pszWindow ) == IDOK );
	return bResult;
}

int CSettingsManagerDlg::DoModal(LPCTSTR pszWindow)
{
	CAudioSettingsPage		pAudio;
	CVideoSettingsPage		pVideo;
	
	AddGroup( &pAudio );
	AddGroup( &pVideo );

	if ( pszWindow ) SetActivePage( GetPage( pszWindow ) );

	int nReturn = CSettingsSheet::DoModal();

	return nReturn;
}

void CSettingsManagerDlg::AddPage(CSettingsPage* pPage)
{
	CString strCaption = pPage->m_sCaption;
	CSettingsSheet::AddPage( pPage, strCaption );
}

void CSettingsManagerDlg::AddGroup(CSettingsPage* pPage)
{
#if 0
	if ( pPage->IsKindOf( RUNTIME_CLASS(CRichSettingsPage) ) )
	{
		CString strCaption = ((CRichSettingsPage*)pPage)->m_sCaption;
		CSettingsSheet::AddGroup( pPage, strCaption );
	}
	else
#endif
	{
		CString strCaption = pPage->m_sCaption;
		CSettingsSheet::AddGroup( pPage, strCaption.GetLength() ? (LPCTSTR)strCaption : NULL );
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSettingsManagerDlg message handlers

BOOL CSettingsManagerDlg::OnInitDialog()
{
	CSettingsSheet::OnInitDialog();

	m_bmHeader.LoadBitmap( IDB_PRODUCTS );

	return TRUE;
}

void CSettingsManagerDlg::DoPaint(CDC& dc)
{
	CRect rc;
	GetClientRect( &rc );

	BITMAP pInfo;
	m_bmHeader.GetBitmap( &pInfo );

	CDC mdc;
	mdc.CreateCompatibleDC( &dc );
	CBitmap* pOldBitmap = (CBitmap*)mdc.SelectObject( &m_bmHeader );
	dc.BitBlt( 0, 0, pInfo.bmWidth, pInfo.bmHeight, &mdc, 0, 0, SRCCOPY );
	mdc.SelectObject( pOldBitmap );
	mdc.DeleteDC();

	CSettingsSheet::DoPaint( dc );
}

void CSettingsManagerDlg::OnOK()
{
	CSettingsSheet::OnOK();
}

void CSettingsManagerDlg::OnApply()
{
	CSettingsSheet::OnApply();
}
