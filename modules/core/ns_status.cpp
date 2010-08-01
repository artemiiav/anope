/* NickServ core functions
 *
 * (C) 2003-2010 Anope Team
 * Contact us at team@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church.
 */

/*************************************************************************/

#include "module.h"

class CommandNSStatus : public Command
{
 public:
	CommandNSStatus() : Command("STATUS", 0, 16)
	{
		this->SetFlag(CFLAG_ALLOW_UNREGISTERED);
	}

	CommandReturn Execute(User *u, const std::vector<Anope::string> &params)
	{
		User *u2;
		Anope::string nick = !params.empty() ? params[0] : u->nick;
		NickAlias *na = findnick(nick);
		spacesepstream sep(nick);
		Anope::string nickbuf;

		while (sep.GetToken(nickbuf))
		{
			if (!(u2 = finduser(nickbuf))) /* Nick is not online */
				notice_lang(Config.s_NickServ, u, NICK_STATUS_REPLY, nickbuf.c_str(), 0, "");
			else if (u2->IsIdentified() && na && na->nc == u2->Account()) /* Nick is identified */
				notice_lang(Config.s_NickServ, u, NICK_STATUS_REPLY, nickbuf.c_str(), 3, u2->Account()->display.c_str());
			else if (u2->IsRecognized()) /* Nick is recognised, but NOT identified */
				notice_lang(Config.s_NickServ, u, NICK_STATUS_REPLY, nickbuf.c_str(), 2, u2->Account() ? u2->Account()->display.c_str() : "");
			else if (!na) /* Nick is online, but NOT a registered */
				notice_lang(Config.s_NickServ, u, NICK_STATUS_REPLY, nickbuf.c_str(), 0, "");
			else
				/* Nick is not identified for the nick, but they could be logged into an account,
				 * so we tell the user about it
				 */
				notice_lang(Config.s_NickServ, u, NICK_STATUS_REPLY, nickbuf.c_str(), 1, u2->Account() ? u2->Account()->display.c_str() : "");
		}
		return MOD_CONT;
	}

	bool OnHelp(User *u, const Anope::string &subcommand)
	{
		notice_help(Config.s_NickServ, u, NICK_HELP_STATUS);
		return true;
	}

	void OnServHelp(User *u)
	{
		notice_lang(Config.s_NickServ, u, NICK_HELP_CMD_STATUS);
	}
};

class NSStatus : public Module
{
	CommandNSStatus commandnsstatus;

 public:
	NSStatus(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator)
	{
		this->SetAuthor("Anope");
		this->SetType(CORE);

		this->AddCommand(NickServ, &commandnsstatus);
	}
};

MODULE_INIT(NSStatus)