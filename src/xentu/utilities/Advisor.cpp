#ifndef XEN_ADVISOR_CPP
#define XEN_ADVISOR_CPP

#include "Advisor.h"


namespace xen
{
	bool Advisor::m_show_errors = false;
	bool Advisor::m_show_warnings = false;
	bool Advisor::m_show_advice = true;
	bool Advisor::m_show_info = true;
	bool Advisor::m_show_date = true;


	void Advisor::setMode(bool errors, bool warnings, bool advice, bool info) {
		m_show_errors = errors;
		m_show_warnings = warnings;
		m_show_advice = advice;
		m_show_info = info;
	}


	void Advisor::setModeDate(bool date) {
		m_show_date = date;
	}
}


#endif