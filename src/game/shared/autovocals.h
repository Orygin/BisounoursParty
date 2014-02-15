#ifndef AUTOVOCALS_H
#define AUTOVOCALS_H

enum // Il faut que l'ordre corresponde au tableau d'en dessous
{
	CRITICALHEALTHPICKUP = 0,
	BATPICKUP,
	PUSHBALLPICKUP,
	HIGHSPEED,
	RAMPAGE,
	NEWPLAYER,
	NEWWEAPON,
	BATFRAG,
	PUSHFRAG,
	MINEFRAG,
	ARMORPICKUP,
	GENOCIDE,
	GRENADEHIT,
	RSASUICIDE, // RSA = Respawn After
	RSACHANGETEAM,
	RSATKED,
	TK,
	HEALED,
	GEOLOCALISATION,
	TAUNT_RAMPAGE,
	TAUNT_GENOCIDE,
	NPCKZOMBIE, // NPCK = NPC Kill
	NPCKPEDOBEAR,
	NPCKBISOUZOMBIE,
	TAKEFLAG
};
static const char *g_ppszAutovocals[] = 
{
	"Autovocal.CriticalHealthPickup",
	"Autovocal.BatPickup",
	"Autovocal.PushBallPickup",
	"Autovocal.HighSpeed",
	"Autovocal.Rampage",
	"Autovocal.NewPlayer",
	"Autovocal.NewWeapon",
	"Autovocal.BatFrag",
	"Autovocal.PushFrag",
	"Autovocal.MineFrag",
	"Autovocal.ArmorPickup",
	"Autovocal.Genocide",
	"Autovocal.GrenadeHit",
	"Autovocal.RespawnAfterSuicide",
	"Autovocal.RespawnAfterChangeteam",
	"Autovocal.RespawnAfterTKed",
	"Autovocal.TK",
	"Autovocal.Healed",
	"Autovocal.Geolocalisation",
	"Autovocal.Taunt",
	"Autovocal.Taunt",
	"Autovocal.Coop.Frag.Epice",
	"Autovocal.Coop.Frag.Pedobear",
	"Autovocal.Coop.Frag.Zombie",
	"Autovocal.Takeflag"
};
#define AUTOVOCAL_NONE -1
#define AUTOVOCAL_MAX ARRAYSIZE( g_ppszAutovocals )
#endif
