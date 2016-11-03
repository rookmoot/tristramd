#include <stdio.h>
#include <stdlib.h>

#include <Eina.h>
#include "net.h"
#include "packet.h"
#include "protocol.h"

#define PROTOCOL_REGISTER(type, opcode, handler) {type, opcode, #handler, handler}

static struct protocol_handler _handlers[] = {
  // BNFTP protocol handlers
  PROTOCOL_REGISTER(PROTOCOL_BNFTP, 0x01, packet_bnftp),
  
  // BNETD protocol handlers
  //PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x00, packet_bnetd_sid_null},
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x02, packet_bnetd_sid_stopadv),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x05, packet_bnetd_sid_clientid),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x06, packet_bnetd_sid_startversioning),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x07, packet_bnetd_sid_reportversion),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x08, packet_bnetd_sid_startadvex),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x09, packet_bnetd_sid_getadvlistex),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x0A, packet_bnetd_sid_enterchat),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x0B, packet_bnetd_sid_getchannellist),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x0C, packet_bnetd_sid_joinchannel),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x0E, packet_bnetd_sid_chatcommand),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x10, packet_bnetd_sid_leavechat),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x12, packet_bnetd_sid_localeinfo),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x14, packet_bnetd_sid_udppingresponse),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x15, packet_bnetd_sid_checkad),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x16, packet_bnetd_sid_clickad),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x17, packet_bnetd_sid_readmemory),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x18, packet_bnetd_sid_registry),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x1A, packet_bnetd_sid_startadvex2),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x1B, packet_bnetd_sid_gamedataaddress),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x1C, packet_bnetd_sid_startadvex3),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x1E, packet_bnetd_sid_clientid2),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x1F, packet_bnetd_sid_leavegame),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x21, packet_bnetd_sid_displayad),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x22, packet_bnetd_sid_notifyjoin),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x24, packet_bnetd_sid_readcookie),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x25, packet_bnetd_sid_ping),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x26, packet_bnetd_sid_readuserdata),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x27, packet_bnetd_sid_writeuserdata),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x29, packet_bnetd_sid_logonresponse),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x2A, packet_bnetd_sid_createaccount),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x2B, packet_bnetd_sid_systeminfo),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x2C, packet_bnetd_sid_gameresult),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x2D, packet_bnetd_sid_geticondata),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x2E, packet_bnetd_sid_getladderdata),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x2F, packet_bnetd_sid_findladderuser),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x30, packet_bnetd_sid_cdkey),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x31, packet_bnetd_sid_changepassword),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x32, packet_bnetd_sid_checkdatafile),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x33, packet_bnetd_sid_getfiletime),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x34, packet_bnetd_sid_queryrealms),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x35, packet_bnetd_sid_profile),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x36, packet_bnetd_sid_cdkey2),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x37, packet_bnetd_sid_unknown_37),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x39, packet_bnetd_sid_unknown_39),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x3A, packet_bnetd_sid_logonresponse2),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x3C, packet_bnetd_sid_checkdatafile2),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x3D, packet_bnetd_sid_createaccount2),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x3E, packet_bnetd_sid_logonrealmex),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x40, packet_bnetd_sid_queryrealms2),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x41, packet_bnetd_sid_queryadurl),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x42, packet_bnetd_sid_cdkey3),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x45, packet_bnetd_sid_netgameport),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x46, packet_bnetd_sid_news_info),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x4B, packet_bnetd_sid_extrawork),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x50, packet_bnetd_sid_auth_info),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x51, packet_bnetd_sid_auth_check),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x52, packet_bnetd_sid_auth_accountcreate),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x53, packet_bnetd_sid_auth_accountlogon),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x54, packet_bnetd_sid_auth_accountlogonproof),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x55, packet_bnetd_sid_auth_accountchange),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x56, packet_bnetd_sid_auth_accountchangeproof),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x57, packet_bnetd_sid_auth_accountupgrade),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x58, packet_bnetd_sid_auth_accountupgradeproof),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x59, packet_bnetd_sid_setemail),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x5A, packet_bnetd_sid_resetpassword),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x5B, packet_bnetd_sid_changeemail),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x5C, packet_bnetd_sid_switchproduct),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x5D, packet_bnetd_sid_reportcrash),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x5E, packet_bnetd_sid_warden),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x60, packet_bnetd_sid_gameplayersearch),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x65, packet_bnetd_sid_friendslist),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x66, packet_bnetd_sid_friendsupdate),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x70, packet_bnetd_sid_clanfindcandidates),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x71, packet_bnetd_sid_claninvitemultiple),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x72, packet_bnetd_sid_clancreationinvitation),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x73, packet_bnetd_sid_clandisband),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x74, packet_bnetd_sid_clanmakechieftain),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x77, packet_bnetd_sid_claninvitation),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x78, packet_bnetd_sid_clanremovemember),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x79, packet_bnetd_sid_claninvitationresponse),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x7A, packet_bnetd_sid_clanrankchange),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x7B, packet_bnetd_sid_clansetmotd),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x7C, packet_bnetd_sid_clanmotd),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x7D, packet_bnetd_sid_clanmemberlist),
  PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x82, packet_bnetd_sid_clanmemberinformation),
  

  // REALM protocol headers
  PROTOCOL_REGISTER(PROTOCOL_REALM, 0x01, packet_realm_mcp_startup),
  PROTOCOL_REGISTER(PROTOCOL_REALM, 0x02, packet_realm_mcp_charcreate),
  PROTOCOL_REGISTER(PROTOCOL_REALM, 0x03, packet_realm_mcp_creategame),
  PROTOCOL_REGISTER(PROTOCOL_REALM, 0x04, packet_realm_mcp_joingame),
  PROTOCOL_REGISTER(PROTOCOL_REALM, 0x05, packet_realm_mcp_gamelist),
  PROTOCOL_REGISTER(PROTOCOL_REALM, 0x06, packet_realm_mcp_gameinfo),
  PROTOCOL_REGISTER(PROTOCOL_REALM, 0x07, packet_realm_mcp_charlogon),
  PROTOCOL_REGISTER(PROTOCOL_REALM, 0x0A, packet_realm_mcp_chardelete),
  PROTOCOL_REGISTER(PROTOCOL_REALM, 0x11, packet_realm_mcp_requestladderdata),
  PROTOCOL_REGISTER(PROTOCOL_REALM, 0x12, packet_realm_mcp_motd),
  PROTOCOL_REGISTER(PROTOCOL_REALM, 0x17, packet_realm_mcp_charlist),
  PROTOCOL_REGISTER(PROTOCOL_REALM, 0x18, packet_realm_mcp_charupgrade),
  PROTOCOL_REGISTER(PROTOCOL_REALM, 0x19, packet_realm_mcp_charlist2),

  /* D2GS protocol headers */
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x01, packet_d2gs_d2gs_walktolocation),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x02, packet_d2gs_d2gs_walktoentity),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x03, packet_d2gs_d2gs_runtolocation),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x04, packet_d2gs_d2gs_runtoentity),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x05, packet_d2gs_d2gs_leftskillonlocation),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x06, packet_d2gs_d2gs_leftskillonentity),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x07, packet_d2gs_d2gs_leftskillonentityex),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x08, packet_d2gs_d2gs_leftskillonlocationex),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x09, packet_d2gs_d2gs_leftskillonentityex2),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x0A, packet_d2gs_d2gs_leftskillonentityex3),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x0C, packet_d2gs_d2gs_rightskillonlocation),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x0D, packet_d2gs_d2gs_rightskillonentity),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x0E, packet_d2gs_d2gs_rightskillonentityex),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x0F, packet_d2gs_d2gs_rightskillonlocationex),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x10, packet_d2gs_d2gs_rightskillonentityex2),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x11, packet_d2gs_d2gs_rightskillonentityex3),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x13, packet_d2gs_d2gs_interactwithentity),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x14, packet_d2gs_d2gs_overheadmessage),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x16, packet_d2gs_d2gs_pickupitem),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x17, packet_d2gs_d2gs_dropitem),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x18, packet_d2gs_d2gs_itemtobuffer),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x19, packet_d2gs_d2gs_pickupbufferitem),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x1A, packet_d2gs_d2gs_itemtobody),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x1B, packet_d2gs_d2gs_swap2handeditem),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x1C, packet_d2gs_d2gs_pickupbodyitem),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x1D, packet_d2gs_d2gs_switchbodyitem),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x1F, packet_d2gs_d2gs_switchinventoryitem),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x20, packet_d2gs_d2gs_useitem),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x21, packet_d2gs_d2gs_stackitem),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x22, packet_d2gs_d2gs_removestackitem),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x23, packet_d2gs_d2gs_itemtobelt),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x24, packet_d2gs_d2gs_removebeltitem),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x25, packet_d2gs_d2gs_switchbeltitem),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x26, packet_d2gs_d2gs_usebeltitem),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x28, packet_d2gs_d2gs_insertsocketitem),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x29, packet_d2gs_d2gs_scrolltotome),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x2A, packet_d2gs_d2gs_itemtocube),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x2D, packet_d2gs_d2gs_unselectobj),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x2F, packet_d2gs_d2gs_npcinit),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x30, packet_d2gs_d2gs_npccancel),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x32, packet_d2gs_d2gs_npcbuy),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x33, packet_d2gs_d2gs_npcsell),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x38, packet_d2gs_d2gs_npctrade),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x3F, packet_d2gs_d2gs_characterphrase),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x49, packet_d2gs_d2gs_waypoint),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x4F, packet_d2gs_d2gs_trade),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x50, packet_d2gs_d2gs_dropgold),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x5E, packet_d2gs_d2gs_party),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x61, packet_d2gs_d2gs_potiontomercenary),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x68, packet_d2gs_d2gs_gamelogon),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x6A, packet_d2gs_d2gs_entergameenvironment),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x6b, packet_d2gs_d2gs_entergameenvironment),
  PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x6D, packet_d2gs_d2gs_ping),
  
  // NULL terminated list
  {-1, -1, NULL, NULL}
};

void protocol_init(void) {}

void protocol_shutdown(void) {}

void protocol_handler_run(struct connection *conn, struct packet *packet) {
  int i;

  for (i = 0; _handlers[i].protocol != -1; i++) {
    if ((unsigned int)_handlers[i].protocol == packet->protocol &&
	_handlers[i].opcode == packet->header.opcode) {
      _handlers[i].handler(conn, packet);
    }
  }
}

int protocol_handler_find(struct packet *packet, struct protocol_handler *handler) {
  int i;

  for (i = 0; _handlers[i].protocol != -1; i++) {
    if ((unsigned int)_handlers[i].protocol == packet->protocol &&
	_handlers[i].opcode == packet->header.opcode) {
      *handler = _handlers[i];
      return 1;
    }
  }
  return 0;
}
