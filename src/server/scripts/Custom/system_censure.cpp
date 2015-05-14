#include "ScriptPCH.h"
#include "Channel.h"
#include "Language.h"
 
class System_Censure : public PlayerScript
{
public:
	System_Censure() : PlayerScript("System_Censure") {}
 
	void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg)
	{
		CheckMessage(player, msg, lang, NULL, NULL, NULL, NULL);
	}
 
	void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Player* receiver)
	{
 
		CheckMessage(player, msg, lang, receiver, NULL, NULL, NULL);
	}
 
	void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Group* group)
	{
		CheckMessage(player, msg, lang, NULL, group, NULL, NULL);
	}
 
	void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Guild* guild)
	{
		CheckMessage(player, msg, lang, NULL, NULL, guild, NULL);
	}
 
 
 
void CheckMessage(Player* player, std::string& msg, uint32 lang, Player* /*receiver*/, Group* /*group*/, Guild* /*guild*/, Channel* channel)
{
    std::string lower = msg;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
 
    QueryResult Result = LoginDatabase.Query("SELECT text FROM chat_filter");
 
    if (!Result)
        return;	
 
    do
    {
        Field* Fields = Result->Fetch();
        std::string moton = Fields[0].GetString();
 
        if (lower.find(moton) != std::string::npos)
        {
            msg = "[ Message Not Sent, Reason : Use Bad Word ]";

// added by lizard.tiny report To GM - Start
    std::string str = ""; 
    SessionMap ss = sWorld->GetAllSessions(); 
    for (SessionMap::const_iterator itr = ss.begin(); itr != ss.end(); ++itr) 
		if (itr->second->GetSecurity() > 0) 
            str = "|cFFFF0000[ Anti Spam ]|cFF00FFFF [ |cFF60FF00" + std::string(player->GetName()) + "|cFF00FFFF ] [ |cFF60FF00" + lower + "|cFF00FFFF ] Check it!"; 
    WorldPacket data(SMSG_NOTIFICATION, (str.size()+1)); 
    data << str; 
    sWorld->SendGlobalGMMessage(&data); 
//	Send Report Use : trinity_string
	sWorld->SendGMText(256235, player->GetName().c_str(), lower.c_str());
// added by lizard.tiny report To GM - End

// added by lizard.tiny Mute - Start
	PreparedStatement * mt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_MUTE_TIME);
	int64 muteTime = time(NULL) + 300;
	player->GetSession()->m_muteTime = muteTime;
	mt->setInt64(0, muteTime);
// added by lizard.tiny Mute - End

	ChatHandler(player->GetSession()).PSendSysMessage("|cFFFF0000[ Anti Spam ]|cffB6FF00 [|cffB6FF00 Your Account|cffFF0000 5|cffB6FF00 Minute Muted|cffB6FF00 ] |cffB6FF00 : Message posted by you is contrary to the rules of the Server");
	QueryResult result = LoginDatabase.PQuery("insert into `spammer` (`name`, `spam`) values('%s','%s')", player->GetName().c_str(), lower.c_str());		
			return;
        }
    }
    while (Result->NextRow());
}

// added by lizard.tiny Anti Farm - Start
void OnPVPKill(Player * killer, Player * killed) 
{ 
		if (killer->GetGUID() == killed->GetGUID()) 
		{ 
				return; 
		} 

		if (killer->GetSession()->GetRemoteAddress() == killed->GetSession()->GetRemoteAddress()) 
		{ 
				std::string str = ""; 
				SessionMap ss = sWorld->GetAllSessions(); 
				for (SessionMap::const_iterator itr = ss.begin(); itr != ss.end(); ++itr) 
						if (itr->second->GetSecurity() > 0) 
								str = "|cFFFFFF00[ Anti Farming ]|cFF00FFFF[ |cFF60FF00" + std::string(killer->GetName()) + "|cFF00FFFF ] Check it!"; 
				WorldPacket data(SMSG_NOTIFICATION, (str.size()+1)); 
				data << str; 
				sWorld->SendGlobalGMMessage(&data); 
		} 
} 
// added by lizard.tiny Anti Farm - End

};
 
void AddSC_System_Censure()
{
	new System_Censure();
}