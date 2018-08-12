// MainInterface.cpp : implementation file
//

#include "stdafx.h"
#include "stu_system.h"
#include "MainInterface.h"
#include "usersql.h" 
#include "goblextern.h"
#include "GM.h"
#include "BZC.h"
#include "Winspool.h"   //�������ں������
#include "Dbt.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CMainInterface dialog
int ser2 = 0;
CString seru;
DWORD pcReturned = 0;  // number of ports received


CMainInterface::CMainInterface(CWnd* pParent /*=NULL*/)
	: CDialog(CMainInterface::IDD, pParent)
	, m_strRecvData(_T(""))
	, m_xgData(_T(""))
{
	//{{AFX_DATA_INIT(CMainInterface)
	//m_edit_name = _T("");
	m_edit_num = _T("");
	//}}AFX_DATA_INIT
}


void CMainInterface::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainInterface)
	DDX_Control(pDX, IDC_LIST_DISPLAY, m_list);
	//DDX_Text(pDX, IDC_EDIT_NAME, m_edit_name);
	DDX_Text(pDX, IDC_EDIT_NUM, m_edit_num);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_RECV, m_strRecvData);
	DDX_Control(pDX, IDC_MSCOMM1, m_cComm);
	DDX_Control(pDX, IDC_COMBO1, m_mdData);
	DDX_Control(pDX, IDC_COMBO_CK, m_ckData);
	DDX_Text(pDX, IDC_EDIT_XG, m_xgData);
	if (result1 == "")
	{
		char sql_insert[200];
		int i = 0;
		int nRow = 0;
		int j = 0;
		sprintf_s(sql_insert, "select bm from dyb where id = '%s';", user);
		mysql_query(&mysql, sql_insert);
		resultset = mysql_store_result(&mysql);// ��ý����         
		if (mysql_num_rows(resultset) != NULL)
		{
			int numRows = (int)mysql_num_rows(resultset); // ��ý�����еļ�¼��
			int numFields = (int)mysql_num_fields(resultset);// ��ñ����ֶ���
			while (row = mysql_fetch_row(resultset))
			{
				m_mdData.AddString(_T(row[0]));

			}
		}
		CRect rc;
		m_mdData.SetCurSel(0);
		m_mdData.GetDroppedControlRect(&rc);
		m_mdData.GetParent()->ScreenToClient(&rc);
		rc.bottom += 150;
		m_mdData.MoveWindow(&rc);
		mysql_free_result(resultset);
	}
	int selectedPos = m_mdData.GetCurSel();
	m_mdData.GetLBText(selectedPos, result1);
	///ö�ٴ���
	if (ser1 == "") //ser1 û�и�ֵ����ö�ٴ���
	{
		//����ע����ȡ������Ϣ
		HKEY hKey;
		if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm", NULL, KEY_READ, &hKey))   //�򿪴���ע����Ӧ�ļ�ֵ 
		{
			int seri = 0;
			TCHAR portName[256], commName[256];
			DWORD dwLong, dwSize;
			while (1)
			{
				dwLong = dwSize = sizeof(portName);
				if (::RegEnumValue(hKey,
					seri,
					portName,
					&dwLong,
					NULL,
					NULL,
					(PUCHAR)commName,
					&dwSize) == ERROR_NO_MORE_ITEMS)//   ö�ٴ���  
					break;
					m_ckData.AddString(commName);   // commName���Ǵ�������  
					ser1 = "1";
					seri++;
			}
			//�߶���Ϣ
			CRect rc;
			m_ckData.SetCurSel(0);
			m_ckData.GetDroppedControlRect(&rc);
			m_ckData.GetParent()->ScreenToClient(&rc);
			rc.bottom += 150;
			m_ckData.MoveWindow(&rc);
			RegCloseKey(hKey);
		}
	}

	//�����Ϣ��ʼ��
	char sql_insert[200];
	MYSQL_ROW column;
	sprintf_s(sql_insert, "select bm from dyb where id = '%s';", user);
	if (mysql_query(&mysql, sql_insert))
	{
		MessageBox("���ݿ��ѯ��������!", "Error", MB_OK);
	}
	else
	{
		resultset = mysql_store_result(&mysql);// ��ý����         
		if (mysql_num_rows(resultset) != NULL)
		{
			while (row = mysql_fetch_row(resultset))
			{
				//д��1
				//MessageBox(row[0], "����", MB_OK);
				sprintf_s(sql_insert, "UPDATE %s SET leav=1 WHERE no in ( select no from qjb)", row[0]);
				mysql_query(&mysql, sql_insert);
			}
		}
		else
		{
			MessageBox("û������!", "����", MB_OK);
		}
		mysql_free_result(resultset);  // �ͷŽ����
	}


}


