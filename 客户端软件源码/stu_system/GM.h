#pragma once
#include "afxwin.h"

// CGM �Ի���

class CGM : public CDialogEx
{
	DECLARE_DYNAMIC(CGM)

public:
	CGM(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGM();

// �Ի�������
	enum { IDD = IDD_GM };
	CString m_mmData1;
	CString m_mmData2;
	CString m_mmData3;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnButtonQue();
	afx_msg void OnEnChangeEditOddmm();
	afx_msg void OnBnClickedButtonTui();
	DECLARE_MESSAGE_MAP()
public:

};
