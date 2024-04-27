#pragma once
#include <string>
class PublicDataObj
{
public:
	std::string m_name;
	std::string m_imagePath;
	std::string m_unitPath;
};

typedef std::shared_ptr<PublicDataObj> PublicDataObjPtr;
Q_DECLARE_METATYPE(PublicDataObjPtr);