BEGIN_MESSAGE_MAP(CMainInterface, CDialog)
	//{{AFX_MSG_MAP(CMainInterface)
	ON_BN_CLICKED(IDC_FIND_BUTTON, OnFindButton)
	ON_BN_CLICKED(IDC_EXIT_SYSTEM, OnExitSystem)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_RECV, &CMainInterface::OnButtonRecv)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DISPLAY, &CMainInterface::OnLvnItemchangedListDisplay)
	ON_EN_CHANGE(IDC_EDIT_RECV, &CMainInterface::OnEnChangeEditRecv)
	ON_BN_CLICKED(IDC_DISPLAY2, &CMainInterface::OnDisplay2)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CMainInterface::OnEnChangeEditName)
	ON_BN_CLICKED(IDC_BUTTON_DM, &CMainInterface::OnButtonDm)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainInterface::OnButtonTJ)
	ON_BN_CLICKED(IDC_BUTTON_GM, &CMainInterface::OnButtonGm)
	ON_BN_CLICKED(IDC_BUTTON_YD, &CMainInterface::OnButtonYd)
	ON_BN_CLICKED(IDC_BUTTON_WD, &CMainInterface::OnButtonWd)
	ON_BN_CLICKED(IDC_BUTTON_QJ, &CMainInterface::OnButtonQj)
	ON_BN_CLICKED(IDC_BUTTON_CJ, &CMainInterface::OnButtonCj)
	ON_WM_DEVICECHANGE()   //�Զ���⴮�����
END_MESSAGE_MAP()



