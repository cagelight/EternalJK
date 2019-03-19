/*
Author: Blackwolf
Discord Integration with some usefull functions, have fun.
You need to link the static library also 'discord_rpc.lib'.
*/
#if defined DISCORD && !(_DEBUG)
#include <discord_rpc.h>
#include <discord_register.h>

#include "client/client.h"
#include "client/cl_cgameapi.h"

//Author: Blackwolf
//This is my Application ID of the Discord Developer section, feel free to use it.
static const char* APPLICATION_ID = "459923453139746816";

typedef struct mapIcon_s {
	char *map;
	char *imap;
} mapIcon_t;

static mapIcon_t mapIcon[] = {
	{ "academy1",			"academy1"			},
	{ "academy2",			"academy2"			},
	{ "academy3",			"academy3"			},
	{ "academy4",			"academy4"			},
	{ "academy5",			"academy5"			},
	{ "academy6",			"academy6"			},
	{ "hoth2",				"hoth2"				},
	{ "hoth3",				"hoth3"				},
	{ "kor1",				"kor1"				},
	{ "kor2",				"kor2"				},
	{ "t1_danger",			"t1_danger1"		},
	{ "t1_fatal",			"t1_fatal"			},
	{ "t1_inter",			"t1_inter"			},
	{ "t1_rail",			"t1_rail"			},
	{ "t1_sour",			"t1_sour"			},
	{ "t1_surprise",		"t1_surprise"		},
	{ "t2_dpred",			"t2_dpred"			},
	{ "t2_rancor",			"t2_rancor"			},
	{ "t2_rogue",			"t2_rogue"			},
	{ "t2_trip",			"t2_trip"			},
	{ "t2_wedge",			"t2_wedge"			},
	{ "t3_bounty",			"t3_bounty"			},
	{ "t3_byss",			"t3_byss"			},
	{ "t3_hevil",			"t3_hevil"			},
	{ "t3_rift",			"t3_rift"			},
	{ "t3_stamp",			"t3_stamp"			},
	{ "taspir1",			"taspir1"			},
	{ "taspir2",			"taspir2"			},
	{ "vjun1",				"vjun1"				},
	{ "vjun2",				"vjun2"				},
	{ "vjun3",				"vjun3"				},
	{ "yavin1b",			"yavin1b"			},
	{ "yavin2",				"yavin2"			},
	{ "mp/ctf1",			"ctf1"				},
	{ "mp/ctf2",			"ctf2"				},
	{ "mp/ctf3",			"ctf3"				},
	{ "mp/ctf4",			"ctf4"				},
	{ "mp/ctf5",			"ctf5"				},
	{ "mp/duel1",			"duel1"				},
	{ "mp/duel2",			"duel2"				},
	{ "mp/duel3",			"duel3"				},
	{ "mp/duel4",			"duel4"				},
	{ "mp/duel5",			"duel5"				},
	{ "mp/duel6",			"duel6"				},
	{ "mp/duel7",			"duel7"				},
	{ "mp/duel8",			"duel8"				},
	{ "mp/duel9",			"duel9"				},
	{ "mp/duel10",			"duel10"			},
	{ "mp/ffa1",			"ffa1"				},
	{ "mp/ffa2",			"ffa2"				},
	{ "mp/ffa3",			"ffa3"				},
	{ "mp/ffa4",			"ffa4"				},
	{ "mp/ffa5",			"ffa5"				},
	{ "mp/siege_desert",	"siege_desert"		},
	{ "mp/siege_hoth",		"siege_hoth"		},
	{ "mp/siege_korriban",	"siege_korriban"	},

	{ "racearena",			"racearena",		},
	{ "racearena_pro",		"racearena",		},
	{ "racepack1",			"racepack1",		},
	{ "racepack2",			"racepack2",		},
	{ "racepack3",			"racepack3",		},
	{ "racepack4",			"racepack4",		},
	{ "racepack5",			"racepack5",		},
	{ "racepack6",			"racepack6",		},
	{ "racepack7",			"racepack7",		},
}; static const size_t numMapIcon = ARRAY_LEN( mapIcon );

