//
// PageSettingsPager.cpp
//

#include "stdafx.h"
#include "Xcenter.h"
#include "Settings.h"

#include "PageSettingsPager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CPagerSettingsPage, CSettingsPage)

BEGIN_MESSAGE_MAP(CPagerSettingsPage, CSettingsPage)
	//{{AFX_MSG_MAP(CPagerSettingsPage)
	ON_BN_CLICKED(IDC_MAILBOX_NAV, OnNavPlay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPagerSettingsPage property page

CPagerSettingsPage::CPagerSettingsPage() : CSettingsPage( CPagerSettingsPage::IDD )
{
	//{{AFX_DATA_INIT(CPagerSettingsPage)
	//}}AFX_DATA_INIT
}

CPagerSettingsPage::~CPagerSettingsPage()
{
}

void CPagerSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPagerSettingsPage)
	DDX_Control(pDX, IDC_PAGER_RETRY, m_wndPageTry);
	DDX_Control(pDX, IDC_PAGER_INTERVAL, m_wndPageVal);
	DDX_Control(pDX, IDC_PAGER_EXPIRE, m_wndPageExp);
	DDX_Control(pDX, IDC_MAILBOX_NAV, m_wndNavPlay);
	DDX_Control(pDX, IDC_MAILBOX_OFFSET, m_wndNavOffset);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CPagerSettingsPage message handlers

BOOL CPagerSettingsPage::OnInitDialog() 
{
	CSettingsPage::OnInitDialog();

	LoadSettings();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPagerSettingsPage::OnOK()
{
	EditSettings();

	CSettingsPage::OnOK();
}

BOOL CPagerSettingsPage::LoadSettings()
{
	CString strValue;
	
	strValue = FormatTimeString( Settings.Pager.PageExpire );
	m_wndPageExp.SetCurSel( m_wndPageExp.FindStringExact( -1, (LPCTSTR)strValue ) );
	
	if ( m_wndPageExp.GetCurSel() == CB_ERR )
	{
		int nItem = m_wndPageExp.AddString( strValue );
		m_wndPageExp.SetCurSel( nItem );
	}
	
	strValue = FormatRetryString( Settings.Pager.PageRetry );
	m_wndPageTry.SetCurSel( m_wndPageTry.FindStringExact( -1, (LPCTSTR)strValue ) );
	
	if ( m_wndPageTry.GetCurSel() == CB_ERR )
	{
		int nItem = m_wndPageTry.AddString( strValue );
		m_wndPageTry.SetCurSel( nItem );
	}
	
	strValue = FormatTimeString( Settings.Pager.PageInterval );
	m_wndPageVal.SetCurSel( m_wndPageVal.FindStringExact( -1, (LPCTSTR)strValue ) );
	
	if ( m_wndPageVal.GetCurSel() == CB_ERR )
	{
		int nItem = m_wndPageVal.AddString( strValue );
		m_wndPageVal.SetCurSel( nItem );
	}
	
	m_wndNavPlay.SetCheck( Settings.Pager.NavPlay == TRUE );
	strValue = Settings.Pager.NavOffset;
	m_wndNavOffset.SetWindowText( (LPCTSTR)strValue );
	SendMessage(WM_COMMAND, MAKELONG(m_wndNavPlay.GetDlgCtrlID(),BN_CLICKED), (LPARAM)m_hWnd);
	
	return TRUE;
}

BOOL CPagerSettingsPage::EditSettings() 
{
	CString strValue;
	
	m_wndPageExp.GetWindowText( strValue );
	Settings.Pager.PageExpire = FormatTimeValue( strValue );
	m_wndPageTry.GetWindowText( strValue );
	Settings.Pager.PageRetry = FormatRetryValue( strValue );
	m_wndPageVal.GetWindowText( strValue );
	Settings.Pager.PageInterval = FormatTimeValue( strValue );
	
	Settings.Pager.NavPlay = m_wndNavPlay.GetCheck() == TRUE;
	m_wndNavOffset.GetWindowText( strValue );
	Settings.Pager.NavOffset = strValue;
	
	return TRUE;
}

void CPagerSettingsPage::OnNavPlay() 
{
	BOOL bEnable = m_wndNavPlay.GetCheck();
	m_wndNavOffset.EnableWindow( bEnable );
}

CString CPagerSettingsPage::FormatTimeString(DWORD nTime) const
{
	CString strValue;
	
	if ( nTime < 60 ) 
		strValue.Format( "%i秒", nTime );
	else
	if ( nTime < 60 * 60 ) 
		strValue.Format( "%i分钟", nTime / 60 );
	else
	if ( nTime < 24 * 3600 )
		strValue.Format( "%i小时", nTime / 3600 );
	else
		strValue.Format( "%i天", nTime / 86400 );
	
	return strValue;
}

int CPagerSettingsPage::FormatTimeValue(LPCTSTR xTime) const
{
	int nTime = 0;
	
	for ( LPCTSTR sptr = xTime; _istdigit(*sptr); sptr++ )
	{
		nTime = nTime * 10 + (*sptr) - '0';
	}
	
	if ( _tcsnistr( xTime, "分钟", 4 ) )
		nTime *= 60;
	else
	if ( _tcsnistr( xTime, "小时", 4 ) )
		nTime *= 3600;
	else
	if ( _tcsnistr( xTime, "天", 2 ) )
		nTime *= 86400;
	
	return nTime;
}

CString CPagerSettingsPage::FormatRetryString(int nRetry) const
{
	CString strValue;
	
	strValue.Format( "%i次", nRetry );
	
	return strValue;
}

int CPagerSettingsPage::FormatRetryValue(LPCTSTR xRetry) const
{
	int nRetry = 0;
	
	for ( LPCTSTR sptr = xRetry; _istdigit(*sptr); sptr++ )
	{
		nRetry = nRetry * 10 + (*sptr) - '0';
	}
	
	return nRetry;
}