BOOL CMainInterface::OnDeviceChange(UINT nEventType, DWORD dwData)
{
	//0x4d36e978L, 0xe325, 0x11ce, 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18  
	//DEV_BROADCAST_DEVICEINTERFACE* dbd = (DEV_BROADCAST_DEVICEINTERFACE*) dwData;  


	switch (nEventType)
	{
	case DBT_DEVICEREMOVECOMPLETE://�Ƴ��豸  
#if 1 
		m_ckData.ResetContent();
		//����ע����ȡ������Ϣ
		MessageBox("�豸���Ƴ��������������豸��", "");

#endif 
		break;
	case DBT_DEVICEARRIVAL://����豸  
#if 1 
		MessageBox("�豸�ѽ��룡", "");
		//����ע����ȡ������Ϣ
		HKEY hKey;
		if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm", NULL, KEY_READ, &hKey))   //�򿪴���ע����Ӧ�ļ�ֵ 
		{
			int seri = 0;
			TCHAR portName[256], commName[256];
			DWORD dwLong, dwSize;
			while (1)
			{
				dwLong = dwSize = sizeof(portName);
				if (::RegEnumValue(hKey,
					seri,
					portName,
					&dwLong,
					NULL,
					NULL,
					(PUCHAR)commName,
					&dwSize) == ERROR_NO_MORE_ITEMS)//   ö�ٴ��� 
					break;
					m_ckData.AddString(commName);   // commName���Ǵ�������  
					ser1 = "1";
					seri++;
			}
			//�߶���Ϣ
			CRect rc;
			m_ckData.SetCurSel(0);
			m_ckData.GetDroppedControlRect(&rc);
			m_ckData.GetParent()->ScreenToClient(&rc);
			rc.bottom += 150;
			m_ckData.MoveWindow(&rc);
			RegCloseKey(hKey);
		}
#endif 
		break;

	default:
		break;
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CMainInterface message handlers

void CMainInterface::OnFindButton() 
{
	OnButtonDm();
	// TODO: Add your control notification handler code here
	int j = 0;
//	int nRow ;//������
	int flag=0;
	CString info;
	char sql_insert[200];
	UpdateData(true);
	sprintf_s(sql_insert, "select %s.no,zb.name,%s.att from zb,%s where %s.no = zb.no", result1, result1, result1, result1);
	if (mysql_query(&mysql, sql_insert))
		{
			MessageBox("���ݿ��ѯ��������!", "Error", MB_OK);
		}
	else
		{                           
           resultset = mysql_store_result(&mysql);// ��ý����         
           if (mysql_num_rows(resultset) != NULL)
           {
				int numRows = (int)mysql_num_rows(resultset); // ��ý�����еļ�¼��
				int numFields = (int)mysql_num_fields(resultset);// ��ñ����ֶ���
                while (row = mysql_fetch_row(resultset))
                {
					int i = 0;
				//	nRow = m_list.InsertItem(j, "");
					CString TEMP = _T("\t");
					CString TEMP1 = _T("\t��ǩ��");
					CString TEMP2 = _T("\tδǩ��");
					for (i = 0; i< numFields; i++)
                    {
                        if((row[i]==m_edit_num))//||(row[i]==m_edit_name)
						{
							flag=1;
							if(row[i]==m_edit_num)
							{	
								if ((CString)row[i + 2] == '1')
								{
									info = (CString)row[i] + TEMP + (CString)row[i + 1] + TEMP1;
								}
								else
								{
									info = (CString)row[i] + TEMP + (CString)row[i + 1] + TEMP2;
								}
							}
							else 
							{
								if ((CString)row[i] == '1')
								{
									info = (CString)row[i - 2] + TEMP + (CString)row[i - 1] + TEMP1;
								}
								else
								{
									info = (CString)row[i - 2] + TEMP + (CString)row[i - 1] + TEMP2;
								}

							}
							MessageBox(info,"����", MB_OK);
						}
                    }
                    j++;					
                 }
				if(flag==0)
					{
						MessageBox("δ�ҵ�!!","����", MB_OK);
					}
			}
			else
			{
				 MessageBox("û������!", "����", MB_OK);
			} 
			mysql_free_result(resultset);  // �ͷŽ����
		}
		UpdateData(false);
}

void CMainInterface::OnExitSystem() 
{
	// TODO: Add your control notification handler code here
	char sql_insert[200];
	for (int i = 0; i <m_list.GetItemCount(); i++)  //��ѡ��   att = 1
	{
		if (m_list.GetCheck(i)) //�����ĳ��ѡ��  //�����ǰ���checkbox��ѡ�� 
		{
			sprintf_s(sql_insert, "update %s set att = 1 where id = '%d';", result1,i + 1);
			mysql_query(&mysql, sql_insert);
		}
	}
	sprintf_s(sql_insert, "select * from %s where att = 1;", result1);
	mysql_query(&mysql, sql_insert);
	resultset = mysql_store_result(&mysql);
	if (mysql_num_rows(resultset) != NULL)
	{
		sprintf_s(sql_insert, "update %s set att = 0,sum = sum+1 where att = 1;", result1);
		mysql_query(&mysql, sql_insert);
	}
	sprintf_s(sql_insert, "select * from %s where leav = 1;", result1);
	mysql_query(&mysql, sql_insert);
	resultset = mysql_store_result(&mysql);
	if (mysql_num_rows(resultset) != NULL)
	{
		sprintf_s(sql_insert, "update %s set leav = 0,lea = lea+1 where leav = 1;", result1);
		mysql_query(&mysql, sql_insert);
	}
	mysql_close(&mysql);
	SendMessage(WM_CLOSE);
	if (m_cComm.get_PortOpen()) //������ִ��ڱ����Ǵ򿪵ģ���رմ���
	{
		m_cComm.put_PortOpen(FALSE);
	}
}

BEGIN_EVENTSINK_MAP(CMainInterface, CDialog)
	ON_EVENT(CMainInterface, IDC_MSCOMM1, 1, CMainInterface::OnComm, VTS_NONE)
END_EVENTSINK_MAP()


void CMainInterface::OnComm()
{
	// TODO:  �ڴ˴������Ϣ����������
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	long len, k;
	byte rxdata[512]; //����BYTE����
	char sql[10] = { 0 };
	CString strtemp;
	if (m_cComm.get_CommEvent() == 2) //ֵΪ��ʾ���ջ����������ַ�
	{
		variant_inp = m_cComm.get_Input(); //����������Ϣ
		safearray_inp = variant_inp; ///����ת��
		len = safearray_inp.GetOneDimSize(); //�õ���Ч�����ݳ���
		for (k = 0; k<len; k++)
			safearray_inp.GetElement(&k, rxdata + k);
		for (k = 0; k<len; k++) //������ת��ΪCString�ͱ���
		{
			char bt = *(char*)(rxdata + k);//�ַ���
			sql[k] = bt;
			strtemp.Format(_T("%c"), bt); //���ַ�������ʱ����strtemp���
			//	m_strRecvData += strtemp; //������ձ༭���Ӧ�ַ���
		}
	}

	MYSQL_ROW column;
	char sql_insert[200];
	//mysql_real_connect(&mysql, "localhost", "root", "new_pass", "test", 3306, NULL, 0);
	//sprintf_s(sql, "6B248AE9");
	sprintf_s(sql_insert, "select * from zb,%s where zb.no = %s.no and zb.cardid = '%c%c%c%c%c%c%c%c';", result1, result1, sql[0], sql[1], sql[2], sql[3], sql[4], sql[5], sql[6], sql[7]);
	mysql_query(&mysql, sql_insert);
	resultset = mysql_store_result(&mysql);
	if (mysql_num_rows(resultset) != NULL)
	{
		sprintf_s(sql_insert, "update %s,zb set %s.att = 1 where zb.no = %s.no and zb.cardid = '%c%c%c%c%c%c%c%c';", result1, result1, result1, sql[0], sql[1], sql[2], sql[3], sql[4], sql[5], sql[6], sql[7]);
	mysql_query(&mysql, sql_insert);
	//��ѯ���Ŷ�Ӧ��ѧ��no ǩ�����att
	CString temp, temp1;
	sprintf_s(sql_insert, "select zb.no,zb.name,%s.att,%s.sum from zb,%s where zb.no = %s.no and cardid = '%c%c%c%c%c%c%c%c';", result1, result1, result1, result1, sql[0], sql[1], sql[2], sql[3], sql[4], sql[5], sql[6], sql[7]);
	mysql_query(&mysql, sql_insert);
	resultset = mysql_store_result(&mysql);
	column = mysql_fetch_row(resultset);
		char *b = "1";
		CString fh;
		fh.Format(_T("1"));
		if (*column[2] == *b)
		{
			m_cComm.put_Output(COleVariant(fh));
			temp = (_T("-ǩ���ɹ�!"));
			temp1 = (_T("-"));  
			temp = temp1 + column[1] + temp;
			m_strRecvData = column[0] + temp;
		}
		else
		{
			temp = (_T("-ǩ��ʧ��!"));
			temp1 = (_T("-"));
			temp = temp1 + column[1] + temp;
			m_strRecvData = column[0] + temp;
		}
	}
	else
	{
		CString temp;
		temp = (_T("û�иÿ�Ƭ������Ϣ!"));
		m_strRecvData = temp;
	}
		//CString temp = (_T("ǩ���ɹ�! \n")); //��ʾ��ɺ�Ҫ�Զ�����
	UpdateData(FALSE); //���±༭������
	OnButtonDm();
}


void CMainInterface::OnButtonRecv()
{
	if (ser1=="")
	{
		MessageBox("δ����ˢ���豸�����������⣡","����");
	}
	else
	{
		int selectedPos1 = m_ckData.GetCurSel();
		m_ckData.GetLBText(selectedPos1, ser1);
		char temp[200];
		for (int i = 1; i < 20; i++)
		{
			sprintf_s(temp, "COM%d", i);
			if (ser1 == temp)
			{
				ser2 = i;
				break;
			}
		}
		// TODO:  �ڴ���ӿؼ�֪ͨ����������
		if (m_cComm.get_PortOpen()) //������ִ��ڱ����Ǵ򿪵ģ���رմ���
		{
			m_cComm.put_PortOpen(FALSE);
		}
		m_cComm.put_CommPort(ser2); //ѡ��COM4�˿�
		m_cComm.put_InputMode(1); //���뷽ʽΪ�����Ʒ�ʽ
		m_cComm.put_InBufferSize(1024); //�������뻺����
		m_cComm.put_OutBufferSize(512); //�������������
		m_cComm.put_Settings(TEXT("9600,n,8,1"));//�����ʣ���У�飬������λ����ֹͣλ
		m_cComm.put_RThreshold(9); //ÿ���յ�һ���ַ�ʱ������OnComm�¼�
		m_cComm.put_PortOpen(true); //�򿪴���
		if (m_cComm.get_PortOpen()) //������ִ򿪣���ʾ����
		{
			OnButtonDm();
		}
	}
}


void CMainInterface::OnLvnItemchangedListDisplay(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
 

void CMainInterface::OnEnChangeEditRecv()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CMainInterface::OnDisplay2()
{
	OnButtonDm();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	char sql_insert[200];
	for (int i = 0; i < m_list.GetItemCount(); i++)   //��ѡ����
	{
		if (m_list.GetCheck(i)) //�����ĳ��ѡ��  //�����ǰ���checkbox��ѡ�� 
		{
			sprintf_s(sql_insert, "update %s,zb set %s.att = 1 where %s.no = zb.no and %s.id = '%d';", result1, result1, result1, result1, i + 1);
			mysql_query(&mysql, sql_insert);
		}
	}
	//��ʾδ��
	int j = 0;//�б��
	UpdateData(true);
	m_list.DeleteAllItems();//ɾ��listview�ؼ��е�������
	int nRow;//������
	while (m_list.DeleteColumn(0));//ɾ��������
	m_list.InsertColumn(0, "ѧ��", LVCFMT_LEFT, 125);//������
	m_list.InsertColumn(1, "����", LVCFMT_LEFT, 55);
	m_list.InsertColumn(2, "����", LVCFMT_LEFT, 42);
	m_list.InsertColumn(3, "�ܼ�", LVCFMT_LEFT, 40);
	m_list.InsertColumn(4, "���", LVCFMT_LEFT, 40);
	m_list.InsertColumn(5, "�ٴ�", LVCFMT_LEFT, 40);
	//mysql_query(&mysql, "update qd set id = 1 where id = 2013");
	sprintf_s(sql_insert, "select %s.no,zb.name,%s.att,%s.sum,%s.leav,%s.lea from %s,zb where %s.no = zb.no and %s.att = 0", result1, result1, result1, result1, result1, result1, result1, result1);
	if (mysql_query(&mysql, sql_insert))
	{
		//	AfxMessageBox(mysql_error(&mysql));
		MessageBox("���ݿ��ѯ��������!", "Error", MB_OK);
	}
	else
	{
		resultset = mysql_store_result(&mysql);// ��ý����         
		if (mysql_num_rows(resultset) != NULL)
		{
			int numRows = (int)mysql_num_rows(resultset); // ��ý�����еļ�¼��
			int numFields = (int)mysql_num_fields(resultset);// ��ñ����ֶ���
			while (row = mysql_fetch_row(resultset))
			{
				int i = 0;
				nRow = m_list.InsertItem(j, "0");
				for (i = 0; i < numFields; i++)
				{
					//fprintf(stdout, " %s", row[i]); // ��ӡ�ֶ�ֵ	
					m_list.SetItemText(nRow, i, row[i]);//��������
				}
				j++;
			}
		}
		else
		{
			MessageBox("û������!", "Error", MB_OK);
		}
		mysql_free_result(resultset);  // �ͷŽ����
	}


	UpdateData(false);
	//AfxMessageBox(result1);
}


void CMainInterface::OnEnChangeEditName()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}





void CMainInterface::OnButtonDm()
{
	char sql_insert[200];
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int j = 0;//�б��
	UpdateData(true);
	m_list.DeleteAllItems();//ɾ��listview�ؼ��е�������
	int nRow;//������
	while (m_list.DeleteColumn(0));//ɾ��������
	m_list.InsertColumn(0, "ѧ��", LVCFMT_LEFT, 125);//������
	m_list.InsertColumn(1, "����", LVCFMT_LEFT, 55);
	m_list.InsertColumn(2, "����", LVCFMT_LEFT, 42);
	m_list.InsertColumn(3, "�ܼ�", LVCFMT_LEFT, 40);
	m_list.InsertColumn(4, "���", LVCFMT_LEFT, 40);
	m_list.InsertColumn(5, "�ٴ�", LVCFMT_LEFT, 40);
	sprintf_s(sql_insert, "select %s.no,zb.name,%s.att,%s.sum,%s.leav,%s.lea from %s,zb where %s.no = zb.no", result1, result1, result1, result1, result1, result1, result1);
	if (mysql_query(&mysql, sql_insert))
	{
		//AfxMessageBox(mysql_error(&mysql));
		MessageBox("���ݿ��ѯ��������!", "Error", MB_OK);
	}
	else
	{
		resultset = mysql_store_result(&mysql);// ��ý����         
		if (mysql_num_rows(resultset) != NULL)
		{
			int numRows = (int)mysql_num_rows(resultset); // ��ý�����еļ�¼��
			int numFields = (int)mysql_num_fields(resultset);// ��ñ����ֶ���
			while (row = mysql_fetch_row(resultset))
			{
				int i = 0;
				nRow = m_list.InsertItem(j, "0");
				for (i = 0; i < numFields; i++)
				{

					m_list.SetItemText(nRow, i, row[i]);//��������
				}
				j++;
				m_list.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
			}
		}
		else
		{
			MessageBox("û������!", "����", MB_OK);
		}
		mysql_free_result(resultset);  // �ͷŽ����
	}
	UpdateData(false);
}



void CMainInterface::OnButtonTJ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	int j = 0;
	int jj = 0;  //��ѡ������
	int jjj = 0;  //�ܼ�¼��
	int jjjj = 0;  //��¼�����
	char sql_insert[200];
	for (int i = 0; i <m_list.GetItemCount(); i++)
	{
		if (m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_list.GetCheck(i)) //�����ĳ��ѡ��  //�����ǰ���checkbox��ѡ�� 
		{
			sprintf_s(sql_insert, "update %s,zb set %s.att = 1 where %s.no = zb.no and %s.id = '%d';", result1, result1, result1, result1, i + 1);
			mysql_query(&mysql, sql_insert);
			j++;
		}
	}
	//��ťbug����
	sprintf_s(sql_insert, "select * from %s,zb where %s.no = zb.no;", result1, result1);  //������
	if (mysql_query(&mysql, sql_insert))
	{
		MessageBox("���ݿ��ѯ����", "ͳ��ʧ��", MB_OK);
	}
	else
	{
		resultset = mysql_store_result(&mysql);
		while (row = mysql_fetch_row(resultset))
		{
			jjj++;
		}
		sprintf_s(sql_insert, "select * from %s where leav = 1;", result1); //�������
		mysql_query(&mysql, sql_insert);
		resultset = mysql_store_result(&mysql);
		while (row = mysql_fetch_row(resultset))
		{
			jjjj++;
		}
		sprintf_s(sql_insert, "select * from zb,%s where %s.no = zb.no and %s.att = 1;", result1, result1, result1);
		mysql_query(&mysql, sql_insert);
		resultset = mysql_store_result(&mysql);
		if ((mysql_num_rows(resultset) != NULL) && (j == 0))  //û�й�ѡ����att��������  //δ������������
		{
			while (row = mysql_fetch_row(resultset))
			{
				jj++;
			}
			str.Format(_T("���ڴι��� %d ��\n������ %d ͬѧδ��\n�� %d �����\n������Ϊ%.1lf%%"), jjj, jjj - jj, jjjj, double(jj) / double(jjj) * 100);

		}
		else
		{
			str.Format(_T("���ڴι��� %d ��\n������ %d ͬѧδ��\n�� %d �����\n������Ϊ%.1lf%%"), jjj, jjj - j, jjjj, double(j) / double(jjj) * 100);
		}
		MessageBox(str, "ͳ����Ϣ", MB_OK);
	}
}


void CMainInterface::OnButtonGm()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	this->ShowWindow(SW_HIDE);//���ص��봰��
	CGM dlg;//���������洰�������
	dlg.DoModal();//��ģ̬��ʽ��ʾһ������
	this->ShowWindow(SW_SHOW);
}



