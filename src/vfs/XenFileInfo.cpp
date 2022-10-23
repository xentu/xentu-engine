#include "XenFileInfo.h"
#include "../StringUtils.h"

namespace xen
{
	XenFileInfo::XenFileInfo()
	{ }

	XenFileInfo::~XenFileInfo()
	{ }

	XenFileInfo::XenFileInfo(const std::string& basePath, const std::string& fileName, bool isDir)
	{
		Initialize(basePath, fileName, isDir);
	}


	XenFileInfo::XenFileInfo(const std::string& filePath)
	{
		std::size_t found = filePath.rfind("/");
		if (found != std::string::npos)
		{
			const std::string basePath = filePath.substr(0, found + 1);
			std::string fileName;
			if (found != filePath.length())
			{
					fileName = filePath.substr(found + 1, filePath.length() - found - 1);
			}
			
			Initialize(basePath, fileName, false);
		}
	}


	void XenFileInfo::Initialize(const std::string& basePath, const std::string& fileName, bool isDir)
	{
		m_BasePath = basePath;
		m_Name = fileName;
		m_IsDir = isDir;
		
		if (!StringUtils::EndsWith(m_BasePath, "/")) {
			m_BasePath += "/";
		}
		
		if (isDir && !StringUtils::EndsWith(m_Name, "/")) {
			m_Name += "/";
		}
		
		if (StringUtils::StartsWith(m_Name, "/")) {
			m_Name = m_Name.substr(1, m_Name.length() - 1);
		}
		
		m_AbsolutePath = m_BasePath + m_Name;
		
		size_t dotsNum = std::count(m_Name.begin(), m_Name.end(), '.');
		bool isDotOrDotDot = (dotsNum == m_Name.length() && isDir);
		
		if (!isDotOrDotDot) {
			std::size_t found = m_Name.rfind(".");
			if (found != std::string::npos) {
				m_BaseName = m_Name.substr(0, found);
				if (found < m_Name.length()) {
					m_Extension = m_Name.substr(found, m_Name.length() - found);
				}
			}
		}
	}


	const std::string& XenFileInfo::Name() const
	{
		return m_Name;
	}


	const std::string& XenFileInfo::BaseName() const
	{
		return m_BaseName;
	}


	const std::string& XenFileInfo::Extension() const
	{
		return m_Extension;
	}


	const std::string& XenFileInfo::AbsolutePath() const
	{
		return m_AbsolutePath;
	}


	const std::string& XenFileInfo::BasePath() const
	{
		return m_BasePath;
	}


	bool XenFileInfo::IsDir() const
	{
		return m_IsDir;
	}


	bool XenFileInfo::IsValid() const
	{
		return !m_AbsolutePath.empty();
	}
}