// GM.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "stu_system.h"
#include "GM.h"
#include "afxdialogex.h"
#include "usersql.h" 
#include "MD5.h" 
#include "goblextern.h"
#include "stu_systemDlg.h"
// CGM �Ի���

IMPLEMENT_DYNAMIC(CGM, CDialogEx)

CGM::CGM(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGM::IDD, pParent)
	, m_mmData1(_T(""))
	, m_mmData2(_T(""))
	, m_mmData3(_T(""))
{

}

CGM::~CGM()
{
}

void CGM::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ODDMM, m_mmData1);
	DDX_Text(pDX, IDC_EDIT_NEWMM, m_mmData2);
	DDX_Text(pDX, IDC_EDIT_CHMM, m_mmData3);
}


BEGIN_MESSAGE_MAP(CGM, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUE, &CGM::OnButtonQue)
	ON_BN_CLICKED(IDC_BUTTON_TUI, &CGM::OnBnClickedButtonTui)
END_MESSAGE_MAP()


// CGM ��Ϣ�������


void CGM::OnButtonQue()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������   m_mmDate1
	MD5 md5;                 //����MD5����
	char sql_insert[200];
	MYSQL_ROW column;;
	GetDlgItem(IDC_EDIT_ODDMM)->GetWindowText(m_mmData1);
	GetDlgItem(IDC_EDIT_NEWMM)->GetWindowText(m_mmData2);
	GetDlgItem(IDC_EDIT_CHMM)->GetWindowText(m_mmData3);
	mysql_query(&mysql, "set names gb2312");
	//	CString sql = "123456";           //��Ҫ���ܵ��ַ���
	md5.update(m_mmData1.GetBuffer());          //��Ϊupdate����ֻ����string���ͣ�����ʹ��getbuffer()����ת��CStringΪstring
	m_mmData1 = md5.toString().c_str();     //toString()������ü����ַ�����c_str();��������ת����CString����
		if (pass == m_mmData1)
		{
			md5.reset();
			mysql_query(&mysql, "set names gb2312");
			//	CString sql = "123456";           //��Ҫ���ܵ��ַ���
			md5.update(m_mmData2.GetBuffer());          //��Ϊupdate����ֻ����string���ͣ�����ʹ��getbuffer()����ת��CStringΪstring
			m_mmData2 = md5.toString().c_str();     //toString()������ü����ַ�����c_str();��������ת����CString����
			md5.reset();
			mysql_query(&mysql, "set names gb2312");
			//	CString sql = "123456";           //��Ҫ���ܵ��ַ���
			md5.update(m_mmData3.GetBuffer());          //��Ϊupdate����ֻ����string���ͣ�����ʹ��getbuffer()����ת��CStringΪstring
			m_mmData3 = md5.toString().c_str();     //toString()������ü����ַ�����c_str();��������ת����CString����
			if ((m_mmData2 == m_mmData3))
			{
				if (m_mmData2.IsEmpty() || m_mmData3.IsEmpty())
				{
					MessageBox("���벻��Ϊ��", "ʧ��", MB_OK);
				}
				else
				{
					sprintf_s(sql_insert, "update jsb set pass = '%s' where id = '%s';", m_mmData2, user);
					mysql_query(&mysql, sql_insert);
					sprintf_s(sql_insert, "select  pass from jsb where id = '%s';", user);
					mysql_query(&mysql, sql_insert);
					resultset = mysql_store_result(&mysql);
					column = mysql_fetch_row(resultset);
					if (column[0] == m_mmData2)
					{
						MessageBox("���ܳɹ�", "�ɹ�", MB_OK);
						pass = m_mmData2;
					}
					else
					{
						MessageBox("����ʧ��(����ԭ��)", "ʧ��", MB_OK);
					}
				}
			}
			else
			{
				MessageBox("���������벻һ�£�", "ʧ��", MB_OK);
			}
		}
		else
		{
			MessageBox("�����벻��ȷ��", "ʧ��", MB_OK);
		}
}




void CGM::OnBnClickedButtonTui()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SendMessage(WM_CLOSE);
}