void CMainInterface::OnButtonYd()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int j = 0;
//	int nRow ;//������
	int flag=0;
	CString info;
	char sql_insert[200];
	UpdateData(true);
	sprintf_s(sql_insert, "select %s.no,zb.name,%s.att from zb,%s where %s.no = zb.no", result1, result1, result1, result1);
	if (mysql_query(&mysql, sql_insert))
		{
			MessageBox("���ݿ��ѯ��������!", "Error", MB_OK);
		}
	else
		{                           
           resultset = mysql_store_result(&mysql);// ��ý����         
           if (mysql_num_rows(resultset) != NULL)
           {
				int numRows = (int)mysql_num_rows(resultset); // ��ý�����еļ�¼��
				int numFields = (int)mysql_num_fields(resultset);// ��ñ����ֶ���
                while (row = mysql_fetch_row(resultset))
                {
					int i = 0;
				//	nRow = m_list.InsertItem(j, "");
					CString TEMP = _T("\t");
					CString TEMP1 = _T("\t�޸ĳɹ�");
					for (i = 0; i< numFields; i++)
                    {
                        if((row[i]==m_xgData))//||(row[i]==m_edit_name)
						{
							flag=1;
							sprintf_s(sql_insert, "update %s set %s.att = 1 where %s.no = %s;", result1, result1, result1, m_xgData);
							mysql_query(&mysql, sql_insert);
							if (row[i] == m_xgData)
							{
								info = (CString)row[i] + TEMP + (CString)row[i + 1] + TEMP1;
							}
							else
							{
								info = (CString)row[i - 2] + TEMP + (CString)row[i - 1] + TEMP1;
							}
							MessageBox(info,"�޸ĳɹ�", MB_OK);
						}
                    }
                    j++;					
                 }
				if(flag==0)
					{
						MessageBox("δ�ҵ���ѧ��!!","�޸�ʧ��", MB_OK);
					}
			}
			else
			{
				 MessageBox("û������!", "����", MB_OK);
			} 
			mysql_free_result(resultset);  // �ͷŽ����
		}
		UpdateData(false);
		OnButtonDm();   //�����ʾ����
}