void LowerChars( char *text ) {
	char *ptr = text;

	while ( *ptr )
	{
		if ( *ptr == '\0' )
			break;

		*ptr = tolower( *ptr );
		*ptr++;
	}
}

char *ReturnMapName() {
	if ( cls.state == CA_DISCONNECTED || cls.state == CA_CONNECTING )
	{
		return "menu";
	}
	
	char *mapname = Info_ValueForKey( cl.gameState.stringData + cl.gameState.stringOffsets[CS_SERVERINFO], "mapname" );
	Q_StripColor( mapname );
	LowerChars( mapname );
	return mapname;
}

char *ReturnServerName() {
	char *servername = Cvar_VariableString("ui_about_hostname");

	Q_strstrip(servername, "\xac\x82\xe2\xa2\x80", NULL);
	while (*servername == '\x20' || *servername == '\x2e') *servername++;

	Q_StripColor( servername );	
	return servername;
}

char *ReturnMapIcon() {
	char *mapname = ReturnMapName();

	for ( int i = 0; i < numMapIcon; i++ )
	{
		if ( !strcmp( mapname, mapIcon[i].map ) )
		{
			return mapIcon[i].imap;
		}

	}

	return "icon";
}

char *GetState()
{
	usercmd_t cmd;
	CL_GetUserCmd( cl.cmdNumber, &cmd );

	if ( cls.state == CA_ACTIVE && cmd.buttons & BUTTON_TALK )
	{
		return "chatting";
	}
	else if ( cls.state == CA_ACTIVE )
	{
		return "playing";
	}
	else if ( cls.state == CA_LOADING || cls.state == CA_CONNECTED || cls.state == CA_CHALLENGING )
	{
		return "connecting";
	}
	else if ( cls.state == CA_DISCONNECTED )
	{
		return "menu";
	}

	return NULL;
}

char *joinSecret() {
	
	if ( cls.state == CA_ACTIVE )
	{
		char *x = (char *)malloc( sizeof( char ) * 128 );

		Q_strncpyz( x, va( "%s %s", cls.servername, Cvar_VariableString( "fs_game" ) ), 128 );
		return x;
	}
		
	return NULL;
}

char *PartyID() {

	if ( cls.state == CA_ACTIVE ) 
	{
		char *x = (char *)malloc( sizeof( char ) * 128 );

		Q_strncpyz( x, va( "%s", cls.servername ), 128 );
		strcat( x, "x" );
		return x;
	}

	return NULL;
}

char *GetServerState() {
	if ( cls.state == CA_ACTIVE )
		return va("%d / %d players [%d BOTS]", cl.playerCount, cl.maxPlayers, cl.botCount);

	if ( cls.state == CA_CONNECTING )
		return "Connecting";

	if ( cls.state == CA_DISCONNECTED || cls.state == CA_CINEMATIC )
		return "";

	return NULL;
}

char *GetServerDetails() {
	if ( cls.state == CA_ACTIVE )
		return ReturnServerName();

	if ( cls.state == CA_CONNECTING )
		return "...";

	if ( cls.state == CA_DISCONNECTED || cls.state == CA_CINEMATIC )
		return "In Menu";

	return NULL;
}

static void handleDiscordReady( const DiscordUser* connectedUser )
{
	if (Q_stricmp(Cvar_VariableString("se_language"), "german")) {
		Com_Printf( "^5Discord: ^6connected to user ^3%s^7#^3%s ^7- ^3%s^7\n",
			connectedUser->username,
			connectedUser->discriminator,
			connectedUser->userId );
	}
	else {
		Com_Printf( "^1Discord: ^7verbunden zu Nutzer ^3%s^7#^3%s ^7- ^3%s^7\n",
			connectedUser->username,
			connectedUser->discriminator,
			connectedUser->userId );
	}
}

