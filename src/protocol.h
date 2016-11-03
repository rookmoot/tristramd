#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stdint.h>
#include "packet.h"

typedef void (*protocol_handler_cb)(struct connection *conn, struct packet *request);

struct protocol_handler {
  int protocol;
  uint8_t opcode;
  char *name;
  protocol_handler_cb handler;
};

void protocol_init(void);
void protocol_shutdown(void);
void protocol_handler_run(struct connection *conn, struct packet *packet);
int  protocol_handler_find(struct packet *packet, struct protocol_handler *handler);

/* BNFTP */
void packet_bnftp(struct connection *conn, struct packet *request);

/* BNETD */
void packet_bnetd_sid_null(struct connection *conn, struct packet *request);
void packet_bnetd_sid_stopadv(struct connection *conn, struct packet *request);
void packet_bnetd_sid_clientid(struct connection *conn, struct packet *request);
void packet_bnetd_sid_startversioning(struct connection *conn, struct packet *request);
void packet_bnetd_sid_reportversion(struct connection *conn, struct packet *request);
void packet_bnetd_sid_startadvex(struct connection *conn, struct packet *request);
void packet_bnetd_sid_getadvlistex(struct connection *conn, struct packet *request);
void packet_bnetd_sid_enterchat(struct connection *conn, struct packet *request);
void packet_bnetd_sid_getchannellist(struct connection *conn, struct packet *request);
void packet_bnetd_sid_joinchannel(struct connection *conn, struct packet *request);
void packet_bnetd_sid_chatcommand(struct connection *conn, struct packet *request);
void packet_bnetd_sid_leavechat(struct connection *conn, struct packet *request);
void packet_bnetd_sid_localeinfo(struct connection *conn, struct packet *request);
void packet_bnetd_sid_udppingresponse(struct connection *conn, struct packet *request);
void packet_bnetd_sid_checkad(struct connection *conn, struct packet *request);
void packet_bnetd_sid_clickad(struct connection *conn, struct packet *request);
void packet_bnetd_sid_readmemory(struct connection *conn, struct packet *request);
void packet_bnetd_sid_registry(struct connection *conn, struct packet *request);
void packet_bnetd_sid_startadvex2(struct connection *conn, struct packet *request);
void packet_bnetd_sid_gamedataaddress(struct connection *conn, struct packet *request);
void packet_bnetd_sid_startadvex3(struct connection *conn, struct packet *request);
void packet_bnetd_sid_clientid2(struct connection *conn, struct packet *request);
void packet_bnetd_sid_leavegame(struct connection *conn, struct packet *request);
void packet_bnetd_sid_displayad(struct connection *conn, struct packet *request);
void packet_bnetd_sid_notifyjoin(struct connection *conn, struct packet *request);
void packet_bnetd_sid_readcookie(struct connection *conn, struct packet *request);
void packet_bnetd_sid_ping(struct connection *conn, struct packet *request);
void packet_bnetd_sid_readuserdata(struct connection *conn, struct packet *request);
void packet_bnetd_sid_writeuserdata(struct connection *conn, struct packet *request);
void packet_bnetd_sid_logonresponse(struct connection *conn, struct packet *request);
void packet_bnetd_sid_createaccount(struct connection *conn, struct packet *request);
void packet_bnetd_sid_systeminfo(struct connection *conn, struct packet *request);
void packet_bnetd_sid_gameresult(struct connection *conn, struct packet *request);
void packet_bnetd_sid_geticondata(struct connection *conn, struct packet *request);
void packet_bnetd_sid_getladderdata(struct connection *conn, struct packet *request);
void packet_bnetd_sid_findladderuser(struct connection *conn, struct packet *request);
void packet_bnetd_sid_cdkey(struct connection *conn, struct packet *request);
void packet_bnetd_sid_changepassword(struct connection *conn, struct packet *request);
void packet_bnetd_sid_checkdatafile(struct connection *conn, struct packet *request);
void packet_bnetd_sid_getfiletime(struct connection *conn, struct packet *request);
void packet_bnetd_sid_queryrealms(struct connection *conn, struct packet *request);
void packet_bnetd_sid_profile(struct connection *conn, struct packet *request);
void packet_bnetd_sid_cdkey2(struct connection *conn, struct packet *request);
void packet_bnetd_sid_unknown_37(struct connection *conn, struct packet *request);
void packet_bnetd_sid_unknown_39(struct connection *conn, struct packet *request);
void packet_bnetd_sid_logonresponse2(struct connection *conn, struct packet *request);
void packet_bnetd_sid_checkdatafile2(struct connection *conn, struct packet *request);
void packet_bnetd_sid_createaccount2(struct connection *conn, struct packet *request);
void packet_bnetd_sid_logonrealmex(struct connection *conn, struct packet *request);
void packet_bnetd_sid_queryrealms2(struct connection *conn, struct packet *request);
void packet_bnetd_sid_queryadurl(struct connection *conn, struct packet *request);
void packet_bnetd_sid_cdkey3(struct connection *conn, struct packet *request);
void packet_bnetd_sid_netgameport(struct connection *conn, struct packet *request);
void packet_bnetd_sid_news_info(struct connection *conn, struct packet *request);
void packet_bnetd_sid_extrawork(struct connection *conn, struct packet *request);
void packet_bnetd_sid_auth_info(struct connection *conn, struct packet *request);
void packet_bnetd_sid_auth_check(struct connection *conn, struct packet *request);
void packet_bnetd_sid_auth_accountcreate(struct connection *conn, struct packet *request);
void packet_bnetd_sid_auth_accountlogon(struct connection *conn, struct packet *request);
void packet_bnetd_sid_auth_accountlogonproof(struct connection *conn, struct packet *request);
void packet_bnetd_sid_auth_accountchange(struct connection *conn, struct packet *request);
void packet_bnetd_sid_auth_accountchangeproof(struct connection *conn, struct packet *request);
void packet_bnetd_sid_auth_accountupgrade(struct connection *conn, struct packet *request);
void packet_bnetd_sid_auth_accountupgradeproof(struct connection *conn, struct packet *request);
void packet_bnetd_sid_setemail(struct connection *conn, struct packet *request);
void packet_bnetd_sid_resetpassword(struct connection *conn, struct packet *request);
void packet_bnetd_sid_changeemail(struct connection *conn, struct packet *request);
void packet_bnetd_sid_switchproduct(struct connection *conn, struct packet *request);
void packet_bnetd_sid_reportcrash(struct connection *conn, struct packet *request);
void packet_bnetd_sid_warden(struct connection *conn, struct packet *request);
void packet_bnetd_sid_gameplayersearch(struct connection *conn, struct packet *request);
void packet_bnetd_sid_friendslist(struct connection *conn, struct packet *request);
void packet_bnetd_sid_friendsupdate(struct connection *conn, struct packet *request);
void packet_bnetd_sid_clanfindcandidates(struct connection *conn, struct packet *request);
void packet_bnetd_sid_claninvitemultiple(struct connection *conn, struct packet *request);
void packet_bnetd_sid_clancreationinvitation(struct connection *conn, struct packet *request);
void packet_bnetd_sid_clandisband(struct connection *conn, struct packet *request);
void packet_bnetd_sid_clanmakechieftain(struct connection *conn, struct packet *request);
void packet_bnetd_sid_claninvitation(struct connection *conn, struct packet *request);
void packet_bnetd_sid_clanremovemember(struct connection *conn, struct packet *request);
void packet_bnetd_sid_claninvitationresponse(struct connection *conn, struct packet *request);
void packet_bnetd_sid_clanrankchange(struct connection *conn, struct packet *request);
void packet_bnetd_sid_clansetmotd(struct connection *conn, struct packet *request);
void packet_bnetd_sid_clanmotd(struct connection *conn, struct packet *request);
void packet_bnetd_sid_clanmemberlist(struct connection *conn, struct packet *request);
void packet_bnetd_sid_clanmemberinformation(struct connection *conn, struct packet *request);

