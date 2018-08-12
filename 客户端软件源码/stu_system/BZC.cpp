// BZC.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "stu_system.h"
#include "BZC.h"
#include "afxdialogex.h"
#include "usersql.h" 
#include "goblextern.h"
#include <afxdb.h>
#include "MainInterface.h"
#include <Windowsx.h>
// CBZC �Ի���

IMPLEMENT_DYNAMIC(CBZC, CDialogEx)

CBZC::CBZC(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBZC::IDD, pParent)
{

}

CBZC::~CBZC()
{
}

void CBZC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_BM, m_mdData2);
}


BEGIN_MESSAGE_MAP(CBZC, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TCHU, &CBZC::OnButtonTchu)
	ON_BN_CLICKED(IDC_BUTTON_DCH, &CBZC::OnButtonDch)
	ON_BN_CLICKED(IDC_BUTTON_SCH, &CBZC::OnButtonSch)
	ON_BN_CLICKED(IDC_BUTTON_CHJ, &CBZC::OnButtonChj)
END_MESSAGE_MAP()


// CBZC ��Ϣ�������


void CBZC::OnButtonTchu()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SendMessage(WM_CLOSE);
}


void CBZC::OnButtonDch()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//���б� //
	CString m_bmData;
	
	m_mdData2.GetWindowText(m_bmData);   //��ȡֵ���е㲻��
	char sql_insert[200];
	sprintf_s(sql_insert, "select * from dyb where bm = '%s' and id = '%s'", m_bmData, user);
	if (mysql_query(&mysql, sql_insert))
	{
		MessageBox("���ݿ��ѯ��������!", "Error", MB_OK);
	}
	else
	{
		resultset = mysql_store_result(&mysql);// ��ý����   
		if (mysql_num_rows(resultset) != NULL)
		{

			int j = 0;//�б��
			UpdateData(true);
			//char sql_insert[200];
			m_list.DeleteAllItems();//ɾ��listview�ؼ��е�������
			int nRow;//������
			while (m_list.DeleteColumn(0));//ɾ��������
			m_list.InsertColumn(0, "ѧ��", LVCFMT_LEFT, 125);//������
			m_list.InsertColumn(1, "����", LVCFMT_LEFT, 55);
			m_list.InsertColumn(2, "��ٴ���", LVCFMT_LEFT, 85);
			m_list.InsertColumn(3, "�����ܼ�", LVCFMT_LEFT, 85);
			sprintf_s(sql_insert, "select %s.no,zb.name,%s.lea,%s.sum from %s,zb where %s.no = zb.no", m_bmData, m_bmData, m_bmData, m_bmData, m_bmData);
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
					mysql_free_result(resultset);  // �ͷŽ����
				}
				else
				{
					MessageBox("û������!", "����", MB_OK);
				}
			}
			UpdateData(false);

			//�����ж��б�����Ƿ��м�¼
			if (m_list.GetItemCount() <= 0)
			{
				AfxMessageBox("�б���û�м�¼��Ҫ���棡");
				return;
			}
			//�����Ϊ�Ի��� ����Ҫ���� #include <Afxdlgs.h>
			CFileDialog dlg(FALSE, "xls", "������Ϣͳ�Ʊ�", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Excel �ļ�(*.xls)|*.xls||");
			dlg.m_ofn.lpstrTitle = "�ļ��б����Ϊ";

			if (dlg.DoModal() != IDOK)
				return;
			CString strFilePath;
			//����ļ�·����
			strFilePath = dlg.GetPathName();
			//�ж��ļ��Ƿ��Ѿ����ڣ�������ɾ���ؽ�
			DWORD dwRe = GetFileAttributes(strFilePath);
			if (dwRe != (DWORD)-1)
			{
				DeleteFile(strFilePath);
			}

			CDatabase database;//���ݿ����Ҫ����ͷ�ļ� #include <afxdb.h>
			CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel����
			CString sSql, strInsert;
			TRY
			{
				// �������д�ȡ���ַ���
				sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s", sDriver, strFilePath, strFilePath);

				// �������ݿ� (��Excel����ļ�)
				if (database.OpenEx(sSql, CDatabase::noOdbcDialog))
				{
					//����б��������
					int iColumnNum, iRowCount;
					LVCOLUMN lvCol;
					CString strColName; //���ڱ����б�������
					int i, j; //�С���ѭ������

					iColumnNum = m_list.GetHeaderCtrl()->GetItemCount();
					iRowCount = m_list.GetItemCount();

					sSql = " CREATE TABLE DSO_DX ( ";
					strInsert = " INSERT INTO DSO_DX ( ";
					//����б�������
					lvCol.mask = LVCF_TEXT; //�������ã�˵��LVCOLUMN������pszText������Ч
					lvCol.cchTextMax = 32; //���裬pszText������ָ����ַ����Ĵ�С
					lvCol.pszText = strColName.GetBuffer(32); //���裬pszText ��ָ����ַ�����ʵ�ʴ洢λ�á�
					//���������������ú����ͨ�� GetColumn()��������б��������
					for (i = 0; i < iColumnNum; i++)
					{
						if (!(m_list.GetColumn(i, &lvCol)))
							return;
						if (i < iColumnNum - 1)
						{
							sSql = sSql + lvCol.pszText + " TEXT , ";
							strInsert = strInsert + lvCol.pszText + " , ";
						}
						else
						{
							sSql = sSql + lvCol.pszText + " TEXT ) ";
							strInsert = strInsert + lvCol.pszText + " )  VALUES ( ";
						}
					}
					//����Excel����ļ�
					database.ExecuteSQL(sSql);

					//ѭ����ȡ��¼�����뵽EXCEL��
					sSql = strInsert;
					char chTemp[33];
					for (j = 0; j < iRowCount; j++)
					{
						memset(chTemp, 0, 33);
						for (i = 0; i < iColumnNum; i++)
						{
							m_list.GetItemText(j, i, chTemp, 33);
							if (i < (iColumnNum - 1))
							{
								sSql = sSql + "'" + chTemp + "' , ";
							}
							else
							{
								sSql = sSql + "'" + chTemp + "' ) ";
							}
						}
						//����¼���뵽�����
						database.ExecuteSQL(sSql);
						sSql = strInsert;
					}
				}

				// �ر�Excel����ļ�
				database.Close();

				AfxMessageBox("�����ѯ���ΪExcel�ļ��ɹ���");
			}
				CATCH_ALL(e)
			{
				//�������ͺܶ࣬������Ҫ���б���
				AfxMessageBox("Excel�ļ�����ʧ�ܡ�");
			}
			END_CATCH_ALL;
		}
		else
		{
			MessageBox("û������!", "����", MB_OK);
		}
		

	}
}