void CMainInterface::OnButtonWd()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int j = 0;
	//	int nRow ;//������
	int flag = 0;
	CString info;
	char sql_insert[200];
	UpdateData(true);
	sprintf_s(sql_insert, "select %s.no,zb.name,%s.att from zb,%s where %s.no = zb.no", result1, result1, result1, result1);
	if (mysql_query(&mysql, sql_insert))
	{
		MessageBox("���ݿ��ѯ��������!", "Error", MB_OK);
	}
	else
	{
		resultset = mysql_store_result(&mysql);// ��ý����         
		if (mysql_num_rows(resultset) != NULL)
		{
			int numRows = (int)mysql_num_rows(resultset); // ��ý�����еļ�¼��
			int numFields = (int)mysql_num_fields(resultset);// ��ñ����ֶ���
			while (row = mysql_fetch_row(resultset))
			{
				int i = 0;
				//	nRow = m_list.InsertItem(j, "");
				CString TEMP = _T("\t");
				CString TEMP1 = _T("\t�޸ĳɹ�");
				for (i = 0; i< numFields; i++)
				{
					if ((row[i] == m_xgData))//||(row[i]==m_edit_name)
					{
						flag = 1;
						sprintf_s(sql_insert, "update %s set %s.att = 0 where %s.no = %s;", result1, result1, result1, m_xgData);
						mysql_query(&mysql, sql_insert);
						if (row[i] == m_xgData)
						{
							info = (CString)row[i] + TEMP + (CString)row[i + 1] + TEMP1;
						}
						else
						{
							info = (CString)row[i - 2] + TEMP + (CString)row[i - 1] + TEMP1;
						}
						MessageBox(info, "�޸ĳɹ�", MB_OK);
					}
				}
				j++;
			}
			if (flag == 0)
			{
				MessageBox("δ�ҵ���ѧ��!!", "�޸�ʧ��", MB_OK);
			}
		}
		else
		{
			MessageBox("û������!", "����", MB_OK);
		}
		mysql_free_result(resultset);  // �ͷŽ����
	}
	UpdateData(false);
	OnButtonDm();   //�����ʾ����
}


