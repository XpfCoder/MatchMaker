#pragma once
#include <string>

namespace Resafety{
	namespace DeepStar{
			//��Դ�б������
			static std::string S_USERACCOUNT = "UserAccount";
			static std::string S_MALEINFO = "MaleInfo";
			static std::string S_FEMALEINFO = "FemaleInfo";

			enum ResourceManagerType
			{
				WebResourceManager = 0,
				RcloneResourceManager
			};
			enum ResourceType
			{
				PublicResource = 0,
				PrivateResource
			};

			enum ResourceUpdateFlag
			{
				Unknown=-1,
				NeedDownLoad = 0,
				DONTUPDATE,
				UPDATE
			};

			//��Դ����Ϣ
			typedef struct ResourceInfo
			{
			public:
				std::string m_resId = "";
				std::string m_key = "";
				std::string m_md5 = "";
				int64_t m_updateTime = 0;
			public:
				ResourceInfo()
				{
					//m_value = nullptr;
				}

				~ResourceInfo()
				{
					//if (m_value)
					//{
					//	m_value->release();
					//}
				}
			}ResourceInfo;

			//��Դ�б���Ϣ����ӦResourceList.sqlite����һ�е�����
			struct ResourceListInfo
			{
				ResourceListInfo()
				{
					m_resourceKey = "";
					m_resourceMdCode = "";
					m_resourceType = PrivateResource;
					m_resourceUpdateTime = "";
					m_resourceGroup = "";
					m_resourceId = "";
					m_folderPath = "";
				}
				std::string		m_resourceKey;//��������Դ����
				std::string		m_resourceMdCode;//������������Դ��md5��
				ResourceType	m_resourceType;//��Դ���� 0 ������Դ��common_resource�ļ����У���1 ˽����Դ��Ӧ���е���Դ��
				std::string		m_resourceUpdateTime;//��Դ����ʱ��
				std::string		m_resourceGroup;//��Դ���飬����Դ�㹻��ʱ�������ڶ��sqlite�ļ���
				std::string		m_resourceId;//��Դid
				std::string		m_absolute_path = "";//��Դ�ľ���·����rclone�ϴ����õ�
				std::string		m_folderPath = "";//��Դ�����·��
			};
	}
}