void CBZC::OnButtonSch()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//MessageBox(m_bmData, "����", MB_OK);
	char sql_insert[200];
	CString m_bmData;
	
	m_mdData2.GetWindowText(m_bmData);   //��ȡֵ���е㲻��
	if (MessageBox("ȷ��ɾ������", "ȷ��", MB_ICONEXCLAMATION | MB_OKCANCEL) == IDCANCEL)
		return;
	else
	{
		sprintf_s(sql_insert, "select * from dyb where bm = '%s' and id = '%s'", m_bmData, user);
		if (mysql_query(&mysql, sql_insert))
		{
			MessageBox("���ݿ��ѯ��������!", "Error", MB_OK);
		}
		else
		{
			resultset = mysql_store_result(&mysql);// ��ý����   
			if (mysql_num_rows(resultset) != NULL)
			{
				sprintf_s(sql_insert, "drop table %s;", m_bmData);  //ɾ���� ���Ҳ���
				mysql_query(&mysql, sql_insert);
				sprintf_s(sql_insert, "delete from dyb where bm = '%s' and id = '%s';", m_bmData, user);   //ɾ����Ӧ������
				mysql_query(&mysql, sql_insert);
				MessageBox("ɾ���ɹ���", "�ɹ�", MB_OK);
			}
			else
			{
				MessageBox("û������!", "����", MB_OK);
			}
			mysql_free_result(resultset);
		}
	}
	m_mdData.ResetContent(); //����б�
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
	int selectedPos = m_mdData.GetCurSel();
	m_mdData.GetLBText(selectedPos, result1);
}