/* REALM */
void packet_realm_mcp_startup(struct connection *conn, struct packet *request);
void packet_realm_mcp_charcreate(struct connection *conn, struct packet *request);
void packet_realm_mcp_creategame(struct connection *conn, struct packet *request);
void packet_realm_mcp_joingame(struct connection *conn, struct packet *request);
void packet_realm_mcp_gamelist(struct connection *conn, struct packet *request);
void packet_realm_mcp_gameinfo(struct connection *conn, struct packet *request);
void packet_realm_mcp_charlogon(struct connection *conn, struct packet *request);
void packet_realm_mcp_chardelete(struct connection *conn, struct packet *request);
void packet_realm_mcp_requestladderdata(struct connection *conn, struct packet *request);
void packet_realm_mcp_motd(struct connection *conn, struct packet *request);
void packet_realm_mcp_charlist(struct connection *conn, struct packet *request);
void packet_realm_mcp_charupgrade(struct connection *conn, struct packet *request);
void packet_realm_mcp_charlist2(struct connection *conn, struct packet *request);

/* D2GS */
void packet_d2gs_d2gs_walktolocation(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_walktoentity(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_runtolocation(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_runtoentity(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_leftskillonlocation(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_leftskillonentity(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_leftskillonentityex(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_leftskillonlocationex(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_leftskillonentityex2(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_leftskillonentityex3(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_rightskillonlocation(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_rightskillonentity(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_rightskillonentityex(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_rightskillonlocationex(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_rightskillonentityex2(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_rightskillonentityex3(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_interactwithentity(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_overheadmessage(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_pickupitem(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_dropitem(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_itemtobuffer(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_pickupbufferitem(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_itemtobody(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_swap2handeditem(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_pickupbodyitem(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_switchbodyitem(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_switchinventoryitem(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_useitem(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_stackitem(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_removestackitem(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_itemtobelt(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_removebeltitem(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_switchbeltitem(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_usebeltitem(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_insertsocketitem(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_scrolltotome(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_itemtocube(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_unselectobj(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_npcinit(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_npccancel(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_npcbuy(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_npcsell(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_npctrade(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_characterphrase(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_waypoint(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_trade(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_dropgold(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_party(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_potiontomercenary(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_gamelogon(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_entergameenvironment(struct connection *conn, struct packet *request);
void packet_d2gs_d2gs_ping(struct connection *conn, struct packet *request);

#endif