void CMainInterface::OnButtonQj()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int j = 0;
	//	int nRow ;//������
	int flag = 0;
	CString info;
	char sql_insert[200];
	UpdateData(true);
	sprintf_s(sql_insert, "select %s.no,zb.name,%s.att from zb,%s where %s.no = zb.no", result1, result1, result1, result1);
	if (mysql_query(&mysql, sql_insert))
	{
		MessageBox("���ݿ��ѯ��������!", "Error", MB_OK);
	}
	else
	{
		resultset = mysql_store_result(&mysql);// ��ý����         
		if (mysql_num_rows(resultset) != NULL)
		{
			int numRows = (int)mysql_num_rows(resultset); // ��ý�����еļ�¼��
			int numFields = (int)mysql_num_fields(resultset);// ��ñ����ֶ���
			while (row = mysql_fetch_row(resultset))
			{
				int i = 0;
				//	nRow = m_list.InsertItem(j, "");
				CString TEMP = _T("\t");
				CString TEMP1 = _T("\t�޸ĳɹ�");
				for (i = 0; i< numFields; i++)
				{
					if ((row[i] == m_xgData))//||(row[i]==m_edit_name)
					{
						flag = 1;
						sprintf_s(sql_insert, "update %s set %s.leav = 1 where %s.no = %s;", result1, result1, result1, m_xgData);
						mysql_query(&mysql, sql_insert);
						if (row[i] == m_xgData)
						{
							info = (CString)row[i] + TEMP + (CString)row[i + 1] + TEMP1;
						}
						else
						{
							info = (CString)row[i - 2] + TEMP + (CString)row[i - 1] + TEMP1;
						}
						MessageBox(info, "�޸ĳɹ�", MB_OK);
					}
				}
				j++;
			}
			if (flag == 0)
			{
				MessageBox("δ�ҵ���ѧ��!!", "�޸�ʧ��", MB_OK);
			}
		}
		else
		{
			MessageBox("û������!", "����", MB_OK);
		}
		mysql_free_result(resultset);  // �ͷŽ����
	}
	UpdateData(false);
	OnButtonDm();   //�����ʾ����
}



void CMainInterface::OnButtonCj()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	this->ShowWindow(SW_HIDE);//���ص��봰��
	CBZC dlg;//���������洰�������
	dlg.DoModal();//��ģ̬��ʽ��ʾһ������
	this->ShowWindow(SW_SHOW);
}