void CBZC::OnButtonChj()   //������
{
	CString m_bmData;
	
	m_mdData2.GetWindowText(m_bmData);   //��ȡֵ���е㲻��
	if (m_bmData == "")
	{
		MessageBox("�����������!", "Error", MB_OK);
	}
	else
	{
		CString strSQL;
		strSQL.Format("select * from dyb where bm = '%s';", m_bmData);//ע��һ��Ҫд��һ�У����ұ���Ҫ��/'/'
		if (mysql_query(&mysql, strSQL))
		{
			MessageBox("���ݿ��ѯ��������!", "Error", MB_OK);
		}
		else
		{
			resultset = mysql_store_result(&mysql);// ��ý����  
			if (mysql_num_rows(resultset) != NULL)
			{
				MessageBox("�����Ѵ���!", "Error", MB_OK);
			}
			else
			{
				// TODO:  �ڴ���ӿؼ�֪ͨ����������
				CFileDialog dlg(TRUE, //TRUE��FALSE��TRUEΪ���ļ���FALSEΪ�����ļ�
					"xls", //Ϊȱʡ����չ��
					NULL, //Ϊ��ʾ���ļ�����Ͽ�ı༭����ļ�����һ���ѡNULL
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,//Ϊ�Ի�����һ��ΪOFN_HIDEREADONLY   |   OFN_OVERWRITEPROMPT,������ֻ��ѡ��͸��������ļ�ǰ��ʾ��
					"Excel �ļ�(*.xls)|*.xls||"//Ϊ�����б�������ʾ�ļ�����
					);
				dlg.m_ofn.lpstrTitle = "��������";
				if (dlg.DoModal() != IDOK)  //�о�û������
					return;
				CString strFilePath;
				//����ļ�·����
				strFilePath = dlg.GetPathName();
				//�ж��ļ��Ƿ��Ѿ����ڣ���������ļ�
				DWORD dwRe = GetFileAttributes(strFilePath);
				if (dwRe != (DWORD)-1)
				{
					//ShellExecute(NULL, NULL, strFilePath, NULL, NULL, SW_RESTORE);
				}
				else return;
				CDatabase db;//���ݿ����Ҫ����ͷ�ļ� #include <afxdb.h>
				CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel����
				CString sSql, arr[2];
				sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s", sDriver, strFilePath, strFilePath);
				if (!db.OpenEx(sSql, CDatabase::noOdbcDialog))//��������ԴDJB��xls
				{
					MessageBox("��EXCEL�ļ�ʧ��!", "����");
					return;
				}
				//��EXCEL��
				CRecordset pset(&db);
				m_list.DeleteAllItems();
				//AfxMessageBox("OK");
				//if (MessageBox("ȷ��������?!", "ȷ��"))
				if (MessageBox("ȷ����������", "ȷ��", MB_ICONEXCLAMATION | MB_OKCANCEL) == IDCANCEL)
					return;
				else
				{
					/* sSql = "SELECT ѧ��,����,�ɼ� "
					"FROM EXCELDEMO";      */
					// "ORDER BY ����";
					sSql.Format("SELECT * FROM DSO_DX");
					pset.Open(CRecordset::forwardOnly, sSql, CRecordset::readOnly);
					int idz = 1; //id����
					strSQL.Format("insert into dyb (bm,id) values('%s','%s') ", m_bmData, user);//ע��һ��Ҫд��һ�У����ұ���Ҫ��/'/'
					mysql_query(&mysql, strSQL);
					strSQL.Format("create table %s (id varchar(20),no varchar(20),att varchar(20),sum varchar(20),lea varchar(20),leav varchar(20));", m_bmData);//ע��һ��Ҫд��һ�У����ұ���Ҫ��/'/'
					mysql_query(&mysql, strSQL);
					int sb = 0;
					int cg = 0;
					//��ȡ��һ����Ԫ���ֵ
					while (!pset.IsEOF())
					{
						arr[0];
						pset.GetFieldValue("no", arr[0]);//ǰ���ֶα�������е���ͬ���������
						int count = m_list.GetItemCount();//���뵽ListCtrl��
						strSQL.Format("insert into %s (id,no) values(%d,'%s')", m_bmData, idz, arr[0]);//ע��һ��Ҫд��һ�У����ұ���Ҫ��/'/'
						//mysql_real_query(&mysql, (char*)(LPCTSTR)strSQL, (UINT)strSQL.GetLength());
						if (mysql_real_query(&mysql, (char*)(LPCTSTR)strSQL, (UINT)strSQL.GetLength()) != 0)
						{
							sb++;
						}
						else
						{
							cg++;
						}
						idz++;
						pset.MoveNext();
					}
					db.Close();
					strSQL.Format("update %s set att=0,sum=0,lea=0,leav=0;", m_bmData);//��ʼ��Ϊ0
					mysql_query(&mysql, strSQL);
					CString temp;
					temp.Format("�ɹ����� %d ������\nʧ�� %d ��", cg, sb);
					MessageBox(temp, "����ɹ�");
					//ˢ����ҳ���б�
					m_mdData.AddString(_T(m_bmData));
					//����Ӧ�߶�
					CRect rc;
					m_mdData.SetCurSel(0);
					m_mdData.GetDroppedControlRect(&rc);
					m_mdData.GetParent()->ScreenToClient(&rc);
					rc.bottom += 150;
					m_mdData.MoveWindow(&rc);
					mysql_free_result(resultset);
					int selectedPos = m_mdData.GetCurSel();
					m_mdData.GetLBText(selectedPos, result1);
				}
			}
		}
	}
}
