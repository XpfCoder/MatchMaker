#pragma once
#include <string>

namespace Resafety{
	namespace DeepStar{
			//资源列表表名字
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

			//资源包信息
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

			//资源列表信息，对应ResourceList.sqlite表中一行的内容
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
				std::string		m_resourceKey;//主键，资源名字
				std::string		m_resourceMdCode;//服务器上面资源的md5码
				ResourceType	m_resourceType;//资源类型 0 公有资源（common_resource文件夹中）；1 私有资源（应用中的资源）
				std::string		m_resourceUpdateTime;//资源更新时间
				std::string		m_resourceGroup;//资源包组，当资源足够多时，可能在多个sqlite文件中
				std::string		m_resourceId;//资源id
				std::string		m_absolute_path = "";//资源的绝对路径，rclone上传会用到
				std::string		m_folderPath = "";//资源的相对路径
			};
	}
}
