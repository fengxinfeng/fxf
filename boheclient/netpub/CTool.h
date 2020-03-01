#pragma once

namespace bh_personpub
{

	class CTool
	{
		CTool();
		~CTool();
	public:
		static std::wstring SelectPath();
		static bool JudgePhoneNumber(string &PhoneNub);
	};

}