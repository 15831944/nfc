#pragma once
#include "afxwin.h"


// CBZC �Ի���

class CBZC : public CDialogEx
{
	DECLARE_DYNAMIC(CBZC)

public:
	CBZC(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBZC();


// �Ի�������
	enum { IDD = IDD_DIALOG_BCZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonTchu();	
	afx_msg void OnButtonDch();
	afx_msg void OnButtonSch();
	CEdit m_mdData2;
	afx_msg void OnButtonChj();
};