static void handleDiscordDisconnected( int errcode, const char* message )
{
	if (Q_stricmp(Cvar_VariableString("se_language"), "german"))
		Com_Printf( "^5Discord: ^3disconnected (^3%d^7: ^3%s^7)\n", errcode, message );
	else
		Com_Printf( "^1Discord: ^7getrennt (^3%d^7: ^3%s^7)\n", errcode, message );
}

static void handleDiscordError( int errcode, const char* message )
{
	if (Q_stricmp(Cvar_VariableString("se_language"), "german"))
		Com_Printf( "^5Discord: ^1Error - (^3%d^7: ^3%s^7)\n", errcode, message );
	else
		Com_Printf( "^1Discord: ^7Fehler (^3%d^7: ^3%s^7)\n", errcode, message );
}

static void handleDiscordJoin( const char* secret )
{
	char ip[60] = { 0 };
	char fsgame[60] = { 0 };

	if (Q_stricmp(Cvar_VariableString("se_language"), "german"))
		Com_Printf( "^5Discord: ^7joining (^3%s^7)\n", secret );
	else
		Com_Printf( "^1Discord: ^7join (^3%s^7)\n", secret );
	sscanf( secret, "%s %s", ip, fsgame );
	Cbuf_AddText( va( "connect %s\n", ip ) );
}

static void handleDiscordSpectate( const char* secret )
{
	if (Q_stricmp(Cvar_VariableString("se_language"), "german"))
		Com_Printf( "^5Discord: ^7spectating (^3%s^7)\n", secret );
	else
		Com_Printf( "^1Discord: ^7spectate (^3%s^7)\n", secret );
}

static void handleDiscordJoinRequest( const DiscordUser* request )
{
	int response = -1;

	if (Q_stricmp(Cvar_VariableString("se_language"), "german")) {
		Com_Printf( "^5Discord: ^7join request from ^3%s^7#^3%s ^7- ^3%s^7\n",
			request->username,
			request->discriminator,
			request->userId );
	}
	else {
		Com_Printf( "^1Discord: ^7join request from ^3%s^7#^3%s ^7- ^3%s^7\n",
				request->username,
				request->discriminator,
				request->userId );
	}

	if ( response != -1 ) {
		Discord_Respond( request->userId, response );
	}
}

static DiscordRichPresence discordPresence;
static DiscordEventHandlers handlers;
void discordInit()
{
	memset( &handlers, 0, sizeof( handlers ) );
	handlers.ready = handleDiscordReady;
	handlers.disconnected = handleDiscordDisconnected;
	handlers.errored = handleDiscordError;
	handlers.joinGame = handleDiscordJoin;
	handlers.spectateGame = handleDiscordSpectate;
	handlers.joinRequest = handleDiscordJoinRequest;
	
	Discord_Initialize( APPLICATION_ID, &handlers, 1, "6020" );
	Discord_Register( APPLICATION_ID, NULL );

	Discord_UpdateHandlers( &handlers );
}

void discordShutdown()
{
	Discord_Shutdown();
}

void updateDiscordPresence()
{
	memset( &discordPresence, 0, sizeof( discordPresence ) );

	char *partyID = PartyID();
	char *joinID = joinSecret();
	
	discordPresence.state = GetServerState();
	discordPresence.details = GetServerDetails();
	discordPresence.largeImageKey = ReturnMapIcon();
	discordPresence.largeImageText = ReturnMapName();
	discordPresence.smallImageKey = GetState();
	discordPresence.smallImageText = GetState();
	discordPresence.partyId = partyID; // Server-IP zum abgleichen discordchat
	discordPresence.partySize = cls.state == CA_ACTIVE ? 1 : NULL;
	discordPresence.partyMax = cls.state == CA_ACTIVE ? ((cl.maxPlayers - cl.playerCount) + discordPresence.partySize) : NULL;
	discordPresence.joinSecret = joinID; // Server-IP zum discordJoin ausf�hren
	Discord_UpdatePresence( &discordPresence );

	Discord_RunCallbacks();
}

#endif