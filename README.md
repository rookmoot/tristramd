# Tristramd
A Diablo II (battle.net classic, bnetd and d2gs) standalone private server. This projet aim to create a full featured Diablo II serveur running without need of externals dependances as (.dll and .mpq) files from Diablo II original game.

# Build
To build tristramd, you previously need to install those system dependances :

 - eina (EFL)
 - libconfig
 - mysql-dev

Then, to build the program from sources :
```
$ make
```

# Current state
Currently, this projects manages all base features to let client (Diablo II 1.13d) connect to bnetd server and to realm server and to connect to a start of a d2gs server (while creating / joining game).

Actually, i can't figure out, how to parse and analyse datas sent from official d2gs games server and reverse them to get initial data sent back to client.

# Features
Those listed features are currently fonctionnal but some may need some more work. Fill free to report them.

## BNETD
 * Connecting to server
  * Creating account
  * Connecting with account
  * Changing password, email, etc.
  * Listing realm and connect to them
 * Chat
  * Listing channels
  * Joining channel
  * Multi accounts supported
 * Disconnecting from server

## Realm
 * Connecting to realm
 * Listing all characters
 * Creating, deleting characters
 * Listing ladders characters

## D2GS
 * Connecting to d2gs server
 * Requesting game created / joined

For now, you can go as to create or join a created game. Btw, this is doing actually nothing as the client get stunk in the loading game screen.

_Any help would be appreciated_.

