#pragma once

struct EnumStringType
{
	char* pstr;
	int index;
};

EnumStringType g_CampaignFlag[2];
char CampaignFlag1[] = "Use_Alternate_Campaign_Finale";
char CampaignFlag2[] = "CFA_CampaignFlag2";

void __fastcall initializeEnumStringType() {
	g_CampaignFlag[0].pstr = CampaignFlag1;
	g_CampaignFlag[0].index = 0;
	g_CampaignFlag[1].pstr = CampaignFlag2;
	g_CampaignFlag[1].index = 1;
}
