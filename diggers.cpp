/* ================================================================================= */
/* ooooooooo.--ooooooo--.oooooo.-----.oooooo.--oooooooooooo-ooooooooo.----.oooooo..o */
/* 888'---`Y8b--`888'--d8P'--`Y8b---d8P'  `Y8b-`888'-----`8-`888---`Y88.-d8P'----`Y8 */
/* 888------888--888--888----------888----------888----------888---.d88'-Y88bo.----- */
/* 888------888--888--888----------888----------888oooo8-----888ooo88P'---`"Y8888o.- */
/* 888------888--888--888-----oOOo-888-----oOOo-888----"-----888`88b.---------`"Y88b */
/* 888-----d88'--888--`88.----.88'-`88.----.88'-888-------o--888--`88b.--oo-----.d8P */
/* 888bood8P'--oo888oo-`Y8bood8P'---`Y8bood8P'-o888ooooood8-o888o--o888o-8""88888P'- */
/* ================================================================================= */
/* Copyright (c) MS-Design, 2011      Copyright (c) Millenium Interactive Ltd., 1994 */
/* ================================================================================= */
/* COMPILER DIRECTIVES                                                               */
/* --------------------------------------------------------------------------------- */
#if _MSC_VER < 1500                // Make sure we are using correct compiler
# error Please use Microsoft compiler 15.00.30729.01 for 80x86
#elif _WIN64                       // Make sure we are not using 64-bit compiler
# error X86-64 build not supported yet as there is no 64-bit uFMOD library
#elif !__cplusplus                 // Make sure we are compiling in c++ mode and not c
# error Must use C++ mode to compile this application
#elif UNICODE                      // Make sure we are not using UNICODE mode
# error Must use ANSI mode to compile this application
#endif                             // Checks completed
/* --------------------------------------------------------------------------------- */
/* COMPILER WARNINGS                                                                 */
/* --------------------------------------------------------------------------------- */
#pragma warning(disable:4201)      // Nonstandard extension used (from mmsystem.h)
#pragma warning(disable:4995)      // Deprecated functions (from dshow.h)
/* --------------------------------------------------------------------------------- */
/* SYSTEM DEFINES                                                                    */
/* --------------------------------------------------------------------------------- */
#define WIN32_LEAN_AND_MEAN        // Compile speed optimisation
#define WINVER              0x0400 // I want this program to work on Windows 95
/* --------------------------------------------------------------------------------- */
#define DIRECTINPUT_VERSION 0x0300 // Required direct input version (rec:0x300)
#define DIRECTDRAW_VERSION  0x0300 // Required direct draw version (rec:0x300)
#define DIRECTSOUND_VERSION 0x0300 // Required direct sound version (rec:0x300)
/* --------------------------------------------------------------------------------- */
/* SYSTEM INCLUDES                                                                   */
/* --------------------------------------------------------------------------------- */
#include <windows.h>               // Windows SDK for system
#include <mmsystem.h>              // Windows Multimedia for timings
#include <ddraw.h>                 // Direct Draw for graphics
#include <dinput.h>                // Direct Input for control
#include <dsound.h>                // Direct Sound for sound effects
#include <dshow.h>                 // Direct Show for streaming audio
#include <winsock.h>               // Windows networking SDK
#include <dbghelp.h>               // Debugging SDK
#include <commctrl.H>              // Windows common control headers
/* --------------------------------------------------------------------------------- */
/* GAME INCLUDES                                                                     */
/* --------------------------------------------------------------------------------- */
#include "lib/ufmod.h"             // uFMOD library
#include "lib/scalex.h"            // ScaleX library
/* --------------------------------------------------------------------------------- */
/* SYSTEM LIBRARIES                                                                  */
/* --------------------------------------------------------------------------------- */
#pragma comment(lib, "DIGGERS")    // Want my own libs (CRC32, MiniLzo, UFMod)
#pragma comment(lib, "KERNEL32")   // Want kernel32.dll exports (Windows System)
#pragma comment(lib, "GDI32")      // Want gdi32.dll exports (Windows Graphics)
#pragma comment(lib, "USER32")     // Want user32.dll exports (Windows User Interface)
#pragma comment(lib, "OLE32")      // Want ole32.dll exports (Windows Ole)
#pragma comment(lib, "WSOCK32")    // Want wsock32.dll exports (Windows Networking)
#pragma comment(lib, "COMCTL32")   // Want comctl32.dll exports (Common controls)
#pragma comment(lib, "WINMM")      // Want winmm.dll exports (Windows Multi-media)
#pragma comment(lib, "DDRAW")      // Want ddraw.dll exports (Direct Draw)
#pragma comment(lib, "DSOUND")     // Want dsound.dll exports (Direct Sound)
#pragma comment(lib, "DINPUT8")    // Want dinput.dll exports (Direct Input)
#pragma comment(lib, "DXGUID")     // Want input device guid's (Direct Input)
#pragma comment(lib, "STRMIIDS")   // Want streaming exports (Direct Show)
#pragma comment(lib, "VERSION")    // Want version exports (Windows System)
#pragma comment(lib, "DBGHELP")    // Want debugging exports (Windows System)
/* --------------------------------------------------------------------------------- */
/* LINKER OPTIONS                                                                    */
/* --------------------------------------------------------------------------------- */
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")   // Microsoft Windows application
/* --------------------------------------------------------------------------------- */
/* APP DEFINES                                                                       */
/* --------------------------------------------------------------------------------- */
#define WSOCK_REQ_MAJOR          1 // Major version of winsock required
#define WSOCK_REQ_MINOR          1 // Minor version of winsock required
/* --------------------------------------------------------------------------------- */
#define ASPECT_WIDTH           320 // Aspect screen resolution width (DO NOT CHANGE!)
#define ASPECT_HEIGHT          240 // Aspect screen resolution height (DO NOT CHANGE!)
#define ASPECT_DEPTH             8 // Default screen resolution depth
/* --------------------------------------------------------------------------------- */
#define NUM_DIGGERS              5 // Maximum number of diggers per player
#define NUM_PLAYERS              2 // Maximum number of players in game
#define NUM_RACES                4 // Number of official digger races
/* --------------------------------------------------------------------------------- */
#define TIMER_FRAMELIMIT_FAST    8 // Hundreths of a ms to wait before next game tick
#define TIMER_FRAMELIMIT_SLOW   16 // Slow frame limiter
#define TIMER_ANIMTERRAIN       10 // Game ticks to wait before animating next terrain
#define TIMER_ANIMNORMAL        10 // Delay before anim next sprite for normal objects
#define TIMER_ANIMFAST           5 // Delay before anim next sprite for fast objects
#define TIMER_PICKUPDELAY       10 // Game ticks to wait before checking for pickups
#define TIMER_DEADWAIT         600 // Wait time before removing ACT_DEATH objects
#define TIMER_MUTATEWAIT      3600 // Wait time before full digger mutation occurs
#define TIMER_DANGERTIMEOUT    600 // Timeout before removing the danger flag
#define TIMER_TARGETTIME      1800 // Timeout before selecting another target (AI)
/* --------------------------------------------------------------------------------- */
#define AFL_WINDOW            0x01 // Window mode
#define AFL_NOSOUNDS          0x02 // Sound effects disabled
#define AFL_NOMUSIC           0x04 // Music track disabled
#define AFL_NOTIPS            0x08 // Tips disabled
#define AFL_EDITOR            0x10 // Level editor enabled
#define AFL_NOENMIXSND        0x20 // No enhanced sound mixing
#define AFL_NOSTREAMING       0x40 // No streaming even when MP3 exists
/* --------------------------------------------------------------------------------- */
#define EFL_BROWSER           0x01 // Sprite/Tile browser activated
/* --------------------------------------------------------------------------------- */
#define INITTYP_FONTS         0x01 // Initialise textures needed to print text
#define INITTYP_BASE          0x02 // Initialise textures required to display a level
#define INITTYP_INGAME        0x04 // Initialise textures related to in-game
#define INITTYP_LOADING       0x08 // Initialise loading screen bitmap
/* --------------------------------------------------------------------------------- */
#define LFL_UNPLAYED           0x1 // Level not attempted
#define LFL_UNFINISHED         0x2 // Level not completed
#define LFL_LOCKED             0x4 // Level not selectable
/* --------------------------------------------------------------------------------- */
#define LEVEL_NUM               34 // Number of levels in game
#define LEVEL_WIDTH            128 // Horizontal number of tiles in level
#define LEVEL_HEIGHT           128 // Vertical number of tiles in level
#define LEVEL_PWIDTH    (LEVEL_WIDTH*TILE_SIZE)    // Level width in pixels
#define LEVEL_PHEIGHT   (LEVEL_HEIGHT*TILE_SIZE)   // Level height in pixels
#define LEVEL_SIZE_WORD (LEVEL_WIDTH*LEVEL_HEIGHT) // Size of a level in words
#define LEVEL_SIZE      (LEVEL_SIZE_WORD<<1)       // Size of a level in bytes
/* --------------------------------------------------------------------------------- */
#define TILE_SIZE               16 // Width and height of a single tile
#define SCR_TILES_WIDTH  (ASPECT_WIDTH/TILE_SIZE)  // Total X onscreen tiles
#define SCR_TILES_HEIGHT (ASPECT_HEIGHT/TILE_SIZE) // Total Y onscreen tiles
#define SCR_TILES        (SCR_TILES_WIDTH*SCR_TILES_HEIGHT) // Total tiles onscreen
/* --------------------------------------------------------------------------------- */
#define TILES_NUM              480 // Number of tiles in a single tileset bitmap
/* --------------------------------------------------------------------------------- */
#define GS_INVENTORY           0x1 // (InGameDisplay flags) Show digger inventory
#define GS_LOCATIONS           0x2 // Locations of all diggers
#define GS_GAMESTATUS          0x4 // Game status
#define GS_HELPANDINFO         0x8 // Help and information
/* --------------------------------------------------------------------------------- */
#define JD_WAIT                  0 // Object will not move while jumping
#define JD_MOVE                  1 // Object should move while jumping
#define JD_SIZE                 24 // Size of the jump
/* --------------------------------------------------------------------------------- */
#define DFM_O               0x0001 // Match over tile or dig failed (DigData)
#define DFM_A               0x0002 // Match above tile or dig failed
#define DFM_B               0x0004 // Match below tile or dig failed
#define DFM_C               0x0008 // Match centre of over tile or dig failed
#define DFS_O               0x0010 // Set over tile if successful match
#define DFS_A               0x0020 // Set above tile if successful match
#define DFS_B               0x0040 // Set below tile if successful match
#define DFO_BUSY            0x0080 // Set object to busy if successful dig
#define DFO_NOTBUSY         0x0100 // Remove busy flag if dig successful
#define DFO_TREASURE        0x0200 // Spin the wheel of fortune if dig successful
/* --------------------------------------------------------------------------------- */
#define PFL_AI                 0x1 // Computer controls this player
/* --------------------------------------------------------------------------------- */
#define OFL_REGISTERED  0x0000000000000001 // Object is registered in the global object list
#define OFL_BUSY        0x0000000000000002 // Object is busy and user cannot enter another command
#define OFL_FALL        0x0000000000000004 // Object should fall
#define OFL_JUMPRISE    0x0000000000000008 // Object is rising while jumping
#define OFL_JUMPFALL    0x0000000000000010 // Object is falling while jumping
#define OFL_PICKUP      0x0000000000000020 // Object can be picked up
#define OFL_TREASURE    0x0000000000000040 // Object is treasure
#define OFL_PURCHASABLE 0x0000000000000080 // Object is purchasable from shop
#define OFL_SELLABLE    0x0000000000000100 // Object is sellable to shop
#define OFL_DIGGER      0x0000000000000200 // Object is a digger
#define OFL_HURTDIGGER  0x0000000000000400 // Object hurts diggers
#define OFL_PHASEDIGGER 0x0000000000000800 // Object teleports diggers anywhere
#define OFL_IGNOREFIGHT 0x0000000000001000 // Object should ignore fighting until next action
#define OFL_DONTFLEE    0x0000000000002000 // Object should not run away when fighting
#define OFL_COMPUTER    0x0000000000004000 // Object is computer controlled
#define OFL_NOANIMLOOP  0x0000000000008000 // Object is not allowed to loop its animation
#define OFL_CONSUME     0x0000000000010000 // Object consumes another object
#define OFL_AQUALUNG    0x0000000000200000 // Object can breathe in water
#define OFL_STATIONARY  0x0000000000400000 // Object does not move and is stationary
#define OFL_DANGEROUS   0x0000000000800000 // Object is dangerous and diggers run away
#define OFL_SOUNDLOOP   0x0000000001000000 // Object sound is looped when sprite anim is reset
#define OFL_TRACK       0x0000000002000000 // Object can only move on tracks
#define OFL_FLOAT       0x0000000004000000 // Object floats in water
#define OFL_BLOCK       0x0000000008000000 // Object is a platform for diggers
#define OFL_DEVICE      0x0000000010000000 // Object is a device
#define OFL_PHASETARGET 0x0000000020000000 // Object is a valid random phase target
#define OFL_EXPLODE     0x0000000040000000 // Object explodes on death
#define OFL_CPUSELL     0x0000000080000000 // Object has items to sell (OFL_COMPUTER)
#define OFL_NOEDITVALID 0x0000000100000000 // Object cant be used in the edtior
#define OFL_FLOATING    0x0000000200000000 // Object is floating right now
#define OFL_DELICATE    0x0000000400000000 // Object is delicate (takes more damage)
#define OFL_VOLATILE    0x0000000800000000 // Object is volatile
/* --------------------------------------------------------------------------------- */
enum CFGCMD                       // Config function commands
{
  CFG_SAVE,                       // Save settings
  CFG_LOAD,                       // Load settings
  CFG_DEFAULT                     // Set default settings
};
/* --------------------------------------------------------------------------------- */
enum BMPID                        // All the bitmaps used in the game
{
  BMP_FONTLARGE,                  // Large font
  BMP_FONTSMALL,                  // Small font
  BMP_FONTTINY,                   // Tiny font
  BMP_HPBAR,                      // Health bar bitmap
  BMP_LOADING,                    // Loading screen
  BMP_LOGO,                       // Diggers logo
  BMP_PLXDESERT,                  // Desert parallax background
  BMP_PLXGRASS,                   // Grass parallax background
  BMP_PLXISLANDS,                 // Islands parallax background
  BMP_PLXJUNGLE,                  // Jungle parallax background
  BMP_PLXMOUNTAIN,                // Mountain parallax background
  BMP_PLXROCK,                    // Rock parallax background
  BMP_PLXSNOW,                    // Snow parallax background
  BMP_PTRANS,                     // Transparency bitmap
  BMP_RACES,                      // Race images
  BMP_TSDESERT,                   // Desert tile set
  BMP_TSGRASS,                    // Grass tile set
  BMP_TSISLANDS,                  // Islands tile set
  BMP_TSJUNGLE,                   // Jungle tile set
  BMP_TSMOUNTAIN,                 // Mountain tile set
  BMP_TSROCK,                     // Rock tile set
  BMP_TSSNOW,                     // Snow tile set
  BMP_TSSPRITES,                  // Sprites tile set
  BMP_TSINTERFACE,                // Interface tile set

  BMP_MAX                         // Maximum number if bitmaps to load
};
/* --------------------------------------------------------------------------------- */
enum SFXID                        // All the sound effects available to the game
{
  SFX_NONE = -1,                  // No sound effect

  SFX_CLICK,                      // Mouse click command successful
  SFX_DIEFTAR,                    // F'Targ death sound
  SFX_DIEGRAB,                    // Grablin death sound
  SFX_DIEHABB,                    // Habbish death sound
  SFX_DIEQUAR,                    // Quarrior death sound
  SFX_DIG,                        // Digging sound
  SFX_ERROR,                      // Mouse click command failed
  SFX_EXPLODE,                    // TNT explosion sound
  SFX_FIND,                       // Treasure found
  SFX_GCLOSE,                     // Gate close
  SFX_GOPEN,                      // Gate open
  SFX_JUMP,                       // Jump sound
  SFX_KICK,                       // Kick sound
  SFX_PHASE,                      // Phase teleport sound
  SFX_PUNCH,                      // Punch sound
  SFX_SELECT,                     // Select digger sound
  SFX_TRADE,                      // Trade complete sound

  SFX_MAX                         // Maximum number of sound effects
};
/* --------------------------------------------------------------------------------- */
enum MUSICCMD                     // Music commands for function
{
  MUSICCMD_PLAY,                  // Play specified music
  MUSICCMD_STOP,                  // Stop playing music
  MUSICCMD_PAUSE,                 // Pause playing music
  MUSICCMD_RESUME,                // Resume playing music
  MUSICCMD_VOLUME,                // Set music volume
  MUSICCMD_MUTE                   // Toggle mute music
};
/* --------------------------------------------------------------------------------- */
enum MUSICID                      // All the music modules available to the game
{
  MUSIC_NULL = -1,                // No music

  MUSIC_BANK,                     // Bank music
  MUSIC_INGAME,                   // In-game music
  MUSIC_LOSE,                     // Game over music
  MUSIC_MENU,                     // In-menu music
  MUSIC_SELECT,                   // App.Game.Level loading music
  MUSIC_SHOP,                     // Shop music
  MUSIC_TITLE,                    // Title music
  MUSIC_WIN,                      // Map most mortem music

  MUSIC_MAX                       // Maximum number of modules to load
};
/* --------------------------------------------------------------------------------- */
enum MUSICTYPEID                  // Music type ID
{
  MUSICTYPE_NULL,                 // No music playing
  MUSICTYPE_MP3,                  // Streaming MP3
  MUSICTYPE_XM,                   // Extended module

  MUSICTYPE_MAX                   // Maximum number of supported music types
};
/* --------------------------------------------------------------------------------- */
enum MENUID                       // Object popup menu identifiers
{
  MNU_NONE = -1,                  // No menu selected

  MNU_MAIN,                       // Main digger menu
  MNU_MOVE,                       // Digger movement menu
  MNU_DIG,                        // Digger digging menu
  MNU_DROP,                       // Digger drop item menu
  MNU_TRADE,                      // Digger trade menu
  MNU_BUY,                        // Digger purchase menu
  MNU_SELL,                       // Digger sell menu
  MNU_TUNNEL,                     // Small and large tunneller menu
  MNU_CORK,                       // Corkscrew menu
  MNU_TNT,                        // Explosives menu
  MNU_MAP,                        // Map menu
  MNU_TRAIN,                      // Train for rails menu
  MNU_TRDROP,                     // Train drop menu
  MNU_FLOAT,                      // Floating device movement menu
  MNU_GATE,                       // Flood gate menu
  MNU_DEPLOY,                     // Object deployment menu
  MNU_LIFT,                       // Lift control menu

  MNU_MAX                         // Maximum number of menus
};
/* --------------------------------------------------------------------------------- */
enum MOUSEID                      // Mouse button id's
{
  MID_LMB,                        // Left mouse button
  MID_RMB,                        // Right mouse button
  MID_MMB,                        // Middle mouse button
  MID_BT4,                        // Thumb button one pressed
  MID_BT5,                        // Thumb button two pressed

  MID_MAX                         // Max number of buttons supported
};
/* --------------------------------------------------------------------------------- */
enum CURSORID                     // Cursor types
{
  CID_NONE = -1,                  // No cursor

  CID_ARROW,                      // Arrow cursor
  CID_OK,                         // Cursor with OK text
  CID_EXIT,                       // Cursor with EXIT text
  CID_LEFT,                       // Cursor with LEFT scroll arrow
  CID_TOP,                        // Cursor with TOP scroll arrow
  CID_RIGHT,                      // Cursor with RIGHT scroll arrow
  CID_BOTTOM,                     // Cursor with BOTTOM scroll arrow

  CID_MAX                         // Max number of supported cursors
};
/* --------------------------------------------------------------------------------- */
enum SCENES                       // Types of scenery
{
  SCENE_DESERT,                   // Desert scenery
  SCENE_GRASS,                    // Grass scenery
  SCENE_ISLANDS,                  // Islands scenery
  SCENE_JUNGLE,                   // Jungle scenery
  SCENE_MOUNTAIN,                 // Mountain scenery
  SCENE_ROCK,                     // Rocky scenery
  SCENE_SNOW,                     // Snow scenery

  SCENE_MAX                       // Total number of sceneries
};
/* --------------------------------------------------------------------------------- */
enum OBJTYP      // All the types of objects
{
  TYP_NULL = -1, // Invalid object

  TYP_FTARG,     // [00] F'Targ race digger
  TYP_HABBISH,   // [01] Habbish race digger
  TYP_GRABLIN,   // [02] Grablin race digger
  TYP_QUARRIOR,  // [03] Quarrior race digger

  TYP_JENNITE,   // [04] The most valuable treasure
  TYP_DIAMOND,   // [05] The near-most valuable treasure
  TYP_GOLD,      // [06] The un-common treasure
  TYP_EMERALD,   // [07] The not so un-common treasure
  TYP_RUBY,      // [08] The most common treasure

  TYP_RZRGHOST,  // [09] A fast moving, random directional, spinning monster
  TYP_SLOWSKEL,  // [0A] A slow moving skeleton that homes in on any digger
  TYP_FASTSKEL,  // [0B] A fast moving skeleton that homes in on any digger
  TYP_SLOWGHOST, // [0C] A slow moving ghost that homes in on any digger
  TYP_SLOWPHASE, // [0D] A slow moving worm hole that transports diggers anywhere
  TYP_FASTPHASE, // [0E] A fast moving worm hole that transports diggers anywhere
  TYP_TOADSTOOL, // [0F] A plant that attacks diggers
  TYP_BADROOTS,  // [10] A root plant that attacks diggers
  TYP_ALIEN,     // [11] A mutated alien from a digger
  TYP_ALIENEGG,  // [12] If digger touches this then TYP_ALIEN is born
  TYP_BIRD,      // [13] Critter. Just flies left and right
  TYP_FISH,      // [14] Critter. Just swims left and right
  TYP_DINOFAST,  // [15] A fast moving dinosour
  TYP_DINOSLOW,  // [16] A slow moving dinosour
  TYP_STEGO,     // [17] Main part of a slow moving stegosaurus
  TYP_STEGOB,    // [18] Attachment part of TYP_STEGMAIN
  TYP_TURTLE,    // [19] Turtle. Just swims left and right (Unused in original)
  TYP_BIGFOOT,   // [1A] A fast moving, intelligent monster (Unused in original)

  TYP_STUNNEL,   // [1B] Small tunneler
  TYP_LTUNNEL,   // [1C] Large tunneler
  TYP_LTUNNELB,  // [1D] Attachment for large tunneler
  TYP_CORK,      // [1E] Corkscrew (Vertical tunneler)
  TYP_TELEPOLE,  // [1F] Telepole
  TYP_TNT,       // [20] Explosives
  TYP_FIRSTAID,  // [21] First aid kit
  TYP_MAP,       // [22] Map that shows everything in the level
  TYP_TRACK,     // [23] Track for train cart
  TYP_TRAIN,     // [24] Train for track
  TYP_BRIDGE,    // [25] Bridge piece
  TYP_BOAT,      // [26] Inflatable boat
  TYP_GATE,      // [27] Flood gate
  TYP_GATEB,     // [28] Deployed flood gate
  TYP_LIFT,      // [29] Elevator
  TYP_LIFTB,     // [2A] Deployed elevator
  TYP_LIFTC,     // [2B] Deployed elevator attachment

  TYP_MAX,       // Total number of object types

  TYP_DIGRANDOM  // For LoadLevelData(). Select a random race
};
/* --------------------------------------------------------------------------------- */
enum OBJAI       // All the types of AI that can control an object
{
  AI_NONE,       // No AI

  AI_DIGGER,     // Object is a digger
  AI_ROAMCREEP,  // Object is a roaming humanoid
  AI_ROAMSLOW,   // Object is a roaming humanoid
  AI_ROAMFAST,   // Object is a roaming humanoid
  AI_CRITTER,    // Object does nothing but go left and right
  AI_FINDSLOW,   // Object slowly homes in on a digger
  AI_FINDFAST,   // Object quickly homes in on a digger
  AI_RANDOM,     // Object moves in 4 directions finding a digger
  AI_BIGFOOT,    // Object moves around like a digger and steals items

  AI_MAX         // Maximum number of AI types
};
/* --------------------------------------------------------------------------------- */
enum OBJACT      // All the actions that an object can do
{
  ACT_HIDE,      // Object is invinsible
  ACT_STOP,      // Object is standing still
  ACT_CREEP,     // Object is creeping
  ACT_WALK,      // Object is walking
  ACT_RUN,       // Object is running
  ACT_DIG,       // Object is digging
  ACT_PHASE,     // Object is teleporting
  ACT_DEATH,     // Object is dead
  ACT_FIGHT,     // Object is fighting (only if both objects have OFL_DIGGER)
  ACT_EATEN,     // Object is eaten by an alien egg

  ACT_MAX,       // Total number of actions

  ACT_KEEP       // Preserve the current action (SETACTION() Command)
};
/* --------------------------------------------------------------------------------- */
enum OBJJOB      // All the jobs that an object can do (Sub-action)
{
  JOB_NONE,      // No job
  JOB_BOUNCE,    // The object bounces in the opposite direction when it is blocked
  JOB_DIG,       // The object digs when it is blocked
  JOB_DIGDOWN,   // The object digs down when in centre of tile
  JOB_EXPLODE,   // The object explodes when
  JOB_HOME,      // The object is to move to its starting position (home point)
  JOB_INDANGER,  // The object is in danger
  JOB_PHASE,     // The object is to teleport
  JOB_SEARCH,    // The object walks around and never stops but picks up treasure
  JOB_SPAWN,     // The object is spawning not teleporting (uses ACT_PHASE)

  JOB_MAX,       // Maximum number of jobs

  JOB_KEEP,      // Preserve the current job (SETACTION() Command)
  JOB_JUMP       // Same as JOB_KEEP but the object should jump as well
};
/* --------------------------------------------------------------------------------- */
enum OBJDIR      // All the directions an object can go in
{
  DIR_UL,        // Move left and dig up-left diagonally
  DIR_U,         // Up direction, but not used
  DIR_UR,        // Move right and dig up-right diagonally
  DIR_L,         // Move left and dig left
  DIR_NONE,      // No direction
  DIR_R,         // Move right and dig right
  DIR_DL,        // Move left and dig down-left diagnoally
  DIR_D,         // Dig down
  DIR_DR,        // Move right and dig down-right diagonally

  DIR_MAX,       // Maximum number of directions

  DIR_LR,        // Go left or right
  DIR_OPPOSITE,  // Move in the opposite direction (SETACTION() Command)
  DIR_KEEP,      // Preserve the current direction (SETACTION() Command)
  DIR_HOME,      // Direction to the player's home point (SETACTION() Command)
  DIR_TCTR       // Move into the centre of the tile (SETACTION() Command)
};
/* ================================================================================= */
/* CORE                                                                              */
/* --------------------------------------------------------------------------------- */
extern "C" { INT _fltused; }          // Stupid MS compiler :/
/* --------------------------------------------------------------------------------- */
typedef struct COORDS { INT X, Y; } COORDS, *PCOORDS; // Co-ordinates
/* --------------------------------------------------------------------------------- */
typedef VOID (*GAMEFUNC)(VOID);       // Function pointer
/* ================================================================================= */
struct MODEDATA                       // Mode data type
{
  UCHAR  TexScale;                    // Texture scale
  USHORT Width;                       // Resolution width
  USHORT Height;                      // Resolution height
  UCHAR  Depth;                       // Bit depth
  UCHAR  Available;                   // Full screen mode available?
}
ModeData[]=                           // Modes that are used in game
{
  // 8-bit modes (256 colour palette mode)
  1, ASPECT_WIDTH,     ASPECT_HEIGHT,      8, FALSE, //  320x240x8  (1:1 Aspect)
  2, ASPECT_WIDTH * 2, ASPECT_HEIGHT * 2,  8, FALSE, //  640x480x8  (2:1 Aspect)
  3, ASPECT_WIDTH * 3, ASPECT_HEIGHT * 3,  8, FALSE, //  960x720x8  (3:1 Aspect)
  4, ASPECT_WIDTH * 4, ASPECT_HEIGHT * 4,  8, FALSE, // 1280x960x8  (4:1 Aspect)
  // 16-bit modes (65,536 colour palette mode)
//  1, ASPECT_WIDTH,     ASPECT_HEIGHT,     16, FALSE, //  320x240x16 (1:1 Aspect)
//  2, ASPECT_WIDTH * 2, ASPECT_HEIGHT * 2, 16, FALSE, //  640x480x16 (2:1 Aspect)
//  3, ASPECT_WIDTH * 3, ASPECT_HEIGHT * 3, 16, FALSE, //  960x720x16 (3:1 Aspect)
//  4, ASPECT_WIDTH * 4, ASPECT_HEIGHT * 4, 16, FALSE, // 1280x960x16 (4:1 Aspect)
  // 32-bit modes (16,777,216 colour palette mode)
//  1, ASPECT_WIDTH,     ASPECT_HEIGHT,     32, FALSE, //  320x240x32 (1:1 Aspect)
//  2, ASPECT_WIDTH * 2, ASPECT_HEIGHT * 2, 32, FALSE, //  640x480x32 (2:1 Aspect)
//  3, ASPECT_WIDTH * 3, ASPECT_HEIGHT * 3, 32, FALSE, //  960x720x32 (3:1 Aspect)
//  4, ASPECT_WIDTH * 4, ASPECT_HEIGHT * 4, 32, FALSE, // 1280x960x32 (4:1 Aspect)
  // Last one always NULL
  NULL
};
/* ================================================================================= */
CHAR MUSICNAMES[MUSIC_MAX][9] =        // Music file names (see above for id's)
{
  "MBANK", "MINGAME", "MLOSE", "MMENU", "MSELECT", "MSHOP", "MTITLE", "MWIN"
};
/* ================================================================================= */
struct CURSOR               // Struct definition for mouse cursor type
{
  USHORT StartId;           // Starting tile animation of mouse cursor
  USHORT EndId;             // Ending tile animation of mouse cursor
  CHAR   OffsetX;           // Offset X co-ordinate
  CHAR   OffsetY;           // Offset Y co-ordinate
}
CursorData[CID_MAX]=        // Available cursors
{
  479, 479,   0,   0,       // CID_ARROW
  459, 462,  -8,  -8,       // CID_OK
  455, 458,  -8,  -8,       // CID_EXIT
  451, 451,   0,   0,       // CID_LEFT
  452, 452,   0,   0,       // CID_TOP
  453, 453, -15,   0,       // CID_RIGHT
  454, 454,   0, -15        // CID_BOTTOM
};
/* --------------------------------------------------------------------------------- */
struct MENU         // Control menu layouts structure
{
  MENUID  Id;       // Id of this menu
  UCHAR   Width;    // Width of this menu
  UCHAR   Height;   // Height of this menu
  USHORT  Flags;    // Which buttons are disabled when object is busy

  struct BUTDATA    // Button data struct
  {
    SHORT Id;       // Interface id of button
    CHAR  Tip[49];  // Button tip
  }
  Data[16];         // Id's of each button (-1 = no button)
}
MenuData[MNU_MAX]=
{
  //    ID    W  H  FLAGS  BID  TIP
  MNU_MAIN ,  8, 1, 0x0f0,  15, "SELECT MOVEMENT MENU FOR %s",
                            11, "SELECT DIG MENU FOR %s",
                             5, "PICKUP OBJECT TOUCHED BY %s",
                             6, "SELECT INVENTORY MENU FOR %s",
                            12, "ORDER %s TO ENTER TRADE CENTRE",
                            10, "ORDER %s TO HALT ALL ACTIONS",
                             9, "ORDER %s TO SEARCH FOR GEMS",
                            22, "ORDER %s TO TELEPORT HOME",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_MOVE,   4, 2, 0x70f,   0, "ORDER %s TO WALK LEFT",
                            13, "ORDER %s TO JUMP",
                             1, "ORDER %s TO WALK RIGHT",
                            10, "ORDER %s TO HALT ALL ACTIONS",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                             3, "ORDER %s TO RUN LEFT",
                             2, "ORDER %s TO STOP",
                             4, "ORDER %s TO RUN RIGHT",
                            21, "RETURN TO MAIN CONTROL MENU",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_DIG,    4, 2, 0xb0f,  23, "ORDER %s TO DIG UPPER-LEFT",
                             0, "ORDER %s TO DIG LEFT",
                             1, "ORDER %s TO DIG RIGHT",
                            25, "ORDER %s TO DIG UPPER-RIGHT",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            24, "ORDER %s TO DIG LOWER-LEFT",
                            14, "ORDER %s TO DIG DOWN",
                            21, "RETURN TO MAIN CONTROL MENU",
                            26, "ORDER %s TO DIG DOWN-RIGHT",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_DROP,   3, 2, 0x000,  27, "SELECT NEXT INVENTORY ITEM",
                            29, "DROP CURRENTLY DISPLAYED ITEM",
                            30, "DROP CURRENTLY DISPLAYED ITEM",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            28, "RETURN TO MAIN CONTROL MENU",
                            31, "DROP CURRENTLY DISPLAYED ITEM",
                            32, "DROP CURRENTLY DISPLAYED ITEM",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_TRADE,  5, 1, 0x00f,  34, "PURCHASE ITEMS FROM TRADE CENTRE",
                            35, "SELL INVENTORY TO TRADE CENTRE",
                            10, "ORDER %s TO REST",
                            16, "RECRUIT A NEW %s DIGGER",
                            21, "RETURN TO MAIN CONTROL MENU",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_BUY,    3, 2, 0x000,  27, "SELECT NEXT ITEM ON SALE",
                            29, "PURCHASE CURRENTLY DISPLAYED ITEM",
                            30, "PURCHASE CURRENTLY DISPLAYED ITEM",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            28, "RETURN TO TRADE CENTRE MENU",
                            31, "PURCHASE CURRENTLY DISPLAYED ITEM",
                            32, "PURCHASE CURRENTLY DISPLAYED ITEM",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_SELL,   3, 2, 0x000,  27, "SELECT NEXT INVENTORY ITEM",
                            29, "SELL CURRENTLY DISPLAYED ITEM",
                            30, "SELL CURRENTLY DISPLAYED ITEM",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            28, "RETURN TO TRADE CENTRE MENU",
                            31, "SELL CURRENTLY DISPLAYED ITEM",
                            32, "SELL CURRENTLY DISPLAYED ITEM",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_TUNNEL, 3, 1, 0x007,   0, "ORDER %s TO MOVE AND DIG LEFT",
                             2, "ORDER %s TO HALT ALL ACTIONS",
                             1, "ORDER %s TO MOVE AND DIG RIGHT",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_CORK,   2, 2, 0x303,   0, "ORDER %s TO MOVE LEFT",
                             1, "ORDER %s TO MOVE RIGHT",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            14, "ORDER %s TO DIG DOWN",
                             2, "ORDER %s TO HALT ALL ACTIONS",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_TNT,    1, 1, 0x001,  19, "DETONATE %s",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_MAP,    1, 1, 0x001,  33, "VIEW %s",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_TRAIN,  3, 2, 0x707,   0, "ORDER %s TO MOVE LEFT",
                             2, "ORDER %s TO HALT ALL ACTIONS",
                             1, "ORDER %s TO MOVE RIGHT",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                             5, "GRAB ANY ITEM %s IS TOUCHING",
                             6, "DISPLAY %s IVENTORY",
                             9, "ORDER %s TO SEARCH FOR TREASURE",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_TRDROP, 3, 2, 0x000,  27, "DISPLAY NEXT ITEM IN INVENTORY",
                            29, "DROP CURRENTLY DISPLAYED ITEM",
                            30, "DROP CURRENTLY DISPLAYED ITEM",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            28, "RETURN TO %s CONTROL MENU",
                            31, "DROP CURRENTLY DISPLAYED ITEM",
                            32, "DROP CURRENTLY DISPLAYED ITEM",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_FLOAT,  3, 1, 0x007,   0, "ORDER %s TO MOVE LEFT",
                             2, "ORDER %s TO HALT",
                             1, "ORDER %s TO MOVE RIGHT",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_GATE,   2, 1, 0x003,  17, "RAISE THE %s",
                            18, "DROP THE %s",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_DEPLOY, 1, 1, 0x001,  20, "DEPLOY %s",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
  MNU_LIFT,   3, 1, 0x007,  13, "SEND %s UP",
                             2, "STOP THE %s",
                            14, "SEND %s DOWN",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
                            -1, "UNDEFINED",
  //    ID    W  H  FLAGS  BID  TIP
};
/* --------------------------------------------------------------------------------- */
struct LEVELINFO                    // Level information structure
{
  CHAR    Name[9];                  // Name of level (Max of 8 chars)
  USHORT  Required;                 // Money required to win level
  UCHAR   Flags;                    // Level flags
}
LevelInfo[LEVEL_NUM]=               // Level information data
{
  // NAME   WINREQ  FLAGS
  "AZERG",     600, LFL_UNPLAYED | LFL_UNFINISHED,
  "DHOBBS",    600, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "ELEVATE",   600, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "DEENA",     600, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "JUSTYN",    600, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "FUJALE",   1000, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "HAEWARD",   600, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "SAIRRUHR", 1000, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "TRAAGHE",   600, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "KURVELYN", 1400, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "SQUEEK",   1400, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "MYKEBORL", 1400, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "ZORLYACK", 1400, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "FTARGUS",  1400, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "TRAISA",   1400, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "KLINDYKE", 1400, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "EEANZONE", 1400, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "CHYSSHIR", 1400, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "DJENNEE",  1400, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "DWINDERA", 1400, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "TWANG",    1400, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "HABBARD",  1800, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "BENJAR",   1400, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "SHRUBREE", 1800, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "BAROK",    1800, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "MUHLAHRD", 1800, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "CHONSKEE", 1800, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "PURTH",    1800, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "ANKH",     1800, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "ZELIOS",   2200, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "FRUER",    2200, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "KLARSH",   2200, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "SUHMNER",  2200, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED,
  "SIMTOB",   2200, LFL_UNPLAYED | LFL_UNFINISHED | LFL_LOCKED
  // NAME   WINREQ  FLAGS
};
/* --------------------------------------------------------------------------------- */
CHAR SceneNames[SCENE_MAX][10]= // Names of sceneries
{
  "DESERT",               // SCENE_DESERT
  "GRASS",                // SCENE_GRASS
  "ISLANDS",              // SCENE_ISLANDS
  "JUNGLE",               // SCENE_JUNGLE
  "MOUNTAIN",             // SCENE_MOUNTAIN
  "ROCK",                 // SCENE_ROCK
  "SNOW"                  // SCENE_SNOW
};
/* --------------------------------------------------------------------------------- */
OBJDIR DirTable[3][3]={DIR_UL,DIR_U,DIR_UR,DIR_L,DIR_NONE,DIR_R,DIR_DL,DIR_D,DIR_DR};
/* --------------------------------------------------------------------------------- */
struct DIRDATA              // Direction structure (data for each direction)
{
  USHORT  AnimStart;        // Start of animation
  USHORT  AnimEnd;          // End of animation
};
struct ACTDATA              // Action structure (data for each action)
{
  struct DIRDATA DirData[DIR_MAX]; // Direction data for each action
  CHAR    OffsetX;          // Offset X coordinates
  CHAR    OffsetY;          // Offset Y coordinates
  SFXID   SoundId;          // Sound to play when action is this
  UINT64  Flags;            // Flags for this action (OFL_*)
};
struct OBJDATA              // Object structure (data for each object type)
{
  struct ACTDATA ActData[ACT_MAX]; // Action data for each object type
  CHAR    Name[9];          // Name of object
  SHORT   DigDelay;         // Object digging delay or drop chance if OFL_TREASURE
  SHORT   PhaseDelay;       // Phase delay for object
  SHORT   Strength;         // Strength (Chance to hit) of object
  SHORT   Stamina;          // Stamina (Delay before next hp tick, -1 = no regen)
  USHORT  Value;            // Value of item at shop
  UCHAR   Weight;           // Weight of item (Max carry weight if its a digger)
  UINT64  Flags;            // Flags for this object (OFL_*)
  MENUID  Menu;             // Control menu used when right clicked on object
  OBJACT  InitialAction;    // Initial action when created
  OBJJOB  InitialJob;       // Initial action job created
  OBJDIR  InitialDirection; // Initial direction when created
  OBJAI   AIType;           // AI type
  UCHAR   AnimTimer;        // Delay before incrementing animation frame
  OBJTYP  Attachment;       // A virtual object can be attached to this one to
                            // make a bigger object. Just specify the TYP_ of object
                            // you want to attach.
}
ObjectData[TYP_MAX]=
{
  // F'TARG       DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 138,140, 138,140, 138,140, 138,140, 138,140, 138,140, 138,140, 138,140, 138,140,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_CREEP */  12, 15,  12, 15,   8, 11,  12, 15,  12, 15,   8, 11,  12, 15,  12, 15,   8, 11,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_WALK  */  12, 15,  12, 15,   8, 11,  12, 15,  12, 15,   8, 11,  12, 15,  12, 15,   8, 11,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_RUN   */  20, 23,  20, 23,  16, 19,  20, 23,  20, 23,  16, 19,  20, 23,  20, 23,  16, 19,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_DIG   */  63, 65,  63, 65,  60, 62,  63, 65,  63, 65,  60, 62,  63, 65,  86, 88,  60, 62,   0,  0, SFX_DIG,     OFL_FALL | OFL_SOUNDLOOP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_PHASE,   OFL_BUSY,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_DIEFTAR, 0,
  /* ACT_FIGHT */ 245,249, 240,244, 240,244, 245,249, 240,244, 240,244, 245,249, 240,244, 240,244,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_EATEN */  77, 79,  74, 76,  74, 76,  77, 79,  74, 76,  74, 76,  77, 79,  74, 76,  74, 76,   0,  0, SFX_NONE,    OFL_FALL | OFL_NOANIMLOOP,

  // NAME
  "FTARG",
  // DIGDELAY  TELEDELAY  STRENGTH   STAMINA    VALUE  WEIGHT  FLAGS
  70,          150,       20,        60,        1000,  0,      OFL_DIGGER | OFL_PHASETARGET | OFL_DELICATE,
  // MENU      ACTION     JOB        DIRECTION  AITYPE         ANIMTIMER             ATTACHMENT
  MNU_MAIN,    ACT_PHASE, JOB_NONE,  DIR_NONE,  AI_DIGGER,     TIMER_ANIMNORMAL,     TYP_NULL,

  // HABBISH      DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 135,137, 135,137, 135,137, 135,137, 135,137, 135,137, 135,137, 135,137, 135,137,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_CREEP */ 120,123, 120,123, 116,119, 120,123, 120,123, 116,119, 120,123, 120,123, 116,119,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_WALK  */ 120,123, 120,123, 116,119, 120,123, 120,123, 116,119, 120,123, 120,123, 116,119,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_RUN   */ 128,131, 128,131, 124,127, 128,131, 128,131, 124,127, 128,131, 128,131, 124,127,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_DIG   */ 228,230, 228,230, 225,227, 228,230, 228,230, 225,227, 228,230, 237,239, 225,227,   0,  0, SFX_DIG,     OFL_FALL | OFL_SOUNDLOOP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_PHASE,   OFL_BUSY,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_DIEHABB, 0,
  /* ACT_FIGHT */ 255,259, 250,254, 250,254, 255,259, 250,254, 250,254, 255,259, 250,254, 250,254,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_EATEN */ 151,153, 141,143, 141,143, 151,153, 141,143, 141,143, 151,153, 141,143, 141,143,   0,  0, SFX_NONE,    OFL_FALL | OFL_NOANIMLOOP,

  // NAME
  "HABBISH",
  // DIGDELAY  TELEDELAY  STRENGTH   STAMINA    VALUE  WEIGHT  FLAGS
  80,          60,        30,        120,       1000,  0,      OFL_DIGGER | OFL_PHASETARGET | OFL_DELICATE,
  // MENU      ACTION     JOB        DIRECTION  AITYPE         ANIMTIMER             ATTACHMENT
  MNU_MAIN,    ACT_PHASE, JOB_NONE,  DIR_NONE,  AI_DIGGER,     TIMER_ANIMNORMAL,     TYP_NULL,

  // GRABLIN      DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 222,224, 222,224, 222,224, 222,224, 222,224, 222,224, 222,224, 222,224, 222,224,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_CREEP */ 204,207, 204,207, 200,203, 204,207, 204,207, 200,203, 204,207, 204,207, 200,203,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_WALK  */ 204,207, 204,207, 200,203, 204,207, 204,207, 200,203, 204,207, 204,207, 200,203,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_RUN   */ 212,215, 212,215, 208,211, 212,215, 212,215, 208,211, 212,215, 212,215, 208,211,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_DIG   */  83, 85,  83, 85,  80, 82,  83, 85,  83, 85,  80, 82,  83, 85,  89, 91,  80, 82,   0,  0, SFX_DIG,     OFL_FALL | OFL_SOUNDLOOP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_PHASE,   OFL_BUSY,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_DIEGRAB, 0,
  /* ACT_FIGHT */ 275,279, 270,274, 270,274, 275,279, 270,274, 270,274, 275,279, 270,274, 270,274,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_EATEN */ 219,221, 216,218, 216,218, 219,221, 216,218, 216,218, 219,221, 216,218, 216,218,   0,  0, SFX_NONE,    OFL_FALL | OFL_NOANIMLOOP,

  // NAME
  "GRABLIN",
  // DIGDELAY  TELEDELAY  STRENGTH   STAMINA    VALUE  WEIGHT  FLAGS
  60,          120,       25,        120,       1000,  0,      OFL_DIGGER | OFL_PHASETARGET | OFL_DELICATE,
  // MENU      ACTION     JOB        DIRECTION  AITYPE         ANIMTIMER             ATTACHMENT
  MNU_MAIN,    ACT_PHASE, JOB_NONE,  DIR_NONE,  AI_DIGGER,     TIMER_ANIMNORMAL,     TYP_NULL,

  // QUARRIOR     DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 178,180, 178,180, 178,180, 178,180, 178,180, 178,180, 178,180, 178,180, 178,180,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_CREEP */ 160,163, 160,163, 156,159, 160,163, 160,163, 156,159, 160,163, 160,163, 156,159,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_WALK  */ 160,163, 160,163, 156,159, 160,163, 160,163, 156,159, 160,163, 160,163, 156,159,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_RUN   */ 168,171, 168,171, 164,167, 168,171, 168,171, 164,167, 168,171, 168,171, 164,167,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_DIG   */ 234,236, 234,236, 231,233, 234,236, 234,236, 231,233, 234,236,  92, 94, 231,233,   0,  0, SFX_DIG,     OFL_FALL | OFL_SOUNDLOOP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_PHASE,   OFL_BUSY,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_DIEQUAR, 0,
  /* ACT_FIGHT */ 265,269, 260,264, 260,264, 265,269, 260,264, 260,264, 265,269, 260,264, 260,264,   0,  0, SFX_NONE,    OFL_FALL | OFL_DIGGER,
  /* ACT_EATEN */ 175,177, 172,174, 172,174, 175,177, 172,174, 172,174, 175,177, 172,174, 172,174,   0,  0, SFX_NONE,    OFL_FALL | OFL_NOANIMLOOP,

  // NAME
  "QUARRIOR",
  // DIGDELAY  TELEDELAY  STRENGTH   STAMINA    VALUE  WEIGHT  FLAGS
  90,          180,       35,        120,       1000,  0,      OFL_DIGGER | OFL_PHASETARGET,
  // MENU      ACTION     JOB        DIRECTION  AITYPE         ANIMTIMER             ATTACHMENT
  MNU_MAIN,    ACT_PHASE, JOB_NONE,  DIR_NONE,  AI_DIGGER,     TIMER_ANIMNORMAL,     TYP_NULL,

  // JENNITE      DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_CREEP */ 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_WALK  */ 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_RUN   */ 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_DIG   */ 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  7, SFX_FIND,    OFL_FALL | OFL_PICKUP,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_EATEN */ 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318, 315,318,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,

  // NAME
  "JENNITE",
  // DIGDELAY  TELEDELAY  STRENGTH   STAMINA    VALUE  WEIGHT  FLAGS
  0,           200,       0,         -1,        280,   5,      OFL_SELLABLE | OFL_TREASURE | OFL_PHASETARGET | OFL_AQUALUNG,
  // MENU      ACTION     JOB        DIRECTION  AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_PHASE, JOB_SPAWN, DIR_NONE,  AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // DIAMOND      DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_CREEP */ 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_WALK  */ 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_RUN   */ 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_DIG   */ 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  7, SFX_FIND,    OFL_FALL | OFL_PICKUP,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_EATEN */ 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431, 428,431,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,

  // NAME
  "DIAMOND",
  // DIGDELAY  TELEDELAY  STRENGTH   STAMINA    VALUE  WEIGHT  FLAGS
  1,           200,       0,         -1,        110,   5,      OFL_SELLABLE | OFL_TREASURE | OFL_PHASETARGET | OFL_AQUALUNG,
  // MENU      ACTION     JOB        DIRECTION  AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_PHASE, JOB_SPAWN, DIR_NONE,  AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // GOLD         DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_CREEP */  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_WALK  */  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_RUN   */  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_DIG   */  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  7, SFX_FIND,    OFL_FALL | OFL_PICKUP,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_EATEN */  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,  96, 99,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,

  // NAME
  "GOLD",
  // DIGDELAY  TELEDELAY  STRENGTH   STAMINA    VALUE  WEIGHT  FLAGS
  2,           200,       0,         -1,        90,    5,      OFL_SELLABLE | OFL_TREASURE | OFL_PHASETARGET | OFL_AQUALUNG,
  // MENU      ACTION     JOB        DIRECTION  AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_PHASE, JOB_SPAWN, DIR_NONE,  AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // EMERALD      DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_CREEP */ 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_WALK  */ 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_RUN   */ 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_DIG   */ 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  7, SFX_FIND,    OFL_FALL | OFL_PICKUP,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_EATEN */ 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435, 432,435,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,

  // NAME
  "EMERALD",
  // DIGDELAY  TELEDELAY  STRENGTH   STAMINA    VALUE  WEIGHT  FLAGS
  3,           200,       0,         -1,        70,    5,      OFL_SELLABLE | OFL_TREASURE | OFL_PHASETARGET | OFL_AQUALUNG,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_PHASE, JOB_SPAWN, DIR_NONE,  AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // RUBY         DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_CREEP */ 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_WALK  */ 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_RUN   */ 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_DIG   */ 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  7, SFX_FIND,    OFL_FALL | OFL_PICKUP,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_EATEN */ 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439, 436,439,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,

  // NAME
  "RUBY",
  // DIGDELAY  TELEDELAY  STRENGTH   STAMINA    VALUE  WEIGHT  FLAGS
  4,           200,       0,         -1,        30,    5,      OFL_SELLABLE | OFL_TREASURE | OFL_PHASETARGET | OFL_AQUALUNG,
  // MENU      ACTION     JOB        DIRECTION  AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_PHASE, JOB_SPAWN, DIR_NONE,  AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // RAZORGHOST   DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 442,445, 446,449, 446,449, 442,445, 446,449, 446,449, 442,445, 446,449, 446,449,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_CREEP */ 442,445, 446,449, 446,449, 442,445, 446,449, 446,449, 442,445, 446,449, 446,449,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_WALK  */ 442,445, 446,449, 446,449, 442,445, 446,449, 446,449, 442,445, 446,449, 446,449,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_RUN   */ 442,445, 446,449, 446,449, 442,445, 446,449, 446,449, 442,445, 446,449, 446,449,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_DIG   */ 442,445, 446,449, 446,449, 442,445, 446,449, 446,449, 442,445, 446,449, 446,449,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 442,445, 446,449, 446,449, 442,445, 446,449, 446,449, 442,445, 446,449, 446,449,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_EATEN */ 442,445, 446,449, 446,449, 442,445, 446,449, 446,449, 442,445, 446,449, 446,449,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,

  // NAME
  "PHANTOM",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      OFL_AQUALUNG,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_RANDOM,     TIMER_ANIMFAST,       TYP_NULL,

  // SLOWSKELE    DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 409,412, 413,416, 413,416, 409,412, 413,416, 413,416, 409,412, 413,416, 413,416,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_CREEP */ 409,412, 413,416, 413,416, 409,412, 413,416, 413,416, 409,412, 413,416, 413,416,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_WALK  */ 409,412, 413,416, 413,416, 409,412, 413,416, 413,416, 409,412, 413,416, 413,416,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_RUN   */ 409,412, 413,416, 413,416, 409,412, 413,416, 413,416, 409,412, 413,416, 413,416,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_DIG   */ 409,412, 413,416, 413,416, 409,412, 413,416, 413,416, 409,412, 413,416, 413,416,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 409,412, 413,416, 413,416, 409,412, 413,416, 413,416, 409,412, 413,416, 413,416,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_EATEN */ 409,412, 413,416, 413,416, 409,412, 413,416, 413,416, 409,412, 413,416, 413,416,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,

  // NAME
  "SKELETON",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      0,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_FINDSLOW,   TIMER_ANIMNORMAL,     TYP_NULL,

  // FASTSKELE    DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 147,148, 149,150, 149,150, 147,148, 149,150, 149,150, 147,148, 149,150, 149,150,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_CREEP */ 147,148, 149,150, 149,150, 147,148, 149,150, 149,150, 147,148, 149,150, 149,150,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_WALK  */ 147,148, 149,150, 149,150, 147,148, 149,150, 149,150, 147,148, 149,150, 149,150,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_RUN   */ 147,148, 149,150, 149,150, 147,148, 149,150, 149,150, 147,148, 149,150, 149,150,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_DIG   */ 147,148, 149,150, 149,150, 147,148, 149,150, 149,150, 147,148, 149,150, 149,150,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 147,148, 149,150, 149,150, 147,148, 149,150, 149,150, 147,148, 149,150, 149,150,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_EATEN */ 147,148, 149,150, 149,150, 147,148, 149,150, 149,150, 147,148, 149,150, 149,150,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,

  // NAME
  "ZOMBIE",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      0,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_FINDFAST,   TIMER_ANIMNORMAL,     TYP_NULL,

  // SLOWGHOST    DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 360,360, 361,361, 361,361, 360,360, 361,361, 361,361, 360,360, 361,361, 361,361,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_CREEP */ 360,360, 361,361, 361,361, 360,360, 361,361, 361,361, 360,360, 361,361, 361,361,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_WALK  */ 360,360, 361,361, 361,361, 360,360, 361,361, 361,361, 360,360, 361,361, 361,361,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_RUN   */ 360,360, 361,361, 361,361, 360,360, 361,361, 361,361, 360,360, 361,361, 361,361,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_DIG   */ 360,360, 361,361, 361,361, 360,360, 361,361, 361,361, 360,360, 361,361, 361,361,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 360,360, 361,361, 361,361, 360,360, 361,361, 361,361, 360,360, 361,361, 361,361,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_EATEN */ 360,360, 361,361, 361,361, 360,360, 361,361, 361,361, 360,360, 361,361, 361,361,   0,  0, SFX_NONE,    OFL_HURTDIGGER | OFL_DANGEROUS,

  // NAME
  "GHOST",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      0,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_FINDSLOW,   TIMER_ANIMNORMAL,     TYP_NULL,

  // SLOWPHASE    DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374,   0,  0, SFX_NONE,    OFL_PHASEDIGGER,
  /* ACT_CREEP */ 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374,   0,  0, SFX_NONE,    OFL_PHASEDIGGER,
  /* ACT_WALK  */ 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374,   0,  0, SFX_NONE,    OFL_PHASEDIGGER,
  /* ACT_RUN   */ 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374,   0,  0, SFX_NONE,    OFL_PHASEDIGGER,
  /* ACT_DIG   */ 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374,   0,  0, SFX_NONE,    OFL_PHASEDIGGER,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374,   0,  0, SFX_NONE,    OFL_PHASEDIGGER,
  /* ACT_EATEN */ 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374, 371,374,   0,  0, SFX_NONE,    OFL_PHASEDIGGER,

  // NAME
  "ZIPPER",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      0,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_FINDSLOW,   TIMER_ANIMNORMAL,     TYP_NULL,

  // FASTPHASE    DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427,   0,  0, SFX_NONE,    OFL_PHASEDIGGER,
  /* ACT_CREEP */ 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427,   0,  0, SFX_NONE,    OFL_PHASEDIGGER,
  /* ACT_WALK  */ 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427,   0,  0, SFX_NONE,    OFL_PHASEDIGGER,
  /* ACT_RUN   */ 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427,   0,  0, SFX_NONE,    OFL_PHASEDIGGER,
  /* ACT_DIG   */ 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427,   0,  0, SFX_NONE,    OFL_PHASEDIGGER,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427,   0,  0, SFX_NONE,    OFL_PHASEDIGGER,
  /* ACT_EATEN */ 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427, 417,427,   0,  0, SFX_NONE,    OFL_PHASEDIGGER,

  // NAME
  "SWRLYPRT",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      0,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_FINDFAST,   TIMER_ANIMFAST,       TYP_NULL,

  // TOAD STOOL   DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 388,388, 393,393, 393,393, 388,388, 393,393, 393,393, 388,388, 393,393, 393,393,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_CREEP */ 388,388, 393,393, 393,393, 388,388, 393,393, 393,393, 388,388, 393,393, 393,393,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_WALK  */ 388,388, 393,393, 393,393, 388,388, 393,393, 393,393, 388,388, 393,393, 393,393,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_RUN   */ 388,388, 393,393, 393,393, 388,388, 393,393, 393,393, 388,388, 393,393, 393,393,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_DIG   */ 388,388, 393,393, 393,393, 388,388, 393,393, 393,393, 388,388, 393,393, 393,393,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 389,392, 394,397, 394,397, 389,392, 394,397, 394,397, 389,392, 394,397, 394,397,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_EATEN */ 388,388, 393,393, 393,393, 388,388, 393,393, 393,393, 388,388, 393,393, 393,393,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,

  // NAME
  "TOADSTOL",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      OFL_STATIONARY | OFL_PHASETARGET,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // BADROOTS     DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_CREEP */ 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_WALK  */ 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_RUN   */ 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_DIG   */ 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 402,408, 402,408, 402,408, 402,408, 402,408, 402,408, 402,408, 402,408, 402,408,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_EATEN */ 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401, 398,401,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,

  // NAME
  "FUNGUS",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      OFL_STATIONARY | OFL_PHASETARGET,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // ALIEN        DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 102,105, 112,115, 112,115, 102,105, 112,115, 112,105, 102,105, 112,115, 112,115,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_CREEP */ 102,105, 112,115, 112,115, 102,105, 112,115, 112,115, 102,105, 112,115, 112,115,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_WALK  */ 102,105, 112,115, 112,115, 102,105, 112,115, 112,115, 102,105, 112,115, 112,115,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_RUN   */ 102,105, 112,115, 112,115, 102,105, 112,115, 112,115, 102,105, 112,115, 112,115,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_DIG   */ 102,105, 112,115, 112,115, 102,105, 112,115, 112,115, 102,105, 112,115, 112,115,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_PHASE */ 102,105, 112,115, 112,115, 102,105, 112,115, 112,115, 102,105, 112,115, 112,115,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 102,105, 112,115, 112,115, 102,105, 112,115, 112,115, 102,105, 112,115, 112,115,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_EATEN */ 102,105, 112,115, 112,115, 102,105, 112,115, 112,115, 102,105, 112,115, 112,115,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,

  // NAME
  "ALIEN",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           20,        0,        -1,         0,     0,      OFL_PHASETARGET,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_RUN,   JOB_BOUNCE, DIR_LR,   AI_ROAMFAST,   TIMER_ANIMNORMAL,     TYP_NULL,

  // ALIENEGG     DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,   0,  0, SFX_NONE,    OFL_FALL | OFL_CONSUME,
  /* ACT_CREEP */  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,   0,  0, SFX_NONE,    OFL_FALL | OFL_CONSUME,
  /* ACT_WALK  */  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,   0,  0, SFX_NONE,    OFL_FALL | OFL_CONSUME,
  /* ACT_RUN   */  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,   0,  0, SFX_NONE,    OFL_FALL | OFL_CONSUME,
  /* ACT_DIG   */  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,   0,  0, SFX_NONE,    OFL_FALL | OFL_CONSUME,
  /* ACT_PHASE */  68, 68,  68, 68,  68, 68,  68, 68,  68, 68,  68, 68,  68, 68,  68, 68,  68, 68,   0,  0, SFX_NONE,    OFL_FALL,
  /* ACT_DEATH */ 375,378, 375,378, 375,378, 375,378, 375,378, 375,378, 375,378, 375,378, 375,378,   0,  0, SFX_NONE,    OFL_FALL | OFL_NOANIMLOOP,
  /* ACT_FIGHT */  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,   0,  0, SFX_NONE,    OFL_FALL,
  /* ACT_EATEN */  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,  71, 71,   0,  0, SFX_NONE,    OFL_FALL | OFL_CONSUME,

  // NAME
  "EGG",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      OFL_PHASETARGET,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_PHASE, JOB_SPAWN,DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // BIRD         DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 297,301, 302,306, 302,306, 297,301, 302,306, 302,306, 297,301, 302,306, 302,306,   0,  0, SFX_NONE,    0,
  /* ACT_CREEP */ 297,301, 302,306, 302,306, 297,301, 302,306, 302,306, 297,301, 302,306, 302,306,   0,  0, SFX_NONE,    0,
  /* ACT_WALK  */ 297,301, 302,306, 302,306, 297,301, 302,306, 302,306, 297,301, 302,306, 302,306,   0,  0, SFX_NONE,    0,
  /* ACT_RUN   */ 297,301, 302,306, 302,306, 297,301, 302,306, 302,306, 297,301, 302,306, 302,306,   0,  0, SFX_NONE,    0,
  /* ACT_DIG   */ 297,301, 302,306, 302,306, 297,301, 302,306, 302,306, 297,301, 302,306, 302,306,   0,  0, SFX_NONE,    0,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 297,301, 302,306, 302,306, 297,301, 302,306, 302,306, 297,301, 302,306, 302,306,   0,  0, SFX_NONE,    0,
  /* ACT_EATEN */ 297,301, 302,306, 302,306, 297,301, 302,306, 302,306, 297,301, 302,306, 302,306,   0,  0, SFX_NONE,    0,

  // NAME
  "BIRD",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      0,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_BOUNCE, DIR_LR,   AI_CRITTER,    TIMER_ANIMNORMAL,     TYP_NULL,

  // FISH         DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */  58, 59,  56, 57,  56, 57,  58, 59,  56, 57,  56, 57,  58, 59,  56, 57,  56, 57,   0,  0, SFX_NONE,    0,
  /* ACT_CREEP */  58, 59,  56, 57,  56, 57,  58, 59,  56, 57,  56, 57,  58, 59,  56, 57,  56, 57,   0,  0, SFX_NONE,    0,
  /* ACT_WALK  */  58, 59,  56, 57,  56, 57,  58, 59,  56, 57,  56, 57,  58, 59,  56, 57,  56, 57,   0,  0, SFX_NONE,    0,
  /* ACT_RUN   */  58, 59,  56, 57,  56, 57,  58, 59,  56, 57,  56, 57,  58, 59,  56, 57,  56, 57,   0,  0, SFX_NONE,    0,
  /* ACT_DIG   */  58, 59,  56, 57,  56, 57,  58, 59,  56, 57,  56, 57,  58, 59,  56, 57,  56, 57,   0,  0, SFX_NONE,    0,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */  58, 59,  56, 57,  56, 57,  58, 59,  56, 57,  56, 57,  58, 59,  56, 57,  56, 57,   0,  0, SFX_NONE,    0,
  /* ACT_EATEN */  58, 59,  56, 57,  56, 57,  58, 59,  56, 57,  56, 57,  58, 59,  56, 57,  56, 57,   0,  0, SFX_NONE,    0,

  // NAME
  "FISH",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      OFL_AQUALUNG,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_BOUNCE, DIR_LR,   AI_CRITTER,    TIMER_ANIMNORMAL,     TYP_NULL,

  // DINOFAST     DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 362,365, 366,369, 366,369, 362,365, 366,369, 366,369, 362,365, 366,369, 366,369,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_CREEP */ 362,365, 366,369, 366,369, 362,365, 366,369, 366,369, 362,365, 366,369, 366,369,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_WALK  */ 362,365, 366,369, 366,369, 362,365, 366,369, 366,369, 362,365, 366,369, 366,369,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_RUN   */ 362,365, 366,369, 366,369, 362,365, 366,369, 366,369, 362,365, 366,369, 366,369,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_DIG   */ 362,365, 366,369, 366,369, 362,365, 366,369, 366,369, 362,365, 366,369, 366,369,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 362,365, 366,369, 366,369, 362,365, 366,369, 366,369, 362,365, 366,369, 366,369,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_EATEN */ 362,365, 366,369, 366,369, 362,365, 366,369, 366,369, 362,365, 366,369, 366,369,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,

  // NAME
  "VRAPTOR",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      OFL_PHASETARGET,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_RUN,   JOB_BOUNCE, DIR_LR,   AI_ROAMFAST,   TIMER_ANIMNORMAL,     TYP_NULL,

  // DINOSLOW     DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 380,383, 384,387, 384,387, 380,383, 384,387, 384,387, 380,383, 384,384, 384,384,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_CREEP */ 380,383, 384,387, 384,387, 380,383, 384,387, 384,387, 380,383, 384,384, 384,384,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_WALK  */ 380,383, 384,387, 384,387, 380,383, 384,387, 384,387, 380,383, 384,384, 384,384,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_RUN   */ 380,383, 384,387, 384,387, 380,383, 384,387, 384,387, 380,383, 384,384, 384,384,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_DIG   */ 380,383, 384,387, 384,387, 380,383, 384,387, 384,387, 380,383, 384,384, 384,384,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 380,383, 384,387, 384,387, 380,383, 384,387, 384,387, 380,383, 384,384, 384,384,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_EATEN */ 380,383, 384,387, 384,387, 380,383, 384,387, 384,387, 380,383, 384,384, 384,384,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,

  // NAME
  "RTRYSRUS",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      OFL_PHASETARGET,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_WALK,  JOB_BOUNCE, DIR_LR,   AI_ROAMSLOW,   TIMER_ANIMNORMAL,     TYP_NULL,

  // STEGOA       DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */  29, 32,  39, 42,  39, 42,  29, 32,  39, 42,  39, 42,  29, 32,  39, 42,  32, 42,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_CREEP */  29, 32,  39, 42,  39, 42,  29, 32,  39, 42,  39, 42,  29, 32,  39, 42,  32, 42,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_WALK  */  29, 32,  39, 42,  39, 42,  29, 32,  39, 42,  39, 42,  29, 32,  39, 42,  32, 42,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_RUN   */  29, 32,  39, 42,  39, 42,  29, 32,  39, 42,  39, 42,  29, 32,  39, 42,  32, 42,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_DIG   */  29, 32,  39, 42,  39, 42,  29, 32,  39, 42,  39, 42,  29, 32,  39, 42,  32, 42,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */  29, 32,  39, 42,  39, 42,  29, 32,  39, 42,  39, 42,  29, 32,  39, 42,  32, 42,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_EATEN */  29, 32,  39, 42,  39, 42,  29, 32,  39, 42,  39, 42,  29, 32,  39, 42,  32, 42,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,

  // NAME
  "STEGSAUR",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      OFL_PHASETARGET,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_CREEP, JOB_BOUNCE, DIR_LR,   AI_ROAMCREEP,  TIMER_ANIMNORMAL,     TYP_STEGOB,

  // STEGOB       DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */  25, 28,  43, 46,  43, 46,  25, 28,  43, 46,  43, 46,  25, 28,  43, 46,  43, 46,   0,  0, SFX_NONE,    0,
  /* ACT_CREEP */  25, 28,  43, 46,  43, 46,  25, 28,  43, 46,  43, 46,  25, 28,  43, 46,  43, 46,   0,  0, SFX_NONE,    0,
  /* ACT_WALK  */  25, 28,  43, 46,  43, 46,  25, 28,  43, 46,  43, 46,  25, 28,  43, 46,  43, 46,   0,  0, SFX_NONE,    0,
  /* ACT_RUN   */  25, 28,  43, 46,  43, 46,  25, 28,  43, 46,  43, 46,  25, 28,  43, 46,  43, 46,   0,  0, SFX_NONE,    0,
  /* ACT_DIG   */  25, 28,  43, 46,  43, 46,  25, 28,  43, 46,  43, 46,  25, 28,  43, 46,  43, 46,   0,  0, SFX_NONE,    0,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */  25, 28,  43, 46,  43, 46,  25, 28,  43, 46,  43, 46,  25, 28,  43, 46,  43, 46,   0,  0, SFX_NONE,    0,
  /* ACT_EATEN */  25, 28,  43, 46,  43, 46,  25, 28,  43, 46,  43, 46,  25, 28,  43, 46,  43, 46,   0,  0, SFX_NONE,    0,

  // NAME
  "!STEGSAU",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           0,         0,        0,          0,     0,      0,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       0,                    TYP_NULL,

  // TURTLE       DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 307,310, 307,310, 311,314, 307,310, 307,310, 311,314, 307,310, 311,314, 311,314,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_CREEP */ 307,310, 307,310, 311,314, 307,310, 307,310, 311,314, 307,310, 311,314, 311,314,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_WALK  */ 307,310, 307,310, 311,314, 307,310, 307,310, 311,314, 307,310, 311,314, 311,314,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_RUN   */ 307,310, 307,310, 311,314, 307,310, 307,310, 311,314, 307,310, 311,314, 311,314,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_DIG   */ 307,310, 307,310, 311,314, 307,310, 307,310, 311,314, 307,310, 311,314, 311,314,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 307,310, 307,310, 311,314, 307,310, 307,310, 311,314, 307,310, 311,314, 311,314,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,
  /* ACT_EATEN */ 307,310, 307,310, 311,314, 307,310, 307,310, 311,314, 307,310, 311,314, 311,314,   0,  0, SFX_NONE,    OFL_FALL | OFL_HURTDIGGER | OFL_DANGEROUS,

  // NAME
  "TURTLE",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      OFL_AQUALUNG,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_BOUNCE, DIR_LR,   AI_CRITTER,    TIMER_ANIMNORMAL,     TYP_NULL,

  // BIGFOOT      DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 329,331, 329,331, 329,331, 329,331, 329,331, 329,331, 329,331, 329,331, 329,331,   0,  0, SFX_NONE,    OFL_FALL | OFL_DANGEROUS,
  /* ACT_CREEP */ 321,324, 321,324, 325,328, 321,324, 321,324, 325,328, 321,324, 321,324, 325,328,   0,  0, SFX_NONE,    OFL_FALL | OFL_DANGEROUS,
  /* ACT_WALK  */ 321,324, 321,324, 325,328, 321,324, 321,324, 325,328, 321,324, 321,324, 325,328,   0,  0, SFX_NONE,    OFL_FALL | OFL_DANGEROUS,
  /* ACT_RUN   */ 321,324, 321,324, 325,328, 321,324, 321,324, 325,328, 321,324, 321,324, 325,328,   0,  0, SFX_NONE,    OFL_FALL | OFL_DANGEROUS,
  /* ACT_DIG   */ 321,324, 321,324, 325,328, 321,324, 321,324, 325,328, 321,324, 321,324, 325,328,   0,  0, SFX_NONE,    OFL_FALL | OFL_DANGEROUS,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_PHASE,   OFL_BUSY,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 321,324, 321,324, 325,328, 321,324, 321,324, 325,328, 321,324, 321,324, 325,328,   0,  0, SFX_NONE,    OFL_FALL | OFL_DANGEROUS,
  /* ACT_EATEN */ 321,324, 321,324, 325,328, 321,324, 321,324, 325,328, 321,324, 321,324, 325,328,   0,  0, SFX_NONE,    OFL_FALL | OFL_DANGEROUS,

  // NAME
  "BIGFOOT",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           100,       100,      -1,         0,     100,    OFL_PHASETARGET,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_WALK,  JOB_BOUNCE, DIR_LR,   AI_BIGFOOT,    TIMER_ANIMNORMAL,     TYP_NULL,

  // STUNNEL      DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 284,284, 280,280, 280,280, 284,284, 280,280, 280,280, 284,284, 280,280, 280,280,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_CREEP */ 284,287, 280,283, 280,283, 284,287, 280,283, 280,283, 284,287, 280,283, 280,283,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_WALK  */ 284,287, 280,283, 280,283, 284,287, 280,283, 280,283, 284,287, 280,283, 280,283,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_RUN   */ 284,287, 280,283, 280,283, 284,287, 280,283, 280,283, 284,287, 280,283, 280,283,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_DIG   */ 284,287, 280,283, 280,283, 284,287, 280,283, 280,283, 284,287, 280,283, 280,283,   0,  0, SFX_DIG,     OFL_FALL | OFL_PICKUP | OFL_SOUNDLOOP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 284,287, 280,283, 280,283, 284,287, 280,283, 280,283, 284,287, 280,283, 280,283,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_EATEN */ 284,287, 280,283, 280,283, 284,287, 280,283, 280,283, 284,287, 280,283, 280,283,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,

  // NAME
  "SMALLTUN",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  30,          200,       0,        -1,         150,   8,      OFL_SELLABLE | OFL_PURCHASABLE | OFL_DEVICE | OFL_PHASETARGET | OFL_EXPLODE,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_TUNNEL,  ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // LTUNNEL      DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 185,185, 189,189, 189,189, 185,185, 189,189, 189,189, 185,185, 189,189, 189,189,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_CREEP */ 185,188, 189,192, 189,192, 185,188, 189,192, 189,192, 185,188, 189,192, 189,192,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_WALK  */ 185,188, 189,192, 189,192, 185,188, 189,192, 189,192, 185,188, 189,192, 189,192,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_RUN   */ 185,188, 189,192, 189,192, 185,188, 189,192, 189,192, 185,188, 189,192, 189,192,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_DIG   */ 185,188, 189,192, 189,192, 185,188, 189,192, 189,192, 185,188, 189,192, 189,192,   0,  0, SFX_DIG,     OFL_FALL | OFL_PICKUP | OFL_SOUNDLOOP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 185,188, 189,192, 189,192, 185,188, 189,192, 189,192, 185,188, 189,192, 189,192,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_EATEN */ 185,188, 189,192, 189,192, 185,188, 189,192, 189,192, 185,188, 189,192, 189,192,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,

  // NAME
  "LARGETUN",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  15,          200,       0,        -1,         230,   12,     OFL_SELLABLE | OFL_PURCHASABLE | OFL_DEVICE | OFL_PHASETARGET | OFL_EXPLODE,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_TUNNEL,  ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_LTUNNELB,

  // LTUNNELB     DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 181,181, 193,193, 193,193, 181,181, 193,193, 193,193, 181,181, 193,193, 193,193,   0,  0, SFX_NONE,    0,
  /* ACT_CREEP */ 181,184, 193,196, 193,196, 181,184, 193,196, 193,196, 181,184, 193,196, 193,196,   0,  0, SFX_NONE,    0,
  /* ACT_WALK  */ 181,184, 193,196, 193,196, 181,184, 193,196, 193,196, 181,184, 193,196, 193,196,   0,  0, SFX_NONE,    0,
  /* ACT_RUN   */ 181,184, 193,196, 193,196, 181,184, 193,196, 193,196, 181,184, 193,196, 193,196,   0,  0, SFX_NONE,    0,
  /* ACT_DIG   */ 181,184, 193,196, 193,196, 181,184, 193,196, 193,196, 181,184, 193,196, 193,196,   0,  0, SFX_NONE,    0,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 181,184, 193,196, 193,196, 181,184, 193,196, 193,196, 181,184, 193,196, 193,196,   0,  0, SFX_NONE,    0,
  /* ACT_EATEN */ 181,184, 193,196, 193,196, 181,184, 193,196, 193,196, 181,184, 193,196, 193,196,   0,  0, SFX_NONE,    0,

  // NAME
  "!LARGETU",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           0,         0,        0,          0,     0,      0,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       0,                    TYP_NULL,

  // CORKSCREW    DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 288,288, 288,288, 288,288, 288,288, 288,288, 288,288, 288,288, 288,288, 288,288,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_CREEP */ 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_WALK  */ 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_RUN   */ 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_DIG   */ 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290,   0,  0, SFX_DIG,     OFL_FALL | OFL_PICKUP | OFL_SOUNDLOOP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_EATEN */ 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290, 288,290,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,

  // NAME
  "CORKSCRW",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  20,          200,       0,        -1,         170,   10,     OFL_SELLABLE | OFL_PURCHASABLE | OFL_DEVICE | OFL_PHASETARGET | OFL_EXPLODE,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_CORK,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // TELEPOLE     DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_CREEP */  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_WALK  */  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_RUN   */  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_DIG   */  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_EATEN */  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,  66, 67,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,

  // NAME
  "TELEPOLE",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         260,   11,     OFL_SELLABLE | OFL_PURCHASABLE | OFL_DEVICE | OFL_EXPLODE,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // TNT          DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_CREEP */  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_WALK  */  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_RUN   */  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_DIG   */  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_PHASE */  53, 55,  53, 55,  53, 55,  53, 55,  53, 55,  53, 55,  53, 55,  53, 55,  53, 55,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP | OFL_VOLATILE | OFL_DANGEROUS,
  /* ACT_DEATH */ 291,296, 291,296, 291,296, 291,296, 291,296, 291,296, 291,296, 291,296, 291,296,   0,  0, SFX_EXPLODE, 0,
  /* ACT_FIGHT */  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_EATEN */  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,  24, 24,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,

  // NAME
  "DYNAMITE",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           600,       0,        -1,         20,    4,      OFL_SELLABLE | OFL_PURCHASABLE | OFL_DEVICE | OFL_AQUALUNG | OFL_PHASETARGET | OFL_EXPLODE,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_TNT,     ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // FIRSTAID     DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_CREEP */ 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_WALK  */ 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_RUN   */ 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_DIG   */ 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_EATEN */ 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450, 450,450,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,

  // NAME
  "FIRSTAID",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         60,    5,      OFL_SELLABLE | OFL_PURCHASABLE | OFL_DEVICE | OFL_PHASETARGET,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // MAP          DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_CREEP */ 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_WALK  */ 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_RUN   */ 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_DIG   */ 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_EATEN */ 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370, 370,370,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,

  // NAME
  "MAP",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         300,   3,      OFL_SELLABLE | OFL_PURCHASABLE | OFL_DEVICE | OFL_PHASETARGET,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_MAP,     ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // TRACK        DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_CREEP */ 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_WALK  */ 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_RUN   */ 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_DIG   */ 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,
  /* ACT_EATEN */ 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441, 441,441,   0,  0, SFX_NONE,    OFL_FALL | OFL_PICKUP,

  // NAME
  "TRACK",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         10,    3,      OFL_SELLABLE | OFL_PURCHASABLE | OFL_DEVICE | OFL_PHASETARGET,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_DEPLOY,  ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // TRAIN        DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */   4,  4,   4,  4,   4,  4,   4,  4,   4,  4,   4,  4,   4,  4,   4,  4,   4,  4,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_CREEP */   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_WALK  */   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_RUN   */   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_DIG   */   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_EATEN */   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   4,  7,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,

  // NAME
  "TRAIN",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       255,      -1,         100,   9,      OFL_SELLABLE | OFL_PURCHASABLE | OFL_TRACK | OFL_DEVICE | OFL_PHASETARGET | OFL_EXPLODE,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_TRAIN,   ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // BRIDGE       DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL | OFL_FLOAT | OFL_BLOCK,
  /* ACT_CREEP */ 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL | OFL_FLOAT | OFL_BLOCK,
  /* ACT_WALK  */ 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL | OFL_FLOAT | OFL_BLOCK,
  /* ACT_RUN   */ 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL | OFL_FLOAT | OFL_BLOCK,
  /* ACT_DIG   */ 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL | OFL_FLOAT | OFL_BLOCK,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL | OFL_FLOAT | OFL_BLOCK,
  /* ACT_EATEN */ 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146, 146,146,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL | OFL_FLOAT | OFL_BLOCK,

  // NAME
  "BRIDGE",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         25,    3,      OFL_SELLABLE | OFL_PURCHASABLE | OFL_DEVICE | OFL_PHASETARGET | OFL_AQUALUNG,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_NONE,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // BOAT         DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL | OFL_FLOAT | OFL_BLOCK,
  /* ACT_CREEP */ 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL | OFL_FLOAT | OFL_BLOCK,
  /* ACT_WALK  */ 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL | OFL_FLOAT | OFL_BLOCK,
  /* ACT_RUN   */ 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL | OFL_FLOAT | OFL_BLOCK,
  /* ACT_DIG   */ 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL | OFL_FLOAT | OFL_BLOCK,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL | OFL_FLOAT | OFL_BLOCK,
  /* ACT_EATEN */ 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155, 154,155,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL | OFL_FLOAT | OFL_BLOCK,

  // NAME
  "BOAT",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         60,    5,      OFL_SELLABLE | OFL_PURCHASABLE | OFL_DEVICE | OFL_PHASETARGET | OFL_AQUALUNG,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_FLOAT,   ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // GATE         DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_CREEP */ 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_WALK  */ 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_RUN   */ 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_DIG   */ 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_EATEN */ 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440, 440,440,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,

  // NAME
  "FLOODGAT",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         80,    10,     OFL_SELLABLE | OFL_PURCHASABLE | OFL_DEVICE | OFL_AQUALUNG | OFL_PHASETARGET,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_DEPLOY,  ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // GATEB        DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475,   0,  0, SFX_NONE,    0,
  /* ACT_CREEP */ 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475,   0,  0, SFX_NONE,    0,
  /* ACT_WALK  */ 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475,   0,  0, SFX_NONE,    0,
  /* ACT_RUN   */ 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475,   0,  0, SFX_NONE,    0,
  /* ACT_DIG   */ 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475,   0,  0, SFX_NONE,    0,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475,   0,  0, SFX_NONE,    0,
  /* ACT_EATEN */ 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475, 475,475,   0,  0, SFX_NONE,    0,

  // NAME
  "!FLOODGA",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         0,     0,      OFL_DEVICE | OFL_AQUALUNG,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_GATE,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // LIFT         DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */ 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476, 476,476,   0,  0, SFX_NONE,    OFL_BUSY,
  /* ACT_STOP  */ 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_CREEP */ 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_WALK  */ 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_RUN   */ 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_DIG   */ 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */ 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,
  /* ACT_EATEN */ 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461, 461,461,   0,  0, SFX_NONE,    OFL_PICKUP | OFL_FALL,

  // NAME
  "LIFT",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           200,       0,        -1,         220,   12,     OFL_SELLABLE | OFL_PURCHASABLE | OFL_DEVICE | OFL_PHASETARGET | OFL_EXPLODE,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_DEPLOY,  ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_NULL,

  // LIFT (DEP)   DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0, SFX_NONE,    OFL_BLOCK,
  /* ACT_STOP  */   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0, SFX_NONE,    OFL_BLOCK,
  /* ACT_CREEP */   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0, SFX_NONE,    OFL_BLOCK,
  /* ACT_WALK  */   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0, SFX_NONE,    OFL_BLOCK,
  /* ACT_RUN   */   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0, SFX_NONE,    OFL_BLOCK,
  /* ACT_DIG   */   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0, SFX_NONE,    OFL_BLOCK,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0, SFX_NONE,    OFL_BLOCK,
  /* ACT_EATEN */   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0, SFX_NONE,    OFL_BLOCK,

  // NAME
  "LIFTB",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           0,         0,        -1,         0,     0,      OFL_DEVICE | OFL_PHASETARGET | OFL_EXPLODE,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_LIFT,    ACT_STOP,  JOB_NONE, DIR_D,      AI_NONE,       TIMER_ANIMNORMAL,     TYP_LIFTC,

  // LIFT (DEP/A) DIR_UL   DIR_U    DIR_UR   DIR_L    NONE     DIR_R    DIR_DL   DIR_D    DIR_DR   OFX OFY  SOUND        FLAGS
  /* ACT_HIDE  */   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   0,  0, SFX_NONE,    0,
  /* ACT_STOP  */   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   0,  0, SFX_NONE,    0,
  /* ACT_CREEP */   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   0,  0, SFX_NONE,    0,
  /* ACT_WALK  */   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   0,  0, SFX_NONE,    0,
  /* ACT_RUN   */   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   0,  0, SFX_NONE,    0,
  /* ACT_DIG   */   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   0,  0, SFX_NONE,    0,
  /* ACT_PHASE */ 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109, 106,109,   0,  0, SFX_NONE,    0,
  /* ACT_DEATH */ 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454, 451,454,   0,  0, SFX_NONE,    0,
  /* ACT_FIGHT */   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   0,  0, SFX_NONE,    0,
  /* ACT_EATEN */   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   1,  1,   0,  0, SFX_NONE,    0,

  // NAME
  "LIFTC",
  // DIGDELAY  TELEDELAY  STRENGTH  STAMINA     VALUE  WEIGHT  FLAGS
  0,           0,         0,        -1,         0,     0,      OFL_DEVICE | OFL_PHASETARGET | OFL_EXPLODE,
  // MENU      ACTION     JOB       DIRECTION   AITYPE         ANIMTIMER             ATTACHMENT
  MNU_LIFT,    ACT_STOP,  JOB_NONE, DIR_NONE,   AI_NONE,       TIMER_ANIMNORMAL,     TYP_LIFTC,
};
/* --------------------------------------------------------------------------------- */
USHORT AIData[ACT_MAX][JOB_MAX][DIR_MAX]=
{
  // ACT_HIDE         DIR_UL  DIR_U DIR_UR  DIR_L   NONE  DIR_R DIR_DL  DIR_D DIR_DR
  /* JOB_NONE      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_BOUNCE    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIG       */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIGDOWN   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_EXPLODE   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_HOME      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_INDANGER  */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_PHASE     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SEARCH    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SPAWN     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,

  // ACT_STOP         DIR_UL  DIR_U DIR_UR  DIR_L   NONE  DIR_R DIR_DL  DIR_D DIR_DR
  /* JOB_NONE      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,   500,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_BOUNCE    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIG       */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIGDOWN   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_EXPLODE   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_HOME      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_INDANGER  */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_PHASE     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SEARCH    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SPAWN     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,

  // ACT_CREEP        DIR_UL  DIR_U DIR_UR  DIR_L   NONE  DIR_R DIR_DL  DIR_D DIR_DR
  /* JOB_NONE      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_BOUNCE    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIG       */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIGDOWN   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_EXPLODE   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_HOME      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_INDANGER  */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_PHASE     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SEARCH    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SPAWN     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,

  // ACT_WALK         DIR_UL  DIR_U DIR_UR  DIR_L   NONE  DIR_R DIR_DL  DIR_D DIR_DR
  /* JOB_NONE      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_BOUNCE    */   1000,0xFFFF,  1000,  1000,0xFFFF,  1000,    50,   500,    50,
  /* JOB_DIG       */    500,0xFFFF,   500,  1000,0xFFFF,  1000,    50,   500,    50,
  /* JOB_DIGDOWN   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,     6,0xFFFF,
  /* JOB_EXPLODE   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_HOME      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_INDANGER  */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_PHASE     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SEARCH    */ 0xFFFF,0xFFFF,0xFFFF,   500,0xFFFF,   500,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SPAWN     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,

  // ACT_RUN          DIR_UL  DIR_U DIR_UR  DIR_L   NONE  DIR_R DIR_DL  DIR_D DIR_DR
  /* JOB_NONE      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_BOUNCE    */ 0xFFFF,0xFFFF,0xFFFF,   100,0xFFFF,   100,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIG       */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIGDOWN   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_EXPLODE   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_HOME      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_INDANGER  */ 0xFFFF,0xFFFF,0xFFFF,   500,0xFFFF,   500,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_PHASE     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SEARCH    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SPAWN     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,

  // ACT_DIG          DIR_UL  DIR_U DIR_UR  DIR_L   NONE  DIR_R DIR_DL  DIR_D DIR_DR
  /* JOB_NONE      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_BOUNCE    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIG       */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIGDOWN   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_EXPLODE   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_HOME      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_INDANGER  */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_PHASE     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SEARCH    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SPAWN     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,

  // ACT_PHASE        DIR_UL  DIR_U DIR_UR  DIR_L   NONE  DIR_R DIR_DL  DIR_D DIR_DR
  /* JOB_NONE      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_BOUNCE    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIG       */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIGDOWN   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_EXPLODE   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_HOME      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_INDANGER  */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_PHASE     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SEARCH    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SPAWN     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,

  // ACT_DEATH        DIR_UL  DIR_U DIR_UR  DIR_L   NONE  DIR_R DIR_DL  DIR_D DIR_DR
  /* JOB_NONE      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_BOUNCE    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIG       */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIGDOWN   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_EXPLODE   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_HOME      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_INDANGER  */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_PHASE     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SEARCH    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SPAWN     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,

  // ACT_FIGHT        DIR_UL  DIR_U DIR_UR  DIR_L   NONE  DIR_R DIR_DL  DIR_D DIR_DR
  /* JOB_NONE      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_BOUNCE    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIG       */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIGDOWN   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_EXPLODE   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_HOME      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_INDANGER  */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_PHASE     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SEARCH    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SPAWN     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,

  // ACT_EATEN        DIR_UL  DIR_U DIR_UR  DIR_L   NONE  DIR_R DIR_DL  DIR_D DIR_DR
  /* JOB_NONE      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_BOUNCE    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIG       */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_DIGDOWN   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_EXPLODE   */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_HOME      */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_INDANGER  */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_PHASE     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SEARCH    */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  /* JOB_SPAWN     */ 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
};
/* --------------------------------------------------------------------------------- */
struct DIGDATAITEM   // Dig tile lookup table structure
{
  USHORT FromOver;           // DFM_O. Tile to match from object's over tile
  USHORT FromAbove;          // DFM_A. Tile to match from object's above tile
  USHORT FromBelow;          // DFM_B. Tile to match from object's below tile
  USHORT FromCentre;         // DFM_C. Tile to match from object's centre (over) tile
  USHORT ToOver;             // DFS_O. Set over tile to this tile on successful match
  USHORT ToAbove;            // DFS_A. Set above tile to this tile on successful match
  USHORT ToBelow;            // DFS_B. Set below tile to this tile on successful match
  USHORT Flags;              // Flags see DFM_*, DFS_* and DFO_* #defines
}
DigDataUL[]=                 // Dig tile lookup table for DIR_UL
{
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    3,   3,   0,  12,  208, 207,   0, DFM_O|DFM_A|DFM_C | DFS_O|DFS_A | DFO_BUSY,
    3,   3,   0,   0,   44,   0,   0, DFM_O|DFM_A       | DFS_O       | DFO_BUSY,
    3,   7,   0,  12,  208,   0,   0, DFM_O|DFM_A|DFM_C | DFS_O       | DFO_BUSY,
    3,   7,   0,   0,   44,   0,   0, DFM_O|DFM_A       | DFS_O       | DFO_BUSY,
    3, 150,   0,  12,  208,   0,   0, DFM_O|DFM_A|DFM_C | DFS_O       | DFO_BUSY,
    3, 150,   0,   0,   44,   0,   0, DFM_O|DFM_A       | DFS_O       | DFO_BUSY,
    3, 170,   0,  12,  208,   0,   0, DFM_O|DFM_A|DFM_C | DFS_O       | DFO_BUSY,
    3, 170,   0,   0,   44,   0,   0, DFM_O|DFM_A       | DFS_O       | DFO_BUSY,
    3, 171,   0,  12,  208,   0,   0, DFM_O|DFM_A|DFM_C | DFS_O       | DFO_BUSY,
    3, 171,   0,   0,   44,   0,   0, DFM_O|DFM_A       | DFS_O       | DFO_BUSY,
    3, 172,   0,  12,  208,   0,   0, DFM_O|DFM_A|DFM_C | DFS_O       | DFO_BUSY,
    3, 172,   0,   0,   44,   0,   0, DFM_O|DFM_A       | DFS_O       | DFO_BUSY,
   44,   3,   0,   0,  203,  45,   0, DFM_O|DFM_A       | DFS_O|DFS_A | DFO_BUSY,
   44,   0,   0,   0,  203,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
  203,  45,   0,   0,   12,  10,   0, DFM_O|DFM_A       | DFS_O|DFS_A | DFO_BUSY,
  203,   0,   0,   0,   12,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
  208, 207,   0,   0,   11, 209,   0, DFM_O|DFM_A       | DFS_O|DFS_A | DFO_BUSY,
  208,   0,   0,   0,   11,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   11,   0,   0,  11,    0,   9,   0, DFM_O|DFM_C       | DFS_A       | DFO_NOTBUSY|DFO_TREASURE,
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    0,   0,   0,   0,    0,   0,   0, 0
},
DigDataU[]=                  // Dig tile lookup table for DIR_U
{
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    0,   0,   0,   0,    0,   0,   0, 0
  // FO FA   FB   FC    TO   TA   TB  FLAGS
},
DigDataUR[]=                 // Dig tile lookup table for DIR_UR
{
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    3,   3,   0,   6,   33,  32,   0, DFM_O|DFM_A|DFM_C | DFS_O|DFS_A | DFO_BUSY,
    3,   3,   0,   0,   26,   0,   0, DFM_O|DFM_A       | DFS_O       | DFO_BUSY,
    3,   7,   0,   6,   33,   0,   0, DFM_O|DFM_A|DFM_C | DFS_O       | DFO_BUSY,
    3,   7,   0,   0,   26,   0,   0, DFM_O|DFM_A       | DFS_O       | DFO_BUSY,
    3, 150,   0,   6,   33,   0,   0, DFM_O|DFM_A|DFM_C | DFS_O       | DFO_BUSY,
    3, 150,   0,   0,   26,   0,   0, DFM_O|DFM_A       | DFS_O       | DFO_BUSY,
    3, 170,   0,   6,   33,   0,   0, DFM_O|DFM_A|DFM_C | DFS_O       | DFO_BUSY,
    3, 170,   0,   0,   26,   0,   0, DFM_O|DFM_A       | DFS_O       | DFO_BUSY,
    3, 171,   0,   6,   33,   0,   0, DFM_O|DFM_A|DFM_C | DFS_O       | DFO_BUSY,
    3, 171,   0,   0,   26,   0,   0, DFM_O|DFM_A       | DFS_O       | DFO_BUSY,
    3, 172,   0,   6,   33,   0,   0, DFM_O|DFM_A|DFM_C | DFS_O       | DFO_BUSY,
    3, 172,   0,   0,   26,   0,   0, DFM_O|DFM_A       | DFS_O       | DFO_BUSY,
   26,   3,   0,   0,   28,  27,   0, DFM_O|DFM_A       | DFS_O|DFS_A | DFO_BUSY,
   26,   0,   0,   0,   28,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
   28,  27,   0,   0,    6,   4,   0, DFM_O|DFM_A       | DFS_O|DFS_A | DFO_BUSY,
   28,   0,   0,   0,    6,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
   33,  32,   0,   0,    8,  34,   0, DFM_O|DFM_A       | DFS_O|DFS_A | DFO_BUSY,
   33,   0,   0,   0,    8,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
    8,   0,   0,   8,    0,   5,   0, DFM_O|DFM_C       | DFS_A       | DFO_NOTBUSY|DFO_TREASURE,
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    0,   0,   0,   0,    0,   0,   0, 0
},
DigDataL[]=                  // Dig tile lookup table for DIR_L
{
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    3,   0,   0,   0,  230,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
  230,   0,   0,   0,  231,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
  231,   0,   0,   0,  232,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
  232,   0,   0,   0,0xFFFF,  0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
    8,   0,   0,   0,    6,   0,   0, DFM_O             | DFS_O       | DFO_BUSY|DFO_TREASURE,
   11,   0,   0,   0,   12,   0,   0, DFM_O             | DFS_O       | DFO_BUSY|DFO_TREASURE,
   10,   0,   0,   0,    9,   0,   0, DFM_O             | DFS_O       | DFO_BUSY|DFO_TREASURE,
    9,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   49,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   50,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   52,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   53,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   55,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   56,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   61,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
    6,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
  173,   0,   0,   0,  174,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
  174,   0,   0,   0,  175,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
  175,   0,   0,   0,0xFFFF,  0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    0,   0,   0,   0,    0,   0,   0, 0
},
DigDataNONE[]=               // Dig tile lookup table for DIR_NONE
{
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    0,   0,   0,   0,    0,   0,   0, 0
  // FO FA   FB   FC    TO   TA   TB  FLAGS
},
DigDataR[]=                  // Dig tile lookup table for DIR_R
{
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    3,   0,   0,   0,  227,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
  227,   0,   0,   0,  228,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
  228,   0,   0,   0,  229,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
  229,   0,   0,   0,0xFFFF,  0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
    8,   0,   0,   0,    6,   0,   0, DFM_O             | DFS_O       | DFO_BUSY|DFO_TREASURE,
   11,   0,   0,   0,   12,   0,   0, DFM_O             | DFS_O       | DFO_BUSY|DFO_TREASURE,
    4,   0,   0,   0,    5,   0,   0, DFM_O             | DFS_O       | DFO_BUSY|DFO_TREASURE,
    5,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   49,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   50,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   52,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   53,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   55,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   56,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   58,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   12,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
  173,   0,   0,   0,  174,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
  174,   0,   0,   0,  175,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
  175,   0,   0,   0,0xFFFF,  0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    0,   0,   0,   0,    0,   0,   0, 0
},
DigDataDL[]=                 // Dig tile lookup table for DIR_DL
{
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    3,   0,   3,   5,   23,   0,  24, DFM_O|DFM_B|DFM_C | DFS_O|DFS_B | DFO_BUSY,
    3,   0,   3,   0,   17,   0,   0, DFM_O|DFM_B       | DFS_O       | DFO_BUSY,
    3,   0,   7,   5,   23,   0,   0, DFM_O|DFM_B|DFM_C | DFS_O       | DFO_BUSY,
    3,   0,   7,   0,   17,   0,   0, DFM_O|DFM_B       | DFS_O       | DFO_BUSY,
    3,   0, 150,   5,   23,   0,   0, DFM_O|DFM_B|DFM_C | DFS_O       | DFO_BUSY,
    3,   0, 150,   0,   17,   0,   0, DFM_O|DFM_B       | DFS_O       | DFO_BUSY,
    3,   0, 170,   5,   23,   0,   0, DFM_O|DFM_B|DFM_C | DFS_O       | DFO_BUSY,
    3,   0, 170,   0,   17,   0,   0, DFM_O|DFM_B       | DFS_O       | DFO_BUSY,
    3,   0, 171,   5,   23,   0,   0, DFM_O|DFM_B|DFM_C | DFS_O       | DFO_BUSY,
    3,   0, 171,   0,   17,   0,   0, DFM_O|DFM_B       | DFS_O       | DFO_BUSY,
    3,   0, 172,   5,   23,   0,   0, DFM_O|DFM_B|DFM_C | DFS_O       | DFO_BUSY,
    3,   0, 172,   0,   17,   0,   0, DFM_O|DFM_B       | DFS_O       | DFO_BUSY,
   17,   0,   3,   0,   18,   0,  19, DFM_O|DFM_B       | DFS_O|DFS_B | DFO_BUSY,
   17,   0,   0,   0,   18,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
   18,   0,  19,   0,    5,   0,   8, DFM_O|DFM_B       | DFS_O|DFS_B | DFO_BUSY,
   18,   0,   0,   0,    5,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
   23,   0,  24,   0,    4,   0,  25, DFM_O|DFM_B       | DFS_O|DFS_B | DFO_BUSY,
   23,   0,   0,   0,    4,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
    4,   0,   0,   0,    0,   0,   6, DFM_O             | DFS_B       | DFO_NOTBUSY|DFO_TREASURE,
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    0,   0,   0,   0,    0,   0,   0, 0
},
DigDataD[]=                  // Dig tile lookup table for DIR_D
{
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    3,   0,   0,   0,  173,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
  173,   0,   0,   0,  174,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
  174,   0,   0,   0,  175,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
  175,   0,   0,   0,0xFFFE,  0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
    8,   0,   0,   0,    6,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
   11,   0,   0,   0,   12,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
    4,   0,   0,   0,    5,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
   10,   0,   0,   0,    9,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
    5,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
    9,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   47,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   48,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   52,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   53,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   12,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
    6,   0,   0,   0,    7,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    0,   0,   0,   0,    0,   0,   0, 0
},
DigDataDR[]=                 // Dig tile lookup table for DIR_DR
{
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    3,   0,   3,   9,   41,   0,  42, DFM_O|DFM_B|DFM_C | DFS_O|DFS_B | DFO_BUSY,
    3,   0,   3,   0,   35,   0,   0, DFM_O|DFM_B       | DFS_O       | DFO_BUSY,
    3,   0,   7,   9,   41,   0,   0, DFM_O|DFM_B|DFM_C | DFS_O       | DFO_BUSY,
    3,   0,   7,   0,   35,   0,   0, DFM_O|DFM_B       | DFS_O       | DFO_BUSY,
    3,   0, 150,   9,   41,   0,   0, DFM_O|DFM_B|DFM_C | DFS_O       | DFO_BUSY,
    3,   0, 150,   0,   35,   0,   0, DFM_O|DFM_B       | DFS_O       | DFO_BUSY,
    3,   0, 170,   9,   41,   0,   0, DFM_O|DFM_B|DFM_C | DFS_O       | DFO_BUSY,
    3,   0, 170,   0,   35,   0,   0, DFM_O|DFM_B       | DFS_O       | DFO_BUSY,
    3,   0, 171,   9,   41,   0,   0, DFM_O|DFM_B|DFM_C | DFS_O       | DFO_BUSY,
    3,   0, 171,   0,   35,   0,   0, DFM_O|DFM_B       | DFS_O       | DFO_BUSY,
    3,   0, 172,   9,   41,   0,   0, DFM_O|DFM_B|DFM_C | DFS_O       | DFO_BUSY,
    3,   0, 172,   0,   35,   0,   0, DFM_O|DFM_B       | DFS_O       | DFO_BUSY,
   35,   0,   3,   0,   36,   0,  37, DFM_O|DFM_B       | DFS_O|DFS_B | DFO_BUSY,
   35,   0,   0,   0,   36,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
   36,   0,  37,   0,    9,   0,  11, DFM_O|DFM_B       | DFS_O|DFS_B | DFO_BUSY,
   36,   0,   0,   0,    9,   0,   0, DFM_O             | DFS_O       | DFO_BUSY,
   41,   0,  42,   0,   10,   0,  43, DFM_O|DFM_B       | DFS_O|DFS_B | DFO_BUSY,
   41,   0,   0,   0,   10,   0,   0, DFM_O             | DFS_O       | DFO_NOTBUSY|DFO_TREASURE,
   10,   0,   0,   0,    0,   0,  12, DFM_O             | DFS_B       | DFO_NOTBUSY|DFO_TREASURE,
  // FO FA   FB   FC    TO   TA   TB  FLAGS
    0,   0,   0,   0,    0,   0,   0, 0
};
/* --------------------------------------------------------------------------------- */
struct DIGDATA               // Digging data
{
  struct DIGDATAITEM *Data;  // Pointer to a list of single items
}
DigData[DIR_MAX]=            // DigData direction array contain all the above data
{
  DigDataUL, DigDataU,  DigDataUR, // DIR_UL, DIR_U,  DIR_UR -- Do not change
  DigDataL, DigDataNONE, DigDataR, // DIR_L, DIR_NONE, DIR_R -- Do not change
  DigDataDL, DigDataD,  DigDataDR  // DIR_DL, DIR_D,  DIR_DR -- Do not change
};
/* --------------------------------------------------------------------------------- */
#define TF_NONE        0x0000 // Tile is insignificant
#define TF_F           0x0001 // Tile is firm ground
#define TF_W           0x0002 // Tile is water
#define TF_AB          0x0004 // Increment terrain tile animation?
#define TF_AE          0x0008 // Reset terrain tile animation?
#define TF_D           0x0010 // Tile is destructable
#define TF_AD          0x0020 // Tile has been artificially dug
#define TF_PO          0x0040 // Player one start position?
#define TF_PT          0x0080 // Player two start position?
#define TF_EL          0x0100 // The left edge of the tile is exposed to flood
#define TF_ET          0x0200 // The top edge of the tile is exposed to flood
#define TF_ER          0x0400 // The right edge of the tile is exposed to flood
#define TF_EB          0x0800 // The bottom edge of the tile is exposed to flood
#define TF_ELT         (TF_EL|TF_ET)             // Left-top edge exposed     (<^ )
#define TF_ELR         (TF_EL|TF_ER)             // Left-right edge exposed   (< >)
#define TF_ETR         (TF_ET|TF_ER)             // Top-right edge exposed    ( ^>)
#define TF_ETB         (TF_ER|TF_EB)             // Top-bottom edge exposed   ( I )
#define TF_ERB         (TF_ER|TF_EB)             // Right-bottom edge exposed ( v>)
#define TF_ELB         (TF_EL|TF_EB)             // Left-bottom edge exposed  (<v )
#define TF_ELTR        (TF_EL|TF_ET|TF_ER)       // Left-top-right exposed    (<^>)
#define TF_ETRB        (TF_ET|TF_ER|TF_EB)       // Top-right-bottom exposed  ( I>)
#define TF_ELRB        (TF_EL|TF_ER|TF_EB)       // Left-right-bottom exposed (<v>)
#define TF_ELTB        (TF_EL|TF_ET|TF_EB)       // Left-top-bottom exposed   (<I )
#define TF_EA          (TF_EL|TF_ET|TF_ER|TF_EB) // All edges exposed         (<I>)
#define TF_P           0x1000 // The tile below is protected from digging
#define TF_T           0x2000 // Tile is a track for train
#define TF_E           0x4000 // Line support for elevator
/* --------------------------------------------------------------------------------- */
USHORT TileData[TILES_NUM]= // Tile flags data. Data about each individual tile
{
  // 0################## 1##################### 2##################### 3##################### 4#####################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_F|TF_D,             TF_F|TF_D|TF_ERB,
  // 5################## 6##################### 7##################### 8##################### 9#####################
  TF_F|TF_D|TF_ELRB,     TF_F|TF_D|TF_ELTR,     TF_AD|TF_EA,           TF_D|TF_ELT,           TF_F|TF_D|TF_ELRB,
  // 10################# 11#################### 12#################### 13#################### 14####################
  TF_F|TF_D|TF_ELB,      TF_D|TF_ETR,           TF_D|TF_ELTR,          TF_F|TF_D|TF_ERB,      TF_D|TF_ELT,
  // 15################# 16#################### 17#################### 18#################### 19####################
  TF_D|TF_ETR,           TF_F|TF_D|TF_ELB,      TF_F|TF_D|TF_ER,       TF_F|TF_D|TF_ERB,      TF_D|TF_ET,
  // 20################# 21#################### 22#################### 23#################### 24####################
  TF_F|TF_D|TF_ERB,      TF_D|TF_ETR,           TF_D|TF_ELT,           TF_F|TF_D|TF_ERB,      TF_D|TF_ETR,
  // 25################# 26#################### 27#################### 28#################### 29####################
  TF_D|TF_ETR,           TF_F|TF_D|TF_EL,       TF_F|TF_D|TF_EB,       TF_D|TF_ELT,           TF_F|TF_D|TF_EB,
  // 30################# 31#################### 32#################### 33#################### 34####################
  TF_D|TF_ELTR,          TF_F|TF_D|TF_EL,       TF_F|TF_D|TF_ELB,      TF_D|TF_ELT,           TF_F|TF_D|TF_ELB,
  // 35################# 36#################### 37#################### 38#################### 39####################
  TF_F|TF_D|TF_ELB,      TF_F|TF_D|TF_ELB,      TF_D|TF_ET,            TF_F|TF_D|TF_ELB,      TF_D|TF_ETR,
  // 40################# 41#################### 42#################### 43#################### 44####################
  TF_D|TF_ELT,           TF_D|TF_ELB,           TF_F|TF_D|TF_ELT,      TF_D|TF_ELT,           TF_D|TF_ER,
  // 45################# 46#################### 47#################### 48#################### 49####################
  TF_F|TF_D|TF_EB,       TF_F|TF_D|TF_ERB,      TF_F|TF_D|TF_ELRB,     TF_F|TF_D|TF_ELRB,     TF_F|TF_D|TF_ETRB,
  // 50################# 51#################### 52#################### 53#################### 54####################
  TF_D|TF_ETRB,          TF_D|TF_ETR,           TF_D|TF_ELTR,          TF_D|TF_ELTR,          TF_D|TF_ELB,
  // 55################# 56#################### 57#################### 58#################### 59####################
  TF_F|TF_D|TF_ELTB,     TF_F|TF_D|TF_ELTB,     TF_D|TF_ELT,           TF_D|TF_EA,            TF_D|TF_EA,
  // 60################# 61#################### 62#################### 63#################### 64####################
  TF_D|TF_EA,            TF_D|TF_EA,            TF_ELRB,               TF_F,                  TF_F,
  // 65################# 66#################### 67#################### 68#################### 69####################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 70################# 71#################### 72#################### 73#################### 74####################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 75################# 76#################### 77#################### 78#################### 79####################
  TF_NONE,               TF_P|TF_EA,            TF_P|TF_EA,            TF_P|TF_EA,            TF_P|TF_EA,
  // 80################# 81#################### 82#################### 83#################### 84####################
  TF_P|TF_EA,            TF_P|TF_EA,            TF_P|TF_EA,            TF_P|TF_EA,            TF_P|TF_EA,
  // 85################# 86#################### 87#################### 88#################### 89####################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_D|TF_P|TF_EA,       TF_NONE,
  // 90################# 91#################### 92#################### 93#################### 94####################
  TF_NONE,               TF_D|TF_P|TF_EA,       TF_NONE,               TF_NONE,               TF_NONE,
  // 95################# 96#################### 97#################### 98#################### 99####################
  TF_EA,                 TF_EA,                 TF_W|TF_AB,            TF_W|TF_AB,            TF_W|TF_AB,
  // 100################ 101################### 102################### 103################### 104###################
  TF_W|TF_AE,            TF_W|TF_AB,            TF_W|TF_AB,            TF_W|TF_AB,            TF_W|TF_AE,
  // 105################ 106################### 107################### 108################### 109###################
  TF_W|TF_AB,            TF_W|TF_AB,            TF_W|TF_AB,            TF_W|TF_AE,            TF_W|TF_AB,
  // 110################ 111################### 112################### 113################### 114###################
  TF_W|TF_AB,            TF_W|TF_AB,            TF_W|TF_AE,            TF_W,                  TF_W,
  // 115################ 116################### 117################### 118################### 119###################
  TF_W,                  TF_W,                  TF_W,                  TF_W|TF_AB,            TF_W|TF_AB,
  // 120################ 121################### 122################### 123################### 124###################
  TF_W|TF_AB,            TF_W|TF_AE,            TF_NONE,               TF_NONE,               TF_NONE,
  // 125################ 126################### 127################### 128################### 129###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 130################ 131################### 132################### 133################### 134###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_P|TF_EA,
  // 135################ 136################### 137################### 138################### 139###################
  TF_P|TF_EA,            TF_P|TF_EA,            TF_P|TF_EA,            TF_W,                  TF_W,
  // 140################ 141################### 142################### 143################### 144###################
  TF_W,                  TF_W,                  TF_NONE,               TF_NONE,               TF_W,
  // 145################ 146################### 147################### 148################### 149###################
  TF_NONE,               TF_NONE,               TF_W,                  TF_NONE,               TF_D|TF_P|TF_T|TF_EA,
  // 150################ 151################### 152################### 153################### 154###################
  TF_D|TF_AD|TF_EA,      TF_F,                  TF_F,                  TF_F,                  TF_F,
  // 155################ 156################### 157################### 158################### 159###################
  TF_F,                  TF_F,                  TF_F,                  TF_F,                  TF_F,
  // 160################ 161################### 162################### 163################### 164###################
  TF_F,                  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 165################ 166################### 167################### 168################### 169###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_D|TF_AD|TF_P|TF_T|TF_EA,
  // 170################ 171################### 172################### 173################### 174###################
  TF_D|TF_AD|TF_EA,      TF_D|TF_AD|TF_EA,      TF_D|TF_AD|TF_EA,      TF_D|TF_ELTR,          TF_D|TF_ELTR,
  // 175################ 176################### 177################### 178################### 179###################
  TF_D|TF_ELTR,          TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 180################ 181################### 182################### 183################### 184###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 185################ 186################### 187################### 188################### 189###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_EA|TF_E,
  // 190################ 191################### 192################### 193################### 194###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 195################ 196################### 197################### 198################### 199###################
  TF_AB|TF_PO,           TF_AB|TF_PO,           TF_AB|TF_PO,           TF_AE|TF_PO,           TF_AB|TF_PT,
  // 200################ 201################### 202################### 203################### 204###################
  TF_AB|TF_PT,           TF_AB|TF_PT,           TF_AE|TF_PT,           TF_D|TF_ELR,           TF_F|TF_D|TF_EB,
  // 205################ 206################### 207################### 208################### 209###################
  TF_F|TF_D|TF_ER,       TF_D|TF_ELTR,          TF_F|TF_D|TF_ERB,      TF_D|TF_ETR,           TF_F|TF_D|TF_ERB,
  // 210################ 211################### 212################### 213################### 214###################
  TF_D|TF_P|TF_T|TF_EA,  TF_F|TF_D|TF_ETR,      TF_F|TF_D|TF_ETRB,     TF_D|TF_ETR,           TF_D|TF_ETR,
  // 215################ 216################### 217################### 218################### 219###################
  TF_D|TF_ELTB,          TF_F|TF_D|TF_ELB,      TF_D|TF_ELTB,          TF_D|TF_ELRB,          TF_D|TF_ELT,
  // 220################ 221################### 222################### 223################### 224###################
  TF_D|TF_ELTB,          TF_D|TF_ELT,           TF_D|TF_ELT,           TF_F|TF_D|TF_ETRB,     TF_W|TF_D|TF_ERB,
  // 225################ 226################### 227################### 228################### 229###################
  TF_F|TF_D|TF_ETRB,     TF_D|TF_ELRB,          TF_D|TF_ELTB,          TF_D|TF_ELTB,          TF_D|TF_ELTB,
  // 230################ 231################### 232################### 233################### 234###################
  TF_D|TF_ETRB,          TF_D|TF_ETRB,          TF_D|TF_ETRB,          TF_NONE,               TF_NONE,
  // 235################ 236################### 237################### 238################### 239###################
  TF_D|TF_EA,            TF_D|TF_EA,            TF_D|TF_ELTR,          TF_D|TF_ELTR,          TF_D|TF_ELTR,
  // 240################ 241################### 242################### 243################### 244###################
  TF_F,                  TF_F,                  TF_F,                  TF_F,                  TF_F|TF_D|TF_W|TF_ERB,
  // 245################ 246################### 247################### 248################### 249###################
  TF_F|TF_D|TF_W|TF_ELRB,TF_D|TF_W|TF_ELTR,     TF_D|TF_W|TF_EA,       TF_D|TF_W|TF_ELT,      TF_F|TF_D|TF_W|TF_ELRB,
  // 250################ 251################### 252################### 253################### 254###################
  TF_F|TF_D|TF_W|TF_ELB, TF_D|TF_W|TF_ETR,      TF_D|TF_W|TF_ELTR,     TF_F|TF_D|TF_W|TF_EA,  TF_D|TF_W|TF_EA,
  // 255################ 256################### 257################### 258################### 259###################
  TF_D|TF_W|TF_EA,       TF_F|TF_D|TF_W|TF_EA,  TF_F|TF_D|TF_W|TF_ERB, TF_F|TF_D|TF_W|TF_ERB, TF_D|TF_W|TF_ET,
  // 260################ 261################### 262################### 263################### 264###################
  TF_F|TF_D|TF_W|TF_ERB, TF_D|TF_W|TF_ETR,      TF_D|TF_W|TF_ELT,      TF_F|TF_D|TF_W|TF_ERB, TF_D|TF_W|TF_ETR,
  // 265################ 266################### 267################### 268################### 269###################
  TF_D|TF_W|TF_ETR,      TF_F|TF_D|TF_W|TF_EL,  TF_F|TF_D|TF_W|TF_EB,  TF_D|TF_W|TF_ELT,      TF_F|TF_D|TF_W|TF_EB,
  // 270################ 271################### 272################### 273################### 274###################
  TF_D|TF_W|TF_ELTR,     TF_F|TF_D|TF_W|TF_EL,  TF_F|TF_D|TF_W|TF_ELB, TF_D|TF_W|TF_ELT,      TF_F|TF_D|TF_W|TF_ELB,
  // 275################ 276################### 277################### 278################### 279###################
  TF_F|TF_D|TF_W|TF_ELB, TF_F|TF_D|TF_W|TF_ELB, TF_D|TF_W|TF_EB,       TF_F|TF_D|TF_W|TF_ELB, TF_D|TF_W|TF_ERB,
  // 280################ 281################### 282################### 283################### 284###################
  TF_D|TF_W|TF_ELT,      TF_D|TF_W|TF_ELB,      TF_F|TF_D|TF_W|TF_ELT, TF_D|TF_W|TF_ELT,      TF_F|TF_D|TF_W|TF_ER,
  // 285################ 286################### 287################### 288################### 289###################
  TF_F|TF_D|TF_W|TF_EB,  TF_F|TF_D|TF_W|TF_ERB, TF_F|TF_D|TF_W|TF_ELRB,TF_F|TF_D|TF_W|TF_ELRB,TF_D|TF_W|TF_ETRB,
  // 290################ 291################### 292################### 293################### 294###################
  TF_D|TF_W|TF_ETRB,     TF_D|TF_W|TF_ETR,      TF_D|TF_W|TF_ELTR,     TF_D|TF_W|TF_ELTR,     TF_F|TF_D|TF_W|TF_ELB,
  // 295################ 296################### 297################### 298################### 299###################
  TF_D|TF_W|TF_ELTB,     TF_D|TF_W|TF_ELTB,     TF_D|TF_W|TF_ELT,      TF_D|TF_W|TF_EA,       TF_D|TF_W|TF_EA,
  // 300################ 301################### 302################### 303################### 304###################
  TF_D|TF_W|TF_EA,       TF_D|TF_W|TF_EA,       TF_D|TF_W|TF_ELRB,     TF_F,                  TF_F,
  // 305################ 306################### 307################### 308################### 309###################
  TF_F,                  TF_F,                  TF_F,                  TF_F,                  TF_F,
  // 310################ 311################### 312################### 313################### 314###################
  TF_F,                  TF_F,                  TF_F,                  TF_F,                  TF_F,
  // 315################ 316################### 317################### 318################### 319###################
  TF_F,                  TF_D|TF_W|TF_P|TF_EA,  TF_D|TF_W|TF_P|TF_EA,  TF_D|TF_W|TF_P|TF_EA,  TF_D|TF_W|TF_P|TF_EA,
  // 320################ 321################### 322################### 323################### 324###################
  TF_D|TF_W|TF_P|TF_EA,  TF_D|TF_W|TF_P|TF_EA,  TF_D|TF_W|TF_P|TF_EA,  TF_D|TF_W|TF_P|TF_EA,  TF_D|TF_W|TF_P|TF_EA,
  // 325################ 326################### 327################### 328################### 329###################
  TF_D|TF_W,             TF_NONE,               TF_NONE,               TF_D|TF_W|TF_P|TF_EA,  TF_NONE,
  // 330################ 331################### 332################### 333################### 334###################
  TF_NONE,               TF_D|TF_W|TF_P|TF_EA,  TF_NONE,               TF_NONE,               TF_F,
  // 335################ 336################### 337################### 338################### 339###################
  TF_D|TF_W|TF_EA,       TF_D|TF_W|TF_EA,       TF_NONE,               TF_NONE,               TF_NONE,
  // 340################ 341################### 342################### 343################### 344###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 345################ 346################### 347################### 348################### 349###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 350################ 351################### 352################### 353################### 354###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 355################ 356################### 357################### 358################### 359###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 360################ 361################### 362################### 363################### 364###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 365################ 366################### 367################### 368################### 369###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 370################ 371################### 372################### 373################### 374###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_D|TF_W|TF_P|TF_EA,
  // 375################ 376################### 377################### 378################### 379###################
  TF_D|TF_W|TF_P|TF_EA,  TF_D|TF_W|TF_P|TF_EA,  TF_D|TF_W|TF_P|TF_EA,  TF_D|TF_W|TF_P|TF_EA,  TF_NONE,
  // 380################ 381################### 382################### 383################### 384###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 385################ 386################### 387################### 388################### 389###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_D|TF_W|TF_T|TF_P|TF_EA,
  // 390################ 391################### 392################### 393################### 394###################
  TF_D|TF_W|TF_EA,       TF_F,                  TF_F,                  TF_F,                  TF_F,
  // 395################ 396################### 397################### 398################### 399###################
  TF_F,                  TF_F,                  TF_F,                  TF_F,                  TF_F,
  // 400################ 401################### 402################### 403################### 404###################
  TF_F,                  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 405################ 406################### 407################### 408################### 409###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_D|TF_D|TF_W|TF_T|TF_P|TF_EA,
  // 410################ 411################### 412################### 413################### 414###################
  TF_D|TF_D|TF_W|TF_EA,  TF_D|TF_W|TF_EA,       TF_D|TF_D|TF_W|TF_EA,  TF_D|TF_D|TF_W|TF_ELTR,TF_D|TF_D|TF_W|TF_ELTR,
  // 415################ 416################### 417################### 418################### 419###################
  TF_D|TF_D|TF_W|TF_ELTR,TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 420################ 421################### 422################### 423################### 424###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,
  // 425################ 426################### 427################### 428################### 429###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_D|TF_W|TF_E|TF_EA,
  // 430################ 431################### 432################### 433################### 434###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_NONE,               TF_D|TF_P,
  // 435################ 436################### 437################### 438################### 439###################
  TF_D|TF_P,             TF_D|TF_P,             TF_D|TF_W|TF_D|TF_P,   TF_D|TF_P,             TF_D|TF_W|TF_D|TF_P,
  // 440################ 441################### 442################### 443################### 444###################
  TF_NONE,               TF_NONE,               TF_NONE,               TF_D|TF_W|TF_ETR,      TF_F|TF_D|TF_W|TF_EB,
  // 445################ 446################### 447################### 448################### 449###################
  TF_F|TF_D|TF_W|TF_ER,  TF_D|TF_W|TF_ELTR,     TF_F|TF_D|TF_W|TF_ERB, TF_D|TF_W|TF_ETR,      TF_F|TF_D|TF_W|TF_ERB,
  // 450################ 451################### 452################### 453################### 454###################
TF_D|TF_W|TF_T|TF_P|TF_EA, TF_D|TF_W|TF_ETR,    TF_D|TF_W|TF_ETRB,     TF_D|TF_W|TF_ETR,      TF_D|TF_W|TF_ETR,
  // 455################ 456################### 457################### 458################### 459###################
  TF_D|TF_W|TF_ELTB,     TF_F|TF_D|TF_W|TF_ELB, TF_D|TF_W|TF_ELT,      TF_F|TF_D|TF_W|TF_ELRB,TF_D|TF_W|TF_ELT,
  // 460################ 461################### 462################### 463################### 464###################
  TF_D|TF_W|TF_ELTB,     TF_D|TF_W|TF_ELB,      TF_D|TF_W|TF_ELB,      TF_D|TF_W|TF_ETRB,     TF_D|TF_W|TF_F|TF_ERB,
  // 465################ 466################### 467################### 468################### 469###################
  TF_D|TF_W|TF_ETRB,     TF_D|TF_W|TF_F|TF_ELRB,TF_D|TF_W|TF_ELTB,     TF_D|TF_W|TF_ELTB,     TF_D|TF_W|TF_ELTB,
  // 470################ 471################### 472################### 473################### 474###################
  TF_D|TF_W|TF_ETRB,     TF_D|TF_W|TF_ETRB,     TF_D|TF_W|TF_ETRB,     TF_D|TF_W,             TF_NONE,
  // 475################ 476################### 477################### 478################### 479###################
  TF_D|TF_W|TF_P|TF_EA,  TF_D|TF_W|TF_P|TF_EA,  TF_D|TF_W|TF_ELTR,     TF_D|TF_W|TF_ELTR,     TF_D|TF_W|TF_ELTR
};
/* --------------------------------------------------------------------------------- */
// The rising and falling jump data table is 24 bytes long (total 48). You must change
// the #define's below if you change the size of this array. The objects actiontimer
// is used to lookup values from this array, if the value is 1 then the object and
// move up one pixel, else if it is 0 then no movement is performed and acts like a
// delay.
/* --------------------------------------------------------------------------------- */
UCHAR JumpDataRising[JD_SIZE]=
{
  JD_MOVE, JD_MOVE, JD_MOVE, JD_MOVE, JD_MOVE, JD_MOVE, JD_MOVE, JD_MOVE,
  JD_MOVE, JD_MOVE, JD_MOVE, JD_MOVE, JD_MOVE, JD_WAIT, JD_MOVE, JD_WAIT,
  JD_WAIT, JD_MOVE, JD_WAIT, JD_WAIT, JD_WAIT, JD_MOVE, JD_WAIT, JD_WAIT
};
/* --------------------------------------------------------------------------------- */
UCHAR JumpDataFalling[JD_SIZE]=
{
  JD_WAIT, JD_WAIT, JD_WAIT, JD_MOVE, JD_WAIT, JD_WAIT, JD_WAIT, JD_MOVE,
  JD_WAIT, JD_WAIT, JD_MOVE, JD_WAIT, JD_MOVE, JD_MOVE, JD_MOVE, JD_MOVE,
  JD_MOVE, JD_MOVE, JD_MOVE, JD_MOVE, JD_MOVE, JD_MOVE, JD_MOVE, JD_MOVE
};
/* --------------------------------------------------------------------------------- */
class SOUND;                       // Sound effect class prototype
/* --------------------------------------------------------------------------------- */
typedef class SOUND *PSOUND;       // Pointer to sound class
/* --------------------------------------------------------------------------------- */
class SOUND                         // Sound file structure
{
  private:
    INT                 Id;         // Unique sound class identifier
    PULONG              Data;       // Wave data file
    ULONG               Size;       // Size of wave data file
    LPDIRECTSOUNDBUFFER Buffer;     // Direct sound buffer
    DSBUFFERDESC        BufferDesc; // Buffer descriptor for sound

  public:
    PSOUND              Prev;       // Previous sound in list
    PSOUND              Next;       // Next sound in list

  public:
    // Constructor
    SOUND(SFXID);
    // Deconstructor
    ~SOUND(VOID);
    // Play sound
    VOID PLAY(LONG);
    // Upload (and restore) sound buffer. Returns next sound in list
    PSOUND UPLOAD(BOOL);
};
/* --------------------------------------------------------------------------------- */
class TEXTURE;                     // Prototype for texture class
/* --------------------------------------------------------------------------------- */
typedef class TEXTURE *PTEXTURE;   // Typedef pointer to texture class
/* --------------------------------------------------------------------------------- */
class TEXTURE                      // Texture class
{
  private:
    ULONG                Id;       // Unique identifier
    LPDIRECTDRAWSURFACE *Surfaces; // Direct draw surfaces
    INT                  Count;    // Number of surfaces in array
    ULONG                BltParam; // Parameters to blit operations
    RECT                 Bounds;   // Bounds of texture in bitmap

  public:
    HBITMAP              Bitmap;   // GDI bitmap handle

    PTEXTURE             Prev;     // Previous texture in list
    PTEXTURE             Next;     // Next texture in list


  public:
    // Constructor
    TEXTURE(BMPID, INT, BOOL);
    // Deconstructor
    ~TEXTURE(VOID);
    // Draw texture with off-screen checking
    VOID BLT(INT, INT, INT);
    // Draw texture without off-screen checking
    VOID BLTFAST(INT, INT, INT);
    // Draw partial texture without off-screen checking
    VOID BLTFASTPARTIAL(INT, INT, INT, INT);
    // Draw from specified bounds without off-screen checking
    VOID BLTFASTCUSTOM(INT, INT, INT, INT, INT, INT, INT);
    // Upload (and restore) texture surface. Returns next texture in list
    PTEXTURE UPLOAD(BOOL);
    // Print a string to the specified co-ordinates
    VOID PRINT(INT, INT, BOOL, PCHAR);
    // Print a formatted string to the specified co-ordinates
    VOID PRINTF(INT, INT, BOOL, PCHAR, ...);
};
/* --------------------------------------------------------------------------------- */
class OBJECT;               // Prototype for object class
class PLAYER;               // Prototype for player class
/* --------------------------------------------------------------------------------- */
typedef class OBJECT *POBJECT; // Typedef pointer to object class
typedef class PLAYER *PPLAYER; // Typedef pointer to player class
/* --------------------------------------------------------------------------------- */
class PLAYER                // Player class
{
  public:
    INT     Id;             // Unique identifier
    INT     Flags;          // Player flags
    PPLAYER Opponent;       // Opponent to this player
    OBJTYP  Race;           // Current digger race
    COORDS  Start;          // Starting position
    INT     Target;         // Target capital needed to win
    INT     Money;          // Amount of money this level
    INT     Reward;         // Reward given if this player wins
    INT     Assets;         // Refund for of all the devices carried
    INT     Salary;         // Salary paid to diggers
    INT     DeathDuty;      // Penalty for killed diggers
    INT     Deposit;        // Deposit/withdrawl made from bank
    INT     Loan;           // Loan taken out of bank
    INT     Balance;        // Bank balance
    INT     DiggerNum;      // Number of diggers remaining
    INT     DirtDug;        // Amount of dirt dug by all diggers
    INT     GemsFound;      // Number of gems found

    POBJECT Diggers[NUM_DIGGERS]; // Diggers that belong to this player

  public:
    // Initialise player variables
    VOID INIT(OBJTYP, INT, INT, BOOL);
    // Create a digger
    POBJECT CREATEDIGGER(INT);
    // Calculate income and expenditures
    VOID CALCINCEXP(BOOL);
    // A digger died
    VOID DIGGERDIED(POBJECT);
    // Capital raised check
    VOID CHECKCAPITALRAISED(VOID);
    // Ressurect a digger
    POBJECT RESSURECTDIGGER(BOOL);
    // Reset diggers
    VOID RESETDIGGERS(VOID);
};
/* --------------------------------------------------------------------------------- */
class OBJECT               // Object class
{
  public:
    POBJECT  Prev;         // The previous object in the list
    POBJECT  Next;         // The next object in the list

    INT      Id;           // Unique identifier
    UINT64   Flags;        // Object flags
    PPLAYER  Owner;        // Which player owns this object? (null = neutral)
    POBJECT  Carrier;      // Object that is carrying this object
    struct ACTDATA *ActData;    // Pointer to action data from lookup table
    struct OBJDATA *ObjData;    // Pointer to object data from lookup table
    struct DIRDATA *DirData;    // Pointer to direction data from lookup table
    struct DIRDATA *DirDataAtt; // Pointer to direction data from lookup table for attachment
    POBJECT  Target;       // Current target digger or telepole to home in on
    INT      Health;       // Current object health
    OBJTYP   Type;         // Type of object (player, treasure, flora, fauna, etc.)
    OBJJOB   Job;          // Current job of object
    OBJACT   Action;       // Current action of object
    OBJDIR   Direction;    // Current direction of object
    COORDS   Position;     // Position of object
    COORDS   PositionAbs;  // Position of object modulous to TILE_SIZE
    INT      ActionTimer;  // Action timer (for walking, digging, etc.)
    INT      FallSpeed;    // Falling speed
    INT      FallDamage;   // Falling damage
    OBJDIR   FailedDig;    // Last failed dig direction
    INT      LastDigTime;  // Time a successful dig occured
    INT      DirtDug;      // Amount of dirt dug
    INT      GemsFound;    // Number of gems found
    INT      AnimId;       // Current tile # of animation
    INT      AnimIdAtt;    // Current tile # of attached animation
    COORDS   AnimIdAttOff; // Offset horizontal pixels for drawing attachment
    INT      AnimTimer;    // Animation timer
    INT      ItemWeight;   // Weight of all items being carried
    POBJECT  Items;        // Which objects is the object carrying?
    POBJECT  ItemsLast;    // Last item in inventory
    VOID (OBJECT::*AIActiveFunc)(VOID); // AI active callback function
    VOID (OBJECT::*AIFunc)(VOID); // AI callback function

  private:
    // Add object to main objects list
    VOID REGISTER(VOID);
    // Remove object from main objects list
    VOID UNREGISTER(VOID);
    // Add specified object to this objects inventory
    BOOL ADDINVENTORY(POBJECT);
    // Object is colliding with background?
    BOOL ISCOLLIDE(INT, INT);
    // Object is colliding with the specified object?
    BOOL GETCOLLISION(POBJECT);
    // Move object left or right and process job if blocked
    VOID MOVEX(INT);
    // Move object up or down
    VOID MOVEY(INT);
    // Play sound at object position (does not play if player cannot see this object)
    VOID SOUND(SFXID);
    // Dig terrain (1 if dig was successful, 0 if failed)
    BOOL DIG(VOID);
    // Roll the dice and if won spawn treasure
    VOID SPAWNTREASURE(VOID);
    // Set object position
    VOID SETPOS(INT, INT);
    // Adjust object position
    VOID ADJUSTPOS(INT, INT);
    // Adjust object health and perform checks
    VOID ADJUSTHEALTH(INT);
    // Centre viewpoint on object
    VOID FOCUS(VOID);
    // Explode at offset position
    VOID EXPLODE(CHAR, CHAR);
    // AI callback function: Find digger
    VOID AIFIND(VOID);
    // AI callback function: Random axis movement
    VOID AIRANDOM(VOID);
    // AI callback function: Digger like movement
    VOID AIBIGFOOT(VOID);
    // AI callback function: Critter
    VOID AICRITTER(VOID);
    // AI callback function: Digger
    VOID AIDIGGER(VOID);
    // Set busy object
    VOID SETBUSY(VOID);
    // Object no longer busy
    VOID SETNOTBUSY(VOID);

  public:
    // Constructor
    OBJECT(PPLAYER, OBJTYP, INT, INT, BOOL);
    // Deconstructor
    ~OBJECT(VOID);
    // Object main procedure (on each game tick for each object). Returns next object.
    POBJECT MAIN(VOID);
    // Object render (on each game tick after scenery is drawn). Returns next object.
    POBJECT RENDER(VOID);
    // Draw status indicator above the object
    VOID DRAWINDICATOR(VOID);
    // Picks up the colliding item, returns TRUE if successful
    BOOL PICKUPITEM(BOOL);
    // Remove specified object from this objects inventory and place it in level
    POBJECT DROPINVENTORY(POBJECT);
    // Drop all inventory
    VOID DROPALLINVENTORY(VOID);
    // Sell specified object in inventory to vendor (returns value sold for)
    POBJECT SELLITEM(POBJECT);
    // Purchase the specified object into inventory and deposit owner funds
    BOOL PURCHASEITEM(OBJTYP);
    // Set new object action, job and direction
    VOID SETACTION(OBJACT, OBJJOB, OBJDIR);
    // Object is colliding with the mouse cursor
    BOOL GETMCOLLISION(VOID);
    // Select object
    VOID SELECT(VOID);
    // Deploy object into something much bigger
    BOOL DEPLOY(VOID);
    // Control flood gate
    VOID GATECONTROL(BOOL);
    // Rest
    BOOL REST(VOID);
};
/* --------------------------------------------------------------------------------- */
class FLOODDATA;                     // Flood data class prototype
/* --------------------------------------------------------------------------------- */
typedef class FLOODDATA *PFLOODDATA; // Pointer to flood data class
/* --------------------------------------------------------------------------------- */
class FLOODDATA                      // Flood data class
{
  private:
    USHORT     Id;                   // Tile id
    USHORT     IgnoreFlags;          // Tile flags to say which direction not to go in
    INT        Pos;                  // Position of new flood
    USHORT     Flags;                // Tile flags

    PFLOODDATA Prev;                 // Previous flood data class
    PFLOODDATA Next;                 // Next flood data class
  public:
    FLOODDATA(INT, USHORT);          // Constructor
    ~FLOODDATA(VOID);                // Deconstructor
    VOID SEED(VOID);                 // Seed new floods
};
/* --------------------------------------------------------------------------------- */
// Initialise application
VOID                Initialise(VOID);
// Exception handler
LONG WINAPI         ExceptionHandler(PEXCEPTION_POINTERS);
// Load version info data
VOID                GetVersionData(PCHAR);
// Load configuration settings
VOID                Config(CFGCMD);
// Set window text using a format syntax
BOOL                SetWindowTextF(HWND, PCHAR, ...);
// Process command line parameters
VOID                ProcessCommandLineParameters(VOID);
// De-Initialise application
VOID                DeInitialise(VOID);
// Show error information
VOID                Exception(INT, INT, PCHAR, ...);
// Convert a string to an integer
INT                 StringToNumber(PCHAR);
// Generate random number
USHORT              Random(VOID);
// Window callback procedure
LRESULT CALLBACK    WindowCallback(HWND, UINT, WPARAM, LPARAM);
// Load a raw data file from executable
PVOID               LoadFile(INT, PCHAR, PULONG);
/* --------------------------------------------------------------------------------- */
// Directx function wrapper
VOID                DX(INT, HRESULT);
// Initialise direct draw
VOID                InitDirectDraw(VOID);
// DeInitialise direct draw
VOID                DeInitDirectDraw(VOID);
// Initialise direct input
VOID                InitDirectInput(VOID);
// DeInitialise direct input
VOID                DeInitDirectInput(VOID);
// Initialise direct sound
VOID                InitDirectSound(VOID);
// DeInitialise direct sound
VOID                DeInitDirectSound(VOID);
// Toggle window and fullscreen mode
VOID                ToggleWindowMode(VOID);
/* --------------------------------------------------------------------------------- */
// Initialise windows networking library
VOID                InitWinSock(VOID);
// DeInitialise windows networking library
VOID                DeInitWinSock(VOID);
/* --------------------------------------------------------------------------------- */
/* Restore and upload all sounds back to their buffers                               */
VOID                UploadSounds(BOOL);
// Unload all sounds
VOID                UnloadSounds(VOID);
// Load base sound effects
VOID                LoadBaseSoundEffects(VOID);
// Toggle sound effects on and off
VOID                ToggleSounds(VOID);
// Toggle music streaming on and off
VOID                ToggleStreaming(VOID);
// Music control function
VOID                MusicControl(MUSICCMD, MUSICID);
/* --------------------------------------------------------------------------------- */
// Set a new cursor id
VOID                SetMouseCursor(CURSORID);
// Draw mouse cursor
VOID                DrawMouseCursor(VOID);
/* --------------------------------------------------------------------------------- */
// Create a DirectDraw surface
LPDIRECTDRAWSURFACE CreateSurface(HRESULT *, INT, INT);
// Upload (and restore) all GDI textures to video memory
VOID                UploadTextures(BOOL);
// Set palette to the 0-127 of first bitmap and 128-255 of second bitmap
VOID                MergePalette(PTEXTURE, PTEXTURE);
// Unload all textures
VOID                UnloadTextures(VOID);
// Load textures
VOID                LoadTextures(INT);
// Cycle texture scale
VOID                CycleTextureScale(VOID);
/* --------------------------------------------------------------------------------- */
// Loop function to call when window is active
VOID CALLBACK       ActiveWindowProc(VOID);
// Loop function to call when window is inactive
VOID CALLBACK       InactiveWindowProc(VOID);
// Draw to window
VOID CALLBACK       WindowFlip(VOID);
// Draw to full-screen window
VOID CALLBACK       FullScreenFlip(VOID);
// Control prevention function
VOID CALLBACK       PreventControl(VOID);
// Main control function                                                             */
VOID CALLBACK       MainControl(VOID);
// Deinitialise level
VOID                DeInitLevel(VOID);
// Demonstration
VOID CALLBACK       DemoProc(VOID);
// Toggle pause
VOID                TogglePause(VOID);
// Pause the game
VOID CALLBACK       GamePauseProc(VOID);
// Quit the game
VOID CALLBACK       QuitGame(VOID);
// Load level data
VOID                LoadLevelData(BOOL);
// Initialise level
VOID                InitLevel(INT, PPLAYER, OBJTYP, OBJTYP);
// Show a tip at top of screen
VOID                ShowTip(PCHAR, ...);
// Intro sequence to in-game
VOID CALLBACK       IntroInGameProc(VOID);
// Transition callback for in-game initialisation
VOID CALLBACK       InitInGameProc(VOID);
// Map post mortem callback function
VOID CALLBACK       MapPostMortemProc(VOID);
// In-game tick callback function
VOID CALLBACK       InGameProc(VOID);
// Configuration callback function
VOID CALLBACK       ConfigProc(VOID);
// Game over game tick callback function
VOID CALLBACK       GameOverProc(VOID);
// Transition out
VOID CALLBACK       TransitionOut(VOID);
// Transition in
VOID CALLBACK       TransitionIn(VOID);
// Create a transition
VOID                CreateTransition(BOOL, GAMEFUNC);
// Draw parallax background and terrain
VOID                DrawTerrain(BOOL);
// Grab primary surface and dump it to a bitmap on disk
VOID                ScreenShot(VOID);
// Set current view on the level
VOID                SetViewport(INT, INT);
// Update level data with tile at specified location
VOID                UpdateLevelData(INT, USHORT);
// Set the current control menu
VOID                SetControlMenu(MENUID);
// Init level editor proc
VOID CALLBACK       InitEditorProc(VOID);
// Level editor proc
VOID CALLBACK       EditorProc(VOID);
/* ================================================================================= */
/* ################################################################################# */
/* ################################################################################# */
/* ###                                                                           ### */
/* ###   C O R E  D A T A   S T R U C T U R E                                    ### */
/* ###                                                                           ### */
/* ################################################################################# */
/* ################################################################################# */
/* --------------------------------------------------------------------------------- */
struct          // Beginning of application data structure
{
  UCHAR                 Flags;               // Global flags

  struct        // Beginning of core data structure
  {
    UCHAR               AddressLength;       // Size of an address (4[32b]|8[64b])
    ULONG               RandomSeed;          // Seed for random number generator
  }
  Core;         // End of core data structure
  struct        // Beginning of version data structure
  {
    UCHAR               Major;               // Major version of application
    UCHAR               Minor;               // Minor version of application
    UCHAR               Bits;                // 32 or 64-Bits
    PCHAR               Date;                // Time when application was compiler
    PCHAR               File;                // Filename of source
  }
  Version;      // End of version data structure
  struct        // Beginning of process data structure
  {
    HANDLE              Heap;                // Handle to process memory heap
    HANDLE              Mutex;               // Mutex to handle multiple instances
    HANDLE              Handle;              // Handle to process
    STARTUPINFO         Info;                // Startup information
    PCHAR               Path;                // Full path to executable
    PCHAR               FullPath;            // Full path to executable including name
  }
  Process;      // End of process data structure
  struct        // Beginning of window data structure
  {
    WNDCLASSEX          Class;               // Window class
    HWND                Handle;              // Handle to window
    MSG                 Message;             // For PeekMessage and GetMessage
    POINT               SourcePoint;         // Source co-ordinates of window
    RECT                Source;              // Source bounds for window flip
    RECT                Destination;         // Destination bounds for window flip
  }
  Window;       // End of window data structure
  struct        // Beginning of timer data structure
  {
    UCHAR               Limiter;             // Frame limiter amount
    ULONG               LastRender;          // Time the frame was last processed
    ULONG               CurrentTime;         // Current game ticks
    GAMEFUNC            Function;            // Tick function for main loop
  }
  Timer;        // End of timer data structure
  struct        // Beginning of video core data structure
  {
    DEVMODE             DesktopInfo;         // Current desktop settings
    struct MODEDATA     *ActiveMode;         // Active display mode
    UCHAR               ActiveModeId;        // Active display mode id

    LPDIRECTDRAW        Device;              // Direct draw object
    DDCAPS              Capabilities;        // Direct draw device capabilities
    DDCAPS              HelCapabilities;     // Direct draw device hel capabilities
    LPDIRECTDRAWSURFACE PrimarySurface;      // Direct draw primary surface
    LPDIRECTDRAWSURFACE SecondarySurface;    // Direct draw secondary surface
    LPDIRECTDRAWPALETTE Palette;             // Direct draw palette for 8bit mode
    LPDIRECTDRAWCLIPPER Clipper;             // Direct draw clipper for window mode

    struct      // Beginning of texture data structure
    {
      PTEXTURE          First;               // First texture in list
      PTEXTURE          Last;                // Last texture in list
      INT               Count;               // Texture reference counter
      INT               Flags;               // INITTYP flags

      PTEXTURE          Terrain;             // Terrain tileset
      PTEXTURE          Sprite;              // Sprite tileset
      PTEXTURE          Interface;           // Interface tileset
      PTEXTURE          Parallax;            // Parallax texture
      PTEXTURE          Trans;               // Fake transparency texture
      PTEXTURE          Aux;                 // Multiple use auxilary texture
      PTEXTURE          AuxTwo;              // Multiple use auxilary texture

      PTEXTURE          FontLarge;           // Large font
      PTEXTURE          FontSmall;           // Small font
      PTEXTURE          FontTiny;            // Tiny font
    }
    Tex;        // End of texture data structure

    GAMEFUNC            Function;            // Flip function to call on game tick
  }
  Video;        // End of video core data structure
  struct        // Beginning of audio core data structure
  {
    LPDIRECTSOUND       Device;              // Direct sound object
    LPDIRECTSOUNDBUFFER PrimaryBuffer;       // Direct sound primary buffer
    DSCAPS              Capabilities;        // Direct soudn device capabilities
    INT                 Rate;                // Sound rate

    struct      // Beginning of music data structure
    {
      MUSICTYPEID       Type;                // Type of music
      MUSICID           SongId;              // Id number of song
      INT               Volume;              // Volume slider

      PVOID             Data;                // Music data (XM ONLY)
      ULONG             Size;                // Size of data (XM ONLY)
      LPDIRECTSOUNDBUFFER Buffer;            // Direct sound buffer (XM ONLY)
      DSBUFFERDESC      BufferDesc;          // Surface descriptor (XM ONLY)

      IGraphBuilder     *GraphBuilder;       // Graph builder (STREAM ONLY)
      IMediaControl     *MediaControl;       // Media control (STREAM ONLY)
      IMediaEventEx     *MediaEvent;         // Media event (STREAM ONLY)
      IMediaPosition    *MediaPosition;      // Media position (STREAM ONLY)
      IBasicAudio       *BasicAudio;         // Media volume (STREAM ONLY)
    }
    Music;      // End of music data structure
    struct      // Beginning of sound effects data structure
    {
      PSOUND            First;               // First sound in list
      PSOUND            Last;                // Last sound in list
      INT               Count;               // Sound reference counter
      INT               Volume;              // Sound volume

      PSOUND            Sound[SFX_MAX];      // All the in-game sounds
    }
    Sfx;        // End of sound effects data structure
  }
  Audio;        // End of audio core data structure
  struct        // Beginning of input core data structure
  {
    LPDIRECTINPUT       Device;              // Direct input object

    struct      // Beginning of keyboard data structure
    {
      LPDIRECTINPUTDEVICE Device;            // Direct input keyboard device
      UCHAR             DeviceState[256];    // Raw dinput keyboard state
      UCHAR             State[256];          // Converted dinput keyboard state
    }
    Keyb;       // End of keyboard data structure
    struct      // Beginning of mouse data structure
    {
      LPDIRECTINPUTDEVICE Device;            // Direct input mouse device
      DIMOUSESTATE      DeviceState;         // Raw dinput mouse state
      UCHAR             State[MID_MAX];      // Converted dinput mouse state
      COORDS            Pos;                 // Mouse co-ordinates and buttons
      struct CURSOR     *Cursor;             // Currently set mouse cursor
      USHORT            CursorAnimId;        // Anim id of mouse cursor
      INT               CursorTimer;         // So the animation doesnt run too fast
    }
    Mouse;      // End of mouse data structure

    GAMEFUNC            Function;            // Control function to call on game tick
  }
  Input;        // End of input core data structure
  struct        // Begining of mask data structure
  {
    HDC                 Tileset;             // Surface to mask tileset
    HBITMAP             TilesetBmp;          // Bitmap to mask tileset
    HDC                 LevelSurface;        // App.Game.Level mask surface
    HBITMAP             LevelSurfaceBmp;     // App.Game.Level mask surface bitmap
    HDC                 Tile;                // Surface to mask tile
    HBITMAP             TileBmp;             // Bitmap to mask tile surface
    LPBITMAPINFO        TileBmpInfo;         // Information about App.Mask.TileBmpInfo;
  }
  Mask;         // End of mask data structure
  struct        // Begining of game data structure
  {
    GAMEFUNC            Function;            // Game function to call on game tick
    INT                 Time;                // Ticks passed since map loaded

    GAMEFUNC            TransEndFunction;    // Game function used when transition has finished
    GAMEFUNC            TransDurFunction;    // Game function used when transition is progressing
    INT                 TransCounter;        // Transition counter

    INT                 Interest;            // Amount of extra money to give when selling
    COORDS              ViewPoint;           // Current map viewport position
    COORDS              VPointAdj;           // Scroll viewport

    struct LEVELINFO    *LevelCurrent;       // Current level selected
    PUSHORT             LevelData;           // Current level data
    INT                 Level;               // Current level
    INT                 NewLevel;            // For InitLevel
    USHORT              LevelScene;          // Scene id

    PFLOODDATA          FirstFlood;          // First flood data item
    PFLOODDATA          LastFlood;           // Last flood data item;
    INT                 FloodCount;          // Number of flood data items;

    struct MENU         *ObjectMenu;         // Currently active menu
    COORDS              ObjectMenuPos;       // Position of menu

    struct      // Beginning of player data structure
    {
      PLAYER            One;                 // Player one
      PLAYER            Two;                 // Player two
      PPLAYER           This;                // Pointer to player controlled by this client
      PPLAYER           Remain;              // The player that has won the game
      OBJTYP            ThisRace;            // Race of this player
      INT               ThisMoney;           // On-screen money of this player (ANIMATION ONLY)
    }
    Player;     // End of player data structure
    struct      // Beginning of object data structure
    {
      POBJECT           First;               // Actual objects array
      POBJECT           Last;                // The last structure in the objects array
      POBJECT           Active;              // Currently selected object
      INT               Count;               // Object reference counter (checking for lost objects)
      POBJECT           Device;              // Currently cycled device object
    }
    Object;     // End of object data structure
    struct      // Beginning of high score data structure
    {
      UCHAR             Name[8];             // Name
      UCHAR             Zone;                // Best zone
      USHORT            Capital;             // Best captial
    }
    HiScore[8]; // End of high score data structure
    struct      // Beginning of level editor data structure
    {
      UCHAR             Flags;               // Editor flags (EFL_*)
      SHORT             BrowserStart;        // Start of tile on browser
      SHORT             BrushTile;           // Selected brush tile
      COORDS            BrushPos;            // Selected position for brush
      INT               BrushAPos;           // Selected position for brush
      OBJTYP            BrushObject;         // Selected brush object
    }
    Editor;     // End of level editor data structure
  }
  Game;         // End of game data structure
  struct        // Beginning of network data structure
  {
    WSADATA             Startup;             // WSA startup data
  }
  Net;          // End of network data structure
}
App;            // End of application data structure
/* ================================================================================= */
/* ################################################################################# */
/* ################################################################################# */
/* ###                                                                           ### */
/* ###   C O R E  F U N C T I O N S                                              ### */
/* ###                                                                           ### */
/* ################################################################################# */
/* ################################################################################# */
/* --------------------------------------------------------------------------------- */
/* Application entry point                                                           */
/* --------------------------------------------------------------------------------- */
INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
  // Set exception filter
  SetUnhandledExceptionFilter(ExceptionHandler);

  // Setup app, window and directx and load game data
  Initialise();
  // Initialise in-game
  if(App.Flags & AFL_EDITOR)
    InitEditorProc();
  else
    InitInGameProc();
  // Loop until WM_QUIT calling tick function
  while(App.Window.Message.message != WM_QUIT) App.Timer.Function();
   // Perform full de-initialisation
  DeInitialise();

  // Successful exit code
  ExitProcess(0);
}
/* --------------------------------------------------------------------------------- */
/* C++ operator to allocate memory
/* --------------------------------------------------------------------------------- */
PVOID _cdecl operator new(size_t Size) { return HeapAlloc(App.Process.Heap, 0xC, Size); }
/* --------------------------------------------------------------------------------- */
/* C++ operator to free memory
/* --------------------------------------------------------------------------------- */
VOID _cdecl operator delete(PVOID Address) { HeapFree(App.Process.Heap, 0x4, Address); }
/* --------------------------------------------------------------------------------- */
/* Initialise application, window and directx                                        */
/* --------------------------------------------------------------------------------- */
VOID Initialise(VOID)
{
  // Initialise registry
  ZeroMemory(&App, sizeof(App));
  // Set size of an address (4[32b]|8[64b])
  App.Core.AddressLength = sizeof(PVOID);
  // Set seed for random number generator
  App.Core.RandomSeed = GetTickCount() / 1000;
  // Set 32 or 64-bit process
  App.Version.Bits  = App.Core.AddressLength << 3;
  // Set time when application was compiled
  App.Version.Date  = __TIMESTAMP__;
  // Set filename of source
  App.Version.File  = __FILE__;
  // Initialise sound frequency
  App.Audio.Rate = 44100;

  // Initialise common controls
  INITCOMMONCONTROLSEX ccData = { sizeof(ccData), ICC_PROGRESS_CLASS };
  // Bail if failed
  if(FAILED(InitCommonControlsEx(&ccData)))
    Exception(__LINE__, GetLastError(), "Failed to initialise common control classes!");

  // Get startup information
  GetStartupInfo(&App.Process.Info);
  // Create a mutex so no more then one instance can be run at any time
  App.Process.Mutex = CreateMutex(0, 1, App.Window.Class.lpszClassName);
  // Bail if function failed
  if(!App.Process.Mutex) Exception(__LINE__, GetLastError(), "Failed to create process mutex");
  // Bail out if there is already an instance running
  if(GetLastError() == ERROR_ALREADY_EXISTS)
  {
    // Find other process
    HWND H = FindWindow(App.Window.Class.lpszClassName, 0);
    // Window found?
    if(H != NULL)
    {
      // Show window
      ShowWindow(H, SW_SHOWNORMAL);
      // Restore window
      ShowWindow(H, SW_RESTORE);
      // Activate window
      SetForegroundWindow(H);
      // Done
    }
    // Deinitialise
    DeInitialise();
    // Done
    ExitProcess(1);
  }
  // Get handle to my heap so I can use my own malloc functions
  App.Process.Heap = GetProcessHeap();
  // Bail if function failed
  if(!App.Process.Heap) Exception(__LINE__, GetLastError(), "Failed to retrieve process heap");
  // Get handle to process so we can set priority class
  App.Process.Handle = GetCurrentProcess();
  // Bail if function failed
  if(!App.Process.Handle) Exception(__LINE__, GetLastError(), "Failed to retrieve process control handle");

  // Allocate memory for path to exe
  App.Process.Path = (PCHAR)HeapAlloc(App.Process.Heap, 0xC, 256);
  // Destination for full executable name
  INT Index = GetModuleFileName(NULL, App.Process.Path, 255);

  // Copy full path
  App.Process.FullPath = (PCHAR)HeapAlloc(App.Process.Heap, 0xC, Index + 1);
  CopyMemory(App.Process.FullPath, App.Process.Path, Index);
  // For each character from the end of the string
  for(--Index; Index >= 0; --Index)
  {
    // If character is not a backslash
    if(App.Process.Path[Index] != '\\')
      // Ignore
      continue;
    // Nullify char after this
    App.Process.Path[++Index] = 0;
    // Done
    break;
  }
  // Realloc the path
  App.Process.Path = (PCHAR)HeapReAlloc(App.Process.Heap, 0x4, App.Process.Path, ++Index);

  // Get version information
  GetVersionData(App.Process.FullPath);

  // Initialise window class structure
  App.Window.Class.cbSize = sizeof(App.Window.Class);
  App.Window.Class.style = CS_HREDRAW | CS_VREDRAW;
  App.Window.Class.lpfnWndProc = (WNDPROC)WindowCallback;
  App.Window.Class.lpszClassName = "Diggers";
  App.Window.Class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  // Get instance handle for this process
  App.Window.Class.hInstance = GetModuleHandle(0);
  // Bail if function failed
  if(!App.Window.Class.hInstance) Exception(__LINE__, GetLastError(), "Failed to retrieve process instance handle");
  // Setup default cursor
  App.Window.Class.hCursor = LoadCursor(NULL, IDC_ARROW);
  // Bail if function failed
  if(!App.Window.Class.hCursor) Exception(__LINE__, GetLastError(), "Failed to load default cursor");
  // Setup large icon for window
  App.Window.Class.hIcon = (HICON)LoadImage(App.Window.Class.hInstance, MAKEINTRESOURCE(1), IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
  // Bail if function failed
  if(!App.Window.Class.hIcon) Exception(__LINE__, GetLastError(), "Failed to load application icon from %s", App.Process.FullPath);
  // Setup small icon for window
  App.Window.Class.hIconSm = (HICON)LoadImage(App.Window.Class.hInstance, MAKEINTRESOURCE(1), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
  // Bail if function failed
  if(!App.Window.Class.hIconSm) Exception(__LINE__, GetLastError(), "Failed to load application small icon from %s", App.Process.FullPath);
  // Register the class
  if(!RegisterClassEx(&App.Window.Class)) Exception(__LINE__, GetLastError(), "Failed to register window class");
  // Create the window in the centre
  App.Window.Handle = CreateWindowEx(WS_EX_APPWINDOW, App.Window.Class.lpszClassName, App.Window.Class.lpszClassName, WS_POPUP, 0, 0, 1, 1, 0, 0, App.Window.Class.hInstance, 0);
  // Bail if function failed
  if(!App.Window.Handle) Exception(__LINE__, GetLastError(), "Failed to create window handle");
  // Set window title
  SetWindowTextF(App.Window.Handle, "Diggers %u.%02u compiled %s", App.Version.Major, App.Version.Minor, App.Version.Date);

  // Load settings
  Config(CFG_LOAD);

  // Process command line arguments
  ProcessCommandLineParameters();

  // Initialise windows networking library
  InitWinSock();

  // Initialise OLE for Direct Show
  HRESULT Result = CoInitialize(NULL);
  if(Result) Exception(__LINE__, Result, "Failed to initialise the COM library");

  // Initialise Direct Draw
  InitDirectDraw();
  // Initialise Direct Input
  InitDirectInput();
  // Initialise Direct Sound
  InitDirectSound();

  // Load base sound effects
  LoadBaseSoundEffects();

  // Initialise context for entire level mask
  App.Mask.LevelSurface = CreateCompatibleDC(0);
  // Bail if function failed
  if(!App.Mask.LevelSurface) Exception(__LINE__, GetLastError(), "Failed to create device context for terrain mask");
  // Crate monochrome bitmap to attach to context
  App.Mask.LevelSurfaceBmp = CreateBitmap(LEVEL_PWIDTH, LEVEL_PHEIGHT, 1, 1, 0);
  // Bail if function failed
  if(!App.Mask.LevelSurfaceBmp) Exception(__LINE__, GetLastError(), "Failed to create %ux%u mono bitmap for terrain mask", LEVEL_PWIDTH, LEVEL_PHEIGHT);
  // Attach new bitmap to new context
  if(SelectObject(App.Mask.LevelSurface, App.Mask.LevelSurfaceBmp) == HGDI_ERROR) Exception(__LINE__, GetLastError(), "Failed to select bitmap into surface for terrain mask");
  // Load mask tileset bitmap
  App.Mask.TilesetBmp = (HBITMAP)LoadImage(App.Window.Class.hInstance, MAKEINTRESOURCE(1), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
  // Initialise context for tileset masks
  App.Mask.Tileset = CreateCompatibleDC(0);
  // Bail if function failed
  if(!App.Mask.Tileset) Exception(__LINE__, GetLastError(), "Failed to create device context for masks tileset");
  // Select masks bitmap into context
  if(SelectObject(App.Mask.Tileset, App.Mask.TilesetBmp) == HGDI_ERROR) Exception(__LINE__, GetLastError(), "Failed to select bitmap into surface for masks tileset");

  // Initialise context for single tile mask
  App.Mask.Tile = CreateCompatibleDC(0);
  // Bail if function failed
  if(!App.Mask.Tile) Exception(__LINE__, GetLastError(), "Failed to create device context for tile mask");
  // Create monochrome bitmap for single tile mask
  App.Mask.TileBmp = CreateBitmap(TILE_SIZE, TILE_SIZE, 1, 1, 0);
  // Bail if function failed
  if(!App.Mask.TileBmp) Exception(__LINE__, GetLastError(), "Failed to create bitmap for tile mask surface");
  // Select new bitmap into context
  if(SelectObject(App.Mask.Tile, App.Mask.TileBmp) == HGDI_ERROR) Exception(__LINE__, GetLastError(), "Failed to select bitmap into surface for tile mask");

  // Allocate bitmap info structure
  App.Mask.TileBmpInfo = (LPBITMAPINFO)new CHAR[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];
  // Initialise bitmap info structure
  App.Mask.TileBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  // Fill bitmap info structure
  if(!GetDIBits(App.Mask.Tile, App.Mask.TileBmp, 0, 0, 0, App.Mask.TileBmpInfo, DIB_RGB_COLORS)) Exception(__LINE__, GetLastError(), "Failed to fill mask tile bitmap info structure");
}
/* --------------------------------------------------------------------------------- */
/* Handle exceptions                                                                 */
/* --------------------------------------------------------------------------------- */
LONG WINAPI ExceptionHandler(PEXCEPTION_POINTERS Pointers)
{
  // Create time structure and get current time
  SYSTEMTIME Time;
  GetLocalTime(&Time);
  // Create temp buffer and get and trim path to executable
  CHAR Temp[1024];
  GetModuleFileName(NULL, Temp, 255);
  Temp[lstrlen(Temp)-4] = '\0';
  // Create and format final path name of dump file name
  CHAR DumpName[1024];
  wsprintf(DumpName, "%s-%04u%02u%02u-%02u%02u%02u%03u.dmp", Temp, Time.wYear, Time.wMonth, Time.wDay, Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);
  // Create the minidump file
  HANDLE DumpFile = CreateFile(DumpName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_WRITE|FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
  // If it succeeded
  if(DumpFile != INVALID_HANDLE_VALUE)
  {
    // Create and initialise minidump parameters structure
    MINIDUMP_EXCEPTION_INFORMATION ExpParam;
    ExpParam.ThreadId = GetCurrentThreadId();
    ExpParam.ExceptionPointers = Pointers;
    ExpParam.ClientPointers = TRUE;
    // Write to the minidump and close the file
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), DumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);
    CloseHandle(DumpFile);
  }
  // Create pointer to the name of the exception and the exception record
  PCHAR ExpName;
  PEXCEPTION_RECORD ExpRecord = Pointers->ExceptionRecord;
  // Compare exception code and assign a readable string
  switch(ExpRecord->ExceptionCode)
  {
    case EXCEPTION_ACCESS_VIOLATION        : ExpName = "Access Violation"; break;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED   : ExpName = "Array Bounds Exceeded"; break;
    case EXCEPTION_BREAKPOINT              : ExpName = "Breakpoint"; break;
    case EXCEPTION_DATATYPE_MISALIGNMENT   : ExpName = "Data Type Misalignment"; break;
    case EXCEPTION_FLT_DENORMAL_OPERAND    : ExpName = "Float Denormal Operand"; break;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO      : ExpName = "Float Division by Zero"; break;
    case EXCEPTION_FLT_INEXACT_RESULT      : ExpName = "Float InExact Result"; break;
    case EXCEPTION_FLT_INVALID_OPERATION   : ExpName = "Float Invalid Operation"; break;
    case EXCEPTION_FLT_OVERFLOW            : ExpName = "Float Overflow"; break;
    case EXCEPTION_FLT_STACK_CHECK         : ExpName = "Float Stack Check"; break;
    case EXCEPTION_FLT_UNDERFLOW           : ExpName = "Float Underflow"; break;
    case EXCEPTION_ILLEGAL_INSTRUCTION     : ExpName = "Illegal Instruction"; break;
    case EXCEPTION_IN_PAGE_ERROR           : ExpName = "In Page"; break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO      : ExpName = "Integer Division by Zero"; break;
    case EXCEPTION_INT_OVERFLOW            : ExpName = "Integer Overflow"; break;
    case EXCEPTION_INVALID_DISPOSITION     : ExpName = "Invalid Disposition"; break;
    case EXCEPTION_NONCONTINUABLE_EXCEPTION: ExpName = "Non-continuable"; break;
    case EXCEPTION_PRIV_INSTRUCTION        : ExpName = "Priveledged Instruction"; break;
    case EXCEPTION_SINGLE_STEP             : ExpName = "Single Step"; break;
    case EXCEPTION_STACK_OVERFLOW          : ExpName = "Stack Overflow"; break;
    default                                : ExpName = "Unknown"; break;
  }
  // If there are more then 2 parameters in the list
  if(ExpRecord->NumberParameters >= 2)
  {
    // Create pointer to memory access type string
    PCHAR Method;
    // Compare access parameter and assign readable string
    switch(ExpRecord->ExceptionInformation[0])
    {
      case 0 : Method = "read"; break;
      case 1 : Method = "written"; break;
      case 8 : Method = "executed"; break;
      default: Method = "unknown"; break;
    }
    // Format message buffer and reuse previous variable
    wsprintf(Temp, "%s (0x%X) occured at 0x%p. The memory at 0x%p could not be %s.", ExpName, ExpRecord->ExceptionCode, ExpRecord->ExceptionAddress, ExpRecord->ExceptionInformation[1], Method);
  }
  // Format message buffer and reuse previous variable
  else wsprintf(Temp, "%s (0x%X) occured at 0x%p.", ExpName, ExpRecord->ExceptionCode, ExpRecord->ExceptionAddress);
  // Display message to user
  MessageBox(NULL, Temp, "Critical Error", MB_ICONSTOP|MB_APPLMODAL);
  // Kill application
  return EXCEPTION_EXECUTE_HANDLER;
}
/* --------------------------------------------------------------------------------- */
/* Get version info data                                                             */
/* --------------------------------------------------------------------------------- */
VOID GetVersionData(PCHAR Filename)
{
  // Create handle
  DWORD Handle;
  // Get size of file version data
  DWORD InfoSize = GetFileVersionInfoSize(Filename, &Handle);
  // Failed? Bail
  if(!InfoSize)
  {
    // No resource section
    if(GetLastError() == 1813) Exception(__LINE__, GetLastError(), "The executable %s does not contain a resource section which contains all the data needed by this program to run and cannot be run without it. Please acquire a fresh copy of the executable and try again", Filename);
    // Other error
    else Exception(__LINE__, GetLastError(), "Failed to query version info data size from %s", Filename);
  }
  // Allocate buffer
  PVOID Buffer = (PVOID)new CHAR[InfoSize];
  // Failed?
  if(!Buffer) Exception(__LINE__, GetLastError(), "Failed to allocate %u bytes for version info data buffer", InfoSize);
  // Get version info data
  if(!GetFileVersionInfo(Filename, Handle, InfoSize, Buffer))
  {
    // Free buffer
    delete []Buffer;
    // Error
    Exception(__LINE__, GetLastError(), "Failed to read version information from %s", Filename);
  }
  // Create file info structure
  VS_FIXEDFILEINFO *FixedInfo;
  // Create storage for length
  UINT FixedInfoLen;
  // Get version data
  if(!VerQueryValue(Buffer, "\\", (PVOID*)&FixedInfo, (UINT *)&FixedInfoLen))
  {
    // Free buffer
    delete []Buffer;
    // Error
    Exception(__LINE__, GetLastError(), "Failed to query value information from version info buffer");
  }
  // Free buffer
  delete []Buffer;
  // Set major version of application
  App.Version.Major = (UCHAR)HIWORD(FixedInfo->dwFileVersionMS);
  // Set minor version of application
  App.Version.Minor = (UCHAR)LOWORD(FixedInfo->dwFileVersionMS);
}
/* --------------------------------------------------------------------------------- */
/* Set formatted window title                                                        */
/* --------------------------------------------------------------------------------- */
BOOL SetWindowTextF(HWND Window, PCHAR Format, ...)
{
  // Create buffer to configuration filename
  CHAR Title[1024];
  // Format buffer
  if(wvsprintf(Title, Format, (PCHAR)(&Format + 1)) <= 0)
    // Nothing written
    return FALSE;
  // Set the title
  return SetWindowText(Window, Title);
}
/* --------------------------------------------------------------------------------- */
/* Process command line arguments                                                    */
/* --------------------------------------------------------------------------------- */
VOID ProcessCommandLineParameters(VOID)
{
  PCHAR *Arguments = NULL;    // Arguments array (argv)
  PCHAR  Argument;            // Current argument being processed
  INT    ArgumentCount = 0;   // Number of arguments in Arguments array (argc)
  PCHAR  ArgumentPointer;     // Pointer to command line
  PCHAR *ArgumentsPointer;    // Pointer to arguments array
  BOOL   InQuotation = FALSE; // In quotations?

  // Retreive command line
  Argument = ArgumentPointer = GetCommandLine();
  // Bail if function failed
  if(!Argument) Exception(__LINE__, GetLastError(), "Failed to retrieve command line");

  // Walk command line string
  for(;;)
  {
    // Character is a white-space
    if(*ArgumentPointer == 32)
    {
      // In quotation?
      if(InQuotation)
      {
        // Continue to next character
        ++ArgumentPointer;
        // Begin loop again
        continue;
      }
      // Out of quotation, make character null and move to next character
      *ArgumentPointer++ = 0;
      // Fall through and store argument
    }
    // Character is a quote?
    else if(*ArgumentPointer == 34)
    {
      // Make character null and move to next character
      *ArgumentPointer++ = 0;
      // Toggle 'In quotation' and if it is now 1?
      if((InQuotation ^= 1))
      {
        // Skip character
        ++Argument;
        // Begin loop again
        continue;
      }
      // Fall through and store argument
    }
    // Character is insignificant
    else if(*ArgumentPointer)
    {
      // Skip to next cahracter
      ++ArgumentPointer;
      // Begin loop again
      continue;
    }
    // Store argument
    if(ArgumentCount)
      // Not first argument
      Arguments = (PCHAR*)HeapReAlloc(App.Process.Heap, 0x4, Arguments, (ArgumentCount + 1) * App.Core.AddressLength);
    else
      // First argument
      Arguments = (PCHAR*)HeapAlloc(App.Process.Heap, 0xC, App.Core.AddressLength);
    // Store argument in array
    Arguments[ArgumentCount++] = Argument;
    // Walk through all the white-space characters
    while(*ArgumentPointer == 32)
      // Remove them and move to next character
      *ArgumentPointer++ = 0;
    // If there are no more characters to process?
    if(!*ArgumentPointer)
      // At end of command line so break loop
      break;
    // Update beginning of next argument
    Argument = ArgumentPointer;
  }
  // Create space for a new argument
  Arguments = (PCHAR*)HeapReAlloc(App.Process.Heap, 0x4, Arguments, (ArgumentCount + 1) * App.Core.AddressLength);
  // Make it null
  Arguments[ArgumentCount] = NULL;
  // Walk arguments array
  for(ArgumentsPointer = Arguments + 1; *ArgumentsPointer; ++ArgumentsPointer)
  {
    // First character not forward slash
    if(**ArgumentsPointer != 47)
      // Not valid switch character, so bail
      Exception(0, 0, "The switch character '%c' is unrecognised. Please use the '/' character to denote the beginning of an option", **ArgumentsPointer);
    // Compare character after slash
    switch(*(++*ArgumentsPointer))
    {
      // Debug mode enabled
      case 'D': App.Flags |= AFL_EDITOR; break;
      // Debug mode disabled
      case 'd': App.Flags &= ~AFL_EDITOR; break;
      // Enable sound effects
      case 'E': App.Flags &= ~AFL_NOSOUNDS; break;
      // Disable sound effects
      case 'e': App.Flags |= AFL_NOSOUNDS; break;
      // Fullscreen mode enabled
      case 'F': App.Flags &= ~AFL_WINDOW; break;
      // Fullscreen mode disabled
      case 'f': App.Flags |= AFL_WINDOW; break;
      // Go straight to in-game mode on the specified level
      case 'L':
      {
        // No number specified?
        if(!*(++*ArgumentsPointer))
          // Bail out
          Exception(0, 0, "Please specify the zone number to start from");
        // Get level number the user specified
        App.Game.NewLevel = StringToNumber(*ArgumentsPointer);
        // Make sure it is a valid level
        if(App.Game.NewLevel < 0 || App.Game.NewLevel >= LEVEL_NUM)
          // Invalid. Reset level to random
          Exception(0, 0, "The specified zone number to start from is invalid. Please specify in range of 0 to %u", LEVEL_NUM - 1);
        // Done
        break;
      }
      // Enable music
      case 'M': App.Flags &= ~AFL_NOMUSIC; break;
      // Disable music
      case 'm': App.Flags |= AFL_NOMUSIC; break;
      // Set race
      case 'R':
      {
        // No number specified?
        if(!*(++*ArgumentsPointer))
          // Bail out
          Exception(0, 0, "Please specify the race id to start with");
        // Get level number the user specified
        App.Game.Player.ThisRace = (OBJTYP)StringToNumber(*ArgumentsPointer);
        // Make sure it is a valid level
        if(App.Game.Player.ThisRace < 0 || App.Game.Player.ThisRace >= NUM_RACES)
          // Invalid. Reset level to random
          Exception(0, 0, "The specified race id is invalid. Please specify in range of 0 to %u", NUM_RACES - 1);
        // Done
        break;
      }
      // Enable streaming
      case 'S': App.Flags &= ~AFL_NOSTREAMING; break;
      // Disable streaming
      case 's': App.Flags |= AFL_NOSTREAMING; break;
      // Specify new video mode
      case 'V':
      {
        // Number not specified
        if(!*(++*ArgumentsPointer))
          // Bail out
          Exception(0, 0, "Please specify the video resolution number");
        // Get number
        App.Video.ActiveModeId = (UCHAR)StringToNumber(*ArgumentsPointer);
        // Is valid?
        if(App.Video.ActiveModeId >= 4)
          // Bail out
          Exception(0, 0, "The specified video resolution number is invalid. Please specify in range of 0 to 4");
        // Done
        break;
      }
      // Enable enhanced sound mixing
      case 'X': App.Flags &= ~AFL_NOENMIXSND; break;
      // Disable enhanced sound mixing
      case 'x': App.Flags |= AFL_NOENMIXSND; break;
      // Version
      case '!': Exception(0, 0, "Diggers™ for Windows® 9x, NT, ME, 2K, XP and Vista.\n"
                                "Version %u.%u (%u-bit); %s; MSVC %u.%u.\n"
                                "Copyright © MS-Design, 2011.\n"
                                "Copyright © Millennium Interactive, 1994.\n"
                                "\n"
                                "UFMOD; Copyright © Asterix and Quantum, 2011.\n"
                                "SCALEX; Copyright © Andrea Mazzoleni, 2011.\n"
                                "DX&MEDIA; Copyright © Microsoft Corporation, 2011",
        App.Version.Major, App.Version.Minor, App.Version.Bits, App.Version.Date, _MSC_VER/100, _MSC_VER%100);
      // Help
      case '?': Exception(0, 0, "Usage: DIGGERS.EXE [Parameters].\n"
                                "\n"
                                "/D\tEnable debug mode.\t/d\tDisable debug mode.\t\n"
                                "/E\tEnable sound effects.\t/e\tDisable sound effects.\n"
                                "/Ln\tStart on zone number n.\n"
                                "/M\tEnable music.\t\t/m\tDisable music.\n"
                                "/Rn\tStart with race number n.\n"
                                "/S\tEnable streaming.\t\t/s\tDisable streaming.\n"
                                "/Vn\tUse alternate video mode n.\n"
                                "/X\tEnable sound mixing.\t/x\tDisable sound mixing.\n"
                                "/?\tShow this list of parameters\t/!\tDisplay version information");
      // Bail on un-supported or invalid parameters
      default : Exception(0, 0, "The parameter '%c' is unrecognised. Please use '/?' to display the usage", **ArgumentsPointer);
    }
  }
  // Done with arguments array
  HeapFree(App.Process.Heap, 0x4, Arguments);
}
/* --------------------------------------------------------------------------------- */
/* Load game settings                                                                */
/* --------------------------------------------------------------------------------- */
VOID Config(CFGCMD Command)
{
  // Create structure for settings
  struct
  {
    #pragma pack(push, 1)
    ULONG Magic;
    UCHAR Flags;
    UCHAR ActiveMode;
    UCHAR Limiter;
    #pragma pack(pop)
  }
  Settings;
  // Zero settings
  ZeroMemory(&Settings, sizeof(Settings));
  // Create buffer to configuration filename
  CHAR ConfigFile[1024];
  // Set filename
  wsprintf(ConfigFile, "%sdiggers.dat", App.Process.Path);
  // Create handle to config file
  HANDLE ConfigHandle = INVALID_HANDLE_VALUE;
  // Delete config bool
  BOOL ConfigDelete = FALSE;
  // Compare command
  switch(Command)
  {
    // Save settings?
    case CFG_SAVE:
      // Create file
      ConfigHandle = CreateFile(ConfigFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
      // Create failed?
      if(ConfigHandle == INVALID_HANDLE_VALUE)
        // Not fatal so just ignore
        break;
      // Set magic
      Settings.Magic = 0x10474643;
      // Set flags
      Settings.Flags = App.Flags;
      // Set mode
      Settings.ActiveMode = App.Video.ActiveModeId;
      // Set limiter
      Settings.Limiter = App.Timer.Limiter;
      // Create variable for bytes written
      ULONG BytesWritten;
      // Write settings structure. Delete file if failed
      if(!WriteFile(ConfigHandle, &Settings, sizeof(Settings), &BytesWritten, NULL) || BytesWritten != sizeof(Settings)) ConfigDelete = TRUE;
      // Done
      break;
    // Load settings?
    case CFG_LOAD:
      // Open file
      ConfigHandle = CreateFile(ConfigFile, GENERIC_READ, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
      // Succeeded?
      if(ConfigHandle != INVALID_HANDLE_VALUE)
      {
        // Get file size
        ULONG ConfigSize = GetFileSize(ConfigHandle, NULL);
        // Create var for bytes read
        ULONG BytesRead = 0;
        // Configuration file read properly?
        if(ConfigSize == sizeof(Settings) && ReadFile(ConfigHandle, &Settings, sizeof(Settings), &BytesRead, NULL) || BytesRead == sizeof(Settings) || Settings.Magic == 0x10474643)
        {
          // Set flags
          App.Flags = Settings.Flags;
          // Set mode
          App.Video.ActiveModeId = Settings.ActiveMode;
          // Set limiter
          App.Timer.Limiter = Settings.Limiter;
          // Done
          break;
        }
      }
      // Open failed or config corrupt so use default settings
      ConfigDelete = TRUE;
    // Default settings?
    case CFG_DEFAULT:
      // Default 640x480x8
      App.Video.ActiveModeId = 1;
      // Default frame limit
      App.Timer.Limiter = TIMER_FRAMELIMIT_FAST;
      // Default window mode
      App.Flags = AFL_WINDOW;
      // Done
      break;
  }
  // Config file opened? Close it
  if(ConfigHandle != INVALID_HANDLE_VALUE) CloseHandle(ConfigHandle);
  // Want to delete the file? Delete it
  if(ConfigDelete == TRUE) DeleteFile(ConfigFile);
}
/* --------------------------------------------------------------------------------- */
/* De-Initialise the application                                                     */
/* --------------------------------------------------------------------------------- */
VOID DeInitialise(VOID)
{
  // Unload level data if loaded
  DeInitLevel();

  // Delete mask tileset bitmap
  if(App.Mask.TilesetBmp) { DeleteObject(App.Mask.TilesetBmp); App.Mask.TilesetBmp = NULL; }
  // Delete mask tileset surface
  if(App.Mask.Tileset) { DeleteDC(App.Mask.Tileset); App.Mask.Tileset = NULL; }
  // Delete mask tile surface
  if(App.Mask.Tile) { DeleteDC(App.Mask.Tile); App.Mask.Tile = NULL; }
  // Delete mask tile bitmap
  if(App.Mask.TileBmp) { DeleteObject(App.Mask.TileBmp); App.Mask.TileBmp = NULL; }
  // Delete mask tile bitmap info structure
  if(App.Mask.TileBmpInfo) { delete []App.Mask.TileBmpInfo; App.Mask.TileBmpInfo = NULL; }
  // Delete level mask surface
  if(App.Mask.LevelSurface) { DeleteDC(App.Mask.LevelSurface); App.Mask.LevelSurface = NULL; }
  // Delete level mask surface bitmap
  if(App.Mask.LevelSurfaceBmp) { DeleteObject(App.Mask.LevelSurfaceBmp); App.Mask.LevelSurfaceBmp = NULL; }

  // Unload all sounds
  UnloadSounds();
  // Unload all textures
  UnloadTextures();
  // Stop and unload music
  MusicControl(MUSICCMD_STOP, MUSIC_NULL);

  // DeInitialise DirectSound
  DeInitDirectSound();
  // DeInitialise DirectInput
  DeInitDirectInput();
  // DeInitialise DirectDraw
  DeInitDirectDraw();

  // DeInitialise the windows sockets library
  DeInitWinSock();

  // DeInitialise the OLE library
  CoUninitialize();

  // Save settings
  Config(CFG_SAVE);

  // Destroy window
  if(App.Window.Handle) DestroyWindow(App.Window.Handle);
  // Unregister window class
  UnregisterClass(App.Window.Class.lpszClassName, App.Window.Class.hInstance);

  // Done with path name to exe
  HeapFree(App.Process.Heap, 0x4, App.Process.Path);
  HeapFree(App.Process.Heap, 0x4, App.Process.FullPath);
  // Unload process mutex
  if(App.Process.Mutex) ReleaseMutex(App.Process.Mutex);
}
/* --------------------------------------------------------------------------------- */
/* Convert a string to an integer                                                    */
/* --------------------------------------------------------------------------------- */
INT StringToNumber(PCHAR String)
{
  INT I, N, S;

  for(I = 0; I == 32; ++I);

  S = (String[I] == '-') ? -1 : 1;

  if(String[I] == '+' || String[I] == '-')
    ++I;

  for(N = 0; String[I] >= '0' && String[I] <= '9'; ++I)
    N = 10 * N + (String[I] - '0');

  return S * N;
}
/* --------------------------------------------------------------------------------- */
/* Show error information and terminate process                                      */
/* --------------------------------------------------------------------------------- */
VOID Exception(INT Line, INT Code, PCHAR Format, ...)
{
  // Create format buffer for wvsprintf
  CHAR FormatBuffer[1024];
  // Format arguments to format buffer
  wvsprintf(FormatBuffer, Format, (PCHAR)(&Format + 1));
  // Create final buffer for wsprintf/messagebox
  CHAR FinalBuffer[1024];
  // Format final buffer
  if(Line)
    // Show error information if line specified
    wsprintf(FinalBuffer, "Error %u-%x-%u: %s.", Code, Code, Line, FormatBuffer);
  else
    // Treat as a PrintF version of MessageBox
    wsprintf(FinalBuffer, "%s.", FormatBuffer);
  // Show final buffer to user
  MessageBox(App.Window.Handle, FinalBuffer, App.Window.Class.lpszClassName, (Line ? MB_ICONEXCLAMATION : MB_ICONINFORMATION) | MB_APPLMODAL);
  // De-initialise game
  DeInitialise();
  // Terminate process
  ExitProcess(Line);
}
/* --------------------------------------------------------------------------------- */
/* Window callback procedure                                                         */
/* --------------------------------------------------------------------------------- */
LRESULT CALLBACK WindowCallback(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam)
{
  // Which message?
  switch(Message)
  {
    // Window about to be resized
    case WM_GETMINMAXINFO:
    {
      // Not in window mode
      if(~App.Flags & AFL_WINDOW)
        // Ignore
        break;
      // Get address of data structure
      PMINMAXINFO Info = (PMINMAXINFO)LParam;
      // Set minimum bounds
      Info->ptMinTrackSize.x = ASPECT_WIDTH;
      Info->ptMinTrackSize.y = ASPECT_HEIGHT;
      // Done
      break;
    }
    // Window moved
    case WM_MOVE:
    // Window resized
    case WM_SIZE:
    {
      // Not in window mode
      if(~App.Flags & AFL_WINDOW)
        // Ignore
        break;
      // Initialise bounds
      ZeroMemory(&App.Window.SourcePoint, sizeof(App.Window.SourcePoint));
      // Get screen co-ordinates of window
      ClientToScreen(App.Window.Handle, &App.Window.SourcePoint);
      // Get size of window
      GetClientRect(App.Window.Handle, &App.Window.Destination);
      // Calcuate offset of window
      OffsetRect(&App.Window.Destination, App.Window.SourcePoint.x, App.Window.SourcePoint.y);
      // Video mode set?
      if(App.Video.ActiveMode)
        // Prepare screen size for WinSrc
        SetRect(&App.Window.Source, 0, 0, App.Video.ActiveMode->Width, App.Video.ActiveMode->Height);
      // Done
      break;
    }
    // System key pressed
    case WM_SYSKEYDOWN:
    {
      // In this we need to disable some of the standard windows shortcuts so
      // Diggers can use these keys if need be and to prevent the player from
      // pressing a key that does something to the window by mistake.

      // Compare virtual code
      switch(WParam)
      {
        // System menu key pressed -- Prevents system menu when FS+Window change
        case VK_F10:
        // Left alt key pressed -- Prevents system menu via alt+space
        case VK_LMENU:
        // Right alt key pressed -- Prevents system menu via alt+space
        case VK_RMENU:
        // Space key pressed -- Prevents system menu via alt+space
        case VK_SPACE: return 0;
        // Allow other keys
        default: break;
      }
      // Done
      break;
    }
    // Window focus restored
    case WM_SETFOCUS:
    {
      // Enable high timer resolution
      timeBeginPeriod(1);
      // Enable game tick
      App.Timer.Function = (GAMEFUNC)ActiveWindowProc;
      // Resume music if stopped and isnt disabled
      if(~App.Flags & AFL_NOMUSIC) MusicControl(MUSICCMD_RESUME, MUSIC_NULL);
      // High priority for process bloated machines
      SetPriorityClass(App.Process.Handle, HIGH_PRIORITY_CLASS);
      // Done
      break;
    }
    // Window focus lost
    case WM_KILLFOCUS:
    {
      // Disable high timer resolution
      timeEndPeriod(1);
      // Disable game tick
      App.Timer.Function = (GAMEFUNC)InactiveWindowProc;
      // Pause music if playing
      MusicControl(MUSICCMD_PAUSE, MUSIC_NULL);
      // Low priority so this app uses as least cpu cycles as possible
      SetPriorityClass(App.Process.Handle, IDLE_PRIORITY_CLASS);
      // Done
      break;
    }
    // Window close requested
    case WM_DESTROY:
    {
      // Nullify window handle
      App.Window.Handle = NULL;
      // Send WM_QUIT to main loop
      PostQuitMessage(0);
      // Done
      break;
    }
    // Resolution changed
    case WM_DISPLAYCHANGE:
    {

      // Update desktop size variables
      App.Video.DesktopInfo.dmPelsWidth = LOWORD(LParam);
      App.Video.DesktopInfo.dmPelsHeight = HIWORD(LParam);
      // Update desktop depth variable
      App.Video.DesktopInfo.dmBitsPerPel = (DWORD)WParam;
      // Done
      break;
    }
    // Streaming media message
    case WM_USER:
    {
      // Music not playing or not streaming?
      if(App.Audio.Music.Type == MUSICTYPE_NULL || !App.Audio.Music.MediaEvent)
         // Done
         break;
      // Get events
      while(App.Audio.Music.MediaEvent->GetEvent((PLONG)&Message, (PLONG_PTR)&WParam, (PLONG_PTR)&LParam, 0) != E_ABORT)
      {
        // Which event?
        switch(Message)
        {
          // All data from a particular stream has been rendered
          case EC_COMPLETE:
          {
            // Begin playing again from the beginning
            App.Audio.Music.MediaPosition->put_CurrentPosition(0);
            // Done
            break;
          }
          // Ignore everything else
          default: break;
        }
        // MS says call this after each even is processed
        App.Audio.Music.MediaEvent->FreeEventParams(Message, WParam, LParam);
      }
      // Done
      break;
    }
    // Repaint window requested
    case WM_PAINT:
    {
      // In window mode and render function set
      if(App.Flags & AFL_WINDOW && App.Video.Function)
        // Re-render
        App.Video.Function();
      // Done
      return DefWindowProc(Handle, Message, WParam, LParam);
    }
    // System command
    case WM_SYSCOMMAND:
    {
      // Which command?
      switch(WParam)
      {
        // Windows requests screen saver activation?
        case SC_SCREENSAVE:
          // Fall through
        // Windows requests monitor switch off?
        case SC_MONITORPOWER:
          // Disallow
          return 0;
      }
      // Fall through
    }
    // For every other event return a default response
    default: return DefWindowProc(Handle, Message, WParam, LParam);
  }
  // Event handled
  return 0;
}
/* --------------------------------------------------------------------------------- */
/* LOAD FILE FROM EXECUTABLE                                                         */
/* --------------------------------------------------------------------------------- */
PVOID LoadFile(INT ResourceId, PCHAR Group, PULONG Size)
{
  // Find resource in executable
  HRSRC ResFind = FindResource(App.Window.Class.hInstance, MAKEINTRESOURCE(ResourceId + 1), Group);
  // Make sure function call was successful
  if(!ResFind) Exception(__LINE__, GetLastError(), "Failed to find resource %s:%u from %s", Group, ResourceId, App.Process.FullPath);
  // Return size of resource
  *Size = SizeofResource(App.Window.Class.hInstance, ResFind);
  // Make sure function call was successful
  if(!*Size) Exception(__LINE__, GetLastError(), "Failed to query size of resource %s:%u from %s", Group, ResourceId, App.Process.FullPath);
  // Load resource
  HRSRC ResLoad = (HRSRC)LoadResource(App.Window.Class.hInstance, ResFind);
  // Make sure function call was successful
  if(!ResFind) Exception(__LINE__, GetLastError(), "Failed to load resource %s:%u from %s", Group, ResourceId, App.Process.FullPath);
  // Return pointer to resource
  return LockResource(ResLoad);
}
/* --------------------------------------------------------------------------------- */
/* RANDOM NUMBER GENERATOR FUNCTIONS                                                 */
/* --------------------------------------------------------------------------------- */
USHORT Random(VOID)
{
  App.Core.RandomSeed = App.Core.RandomSeed * 1103515245 + 12345;

  return ((USHORT)(App.Core.RandomSeed / 65536) % 32767);
}
/* ================================================================================= */
/* ################################################################################# */
/* ################################################################################# */
/* ###                                                                           ### */
/* ###   D I R E C T  X  F U N C T I O N S                                       ### */
/* ###                                                                           ### */
/* ################################################################################# */
/* ################################################################################# */
/* ================================================================================= */
/* DirectX function wrapper to trap error messages                                   */
/* --------------------------------------------------------------------------------- */
VOID DX(INT Line, HRESULT Result)
{
  // Compare result
  switch(Result)
  {
    // Fuction returned no error
    case S_OK:
    {
      // So return
      return;
    }
    // A Direct Draw surface was lost
    case DDERR_SURFACELOST:
    {
      // Restore secondary surface
      App.Video.SecondarySurface->Restore();
      // Restore primary surface
      App.Video.PrimarySurface->Restore();
      // Restore all texture and font surfaces and re-copy all bitmaps to them
      UploadTextures(TRUE);
      // Do not report error
      return;
    }
    // Input was lost on a Direct Input device
    case DIERR_INPUTLOST:
    // or a Direct Input device isnt acquired
    case DIERR_NOTACQUIRED:
    {
      // (Re)Acquire keyboard device
      App.Input.Keyb.Device->Acquire();
      // (Re)Acquire mouse device
      App.Input.Mouse.Device->Acquire();
      // Do not report error
      return;
    }
    // DirectSound buffer was lost for some reason
    case DSERR_BUFFERLOST:
    {
      // Restore all sound effects
      UploadSounds(TRUE);
      // Do not report error
      return;
    }
  }
  // Bail out and show user an error. We probably would only get here for two reasons
  // Either the user doesnt have compatible hardware or there is a bug in our code
  Exception(Line, Result, "Directx function call failed");
}
/* --------------------------------------------------------------------------------- */
/* Initialise DirectDraw                                                             */
/* --------------------------------------------------------------------------------- */
VOID InitDirectDraw(VOID)
{
  // First we need to test the video resolutions the game uses to see if the clients'
  // machine can use these video resolutions.

  // If active mode id is invalid? Reset
  if(App.Video.ActiveModeId >= 4) App.Video.ActiveModeId = 0;
  // Get video mode from the active mode id
  App.Video.ActiveMode = &ModeData[App.Video.ActiveModeId];

  // Save current resolution
  struct MODEDATA *CurrentMode = App.Video.ActiveMode;

  // Initialise size of DEVMODE structure
  App.Video.DesktopInfo.dmSize = sizeof(App.Video.DesktopInfo);
  // Initialise display frequency
  App.Video.DesktopInfo.dmDisplayFrequency = 60;
  // Initialise flags for DEVMODE structure
  App.Video.DesktopInfo.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

  // Walk each mode and test and keep testing until we find a valid mode
  do
  {
    // End of mode list?
    if(!App.Video.ActiveMode->TexScale)
    {
      // First id
      App.Video.ActiveModeId = 0;
      // Back to first mode
      App.Video.ActiveMode = ModeData;
      // Try again
      continue;
    }
    // Get and set width
    App.Video.DesktopInfo.dmPelsWidth = App.Video.ActiveMode->Width;
    // Get and set height
    App.Video.DesktopInfo.dmPelsHeight = App.Video.ActiveMode->Height;
    // Get and set depth
    App.Video.DesktopInfo.dmBitsPerPel = App.Video.ActiveMode->Depth;
    // In window mode or mode available?
    if(App.Flags & AFL_WINDOW || ChangeDisplaySettings(&App.Video.DesktopInfo, CDS_TEST) == DISP_CHANGE_SUCCESSFUL)
    {
      // Nullify CurrentMode. It tells the code after the loop we found a match
      CurrentMode = NULL;
      // Use mode
      break;
    }
    // Set next id
    ++App.Video.ActiveModeId;
    // Try next mode
    *++App.Video.ActiveMode;
  }
  while(App.Video.ActiveMode != CurrentMode);
  // Mode not selected?
  if(CurrentMode)
    // Force window mode
    App.Flags |= AFL_WINDOW;

  // Create direct sound object
  HRESULT Result = CoCreateInstance(CLSID_DirectDraw, NULL, CLSCTX_ALL, IID_IDirectDraw, (PVOID*)&App.Video.Device);
  if(Result) Exception(__LINE__, Result, "Failed to create direct draw object");
  // Initialise direct sound object
  Result = App.Video.Device->Initialize(NULL);
  if(Result) Exception(__LINE__, Result, "Failed to initialise direct draw object");
  // Initialise capabilities structures
  App.Video.Capabilities.dwSize = sizeof(App.Video.Capabilities);
  App.Video.HelCapabilities.dwSize = sizeof(App.Video.HelCapabilities);
  // Get device capabilities
  Result = App.Video.Device->GetCaps(&App.Video.Capabilities, &App.Video.HelCapabilities);
  if(Result) Exception(__LINE__, Result, "Failed to query direct draw device capabilities");

  // Create surface descriptor
  DDSURFACEDESC SurfaceDesc;
  // Initialise surface description structure
  ZeroMemory(&SurfaceDesc, sizeof(SurfaceDesc));
  // Set size of surface description structure
  SurfaceDesc.dwSize = sizeof(SurfaceDesc);

  // Window mode requested?
  if(App.Flags & AFL_WINDOW)
  {
    // Get information about the current desktop resolution because we
    // we need to know how to handle the palette when the game is in window mode. We
    // also want the info so we can put the window in the middle of the screen.

    // Get display device information
    if(!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &App.Video.DesktopInfo))
      Exception(__LINE__, GetLastError(), "Unable to retrieve information about the current screen resolution");

    // DO NOT run on 2 or 4 BPP modes
    if(App.Video.DesktopInfo.dmBitsPerPel < 8)
      Exception(__LINE__, GetLastError(), "Your current desktop bit-depth of %u-bit is not supported in this application. Please switch to a 8, 15, 16 or 32 bits depth", App.Video.DesktopInfo.dmBitsPerPel);

    // Attach direct draw object to window
    Result = App.Video.Device->SetCooperativeLevel(App.Window.Handle, DDSCL_NORMAL);
    if(Result) Exception(__LINE__, Result, "Failed to set display co-operative level for window mode");

    // Initialise surface description structure
    SurfaceDesc.dwFlags = DDSD_CAPS;
    SurfaceDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    // Create primary and secondary surfaces
    Result = App.Video.Device->CreateSurface(&SurfaceDesc, &App.Video.PrimarySurface, 0);
    if(Result) Exception(__LINE__, Result, "Failed to create primary display surface for window mode");

    // Create a clipper to ensure that our drawing stays inside our window
    Result = App.Video.Device->CreateClipper(0, &App.Video.Clipper, 0);
    if(Result) Exception(__LINE__, Result, "Failed to create direct draw clipper for window mode");
    // Attach it to the window
    Result = App.Video.Clipper->SetHWnd(0, App.Window.Handle);
    if(Result) Exception(__LINE__, Result, "Failed to attach direct draw clipper to window");
    // Attach the clipper to the primary surface
    Result = App.Video.PrimarySurface->SetClipper(App.Video.Clipper);
    if(Result) Exception(__LINE__, Result, "Failed to attach direct draw clipper to primary surface");
    // Create secondary surface
    App.Video.SecondarySurface = CreateSurface(&Result, App.Video.ActiveMode->Width, App.Video.ActiveMode->Height);
    // Function failed?
    if(!App.Video.SecondarySurface) Exception(__LINE__, Result, "Failed to create secondary surface for window mode");

    // Create RECT for window position and scale
    RECT Window;
    // Create storage for new window EX style
    ULONG NewExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    // Create storage for new window style
    ULONG NewStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    // Fill data
    Window.left = Window.top = 0;
    Window.right = App.Video.ActiveMode->Width;
    Window.bottom = App.Video.ActiveMode->Height;

    // Set window extended properties
    SetWindowLongPtr(App.Window.Handle, GWL_EXSTYLE, NewExStyle);
    // Set window properties
    SetWindowLongPtr(App.Window.Handle, GWL_STYLE, NewStyle);
    // Update window data structure
    AdjustWindowRectEx(&Window, NewStyle, FALSE, NewExStyle);

    // Reuse Window structure and re-calculate size and position of window
    // so that the window is in the middle of the desktop
    Window.right -= Window.left;
    Window.bottom -= Window.top;
    Window.left = (App.Video.DesktopInfo.dmPelsWidth / 2) - (Window.right / 2);
    Window.top = (App.Video.DesktopInfo.dmPelsHeight / 2) - (Window.bottom / 2);
    // If X window position goes out of range
    if(Window.left < 0 || Window.left >= (LONG)App.Video.DesktopInfo.dmPelsWidth)
      // Clamp it
      Window.left = 0;
    // If Y window position goes out of range
    if(Window.top < 0 || Window.top >= (LONG)App.Video.DesktopInfo.dmPelsHeight)
      // Clamp it
      Window.top = 0;

    // Update window position and visibility
    SetWindowPos(App.Window.Handle, HWND_NOTOPMOST, Window.left, Window.top, Window.right, Window.bottom, SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    // Repaint all windows
    InvalidateRect(NULL, &Window, FALSE);

    // Set window flip function
    App.Video.Function = (GAMEFUNC)WindowFlip;
  }
  // Full-screen mode requested
  else
  {
    // Set window extended properties
    SetWindowLongPtr(App.Window.Handle, GWL_EXSTYLE, WS_EX_APPWINDOW);
    // Set window properties
    SetWindowLongPtr(App.Window.Handle, GWL_STYLE, WS_POPUP);
    // Update window position and visibility
    SetWindowPos(App.Window.Handle, HWND_NOTOPMOST, 0, 0, App.Video.ActiveMode->Width, App.Video.ActiveMode->Height, SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_HIDEWINDOW);

    // Attach direct draw object to window
    Result = App.Video.Device->SetCooperativeLevel(App.Window.Handle, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT);
    if(Result) Exception(__LINE__, Result, "Failed to set display co-operative level for full-screen mode");
    // Set display resolution to App.Video.ActiveMode->Width * App.Video.ActiveMode->Height * ASPECT_DEPTH
    Result = App.Video.Device->SetDisplayMode(App.Video.ActiveMode->Width, App.Video.ActiveMode->Height, App.Video.ActiveMode->Depth);
    if(Result) Exception(__LINE__, Result, "Failed to set display resolution to %ux%ux%u", App.Video.ActiveMode->Width, App.Video.ActiveMode->Height, App.Video.ActiveMode->Depth);
    // Wait for the display device to complete setting the display mode.
    // This fails on VirtualBox for iMac 12,2 and is not important if it fails so just ignore it.
    App.Video.Device->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);

    // Initialise surface description structure
    SurfaceDesc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    SurfaceDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    SurfaceDesc.dwBackBufferCount = 1;
    // Create primary and secondary surfaces
    Result = App.Video.Device->CreateSurface(&SurfaceDesc, &App.Video.PrimarySurface, 0);
    if(Result) Exception(__LINE__, Result, "Failed to create primary display surface for full-screen mode");
    // Initialise surface description capabilities structure
    SurfaceDesc.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
    // Get attached secondary surface
    Result = App.Video.PrimarySurface->GetAttachedSurface(&SurfaceDesc.ddsCaps, &App.Video.SecondarySurface);
    if(Result) Exception(__LINE__, Result, "Failed to retrieve secondary display surface");

    // Set fullscreen flip function
    App.Video.Function = (GAMEFUNC)FullScreenFlip;
  }

  // Full screen mode or desktop mode is 8bpp
  if((~App.Flags & AFL_WINDOW && App.Video.ActiveMode->Depth <= 8) || (App.Flags & AFL_WINDOW && App.Video.DesktopInfo.dmBitsPerPel <= 8))
  {
    // Create temporary storage
    PALETTEENTRY PAL[256];
    // Zero it
    ZeroMemory(&PAL, sizeof(PAL));
    // Create the palette
    Result = App.Video.Device->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, PAL, &App.Video.Palette, 0);
    if(Result) Exception(__LINE__, Result, "Failed to create palette");
    // Activate the palette
    Result = App.Video.PrimarySurface->SetPalette(App.Video.Palette);
    if(Result) Exception(__LINE__, Result, "Failed to activate palette");
  }
}
/* --------------------------------------------------------------------------------- */
/* Initialise DirectInput                                                            */
/* --------------------------------------------------------------------------------- */
VOID InitDirectInput(VOID)
{
  // Create direct input object
  HRESULT Result = CoCreateInstance(CLSID_DirectInput, NULL, CLSCTX_ALL, IID_IDirectInput, (PVOID*)&App.Input.Device);
  if(Result) Exception(__LINE__, Result, "Failed to create direct input object");
  // Initialise direct input object
  Result = App.Input.Device->Initialize(App.Window.Class.hInstance, DIRECTINPUT_VERSION);
  if(Result) Exception(__LINE__, Result, "Failed to initialise direct input object");
  // Create keyboard device
  Result = App.Input.Device->CreateDevice(GUID_SysKeyboard, &App.Input.Keyb.Device, 0);
  if(Result) Exception(__LINE__, Result, "Failed to create keyboard device");
  // Set data format for the keyboard
  Result = App.Input.Keyb.Device->SetDataFormat(&c_dfDIKeyboard);
  if(Result) Exception(__LINE__, Result, "Failed to set keyboard data format");
  // Attach keyboard device to the window handle
  Result = App.Input.Keyb.Device->SetCooperativeLevel(App.Window.Handle, App.Flags & AFL_WINDOW ? DISCL_BACKGROUND | DISCL_NONEXCLUSIVE : DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
  if(Result) Exception(__LINE__, Result, "Failed to set keyboard cooperative level");

  // Create mouse device
  Result = App.Input.Device->CreateDevice(GUID_SysMouse, &App.Input.Mouse.Device, 0);
  if(Result) Exception(__LINE__, Result, "Failed to create mouse device");
  // Set data format for the mouse
  Result = App.Input.Mouse.Device->SetDataFormat(&c_dfDIMouse);
  if(Result) Exception(__LINE__, Result, "Failed to set mouse data format");
  // Attach the mouse device to the window handle
  // Use this when using windbg or the function fails:
  // Result = App.Input.Mouse.Device->SetCooperativeLevel(App.Window.Handle, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
  Result = App.Input.Mouse.Device->SetCooperativeLevel(App.Window.Handle, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
  if(Result) Exception(__LINE__, Result, "Failed to set mouse cooperative level");

  // Initialise mouse cursor to middle of screen
  App.Input.Mouse.Pos.X = ASPECT_WIDTH / 2;
  App.Input.Mouse.Pos.Y = ASPECT_HEIGHT / 2;
}
/* --------------------------------------------------------------------------------- */
/* Initialise DirectSound                                                            */
/* --------------------------------------------------------------------------------- */
VOID InitDirectSound(VOID)
{
  // Create direct sound object
  HRESULT Result = CoCreateInstance(CLSID_DirectSound, NULL, CLSCTX_ALL, IID_IDirectSound, (PVOID*)&App.Audio.Device);
  if(Result) Exception(__LINE__, Result, "Failed to create direct sound object");
  // Initialise direct sound object
  Result = App.Audio.Device->Initialize(NULL);
  if(Result) Exception(__LINE__, Result, "Failed to initialise direct sound object");
  // Initialise device capabilities structure
  App.Audio.Capabilities.dwSize = sizeof(App.Audio.Capabilities);
  // Get device capabilities
  Result = App.Audio.Device->GetCaps(&App.Audio.Capabilities);
  if(Result) Exception(__LINE__, Result, "Failed to query direct sound device capabilities");
  // Attach direct sound object to window handle
  Result = App.Audio.Device->SetCooperativeLevel(App.Window.Handle, DSSCL_NORMAL | DSSCL_PRIORITY);
  if(Result) Exception(__LINE__, Result, "Failed to set sound co-operative level");
  // Direct sound buffer descriptor
  DSBUFFERDESC BufferDesc;
  // Setup structure for primary sound buffer
  ZeroMemory(&BufferDesc, sizeof(BufferDesc));
  BufferDesc.dwSize = sizeof(BufferDesc);
  BufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_LOCSOFTWARE;
  // Create primary sound buffer
  Result = App.Audio.Device->CreateSoundBuffer(&BufferDesc, &App.Audio.PrimaryBuffer, 0);
  if(Result) Exception(__LINE__, Result, "Failed to create primary sound buffer");
  // Create WAVEFORMAT structure for SetFormat parameter
  WAVEFORMATEX PCM = { WAVE_FORMAT_PCM, 2, App.Audio.Rate, App.Audio.Rate * 4, 4, 16 }; // PCM header
  // Set format of primary sound buffer
  Result = App.Audio.PrimaryBuffer->SetFormat(&PCM);
  if(Result) Exception(__LINE__, Result, "Failed to set format of primary sound buffer");
}
/* --------------------------------------------------------------------------------- */
/* DeInitialise DirectSound                                                          */
/* --------------------------------------------------------------------------------- */
VOID DeInitDirectSound(VOID)
{
  // Unload primary buffer
  if(App.Audio.PrimaryBuffer) { App.Audio.PrimaryBuffer->Release(); App.Audio.PrimaryBuffer = NULL; }
  // Unload DirectSound
  if(App.Audio.Device) { App.Audio.Device->Release(); App.Audio.Device = NULL; }
}
/* --------------------------------------------------------------------------------- */
/* Toggle window and full screen mode                                                */
/* --------------------------------------------------------------------------------- */
VOID ToggleWindowMode(VOID)
{
  // Unload textures
  UnloadTextures();
  // Deinitialise direct input
  DeInitDirectInput();
  // DeInitialise direct draw
  DeInitDirectDraw();
  // Toggle window mode flag
  App.Flags ^= AFL_WINDOW;
  // ReInitialise direct draw
  InitDirectDraw();
  // Reinitialise direct input
  InitDirectInput();
  // Reload in-game textures
  LoadTextures(App.Video.Tex.Flags);
}
/* --------------------------------------------------------------------------------- */
/* DeInitialise DirectInput                                                          */
/* --------------------------------------------------------------------------------- */
VOID DeInitDirectInput(VOID)
{
  // Unload keyboard device
  if(App.Input.Keyb.Device) { App.Input.Keyb.Device->Release(); App.Input.Keyb.Device = NULL; }
  // Unload mouse device
  if(App.Input.Mouse.Device) { App.Input.Mouse.Device->Release(); App.Input.Mouse.Device = NULL; }
  // Unload DirectInput
  if(App.Input.Device) { App.Input.Device->Release(); App.Input.Device = NULL; }
}
/* --------------------------------------------------------------------------------- */
/* DeInitialise DirectDraw                                                           */
/* --------------------------------------------------------------------------------- */
VOID DeInitDirectDraw(VOID)
{
  // Unload palette
  if(App.Video.Palette) { App.Video.Palette->Release(); App.Video.Palette = NULL; }
  // Unload secondary surface
  if(App.Video.SecondarySurface) { App.Video.SecondarySurface->Release(); App.Video.SecondarySurface = NULL; }
  // Unload clipper
  if(App.Video.Clipper) { App.Video.Clipper->Release(); App.Video.Clipper = NULL; }
  // Unload primary surface
  if(App.Video.PrimarySurface) { App.Video.PrimarySurface->Release(); App.Video.PrimarySurface = NULL; }
  // Unload DirectDraw
  if(App.Video.Device) { App.Video.Device->Release(); App.Video.Device = NULL; }
}
/* ================================================================================= */
/* ################################################################################# */
/* ################################################################################# */
/* ###                                                                           ### */
/* ###   S O U N D  S Y S T E M  F U N C T I O N S                               ### */
/* ###                                                                           ### */
/* ################################################################################# */
/* ################################################################################# */
/* ================================================================================= */
// Initialise windows networking library
/* --------------------------------------------------------------------------------- */
VOID InitWinSock(VOID)
{
  // Version required
  USHORT VersionRequested = MAKEWORD(WSOCK_REQ_MAJOR, WSOCK_REQ_MINOR);

  // Startup windows networking library
  if(WSAStartup(VersionRequested, &App.Net.Startup))
    // Throw error
    Exception(__LINE__, WSAGetLastError(), "Failed to initialise the Windows sockets library");

  // Make sure version is right
  if(App.Net.Startup.wVersion != VersionRequested)
    // Throw error
    Exception(__LINE__, 0, "This program requires the Windows sockets library version %u.%u to operate correctly. This machine only supports version %u.%u", WSOCK_REQ_MAJOR, WSOCK_REQ_MINOR, LOBYTE(App.Net.Startup.wVersion), HIBYTE(App.Net.Startup.wVersion));
}
/* --------------------------------------------------------------------------------- */
VOID DeInitWinSock(VOID)
{
  // Winsock started?
  if(App.Net.Startup.wVersion)
    // Clean up
    WSACleanup();
}
/* ================================================================================= */
/* ################################################################################# */
/* ################################################################################# */
/* ###                                                                           ### */
/* ###   S O U N D  S Y S T E M  F U N C T I O N S                               ### */
/* ###                                                                           ### */
/* ################################################################################# */
/* ################################################################################# */
/* ================================================================================= */
/* SOUND::CONSTRUCTOR                                                                */
/* --------------------------------------------------------------------------------- */
SOUND::SOUND(SFXID ResId)
{
  // Sound list already exists?
  if(App.Audio.Sfx.Last)
  {
    // Set identifier
    Id = App.Audio.Sfx.Last->Id + 1;
    // Set previous sound
    Prev = App.Audio.Sfx.Last;
    // Else set next sound of last sound to this sound
    App.Audio.Sfx.Last->Next = this;
  }
  else
  {
    // Else set previous sound to null
    Prev = NULL;
    // Set identifier
    Id = 0;
    // Set first sound to this sound
    App.Audio.Sfx.First = this;
  }
  // Set next sound to null
  Next = NULL;
  // Set last sound to this sound
  App.Audio.Sfx.Last = this;
  // Increment reference counter
  ++App.Audio.Sfx.Count;

  // No sound needs loading? (used with duplicatesoundbuffer)
  if(ResId == SFX_NONE)
    // Don't load a sound
    return;

  // Load data from specified file
  Data = (LPDWORD)LoadFile(ResId, "WAVEDATA", &Size);
  // Check magic and that the RIFF file has the WAVEfmt string
  if(Size < 64 || *Data != 0x46464952 || *(PUINT64)(Data + 2) != 0x20746D6645564157)
    Exception(__LINE__, 0, "Wave resource #%u is not a valid WAVE file", ResId);
  // Fill structure for buffer (needed when uploading sounds too)
  ZeroMemory(&BufferDesc, sizeof(BufferDesc));
  BufferDesc.dwSize = sizeof(BufferDesc);
  BufferDesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
  BufferDesc.dwBufferBytes = *(Data + 10);
  BufferDesc.lpwfxFormat = (LPWAVEFORMATEX)(Data + 5);
  // Create the buffer
  HRESULT Result = App.Audio.Device->CreateSoundBuffer(&BufferDesc, &Buffer, 0);
  if(Result) Exception(__LINE__, Result, "Failed to create direct sound buffer for WAVE resource #%u", ResId);
}
/* --------------------------------------------------------------------------------- */
/* SOUND::DECONSTRUCTOR                                                              */
/* --------------------------------------------------------------------------------- */
SOUND::~SOUND(VOID)
{
  // Sound data loaded?
  if(Data)
  {
    // Delete sound data
    // XXXX delete []Data;
    // Nullify the address
    Data = NULL;
  }
  // Unload sound buffer
  if(Buffer) Buffer->Release();
  // If there is a previous and a next sound
  if(Prev && Next)
  {
    // Set next sound of previous sound to my next sound
    Prev->Next = Next;
    // Set previous sound of next sound to my previous sound
    Next->Prev = Prev;
  }
  // Else if there is a previous but not a next sound (last in list)
  else if(Prev && !Next)
  {
    // Set next sound of previous sound to NULL
    Prev->Next = NULL;
    // Set the last sound to my previous sound
    App.Audio.Sfx.Last = Prev;
  }
  // Else if there is no previous sound but a next sound (first in list)
  else if(!Prev && Next)
  {
    // Set the previous sound of the next sound to NULL
    Next->Prev = NULL;
    // Set the first sound
    App.Audio.Sfx.First = Next;
  }
  // Else if there is no previous and next sound (Sole sound)
  else
    // Set both sound and sound last to NULL
    App.Audio.Sfx.First = App.Audio.Sfx.Last = NULL;
  // Decrement reference counter
  --App.Audio.Sfx.Count;
}
/* --------------------------------------------------------------------------------- */
/* SOUND::PLAY SOUND                                                                 */
/* --------------------------------------------------------------------------------- */
VOID SOUND::PLAY(LONG Pan)
{
  // Sound is disabled?
  if(App.Flags & AFL_NOSOUNDS)
    // Dont play sound
    return;

  // For storing sound status code
  register ULONG Status;
  // Get current status of sound
  DX(__LINE__, Buffer->GetStatus(&Status));
  // Is the sound already playing?
  if(Status == DSBSTATUS_PLAYING)
  {
    // We don't want enhanced sound mixing?
    if(App.Flags & AFL_NOENMIXSND)
    {
      // Set panning
      DX(__LINE__, Buffer->SetPan(Pan));
      // Just adjust pointer to the beginning
      DX(__LINE__, Buffer->SetCurrentPosition(0));
      // Done
      return;
    }

    // This routine is to check for stopped 'child' sound classes that have been
    // duplicated from the 'parent' class. If the sound class is a child and
    // the sound class is not playing, then it is safe to delete, else ignore
    // the sound.

    // Get pointer to first sound in list
    register PSOUND Sound = App.Audio.Sfx.First;
    // Create pointer to store the next sound
    register PSOUND NextSound;
    // For each sound
    do
    {
      // Set next sound
      NextSound = Sound->Next;
      // Sound is a child sound? (Created by duplicatesoundbuffer)
      if(!Sound->Size)
      {
        // Get current status of sound
        DX(__LINE__, Sound->Buffer->GetStatus(&Status));
        // Sound is not playing?
        if(Status != DSBSTATUS_PLAYING)
          // Delete the sound
          delete Sound;
      }
      // Set next sound
      Sound = NextSound;
    }
    while(Sound);

    // Create a new uninitialised sound class
    Sound = new SOUND(SFX_NONE);
    // Duplicate sound buffer
    DX(__LINE__, App.Audio.Device->DuplicateSoundBuffer(Buffer, &Sound->Buffer));
    // Set panning
    DX(__LINE__, Sound->Buffer->SetPan(Pan));
    // Play the sound
    DX(__LINE__, Sound->Buffer->Play(0, 0, 0));
    // Done
    return;
  }
  // Set panning
  DX(__LINE__, Buffer->SetPan(Pan));
  // Else play the sound
  DX(__LINE__, Buffer->Play(0, 0, 0));
}
/* --------------------------------------------------------------------------------- */
/* SOUND::UPLOAD SOUND                                                               */
/* --------------------------------------------------------------------------------- */
PSOUND SOUND::UPLOAD(BOOL Restore)
{
  // This is a child sound effect created with duplicatesoundbuffer
  if(!Size)
  {
    // Get next sound
    register PSOUND SfxNext = Next;
    // This shouldn't hurt I hope
    delete this;
    // Return next sound
    return SfxNext;
  }
  // Create storage for error handling
  register HRESULT Result;
  // Sound buffer needs restoring?
  if(Restore)
  {
    // Restore sound buffer
    Result = Buffer->Restore();
    if(Result) Exception(__LINE__, Result, "Failed to restore sound buffer #%u", Id);
  }
  // Create pointers to wave data
  PVOID WavePTR1, WavePTR2;
  // Create storage for wave data buffer lengths
  ULONG WaveLen1, WaveLen2;
  // Lock the sound buffer so we can write the wave data to it
  Result = Buffer->Lock(0, BufferDesc.dwBufferBytes, &WavePTR1, &WaveLen1, &WavePTR2, &WaveLen2, 0);
  if(Result) Exception(__LINE__, Result, "Failed to lock direct sound buffer #%u", Id);
  // Copy the wave data into the sound buffer
  CopyMemory(WavePTR1, (PCHAR)(Data + 11), WaveLen1);
  // Not sure why we need to do a second buffer so here it is anyway
  if(WaveLen2) CopyMemory(WavePTR2, (PCHAR)(Data + 11) + WaveLen1, WaveLen2);
  // Unlock the sound buffer back to direct sound
  Result = Buffer->Unlock(WavePTR1, WaveLen1, WavePTR2, WaveLen2);
  if(Result) Exception(__LINE__, Result, "Failed to unlock direct sound buffer #%u", Id);
  // Return the next sound
  return Next;
}
/* --------------------------------------------------------------------------------- */
/* Restore and upload all sounds back to their buffers                               */
/* --------------------------------------------------------------------------------- */
VOID UploadSounds(BOOL Restore)
{
  // No sounds loaded?
  if(!App.Audio.Sfx.First)
    // Nothing to do
    return;
  // Alias sounds list
  register PSOUND Sound = App.Audio.Sfx.First;
  // Walk sounds list, restore and upload each one
  do Sound = Sound->UPLOAD(Restore);
    // Process the next sound
    while(Sound);
}
/* --------------------------------------------------------------------------------- */
/* Unload all sounds                                                                 */
/* --------------------------------------------------------------------------------- */
VOID UnloadSounds(VOID)
{
  // For each sound
  while(App.Audio.Sfx.First)
    // Free the sound structure
    delete App.Audio.Sfx.First;
  // Clear sound, last sound pointers
  App.Audio.Sfx.First = App.Audio.Sfx.Last = NULL;
  // Did we not delete every sound?
  if(App.Audio.Sfx.Count > 0)
    // Bail out!
    Exception(__LINE__, 0, "%u sounds are un-accounted for", App.Audio.Sfx.Count);
}
/* --------------------------------------------------------------------------------- */
/* Load base sound effects                                                           */
/* --------------------------------------------------------------------------------- */
VOID LoadBaseSoundEffects(VOID)
{
  // Storage for loop counter
  register INT Index;
  // Go through all in game sounds
  for(Index = 0; Index < SFX_MAX; ++Index)
    // Load sound
    App.Audio.Sfx.Sound[Index] = new SOUND((SFXID)Index);
}
/* --------------------------------------------------------------------------------- */
/* Toggle sound effects                                                              */
/* --------------------------------------------------------------------------------- */
VOID ToggleSounds(VOID)
{
  // Toggle sounds
  App.Flags ^= AFL_NOSOUNDS;
}
/* ================================================================================= */
/* ################################################################################# */
/* ################################################################################# */
/* ###                                                                           ### */
/* ###   M U S I C  S Y S T E M  F U N C T I O N S                               ### */
/* ###                                                                           ### */
/* ################################################################################# */
/* ################################################################################# */
/* ================================================================================= */
/* Music control function                                                            */
/* --------------------------------------------------------------------------------- */
VOID MusicControl(MUSICCMD Command, MUSICID ResId)
{
  // Which command?
  switch(Command)
  {
    // Play music
    case MUSICCMD_PLAY:
    {
      // Specified song is already playing?
      if(App.Audio.Music.Type != MUSICTYPE_NULL && App.Audio.Music.SongId == ResId)
        // Done
        break;

      // Stop and unload previous music
      MusicControl(MUSICCMD_STOP, MUSIC_NULL);
      // Set new song id
      App.Audio.Music.SongId = ResId;

      // Music disabled? Dont play
      if(App.Flags & AFL_NOMUSIC) break;

      // Create handle storage for checking if mp3 file exists
      HANDLE File = INVALID_HANDLE_VALUE;
      // Create buffer for path to mp3
      WCHAR Buffer[1024];
      // Fill buffer
      wsprintfW(Buffer, L"%SMUSIC\\%S.MP3", App.Process.Path, MUSICNAMES[ResId]);
      // If we are allowed to stream music
      if(~App.Flags & AFL_NOSTREAMING)
        // Check if the mp3 exists
        File = CreateFileW(Buffer, 0, 0, NULL, OPEN_EXISTING, 0, NULL);
      // Result for direct sound calls
      HRESULT Result;
      // Success?
      if(File != INVALID_HANDLE_VALUE)
      {
        // Close file
        CloseHandle(File);
        // Set type
        App.Audio.Music.Type = MUSICTYPE_MP3;

        // Initialise filter graph manager for Direct Show
        Result = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (PVOID*)&App.Audio.Music.GraphBuilder);
        if(Result) Exception(__LINE__, Result, "Failed to create the filter graph manager for Direct Show");
        // Query COM interface for media control
        Result = App.Audio.Music.GraphBuilder->QueryInterface(IID_IMediaControl, (PVOID*)&App.Audio.Music.MediaControl);
        if(Result) Exception(__LINE__, Result, "Failed to obtain IID_IMediaControl COM interface");
        // Query COM interface for media event
        Result = App.Audio.Music.GraphBuilder->QueryInterface(IID_IMediaEventEx, (PVOID*)&App.Audio.Music.MediaEvent);
        if(Result) Exception(__LINE__, Result, "Failed to obtain IID_IMediaEventEx COM interface");
        // Query COM interface for media position
        Result = App.Audio.Music.GraphBuilder->QueryInterface(IID_IMediaPosition, (PVOID*)&App.Audio.Music.MediaPosition);
        if(Result) Exception(__LINE__, Result, "Failed to obtain IID_IMediaPosition COM interface");
        // Query COM interface for basic audio
        Result = App.Audio.Music.GraphBuilder->QueryInterface(IID_IBasicAudio, (PVOID*)&App.Audio.Music.BasicAudio);
        if(Result) Exception(__LINE__, Result, "Failed to obtain IID_IBasicAudio COM interface");
        // Load MP3
        Result = App.Audio.Music.GraphBuilder->RenderFile(Buffer, NULL);
        if(Result) Exception(__LINE__, Result, "Failed to load '%S' audio stream", Buffer);
        // Attach to our window to receive notifications
        Result = App.Audio.Music.MediaEvent->SetNotifyWindow((OAHWND)App.Window.Handle, WM_USER, ResId);
        if(Result) Exception(__LINE__, Result, "Failed to attach streaming object to game window");
        // Set noficiation flags
        Result = App.Audio.Music.MediaEvent->SetNotifyFlags(0);
        if(Result) Exception(__LINE__, Result, "Failed to set streaming object parameters");
        // Play music
        App.Audio.Music.MediaControl->Run();
      }
      // Else load the XM version instead
      else
      {
        App.Audio.Music.Type = MUSICTYPE_XM;
        // Load music file
        App.Audio.Music.Data = LoadFile(ResId, "XMDATA", &App.Audio.Music.Size);
        // Check magic
        if(*(PUINT64)App.Audio.Music.Data != 0x6465646E65747845)
          Exception(__LINE__, GetLastError(), "File %s is not a valid XM module", MUSICNAMES[ResId]);
        // Create WAVEFORMAT structure for SetFormat parameter
        WAVEFORMATEX PCM = { WAVE_FORMAT_PCM, 2, App.Audio.Rate, App.Audio.Rate * 4, 4, 16 }; // PCM header
        // Create direct sound buffer for XM
        ZeroMemory(&App.Audio.Music.BufferDesc, sizeof(App.Audio.Music.BufferDesc));
        App.Audio.Music.BufferDesc.dwSize = sizeof(App.Audio.Music.BufferDesc);
        App.Audio.Music.BufferDesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STICKYFOCUS | DSBCAPS_CTRLVOLUME;
        App.Audio.Music.BufferDesc.dwBufferBytes = uFMOD_BUFFER_SIZE;
        App.Audio.Music.BufferDesc.lpwfxFormat = &PCM;
        // Create the music sound buffer
        Result = App.Audio.Device->CreateSoundBuffer(&App.Audio.Music.BufferDesc, &App.Audio.Music.Buffer, 0);
        if(Result) Exception(__LINE__, Result, "Failed to create direct sound buffer for %s", MUSICNAMES[ResId]);
        // Play song
        Result = uFMOD_DSPlaySong(App.Audio.Music.Data, &App.Audio.Music.Size, XM_MEMORY, App.Audio.Music.Buffer);
        // Function failed? Bail out
        if(Result < 0) Exception(__LINE__, Result, "Failed to play music module %s", MUSICNAMES[ResId]);
        // Set volume
      }
      // Music disabled?
      if(App.Flags & AFL_NOMUSIC)
        // Halt playback
        MusicControl(MUSICCMD_PAUSE, MUSIC_NULL);
      // Done
      break;
    }
    // Stop music
    case MUSICCMD_STOP:
    {
      // Music not loaded?
      if(App.Audio.Music.Type == MUSICTYPE_NULL)
        // Done
        break;

      // Which extension was it?
      switch(App.Audio.Music.Type)
      {
        // MP3 stream
        case MUSICTYPE_MP3:
        {
          // Stop music playing
          App.Audio.Music.MediaControl->Stop();
          // Unload media position control
          if(App.Audio.Music.MediaPosition) { App.Audio.Music.MediaPosition->Release(); App.Audio.Music.MediaPosition = NULL; }
          // Unload media event control
          if(App.Audio.Music.MediaEvent) { App.Audio.Music.MediaEvent->Release(); App.Audio.Music.MediaEvent = NULL; }
          // Unload media control control
          if(App.Audio.Music.MediaControl) { App.Audio.Music.MediaControl->Release(); App.Audio.Music.MediaControl = NULL; }
          // Release graph builder
          if(App.Audio.Music.GraphBuilder) { App.Audio.Music.GraphBuilder->Release(); App.Audio.Music.GraphBuilder = NULL; }
          // Done
          break;
        }
        // Extended module
        case MUSICTYPE_XM:
        {
          // Stop music playing
          uFMOD_DSPlaySong(0, 0 ,0 ,0);
          // Music data loaded?
          if(App.Audio.Music.Data)
            // Nullify the address
            App.Audio.Music.Data = NULL;
          // Music buffer loaded?
          if(App.Audio.Music.Buffer) { App.Audio.Music.Buffer->Release(); App.Audio.Music.Buffer = NULL; }
          // Done
          break;
        }
      }
      // Reset music data structure
      ZeroMemory(&App.Audio.Music, sizeof(App.Audio.Music));
      // Done
      break;
    }
    // Pause music
    case MUSICCMD_PAUSE:
    {
      // Music not loaded or music is disabled?
      if(App.Audio.Music.Type == MUSICTYPE_NULL)
        // Do not do anything
        break;

      // Which extension was it?
      switch(App.Audio.Music.Type)
      {
        // MP3 stream
        case MUSICTYPE_MP3:
        {
          // Pause it
          App.Audio.Music.MediaControl->Pause();
          // Done
          break;
        }
        // Extended module
        case MUSICTYPE_XM:
        {
          // Pause it
          uFMOD_Pause();
          // Done
          break;
        }
      }
      // Done
      break;
    }
    // Resume music
    case MUSICCMD_RESUME:
    {
      // Music not loaded or music is disabled?
      if(App.Audio.Music.Type == MUSICTYPE_NULL)
        // Do not do anything
        break;

      // Which extension was it?
      switch(App.Audio.Music.Type)
      {
        // MP3 stream
        case MUSICTYPE_MP3:
        {
          // Resume it
          App.Audio.Music.MediaControl->Run();
          // Done
          break;
        }
        // Extended module
        case MUSICTYPE_XM:
        {
          // Resume it
          uFMOD_Resume();
          // Done
          break;
        }
      }
      // Done
      break;
    }
    // Set volume
    case MUSICCMD_VOLUME:
    {
      // Which extension was it?
      switch(App.Audio.Music.Type)
      {
        // MP3 stream
        case MUSICTYPE_MP3:
        {
          // Set volume
          App.Audio.Music.BasicAudio->put_Volume(ResId);
          // Done
          break;
        }
        // Extended module
        case MUSICTYPE_XM:
        {
          // Set volume
          uFMOD_SetVolume(ResId);
          // Done
          break;
        }
      }
      // Done
      break;
    }
    // Toggle music
    case MUSICCMD_MUTE:
    {
      // Toggle music flag
      App.Flags ^= AFL_NOMUSIC;
      // Music loaded?
      if(App.Audio.Music.Type != MUSICTYPE_NULL)
        // Toggle music playback
        MusicControl(~App.Flags & AFL_NOMUSIC ? MUSICCMD_RESUME : MUSICCMD_PAUSE, MUSIC_NULL);
      // Music not loaded and just enabled?
      else if(~App.Flags & AFL_NOMUSIC)
        // Play music
        MusicControl(MUSICCMD_PLAY, App.Audio.Music.SongId);
      // Done
      break;
    }
  }
}
/* --------------------------------------------------------------------------------- */
/* Toggle streaming music                                                            */
/* --------------------------------------------------------------------------------- */
VOID ToggleStreaming(VOID)
{
  // Toggle streaming
  App.Flags ^= AFL_NOSTREAMING;
  // Record current song
  MUSICID Id = App.Audio.Music.SongId;
  // If music was actually playing
  if(Id != MUSIC_NULL)
  {
    // Stop music
    MusicControl(MUSICCMD_STOP, MUSIC_NULL);
    // Play music
    MusicControl(MUSICCMD_PLAY, Id);
  }
}
/* ================================================================================= */
/* ################################################################################# */
/* ################################################################################# */
/* ###                                                                           ### */
/* ###   T E X T U R E  S Y S T E M  F U N C T I O N S                           ### */
/* ###                                                                           ### */
/* ################################################################################# */
/* ################################################################################# */
/* ================================================================================= */
/* TEXTURE::CONSTRUCTOR                                                              */
/* --------------------------------------------------------------------------------- */
TEXTURE::TEXTURE(BMPID ResId, INT SplitWidth, BOOL Transparent)
{
  // Texture list already exists?
  if(App.Video.Tex.Last)
  {
    // Set identifier
    Id = App.Video.Tex.Last->Id + 1;
    // Set previous texture
    Prev = App.Video.Tex.Last;
    // Else set next texture of last texture to this texture
    App.Video.Tex.Last->Next = this;
  }
  else
  {
    // Else set previous texture to null
    Prev = NULL;
    // Set identifier
    Id = 0;
    // Set first texture to this texture
    App.Video.Tex.First = this;
  }
  // Set next texture to null
  Next = NULL;
  // Set last texture to this texture
  App.Video.Tex.Last = this;
  // Increment reference counter
  ++App.Video.Tex.Count;

  // Initialise tile count
  Count = 1;
  // Create storage for bitmap size (entire file)
  ULONG BitmapSize;
  // Get file from package
  PCHAR BitmapData = (PCHAR)LoadFile(ResId, "TEXDATA", &BitmapSize);
  // Set pointer to file header structure in file
  PBITMAPFILEHEADER BitmapFileHeader = (PBITMAPFILEHEADER)BitmapData;
  // Make sure file is a bitmap
  if(BitmapFileHeader->bfType != 0x4D42)
    Exception(__LINE__, 0, "Bitmap resource #%u is not a valid bitmap file. Got 0x%X instead", ResId, BitmapFileHeader->bfType);
  // Integrity check
  if(BitmapFileHeader->bfSize != BitmapSize)
    Exception(__LINE__, 0, "The size of bitmap resource #%u which is %u does not match actual bitmap size of %u", ResId, BitmapFileHeader->bfSize, BitmapSize);
  // Set pointer to bitmap info structure in file
  PBITMAPINFO BitmapInfo = (PBITMAPINFO)((PCHAR)BitmapData + sizeof(BITMAPFILEHEADER));
  // Make sure bitmap does not have compression
  if(BitmapInfo->bmiHeader.biCompression != BI_RGB)
    Exception(__LINE__, 0, "Bitmap resource #%u has compression id of 0x%x and is un-supported", ResId, BitmapInfo->bmiHeader.biCompression);

  // Backup original bitmap size
  COORDS BitmapOrigSize = { BitmapInfo->bmiHeader.biWidth, BitmapInfo->bmiHeader.biHeight };

  // Extend bitmap width
  BitmapInfo->bmiHeader.biWidth = BitmapInfo->bmiHeader.biWidth * App.Video.ActiveMode->TexScale;
  // Extend bitmap height
  BitmapInfo->bmiHeader.biHeight = BitmapInfo->bmiHeader.biHeight * App.Video.ActiveMode->TexScale;
  // Extend bitmap size
  BitmapInfo->bmiHeader.biSizeImage = BitmapInfo->bmiHeader.biSizeImage * App.Video.ActiveMode->TexScale;

  // Create temporary device context for CreateDIBSection
  HDC BitmapSurface = CreateCompatibleDC(0);
  // Function failed?
  if(!BitmapSurface)
    // Bail out
    Exception(__LINE__, GetLastError(), "Failed to create a compatible device context for loading bitmap resource #%u", ResId);
  // Storage for new bitmap bits
  PCHAR BitmapBits;
  // Create the bitmap
  Bitmap = CreateDIBSection(BitmapSurface, BitmapInfo, DIB_RGB_COLORS, (PVOID*)&BitmapBits, 0, 0);
  // Function failed? Bail out
  if(!Bitmap) Exception(__LINE__, GetLastError(), "Failed to create a %ux%ux%u-bit bitmap handle for bitmap resource #%u", BitmapInfo->bmiHeader.biWidth, BitmapInfo->bmiHeader.biHeight, BitmapInfo->bmiHeader.biBitCount, ResId);
  // Done with surface
  if(!DeleteDC(BitmapSurface))
    Exception(__LINE__, GetLastError(), "Failed to free temporary device context while loading bitmap resource #%u", BitmapInfo->bmiHeader.biWidth, BitmapInfo->bmiHeader.biHeight, BitmapInfo->bmiHeader.biBitCount, ResId);

  // What scaling is required?
  switch(App.Video.ActiveMode->TexScale)
  {
    // No scaling
    case 1:
    {
      // Copy the data bits into our bitmap bits
      CopyMemory(BitmapBits, BitmapData + BitmapFileHeader->bfOffBits, BitmapFileHeader->bfSize - BitmapFileHeader->bfOffBits);
      // Done
      break;
    }
    // Double size scaling
    case 2:
    {
      // Scale bits into new bits buffer
      scale2x((PUCHAR)BitmapBits, BitmapInfo->bmiHeader.biWidth, (PUCHAR)BitmapData + BitmapFileHeader->bfOffBits, BitmapOrigSize.X, 1, BitmapOrigSize.X, BitmapOrigSize.Y);
      // Done
      break;
    }
    // Triple size scaling
    case 3:
    {
      // Scale bits into new bits buffer
      scale3x((PUCHAR)BitmapBits, BitmapInfo->bmiHeader.biWidth, (PUCHAR)BitmapData + BitmapFileHeader->bfOffBits, BitmapOrigSize.X, 1, BitmapOrigSize.X, BitmapOrigSize.Y);
      // Done
      break;
    }
    // Quad size scaling
    case 4:
    {
      // Theres no 'direct' scale4x so we have to use scale2x twice, and also, we
      // have to create a 'middle buffer'.
      PUCHAR BitmapBitsInterim = new UCHAR[((BitmapOrigSize.X << 1) * BitmapOrigSize.Y) << 1];
      // Scale bits into interim bits buffer
      scale2x((PUCHAR)BitmapBitsInterim, BitmapOrigSize.X << 1, (PUCHAR)BitmapData + BitmapFileHeader->bfOffBits, BitmapOrigSize.X, 1, BitmapOrigSize.X, BitmapOrigSize.Y);
      // Scale bits into new bits buffer
      scale2x((PUCHAR)BitmapBits, BitmapInfo->bmiHeader.biWidth, BitmapBitsInterim, BitmapOrigSize.X << 1, 1, BitmapOrigSize.X << 1, BitmapOrigSize.Y << 1);
      // Done with interim buffer
      delete []BitmapBitsInterim;
      // Done
      break;
    }
    // Unknown scale? Shouldn't get here, but just incase.
    default:
    {
      // Bail out
      Exception(__LINE__, 0, "An un-supported texture mode of %u was detected", App.Video.ActiveMode->TexScale);
    }
  }

  // Initialise top and left of bounds
  Bounds.left = Bounds.top = 0;
  // Set blit parameters
  BltParam = DDBLTFAST_WAIT | (Transparent ? DDBLTFAST_SRCCOLORKEY : DDBLTFAST_NOCOLORKEY);
  // Bitmap is a tileset?
  if(SplitWidth)
  {
    // Multiply splitwidth by texture scale
    SplitWidth = SplitWidth * App.Video.ActiveMode->TexScale;
    // Calculate number of tiles in tileset
    Count = BitmapInfo->bmiHeader.biWidth / SplitWidth;
    // Initialise right bounds
    Bounds.right = SplitWidth;
    // Initialise bottom bounds
    Bounds.bottom = BitmapInfo->bmiHeader.biHeight;
  }
  else
  {
    // Initialise right bounds
    Bounds.right = BitmapInfo->bmiHeader.biWidth;
    // Initialise bottom bounds
    Bounds.bottom = BitmapInfo->bmiHeader.biHeight;
  }

  // Restore bitmap width
  BitmapInfo->bmiHeader.biWidth = BitmapInfo->bmiHeader.biWidth / App.Video.ActiveMode->TexScale;
  // Restore bitmap height
  BitmapInfo->bmiHeader.biHeight = BitmapInfo->bmiHeader.biHeight / App.Video.ActiveMode->TexScale;
  // Restore bitmap size
  BitmapInfo->bmiHeader.biSizeImage = BitmapInfo->bmiHeader.biSizeImage / App.Video.ActiveMode->TexScale;

  // Free file data
  // XXXX delete []BitmapData;

  DDCOLORKEY        DDColorKey; // Transparency value
  HRESULT           Result;     // Result holder for direct draw calls

  // Set transparency to white by default
  DDColorKey.dwColorSpaceLowValue = DDColorKey.dwColorSpaceHighValue = (~App.Flags & AFL_WINDOW && App.Video.ActiveMode->Depth <= 8) || (App.Flags & AFL_WINDOW && App.Video.DesktopInfo.dmBitsPerPel <= 8) ? 0x00 : 0xFFFFFFFF;

  // Allocate surfaces with a spare address at the end for NULL pointer
  Surfaces = (LPDIRECTDRAWSURFACE*)new CHAR[Count * App.Core.AddressLength];
  // Make sure memory was allocated
  if(!Surfaces)
    Exception(__LINE__, GetLastError(), "Failed to allocate %u bytes of address space for %u tiles for texture #%u", Count * App.Core.AddressLength, Count, ResId);

  // Create a pointer to a surface
  LPDIRECTDRAWSURFACE Surface;
  // Index counter for loop
  INT Index;
  // Create surface for each tile
  for(Index = 0; Index < Count; ++Index)
  {
    // Create the surface
    Surface = CreateSurface(&Result, Bounds.right, Bounds.bottom);
    // Bail out if function failed
    if(!Surface) Exception(__LINE__, GetLastError(), "Failed to create a %ux%u tile %u for texture #%u", Bounds.right, Bounds.bottom, Index, ResId);
    // If transparency is requested?
    if(Transparent)
    {
      // Set transparency
      Result = Surface->SetColorKey(DDCKEY_SRCBLT, &DDColorKey);
      if(Result) Exception(__LINE__, Result, "Failed to create colour key for texture #%u", ResId);
    }
    // Store address of surface
    Surfaces[Index] = Surface;
  }

}
/* --------------------------------------------------------------------------------- */
/* TEXTURE::DECONSTRUCTOR                                                            */
/* --------------------------------------------------------------------------------- */
TEXTURE::~TEXTURE(VOID)
{
  // Unload bitmap if allocated
  if(Bitmap && !DeleteObject(Bitmap))
    Exception(__LINE__, GetLastError(), "Failed to delete bitmap object from texture #%u", Id);
  // Unload surfaces if allocated
  if(Surfaces)
  {
    // Unload each surface
    for(--Count; Count >= 0; --Count)
      // Unload surface
      if(Surfaces[Count]) Surfaces[Count]->Release();
    // Free the address space
    delete []Surfaces;
    // Nullify the address
    Surfaces = NULL;
  }
  // If there is a previous and a next texture
  if(Prev && Next)
  {
    // Set next texture of previous Texture to my next texture
    Prev->Next = Next;
    // Set previous texture of next texture to my previous texture
    Next->Prev = Prev;
  }
  // Else if there is a previous but not a next texture (last in list)
  else if(Prev && !Next)
  {
    // Set next Texture of previous texture to NULL
    Prev->Next = NULL;
    // Set the last texture to my previous texture
    App.Video.Tex.Last = Prev;
  }
  // Else if there is no previous Texture but a next texture (first in list)
  else if(!Prev && Next)
  {
    // Set the previous texture of the next texture to NULL
    Next->Prev = NULL;
    // Set the first texture
    App.Video.Tex.First = Next;
  }
  // Else if there is no previous and next texture (Sole Texture)
  else
    // Set both textures and texture last to NULL
    App.Video.Tex.First = App.Video.Tex.Last = NULL;
  // Decrement reference counter
  --App.Video.Tex.Count;
}
/* --------------------------------------------------------------------------------- */
/* TEXTURE::Blit the specified tile without performing any off-screen checking                */
/* --------------------------------------------------------------------------------- */
VOID TEXTURE::BLTFAST(INT X, INT Y, INT Id)
{
  // Scale co-ordinates
  X = X * App.Video.ActiveMode->TexScale;
  Y = Y * App.Video.ActiveMode->TexScale;

  // Draw the tile to screen
  DX(__LINE__, App.Video.SecondarySurface->BltFast(X, Y, Surfaces[Id], &Bounds, BltParam));
}
/* --------------------------------------------------------------------------------- */
/* TEXTURE::Blit a partial part of the the specified tile w/ no off-screen checking                */
/* --------------------------------------------------------------------------------- */
VOID TEXTURE::BLTFASTPARTIAL(INT X, INT Y, INT W, INT Id)
{
  // Scale co-ordinates
  X = X * App.Video.ActiveMode->TexScale;
  Y = Y * App.Video.ActiveMode->TexScale;
  W = W * App.Video.ActiveMode->TexScale;

  // Initialise rect
  register RECT Rect = { 0, 0, W, Bounds.bottom } ;

  // Draw the tile to screen
  DX(__LINE__, App.Video.SecondarySurface->BltFast(X, Y, Surfaces[Id], &Rect, BltParam));
}
/* --------------------------------------------------------------------------------- */
/* TEXTURE::Blit a partial part of the the specified tile w/ no off-screen checking                */
/* --------------------------------------------------------------------------------- */
VOID TEXTURE::BLTFASTCUSTOM(INT X, INT Y, INT L, INT T, INT R, INT B, INT Id)
{
  // Scale co-ordinates
  X = X * App.Video.ActiveMode->TexScale;
  Y = Y * App.Video.ActiveMode->TexScale;
  L = L * App.Video.ActiveMode->TexScale;
  T = T * App.Video.ActiveMode->TexScale;
  R = R * App.Video.ActiveMode->TexScale;
  B = B * App.Video.ActiveMode->TexScale;

  // Initialise rect
  register RECT Rect = { L, T, R, B };

  // Draw the tile to screen
  DX(__LINE__, App.Video.SecondarySurface->BltFast(X, Y, Surfaces[Id], &Rect, BltParam));
}
/* --------------------------------------------------------------------------------- */
/* TEXTURE::Blit the specified tile while performing 'fake' off-screen drawing                */
/* --------------------------------------------------------------------------------- */
VOID TEXTURE::BLT(INT X, INT Y, INT Id)
{
  register RECT Rect; // Parameter for BltFast

  // Scale co-ordinates
  X = X * App.Video.ActiveMode->TexScale;
  Y = Y * App.Video.ActiveMode->TexScale;

  // Initialise bounds
  CopyMemory(&Rect, &Bounds, sizeof(Rect));

  // The following code performs 'fake' off-screen drawing by modifiying co-ordinates
  // and sizes to make sure the tile is drawn on the screen properly. We have to do
  // this or BltFast will fail when it trys to draw off-screen.
  // We could just have a tertiary screen surface and have space to draw stuff around
  // it, but I believe this way would be best method.

  // If X co-ordinate is off the left of the screen
  if(X < 0)
  {
    // Clip amount off the left of the sprite
    Rect.left -= X;
    // Adjust position to the amount clipped
    X += Rect.left;
  }
  // If X co-ordinate is off the right of the screen
  else if(X > App.Video.ActiveMode->Width - Bounds.right)
    // Clip amount off the right of the sprite
    Rect.right -= X - (App.Video.ActiveMode->Width - Bounds.right);
  // If Y co-ordinate is off the top of the screen
  if(Y < 0)
  {
    // Clip amount off the top of the sprite
    Rect.top -= Y;
    // Adjust position to the amount clipped
    Y += Rect.top;
  }
  // If Y co-ordinate is off the bottom of the screen
  else if(Y > App.Video.ActiveMode->Height - Bounds.bottom)
    // Clip amount off the bottom of the sprite
    Rect.bottom -= Y - (App.Video.ActiveMode->Height - Bounds.bottom);

  // Perform final validation to make sure the bounds are ok
  if(Rect.left >= Bounds.right || Rect.top >= Bounds.bottom || Rect.right <= 0 || Rect.bottom <= 0)
    return;
  // Draw the tile to screen
  DX(__LINE__, App.Video.SecondarySurface->BltFast(X, Y, Surfaces[Id], &Rect, BltParam));
}
/* --------------------------------------------------------------------------------- */
/* TEXTURE::Upload a GDI bitmap to DDRAW surface                                     */
/* --------------------------------------------------------------------------------- */
PTEXTURE TEXTURE::UPLOAD(BOOL Restore)
{
  HDC                 DCSurface;  // GDI contaxt of DDRAW surface
  HDC                 DCBitmap;   // GDI context for source bitmap surfaces
  HRESULT             Result;     // Result var for error checking
  DDCOLORKEY          DDColorKey; // Transparency value
  RECT                Rect;       // Co-ordinates
  INT                 Index;      // For loop
  LPDIRECTDRAWSURFACE Surface;    // Pointer to actual surface

  // Create GDI surface
  DCBitmap = CreateCompatibleDC(0);
  // Function failed? Bail out
  if(!DCBitmap) Exception(__LINE__, GetLastError(), "Failed to create temporary context for texture #%u", Id);
  // Attach GDI bitmap to GDI surface
  if(SelectObject(DCBitmap, Bitmap) == HGDI_ERROR)
    Exception(__LINE__, GetLastError(), "Failed to select bitmap into temporary context for texture #%u", Id);

  // Initialise rect
  Rect.left = Rect.top = 0;
  Rect.right = Bounds.right;
  Rect.bottom = Bounds.bottom;

  // Set transparency to white by default
  DDColorKey.dwColorSpaceLowValue = DDColorKey.dwColorSpaceHighValue = 0xFF;

  // Process each surface
  for(Index = 0; Index < Count; Index++, Rect.left += Bounds.right, Rect.right += Bounds.right)
  {
    // Alias requested surface
    Surface = Surfaces[Index];
    // If surface restore requested?
    if(Restore)
    {
      // Restore the surface
      Result = Surface->Restore();
      if(Result) Exception(__LINE__, Result, "Failed to restore surface for texture #%u", Id);
    }
    // Unlock DDRAW surface
    Result = Surface->GetDC(&DCSurface);
    if(Result) Exception(__LINE__, Result, "Failed to lock surface for texture #%u", Id);
    // Copy GDI surface to DDRAW surface
    if(!BitBlt(DCSurface, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, DCBitmap, Rect.left, Rect.top, SRCCOPY))
      Exception(__LINE__, GetLastError(), "Failed to draw bitmap onto surface of texture #%u", Id);
    // Lock DDRAW surface
    Result = Surface->ReleaseDC(DCSurface);
    if(Result) Exception(__LINE__, Result, "Failed to unlock surface for texture #%u", Id);
  }
  // Delete GDI surface
  if(!DeleteDC(DCBitmap)) Exception(__LINE__, GetLastError(), "Failed to release temporary device context for texture #%u", Id);

  // Return next texture in list
  return Next;
}
/* --------------------------------------------------------------------------------- */
/* TEXTURE::PRINT TEXT                                                               */
/* --------------------------------------------------------------------------------- */
VOID TEXTURE::PRINT(INT X, INT Y, BOOL C, PCHAR String)
{
  // Want centre alignment?
  if(C)
  {
    // Get length of string
    register INT StringLen = lstrlen(String);
    // Move X coordinate back
    X -= (StringLen >> 1) * (Bounds.right / App.Video.ActiveMode->TexScale);
    // If length of string is odd then move back some more
    if(StringLen % 2) X -= (Bounds.right / App.Video.ActiveMode->TexScale) >> 1;
  }
  // Original X position
  register INT OX = X;
  // Read 'Buffer' until NUL terminator
  for(register PUCHAR Pointer = (PUCHAR)String; *Pointer; *Pointer++)
  {
    // Line feed detected?
    if(*Pointer == 10)
    {
      // Set new position
      X = OX, Y += (Bounds.bottom / App.Video.ActiveMode->TexScale);
      // Skip drawing character
      continue;
    }
    // Non-valid character?
    if(*Pointer < 0x20 || *Pointer >= 0x80)
      // Skip
      continue;
    // Blit the character, lets use PutTileFast but watch you dont draw off-screen
    BLT(X, Y, *Pointer - 0x20);
    // Adjust X co-ordinate
    X += Bounds.right / App.Video.ActiveMode->TexScale;
  }
}
/* --------------------------------------------------------------------------------- */
/* TEXTURE::PRINT FORMATTED TEXT                                                     */
/* --------------------------------------------------------------------------------- */
VOID TEXTURE::PRINTF(INT X, INT Y, BOOL C, PCHAR Format, ...)
{
  CHAR Buffer[1024]; // Buffer holds output from wvsprintf

  // Format buffer
  if(!wvsprintf(Buffer, Format, (PCHAR)(&Format + 1)))
    // No characters written to buffer, bail out
    return;
  // Print buffer
  PRINT(X, Y, C, Buffer);
}
/* --------------------------------------------------------------------------------- */
/* Create an off-screen surface                                                      */
/* --------------------------------------------------------------------------------- */
LPDIRECTDRAWSURFACE CreateSurface(HRESULT *Result, INT Width, INT Height)
{
  LPDIRECTDRAWSURFACE Surface;     // Pointer to a new ddraw surface
  DDSURFACEDESC       SurfaceDesc; // Surface descriptor structure

  // Initialise the structure
  ZeroMemory(&SurfaceDesc, sizeof(SurfaceDesc));
  SurfaceDesc.dwSize = sizeof(SurfaceDesc);
  SurfaceDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
  SurfaceDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
  SurfaceDesc.dwWidth = Width;
  SurfaceDesc.dwHeight = Height;

  // Create the surface
  *Result = App.Video.Device->CreateSurface(&SurfaceDesc, &Surface, 0);
  // If function succeeded return the surface else NULL.
  return *Result ? NULL : Surface;
}
/* --------------------------------------------------------------------------------- */
/* Upload 0-127 entires of bitmap A and 128-255 of bitmap B to display adapter       */
/* --------------------------------------------------------------------------------- */
VOID MergePalette(PTEXTURE A, PTEXTURE B)
{
  // Not 8bit mode?
  if((~App.Flags & AFL_WINDOW && App.Video.ActiveMode->Depth > 8) || (App.Flags & AFL_WINDOW && App.Video.DesktopInfo.dmBitsPerPel > 8))
    // Ignore
    return;
  // Create temporary device context
  HDC DC = CreateCompatibleDC(0);
  // Function failed? Bail out
  if(!DC) Exception(__LINE__, GetLastError(), "Failed to create compatible context to upload palette");
  // Select first bitmap into context
  if(SelectObject(DC, A->Bitmap) == HGDI_ERROR)
    Exception(__LINE__, GetLastError(), "Failed to select first bitmap into context to upload palette");
  // Create storage for RGB values stored by GetDibBits
  RGBQUAD RGB[256];
  // Get palette of first bitmap
  if(!GetDIBColorTable(DC, 0, 256, RGB))
    Exception(__LINE__, GetLastError(), "Failed to retrieve palette of first bitmap");
  // Create storage for palette values for CreatePalette/SetEntries
  PALETTEENTRY PAL[256];
  // Create index for colour id array copying
  INT Index;
  // Copy first 128 entries into structure
  for(Index = 0; Index < 128; ++Index)
  {
    PAL[Index].peRed = RGB[Index].rgbRed;
    PAL[Index].peGreen = RGB[Index].rgbGreen;
    PAL[Index].peBlue = RGB[Index].rgbBlue;
  }
  // Select second bitmap into context
  if(SelectObject(DC, B->Bitmap) == HGDI_ERROR)
    Exception(__LINE__, GetLastError(), "Failed to select second bitmap into context to upload palette");
  // Get palette of second bitmap
  if(!GetDIBColorTable(DC, 0, 256, RGB))
    Exception(__LINE__, GetLastError(), "Failed to retrieve palette of second bitmap");
  // Copy last 128 entries into structure
  for(Index = 128; Index < 256; ++Index)
  {
    PAL[Index].peRed = RGB[Index].rgbRed;
    PAL[Index].peGreen = RGB[Index].rgbGreen;
    PAL[Index].peBlue = RGB[Index].rgbBlue;
  }
  // Delete temporary device context
  if(!DeleteDC(DC))
    Exception(__LINE__, GetLastError(), "Failed to free temporary device context");
  // Activate new palette
  DX(__LINE__, App.Video.Palette->SetEntries(0, 0, 256, PAL));
}
/* --------------------------------------------------------------------------------- */
/* Restore and upload all textures back to graphics device                           */
/* --------------------------------------------------------------------------------- */
VOID UploadTextures(BOOL Restore)
{
  // No textures loaded?
  if(!App.Video.Tex.First)
    // Why bother?
    return;
  // Alias textures list
  PTEXTURE Texture = App.Video.Tex.First;
  // Walk textures list, restore and upload each one
  do Texture = Texture->UPLOAD(Restore);
    // Process the next texture
    while(Texture);
}
/* --------------------------------------------------------------------------------- */
/* Cycle texture scale                                                               */
/* --------------------------------------------------------------------------------- */
VOID CycleTextureScale(VOID)
{
  // Set next mode
  ++App.Video.ActiveModeId;
  // Unload textures
  UnloadTextures();
  // DeInitialise direct draw
  DeInitDirectDraw();
  // ReInitialise direct draw
  InitDirectDraw();
  // Reload in-game textures
  LoadTextures(App.Video.Tex.Flags);
}
/* --------------------------------------------------------------------------------- */
/* Unload all textures                                                               */
/* --------------------------------------------------------------------------------- */
VOID UnloadTextures(VOID)
{
  // Unloading the textures will also unload the mouse cursor
  SetMouseCursor(CID_NONE);
  // For each texture
  while(App.Video.Tex.First)
    // Free the texture structure
    delete App.Video.Tex.First;
  // Clear texture, last texture pointers
  App.Video.Tex.First = App.Video.Tex.Last = NULL;
  // Did we not delete every texture?
  if(App.Video.Tex.Count > 0)
    // Bail out!
    Exception(__LINE__, 0, "%u textures are un-accounted for", App.Video.Tex.Count);
}
/* --------------------------------------------------------------------------------- */
/* Load base textures                                                                */
/* --------------------------------------------------------------------------------- */
VOID LoadTextures(INT InitType)
{
  // Unload any existing textures
  UnloadTextures();

  // Set texture flags
  App.Video.Tex.Flags = InitType;

  // Want to load loading screen?
  if(App.Video.Tex.Flags & INITTYP_LOADING)
  {
    // Load loading screen bitmap
    App.Video.Tex.Aux = new TEXTURE(BMP_LOADING, 0, FALSE);
    // Init palette
    MergePalette(App.Video.Tex.Aux, App.Video.Tex.Aux);
  }
  // Want to load fonts?
  if(App.Video.Tex.Flags & INITTYP_FONTS)
  {
    // Load tiny font
    App.Video.Tex.FontTiny = new TEXTURE(BMP_FONTTINY, 4, TRUE);
    // Load small font
    App.Video.Tex.FontSmall = new TEXTURE(BMP_FONTSMALL, 6, TRUE);
    // Load large font
    App.Video.Tex.FontLarge = new TEXTURE(BMP_FONTLARGE, 12, TRUE);
  }
  // Want to load base level stuff?
  if(App.Video.Tex.Flags & INITTYP_BASE)
  {
    // Load sprites
    App.Video.Tex.Sprite = new TEXTURE(BMP_TSSPRITES, TILE_SIZE, TRUE);
    // Load interface
    App.Video.Tex.Interface = new TEXTURE(BMP_TSINTERFACE, TILE_SIZE, TRUE);
    // Load transparency
    App.Video.Tex.Trans = new TEXTURE(BMP_PTRANS, 0, TRUE);
    // Load terrain
    App.Video.Tex.Terrain = new TEXTURE((BMPID)(BMP_TSDESERT + App.Game.LevelScene), TILE_SIZE, TRUE);
    // Load parallax
    App.Video.Tex.Parallax = new TEXTURE((BMPID)(BMP_PLXDESERT + App.Game.LevelScene), 0, FALSE);
    // Init palette
    MergePalette(App.Video.Tex.Terrain, App.Video.Tex.Parallax);
    // Show default mouse cursor
    SetMouseCursor(CID_ARROW);
  }
  // Want to load in-game stuff?
  if(App.Video.Tex.Flags & INITTYP_INGAME)
  {
    // Load health bar
    App.Video.Tex.Aux = new TEXTURE(BMP_HPBAR, 100, FALSE);
  }

  // Upload textures
  UploadTextures(FALSE);
}
/* ================================================================================= */
/* ################################################################################# */
/* ################################################################################# */
/* ###                                                                           ### */
/* ###   C U R S O R  F U N C T I O N S                                          ### */
/* ###                                                                           ### */
/* ################################################################################# */
/* ################################################################################# */
/* ================================================================================= */
/* Set a new mouse cursor                                                            */
/* --------------------------------------------------------------------------------- */
VOID SetMouseCursor(CURSORID Id)
{
  // Hide mouse cursor?
  if(Id == CID_NONE)
  {
    // Nullify address
    App.Input.Mouse.Cursor = NULL;
    // Done
    return;
  }
  // This cursor is already set?
  if(App.Input.Mouse.Cursor == &CursorData[Id])
    // Ignore
    return;
  // Set new cursor
  App.Input.Mouse.Cursor = &CursorData[Id];
  // Set new animation id
  App.Input.Mouse.CursorAnimId = App.Input.Mouse.Cursor->StartId;
  // Reset delay timer
  App.Input.Mouse.CursorTimer = 0;
}
/* --------------------------------------------------------------------------------- */
/* Draw mouse cursor                                                                 */
/* --------------------------------------------------------------------------------- */
VOID DrawMouseCursor(VOID)
{
  // There is no mouse cursor?
  if(!App.Input.Mouse.Cursor)
    // Don't bother drawing it then
    return;
  // Draw mouse cursor
  App.Video.Tex.Interface->BLT(App.Input.Mouse.Pos.X + App.Input.Mouse.Cursor->OffsetX, App.Input.Mouse.Pos.Y + App.Input.Mouse.Cursor->OffsetY, App.Input.Mouse.CursorAnimId);
  // Anim delay timer not reached yet?
  if(++App.Input.Mouse.CursorTimer < TIMER_ANIMFAST)
    // Dont change animation id just yet
    return;
  // Mouse cursor animation reached the end?
  if(++App.Input.Mouse.CursorAnimId > App.Input.Mouse.Cursor->EndId)
    // Reset animation
    App.Input.Mouse.CursorAnimId = App.Input.Mouse.Cursor->StartId;
  // Reset mouse cursor timer
  App.Input.Mouse.CursorTimer = 0;
}
/* ================================================================================= */
/* ################################################################################# */
/* ################################################################################# */
/* ###                                                                           ### */
/* ###   G A M E  F U N C T I O N S                                              ### */
/* ###                                                                           ### */
/* ################################################################################# */
/* ################################################################################# */
/* ================================================================================= */
/* Window active function                                                            */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK ActiveWindowProc(VOID)
{
  // Window message available?
  if(PeekMessage(&App.Window.Message, 0, 0, 0, PM_REMOVE))
  {
    // Translate message for callback
    TranslateMessage(&App.Window.Message);
    // Send message to callback
    DispatchMessage(&App.Window.Message);
    // Do not process anymore frames or dx functions will fail
    return;
  }

  // Get current time
  App.Timer.CurrentTime = GetTickCount();
  // Can we process the next frame yet?
  if(App.Timer.CurrentTime - App.Timer.LastRender <= App.Timer.Limiter)
    // Nope
    return;
  // Update last render time
  App.Timer.LastRender = App.Timer.CurrentTime;

  // Process controls
  App.Input.Function();
  // Process desired game function
  App.Game.Function();
  // Draw mouse cursor
  DrawMouseCursor();
  // Draw buffer to screen
  App.Video.Function();
  // Increase game tick
  ++App.Game.Time;
}
/* --------------------------------------------------------------------------------- */
/* Window inactive function                                                          */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK InactiveWindowProc(VOID)
{
  // Window message available?
  if(GetMessage(&App.Window.Message, NULL, 0, 0))
  {
    // Translate message for callback
    TranslateMessage(&App.Window.Message);
    // Send message to callback
    DispatchMessage(&App.Window.Message);
  }
}
/* --------------------------------------------------------------------------------- */
/* Flip to window                                                                    */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK WindowFlip(VOID)
{
  // Copy secondary to primary surface
  DX(__LINE__, App.Video.PrimarySurface->Blt(&App.Window.Destination, App.Video.SecondarySurface, &App.Window.Source, DDBLT_WAIT, 0));
}
/* --------------------------------------------------------------------------------- */
/* Flip to full-screen window                                                        */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK FullScreenFlip(VOID)
{
  // Copy secondary to primary surface
  DX(__LINE__, App.Video.PrimarySurface->Flip(0, DDFLIP_WAIT));
}
/* --------------------------------------------------------------------------------- */
/* No control function (Empty)                                                       */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK PreventControl(VOID)
{
  // Keep keyboard button state clear
  ZeroMemory(App.Input.Keyb.State, sizeof(App.Input.Keyb.State));
  // Keep mouse button press state clear
  ZeroMemory(App.Input.Mouse.State, sizeof(App.Input.Mouse.State));
}
/* --------------------------------------------------------------------------------- */
/* Main control function                                                             */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK MainControl(VOID)
{
  // Get keyboard state from direct input
  DX(__LINE__, App.Input.Keyb.Device->GetDeviceState(sizeof(App.Input.Keyb.DeviceState), App.Input.Keyb.DeviceState));
  // Convert direct input keyboard state array to a more managable array
  // The new array will consist of 3 values:
  //    0 = The key is not pressed
  //    1 = The key was pressed at least once
  //    2 = The key is held down
  for(register INT Index = 0; Index < 256; ++Index)
  {
    if(App.Input.Keyb.DeviceState[Index] & 0x80)
    {
      if(App.Input.Keyb.State[Index] < 2)
        ++App.Input.Keyb.State[Index];
    }
    else App.Input.Keyb.State[Index] = 0;
  }

  // If user pressed F7
  if(App.Input.Keyb.State[DIK_F7] & 1)
    // Toggle tips on and off
    App.Flags ^= AFL_NOTIPS;
  // If user pressed F8?
  else if(App.Input.Keyb.State[DIK_F8] & 1)
    // Toggle sound effects on and off
    ToggleSounds();
  // If user pressed F9?
  else if(App.Input.Keyb.State[DIK_F9] & 1)
    // Toggle xm module playback on and off
    MusicControl(MUSICCMD_MUTE, MUSIC_NULL);
  // If user pressed F10?
  else if(App.Input.Keyb.State[DIK_F10] & 1)
    // Toggle window mode on and off
    ToggleWindowMode();
  // If user pressed F11?
  else if(App.Input.Keyb.State[DIK_F11] & 1)
    // Cycle texture scale / resolution
    CycleTextureScale();
  // If user pressed F12?
  else if(App.Input.Keyb.State[DIK_F12] & 1)
    // Save a screenshot to disk
    ScreenShot();

  // Get mouse state from direct input
  DX(__LINE__, App.Input.Mouse.Device->GetDeviceState(sizeof(App.Input.Mouse.DeviceState), &App.Input.Mouse.DeviceState));
  // Convert direct input mouse button state to a more managable array
  // The new array will consist of 3 values:
  //    0 = The button is not pressed
  //    1 = The button was pressed at least once
  //    2 = The button is held down
  for(register INT Index = 0; Index < MID_MAX; ++Index)
  {
    // Is mouse button down?
    if(App.Input.Mouse.DeviceState.rgbButtons[Index])
    {
      if(App.Input.Mouse.State[Index] < 2)
        ++App.Input.Mouse.State[Index];
    }
    else App.Input.Mouse.State[Index] = 0;
  }

  // Update mouse information
  App.Input.Mouse.Pos.X += App.Input.Mouse.DeviceState.lX;
  App.Input.Mouse.Pos.Y += App.Input.Mouse.DeviceState.lY;

  // Clamp mouse co-ordinates
  if(App.Input.Mouse.Pos.X < 0) App.Input.Mouse.Pos.X = 0;
  else if(App.Input.Mouse.Pos.X >= ASPECT_WIDTH) App.Input.Mouse.Pos.X = ASPECT_WIDTH - 1;
  if(App.Input.Mouse.Pos.Y < 0) App.Input.Mouse.Pos.Y = 0;
  else if(App.Input.Mouse.Pos.Y >= ASPECT_HEIGHT) App.Input.Mouse.Pos.Y = ASPECT_HEIGHT - 1;
}
/* --------------------------------------------------------------------------------- */
/* Go pause function                                                                 */
/* --------------------------------------------------------------------------------- */
VOID TogglePause(VOID)
{
  // Game is paused?
  if(App.Game.Function == (GAMEFUNC)GamePauseProc)
  {
    // Set ingame function
    App.Game.Function = (GAMEFUNC)InGameProc;
    // Resume playback
    MusicControl(MUSICCMD_RESUME, MUSIC_NULL);
  }
  else
  {
    // Set paused function
    App.Game.Function = (GAMEFUNC)GamePauseProc;
    // Pause playback
    MusicControl(MUSICCMD_PAUSE, MUSIC_NULL);
  }
}
/* --------------------------------------------------------------------------------- */
/* Game paused function                                                              */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK GamePauseProc(VOID)
{
  // Player wants to unpause the game
  if(App.Input.Keyb.State[DIK_PAUSE] & 1)
    // Unpause game
    TogglePause();

  // Draw level scenery
  DrawTerrain(FALSE);

  // Draw objects
  if(App.Game.Object.First)
  {
    // Create pointer to objects list
    register POBJECT Object = App.Game.Object.First;
    // Walk objects list and render each object
    do Object = Object->RENDER();
      // Goto next object
      while(Object);
  }

  // Draw fake transparent background
  App.Video.Tex.Trans->BLTFAST(0, 0, 0);

  // Print paused text
  App.Video.Tex.FontLarge->PRINT(160, 112, TRUE, "GAME PAUSED");
}
/* --------------------------------------------------------------------------------- */
/* Game over proc function                                                           */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK GameOverProc(VOID)
{
  // Button clicked?
  if(App.Input.Mouse.State[MID_LMB] == 1)
  {
    // Player won?
    if(App.Game.Player.Remain == App.Game.Player.This || App.Game.Player.This->Money >= App.Game.Player.This->Opponent->Money)
    {
      // Set music
      MusicControl(MUSICCMD_PLAY, MUSIC_WIN);
      // Map post mortem
      App.Game.Function = (GAMEFUNC)MapPostMortemProc;
    }
    // Player lost. Restart level
    else CreateTransition(TRUE, (GAMEFUNC)InitInGameProc);
  }

  // Draw level scenery
  DrawTerrain(FALSE);

  // Draw objects
  if(App.Game.Object.First)
  {
    // Create pointer to objects list
    register POBJECT Object = App.Game.Object.First;
    // Walk objects list and render each object
    do Object = Object->RENDER();
      // Goto next object
      while(Object);
  }

  App.Video.Tex.FontLarge->PRINT (160, 16,  TRUE,
     App.Game.Player.Remain ? (App.Game.Player.Remain == App.Game.Player.This ? "YOUR OPPONENTS ARE DEAD" :
                                                  "ALL YOUR DIGGERS DIED") :
                    (App.Game.Player.This->Money >= App.Game.Player.This->Target ?
                                                "YOU HAVE RAISED THE CASH" :
                                                "YOUR OPPONENT HAS WON"));

  App.Video.Tex.FontSmall->PRINTF(160, 40,  TRUE, "%s TRADE CENTRE FINANCE DATA", App.Game.LevelCurrent->Name);

  App.Video.Tex.FontSmall->PRINTF(160, 56,  TRUE, "GAME TIME WAS %u MINUTES", App.Game.Time / 3600);

  App.Video.Tex.FontSmall->PRINTF(16 , 72, FALSE, "YOUR FINANCES                  OPPONENT FINANCES\n\n"
                                    "TARGET CAPTIAL %7dz  TARGET CAPITAL %7dz\n"
                                    "CAPITAL RAISED %7dz  CAPITAL RAISED %7dz\n"
                                    "REFUNDED ASSETS %6dz  REFUNDED ASSETS %6dz\n"
                                    "REWARD BONUS %9dz  REWARD BONUS %9dz\n\n"
                                    "SALARY PAID %10dz  SALARY PAID %10dz\n"
                                    "DEATH DUTIES %9dz  DEATH DUTIES %9dz\n"
                                    "LOAN REPAYMENT %7dz  LOAN REPAYMENT %7dz\n\n"
                                    "BANK DEPOSIT %9dz  BANK DEPOSIT %9dz\n"
                                    "BANK BALANCE %9dz  BANK BALANCE %9dz",
    App.Game.Player.This->Target, App.Game.Player.This->Opponent->Target,
    App.Game.Player.This->Money, App.Game.Player.This->Opponent->Money,
    App.Game.Player.This->Assets, App.Game.Player.This->Opponent->Assets,
    App.Game.Player.This->Reward, App.Game.Player.This->Opponent->Reward,
    App.Game.Player.This->Salary, App.Game.Player.This->Opponent->Salary,
    App.Game.Player.This->DeathDuty, App.Game.Player.This->Opponent->DeathDuty,
    App.Game.Player.This->Loan, App.Game.Player.This->Opponent->Loan,
    App.Game.Player.This->Deposit, App.Game.Player.This->Opponent->Deposit,
    App.Game.Player.This->Balance, App.Game.Player.This->Opponent->Balance);

  App.Video.Tex.FontSmall->PRINTF(160, 184,  TRUE, "ALL TRANSACTIONS ARE FINAL");

  App.Video.Tex.FontLarge->PRINT(160, 210, TRUE, "PRESS BUTTON TO CONTINUE");
}
/* --------------------------------------------------------------------------------- */
/* Create a transition                                                                     */
/* --------------------------------------------------------------------------------- */
VOID CreateTransition(BOOL Out, GAMEFUNC Function)
{
  // Transition already in progress?
  if(App.Game.TransCounter)
    // Ignore command
    return;
  // Set transition function when transition has finished
  App.Game.TransEndFunction = Function;
  // Transition out requested?
  if(Out)
  {
    // Initialise transition counter
    App.Game.TransCounter = 1;
    // Set transition function when transition is progressing
    App.Game.TransDurFunction = App.Game.Function;
    // Initialise game function
    App.Game.Function = (GAMEFUNC)TransitionOut;
  }
  // Transition in requested?
  else
  {
    // Initialise transition counter
    App.Game.TransCounter = 0;
    // Set transition function when transition is progressing
    App.Game.TransDurFunction = Function;
    // Initialise game function
    App.Game.Function = (GAMEFUNC)TransitionIn;
  }
  // Prevent control
  App.Input.Function = (GAMEFUNC)PreventControl;
}
/* --------------------------------------------------------------------------------- */
/* Create black area for transition                                                  */
/* --------------------------------------------------------------------------------- */
VOID Transition(INT Start, INT End)
{
  // Create rect for destination area to blank
  RECT R = { Start * TILE_SIZE * App.Video.ActiveMode->TexScale, 0, End * TILE_SIZE * App.Video.ActiveMode->TexScale, ASPECT_HEIGHT * App.Video.ActiveMode->TexScale };
  // Create storage for clear colour
  DDBLTFX CLR;
  // Clear structure
  ZeroMemory(&CLR, sizeof(CLR));
  // Initialise
  CLR.dwSize = sizeof(CLR);
  // Set colour to black
  CLR.dwFillColor = (~App.Flags & AFL_WINDOW && App.Video.ActiveMode->Depth <= 8) || (App.Flags & AFL_WINDOW && App.Video.DesktopInfo.dmBitsPerPel <= 8) ? 0x40 : 0;
  // Clear secondary buffer
  DX(__LINE__, App.Video.SecondarySurface->Blt(&R, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &CLR));
  // Clear primary buffer
  DX(__LINE__, App.Video.PrimarySurface->Blt(&R, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &CLR));
}
/* --------------------------------------------------------------------------------- */
/* Transition out palette                                                                  */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK TransitionOut(VOID)
{
  // Game tick
  App.Game.TransDurFunction();

  // Blank from left
  Transition(0, App.Game.TransCounter);

  // Fading not completed?
  if(App.Game.TransCounter++ < SCR_TILES_WIDTH)
    // Done
    return;

  // Create storage for clear colour
  DDBLTFX CLR;
  // Clear structure
  ZeroMemory(&CLR, sizeof(CLR));
  // Initialise
  CLR.dwSize = sizeof(CLR);
  // Set colour to black
  CLR.dwFillColor = (~App.Flags & AFL_WINDOW && App.Video.ActiveMode->Depth <= 8) || (App.Flags & AFL_WINDOW && App.Video.DesktopInfo.dmBitsPerPel <= 8) ? 0x40 : 0;
  // Clear secondary buffer
  DX(__LINE__, App.Video.SecondarySurface->Blt(0, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &CLR));
  // Clear primary buffer
  DX(__LINE__, App.Video.PrimarySurface->Blt(0, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &CLR));

  // Set new requested game function
  App.Game.Function = App.Game.TransEndFunction;
  // Reset counter
  App.Game.TransCounter = 0;
  // and function pointers
  App.Game.TransDurFunction = App.Game.TransEndFunction = NULL;

  // Restore user control
  App.Input.Function = (GAMEFUNC)MainControl;
}
/* --------------------------------------------------------------------------------- */
/* Transition in palette                                                                  */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK TransitionIn(VOID)
{
  // Game tick
  App.Game.TransDurFunction();
  // Blank from left
  Transition(App.Game.TransCounter, SCR_TILES_WIDTH);
  // Fading not completed?
  if(++App.Game.TransCounter < SCR_TILES_WIDTH)
    // Done
    return;
  // Set new requested game function
  App.Game.Function = App.Game.TransEndFunction;
  // Reset counter
  App.Game.TransCounter = 0;
  // and function pointers
  App.Game.TransDurFunction = App.Game.TransEndFunction = NULL;

  // Restore user control
  App.Input.Function = (GAMEFUNC)MainControl;
}
/* --------------------------------------------------------------------------------- */
/* Show tip function                                                                 */
/* --------------------------------------------------------------------------------- */
VOID ShowTip(PCHAR Format, ...)
{
  // Tips not enabled?
  if(App.Flags & AFL_NOTIPS)
    // Ignore
    return;
  // Paint left part of box
  App.Video.Tex.Interface->BLTFAST(8, 8, 97);
  // Create index variable
  register INT Index;
  // Paint middle part of box
  for(Index = 24; Index < 296; Index += TILE_SIZE)
    App.Video.Tex.Interface->BLTFAST(Index, 8, 98);
  // Paint right part of box
  App.Video.Tex.Interface->BLTFAST(Index, 8, 99);
  // Create buffer for tip
  CHAR Buffer[1024];
  // Format buffer
  wvsprintf(Buffer, Format, (PCHAR)(&Format + 1));
  // Show text
  App.Video.Tex.FontSmall->PRINT(160, 12, TRUE, Buffer);
}
/* --------------------------------------------------------------------------------- */
/* Intro game proc                                                                   */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK IntroInGameProc(VOID)
{
  // Ten seconds elapsed?
  if(App.Game.Time >= 300)
  {
    // Play ingame music
    MusicControl(MUSICCMD_PLAY, MUSIC_INGAME);
    // Play teleporter sound
    App.Audio.Sfx.Sound[SFX_PHASE]->PLAY(0);
    // Select in game function
    App.Game.Function = (GAMEFUNC)InGameProc;
  }

  // Draw terrain
  DrawTerrain(TRUE);

  // Show title message
  App.Video.Tex.FontLarge->PRINTF(160,   8, TRUE, "ENTERING %s", App.Game.LevelCurrent->Name);
  // Show subtitle message
  App.Video.Tex.FontSmall->PRINTF(160,  30, TRUE, "%s TERRAIN", SceneNames[App.Game.LevelScene]);
  // Show status message
  App.Video.Tex.FontSmall->PRINTF(160, 200, TRUE, "ARRIVAL IN %u.%u SECONDS", (300 - App.Game.Time) / 60, (600 - App.Game.Time) / 6 % 10);
  // Show requirement message
  App.Video.Tex.FontLarge->PRINTF(160, 216, TRUE, "RAISE %u ZOGS TO WIN", App.Game.LevelCurrent->Required);
}
/* --------------------------------------------------------------------------------- */
/* Map post mortem proc                                                              */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK MapPostMortemProc(VOID)
{
  // If escape pressed
  if(App.Input.Keyb.State[DIK_ESCAPE] == 1 || App.Input.Mouse.State[MID_RMB] == 1)
  {
    // Next level
    App.Game.NewLevel = App.Game.Level + 1;
    // Load previous level
    CreateTransition(TRUE, (GAMEFUNC)InitInGameProc);
  }
  // Mouse is at left of screen
  else if(!App.Input.Mouse.Pos.X)
  {
    // Set left scroll arrow
    SetMouseCursor(CID_LEFT);
    // If button pressed?
    if(App.Input.Mouse.State[MID_LMB])
      // Scroll left
      SetViewport(App.Game.ViewPoint.X - 1, App.Game.ViewPoint.Y);
  }
  // Mouse is at the top of screen
  else if(!App.Input.Mouse.Pos.Y)
  {
    // Set top scroll arrow
    SetMouseCursor(CID_TOP);
    // If button pressed?
    if(App.Input.Mouse.State[MID_LMB])
      // Scroll left
      SetViewport(App.Game.ViewPoint.X, App.Game.ViewPoint.Y - 1);
  }
  // Mouse is at the right of screen
  else if(App.Input.Mouse.Pos.X == ASPECT_WIDTH - 1)
  {
    // Set right scroll arrow
    SetMouseCursor(CID_RIGHT);
    // If button pressed?
    if(App.Input.Mouse.State[MID_LMB])
      // Scroll left
      SetViewport(App.Game.ViewPoint.X + 1, App.Game.ViewPoint.Y);
  }
  // Mouse is at the bottom of screen
  else if(App.Input.Mouse.Pos.Y == ASPECT_HEIGHT - 1)
  {
    // Set right scroll arrow
    SetMouseCursor(CID_BOTTOM);
    // If button pressed?
    if(App.Input.Mouse.State[MID_LMB])
      // Scroll left
      SetViewport(App.Game.ViewPoint.X, App.Game.ViewPoint.Y + 1);
  }
  // Else just show arrow
  else SetMouseCursor(CID_ARROW);

  // Draw terrain
  DrawTerrain(FALSE);

  // Objects exist
  if(App.Game.Object.First)
  {
    // Create pointer to objects list
    register POBJECT Object = App.Game.Object.First;
    // Walk objects list. Render object
    do Object = Object->RENDER();
      // Goto next object
      while(Object);
  }

  // Objects exist
  if(App.Game.Object.First)
  {
    // Create pointer to objects list
    register POBJECT Object = App.Game.Object.First;
    // Walk objects list.
    do
    {
      // Mouse is over this object?
      if(Object->GETMCOLLISION())
        // Done
        break;
      // Get next object
      Object = Object->Next;
    }
    // Goto next object
    while(Object);
    // Object matched? Show info about the object
    if(Object) App.Video.Tex.FontTiny->PRINTF(160, 210, TRUE, "%s (%u%%) @ %ux%u", Object->ObjData->Name, Object->Health, Object->Position.X, Object->Position.Y);
  }

  // Show title message
  App.Video.Tex.FontSmall->PRINTF(160,   8, TRUE, "MAP POST MORTEM FOR %s", App.Game.LevelCurrent->Name);
  // Show status message
  App.Video.Tex.FontSmall->PRINT(160, 224, TRUE, "PRESS ESC OR RMB WHEN DONE");
}
/* --------------------------------------------------------------------------------- */
/* Transition function callback for in-game initialisation                           */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK InitInGameProc(VOID)
{
  // Load loading screen
  LoadTextures(INITTYP_LOADING);
  // Blit loading screen
  App.Video.Tex.Aux->BLTFAST(0, 0, 0);
  // Flip buffers
  App.Video.Function();
  // Load level data
  InitLevel(INITTYP_BASE | INITTYP_FONTS | INITTYP_INGAME, &App.Game.Player.One, App.Game.Player.ThisRace, TYP_DIGRANDOM);
  // Transition in
  CreateTransition(FALSE, (GAMEFUNC)IntroInGameProc);
  // Play music
  MusicControl(MUSICCMD_PLAY, MUSIC_SELECT);
}
/* --------------------------------------------------------------------------------- */
/* Config game proc                                                                  */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK ConfigProc(VOID)
{
  // Draw terrain
  DrawTerrain(FALSE);
  // Pointer to the first object in the list
  register POBJECT Object;
  // For each object
  for(Object = App.Game.Object.First; Object; Object = Object->Next)
    // Render
    Object->RENDER();
  // Paint decoration
  App.Video.Tex.Interface->BLTFAST(  8,   8, 20);
  App.Video.Tex.Interface->BLTFAST(296,   8, 20);
  App.Video.Tex.Interface->BLTFAST(  8, 216, 20);
  App.Video.Tex.Interface->BLTFAST(296, 216, 20);
  App.Video.Tex.Trans->BLTFASTCUSTOM(24,   8, 0, 0, 272,  16, 0);
  App.Video.Tex.Trans->BLTFASTCUSTOM(24, 216, 0, 0, 272,  16, 0);
  App.Video.Tex.Trans->BLTFASTCUSTOM( 8,  32, 0, 0, 304, 176, 0);
  // Print title and status
  App.Video.Tex.FontSmall->PRINTF(160, 12, TRUE, "ENGINE CONFIGURATION");
  App.Video.Tex.FontSmall->PRINTF(160,220, TRUE, "USE MOUSE TO TOGGLE OPTIONS");
  // Create base mouse position
  register INT X = App.Input.Mouse.Pos.X - 16;
  register INT Y = App.Input.Mouse.Pos.Y - 40;
  // Escape key pressed? Return to game
  if(App.Input.Keyb.State[DIK_ESCAPE] & 1) App.Game.Function = (GAMEFUNC)InGameProc;
  // Test position
  else if(X >= 0 && Y >= 0 && X <= 288 && Y < 160)
  {
    // Get id
    register INT Id = Y / 8;
    // Show selection
    App.Video.Tex.Trans->BLTFASTCUSTOM(17, 40 + (8 * Id), 0, 0, 286, 8, 0);
    // Left click
    register BOOL LeftClick = App.Input.Mouse.State[MID_LMB] & 1;
    // Right click
    register BOOL RightClick = App.Input.Mouse.State[MID_RMB] & 1;
    // Compare id
    switch(Id)
    {
      // Texture scale
      case 0: if(LeftClick || RightClick) CycleTextureScale(); break;
      // Full-screen mode enabled
      case 1: if(LeftClick || RightClick) ToggleWindowMode(); break;
      // Sound effects enabled
      case 2: if(LeftClick || RightClick) ToggleSounds(); break;
      // Sound effects volume
      case 3: break;
      // Sound effects mixing
      case 4: if(LeftClick || RightClick) App.Flags ^= AFL_NOENMIXSND; break;
      // Music enabled
      case 5: if(LeftClick || RightClick) MusicControl(MUSICCMD_MUTE, MUSIC_NULL); break;
      // Music volume
      case 6: break;
      // Music streaming enabled
      case 7: if(LeftClick || RightClick) ToggleStreaming(); break;
      // No tips
      case 8: if(LeftClick || RightClick) App.Flags ^= AFL_NOTIPS; break;
      // Speed
      case 9: if(LeftClick || RightClick) App.Timer.Limiter = App.Timer.Limiter == TIMER_FRAMELIMIT_FAST ? TIMER_FRAMELIMIT_SLOW : TIMER_FRAMELIMIT_FAST; break;
      // Done
      case 10: if(LeftClick || RightClick) App.Game.Function = (GAMEFUNC)InGameProc; break;
    }
  }
  // Print options
  App.Video.Tex.FontSmall->PRINTF( 16, 40, FALSE, "TEXTURE SCALE        %uX", ModeData[App.Video.ActiveModeId].TexScale);
  App.Video.Tex.FontTiny->PRINT(160, 41, FALSE, "Adjusts the quality of the graphics");
  App.Video.Tex.FontSmall->PRINTF( 16, 48, FALSE, "FULL-SCREEN MODE   %4s", App.Flags & AFL_WINDOW ? "NO" : "YES");
  App.Video.Tex.FontTiny->PRINT(160, 49, FALSE, "Use full-screen or window mode");
  App.Video.Tex.FontSmall->PRINTF( 16, 56, FALSE, "SOUND FX ENABLED   %4s", App.Flags & AFL_NOSOUNDS ? "NO" : "YES");
  App.Video.Tex.FontTiny->PRINT(160, 57, FALSE, "Toggle playback of sound effects");
  App.Video.Tex.FontSmall->PRINTF( 16, 64, FALSE, "SOUND FX VOLUME    %3u%%", App.Audio.Sfx.Volume);
  App.Video.Tex.FontTiny->PRINT(160, 65, FALSE, "Adjust sound effect volume");
  App.Video.Tex.FontSmall->PRINTF( 16, 72, FALSE, "SOUND FX MIXING    %4s", App.Flags & AFL_NOENMIXSND ? "NO" : "YES");
  App.Video.Tex.FontTiny->PRINT(160, 73, FALSE, "Same sound allowed to play");
  App.Video.Tex.FontSmall->PRINTF( 16, 80, FALSE, "MUSIC ENABLED      %4s", App.Flags & AFL_NOMUSIC ? "NO" : "YES");
  App.Video.Tex.FontTiny->PRINT(160, 81, FALSE, "Toggle music playback");
  App.Video.Tex.FontSmall->PRINTF( 16, 88, FALSE, "MUSIC VOLUME       %3u%%", App.Audio.Music.Volume);
  App.Video.Tex.FontTiny->PRINT(160, 89, FALSE, "Adjust music playback volume");
  App.Video.Tex.FontSmall->PRINTF( 16, 96, FALSE, "MP3 STREAMING      %4s", App.Flags & AFL_NOSTREAMING ? "NO" : "YES");
  App.Video.Tex.FontTiny->PRINT(160, 97, FALSE, "Play MP3 music instead of XM");
  App.Video.Tex.FontSmall->PRINTF( 16,104, FALSE, "DISPLAY GAME TIPS  %4s", App.Flags & AFL_NOTIPS ? "NO" : "YES");
  App.Video.Tex.FontTiny->PRINT(160, 105, FALSE, "Display helpful tips over buttons");
  App.Video.Tex.FontSmall->PRINTF( 16,112, FALSE, "GAME SPEED         %4s", App.Timer.Limiter == TIMER_FRAMELIMIT_FAST ? "FAST" : "SLOW");
  App.Video.Tex.FontTiny->PRINT(160, 113, FALSE, "Set the overall speed of game");
}
/* --------------------------------------------------------------------------------- */
/* In game function                                                                  */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK InGameProc(VOID)
{
  static POBJECT DropObject;              // For inventory drop
  static POBJECT DropObjectFirst;         // First object in inventory
  static INT     BuyObject = TYP_STUNNEL; // Object to buy
  static INT     InGameDisplay = 0;       // Ingame display

  // Can process what the user is pressing now
  if(App.Input.Keyb.State[DIK_ESCAPE] & 1)     // User wants to quit the game
    CreateTransition(TRUE, (GAMEFUNC)QuitGame);
  else if(App.Input.Keyb.State[DIK_PAUSE] & 1) // Player wants to unpause the game
    TogglePause();
  else if(App.Input.Keyb.State[DIK_1] & 1 && App.Game.Player.This->Diggers[0]) // Player wants to unpause the game
    App.Game.Player.This->Diggers[0]->SELECT();
  else if(App.Input.Keyb.State[DIK_2] & 1 && App.Game.Player.This->Diggers[1]) // Player wants to unpause the game
    App.Game.Player.This->Diggers[1]->SELECT();
  else if(App.Input.Keyb.State[DIK_3] & 1 && App.Game.Player.This->Diggers[2]) // Player wants to unpause the game
    App.Game.Player.This->Diggers[2]->SELECT();
  else if(App.Input.Keyb.State[DIK_4] & 1 && App.Game.Player.This->Diggers[3]) // Player wants to unpause the game
    App.Game.Player.This->Diggers[3]->SELECT();
  else if(App.Input.Keyb.State[DIK_5] & 1 && App.Game.Player.This->Diggers[4]) // Player wants to unpause the game
    App.Game.Player.This->Diggers[4]->SELECT();
  //else if(App.Input.Keyb.State[DIK_UP])      // User wants to move up
  //  SetViewport(App.Game.ViewPoint.X, App.Game.ViewPoint.Y - 1);
  //else if(App.Input.Keyb.State[DIK_DOWN])    // User wants to move down
  //  SetViewport(App.Game.ViewPoint.X, App.Game.ViewPoint.Y + 1);
  //else if(App.Input.Keyb.State[DIK_LEFT])    // User wants to move left
  //  SetViewport(App.Game.ViewPoint.X - 1, App.Game.ViewPoint.Y);
  //else if(App.Input.Keyb.State[DIK_RIGHT])   // User wants to move right
  //  SetViewport(App.Game.ViewPoint.X + 1, App.Game.ViewPoint.Y);
  // else if(App.Input.Keyb.State[DIK_SPACE])
  //  new OBJECT(NULL, TYP_JENNITE, (App.Game.ViewPoint.X * TILE_SIZE) + App.Input.Mouse.Pos.X, (App.Game.ViewPoint.Y * TILE_SIZE) + App.Input.Mouse.Pos.Y, TRUE);
  // If user pressed F5?
  else if(App.Input.Keyb.State[DIK_F5] & 1)
    { App.Game.Function = (GAMEFUNC)ConfigProc; return; }
  // If user pressed F1?
  else if(App.Input.Keyb.State[DIK_F1] & 1 && App.Game.Level > 0)
  {
    // Previous level
    App.Game.NewLevel = App.Game.Level - 1;
    // Load previous level
    CreateTransition(TRUE, (GAMEFUNC)InitInGameProc);
  }
  // If user pressed F2?
  else if(App.Input.Keyb.State[DIK_F2] & 1 && App.Game.Level < LEVEL_NUM)
  {
    // Previous level
    App.Game.NewLevel = App.Game.Level + 1;
    // Load previous level
    CreateTransition(TRUE, (GAMEFUNC)InitInGameProc);
  }

  // Was the left mouse button pressed?
  if(App.Input.Mouse.State[MID_LMB] & 1)
  {
    // Digger button clicked?
    if(App.Input.Mouse.Pos.X >= 144 && App.Input.Mouse.Pos.X < 224 && App.Input.Mouse.Pos.Y >= 216 && App.Input.Mouse.Pos.Y < 232)
    {
      // Create counter
      register INT X, Z;
      // Walk through each digger for this client player
      for(X = 0, Z = 144; X < NUM_DIGGERS; ++X, Z += TILE_SIZE)
      {
        // User didnt click this button?
        if(App.Input.Mouse.Pos.X < Z || App.Input.Mouse.Pos.X >= Z + TILE_SIZE)
          // Try next button
          continue;
        // Digger exists?
        if(App.Game.Player.This->Diggers[X])
          // Activate this object
          App.Game.Player.This->Diggers[X]->SELECT();
        else
        // Else set index for error sound
          X = NUM_DIGGERS;
        // No point trying anymore buttons
        break;
      }
      // Play sound effect
      App.Audio.Sfx.Sound[X == NUM_DIGGERS ? SFX_ERROR : SFX_CLICK]->PLAY(0);
    }
    // One of the misc buttons was clicked?
    else if(App.Input.Mouse.Pos.X >= 232 && App.Input.Mouse.Pos.X < 312 && App.Input.Mouse.Pos.Y >= 216 && App.Input.Mouse.Pos.Y < 232)
    {
      // Cycle device button clicked?
      if(App.Input.Mouse.Pos.X < 248)
      {
        // App.Game.Object.First exist?
        if(App.Game.Object.First)
        {
          // Set current stop point so we dont loop forever
          register POBJECT Object = App.Game.Object.Device;
          // Select first object to search
          App.Game.Object.Device = !App.Game.Object.Device || !App.Game.Object.Device->Next ? App.Game.Object.First : App.Game.Object.Device->Next;
          // Loop forever. Break is completely handled.
          for(;;)
          {
            // If object is a device owned by this clients player?
            if(App.Game.Object.Device->Owner == App.Game.Player.This && App.Game.Object.Device->ObjData->Flags & OFL_DEVICE)
            {
              // Select it
              App.Game.Object.Device->SELECT();
              // Play click sound
             App.Audio.Sfx.Sound[SFX_CLICK]->PLAY(0);
              // Break the loop
              break;
            }
            // We are back at the starting object?
            if(App.Game.Object.Device == Object || (!Object && !App.Game.Object.Device->Next))
            {
              // Nullify object and device object
              Object = App.Game.Object.Device = NULL;
              // Break loop
              break;
            }
            // Are we at the end of the objects list?
            if(!App.Game.Object.Device->Next)
            {
              // Go back to start of list
              App.Game.Object.Device = App.Game.Object.First;
              // Check again
              continue;
            }
            // Process next object
            App.Game.Object.Device = App.Game.Object.Device->Next;
          }
          // Did not find a valid object?
          if(!App.Game.Object.Device)
            // Play error sound
            App.Audio.Sfx.Sound[SFX_ERROR]->PLAY(0);
        }
      }
      // Diggers inventory button clicked?
      else if(App.Input.Mouse.Pos.X < 264)
      {
        // Play sound
        App.Audio.Sfx.Sound[SFX_CLICK]->PLAY(0);
        // Toggle digger inventory screen
        InGameDisplay &= GS_INVENTORY;
        InGameDisplay ^= GS_INVENTORY;
      }
      // Diggers location button clicked?
      else if(App.Input.Mouse.Pos.X < 280)
      {
        // Play sound
        App.Audio.Sfx.Sound[SFX_CLICK]->PLAY(0);
        // Toggle digger location screen
        InGameDisplay &= GS_LOCATIONS;
        InGameDisplay ^= GS_LOCATIONS;
      }
      // Game status button clicked?
      else if(App.Input.Mouse.Pos.X < 296)
      {
        // Play sound
        App.Audio.Sfx.Sound[SFX_CLICK]->PLAY(0);
        // Toggle game status screen
        InGameDisplay &= GS_GAMESTATUS;
        InGameDisplay ^= GS_GAMESTATUS;
      }
      // Game menu button clicked?
      else
      {
        // Play sound
        App.Audio.Sfx.Sound[SFX_CLICK]->PLAY(0);
        // Toggle help and information
        InGameDisplay &= GS_HELPANDINFO;
        InGameDisplay ^= GS_HELPANDINFO;
      }
    }
    // Else is the control menu open and the mouse cursor in the menu's bounds?
    else if(App.Game.ObjectMenu && App.Input.Mouse.Pos.X >= App.Game.ObjectMenuPos.X && App.Input.Mouse.Pos.Y >= App.Game.ObjectMenuPos.Y && App.Input.Mouse.Pos.X < App.Game.ObjectMenuPos.X + (App.Game.ObjectMenu->Width * TILE_SIZE) && App.Input.Mouse.Pos.Y < App.Game.ObjectMenuPos.Y + (App.Game.ObjectMenu->Height * TILE_SIZE))
    {
      // Calculate the tile location that was clicked
      register INT X = (((App.Input.Mouse.Pos.Y - App.Game.ObjectMenuPos.Y) / TILE_SIZE) * 8) + ((App.Input.Mouse.Pos.X - App.Game.ObjectMenuPos.X) / TILE_SIZE);
      // If object is busy and button is disabled then deny
      if(App.Game.Object.Active->Flags & OFL_BUSY && App.Game.ObjectMenu->Flags & 1 << X)
        // Play error sound
        App.Audio.Sfx.Sound[SFX_ERROR]->PLAY(0);
      // Button is enabled
      else
      {
        // Play success sound
        App.Audio.Sfx.Sound[SFX_SELECT]->PLAY(0);
        // Calculate the tile id that was clicked
        X = App.Game.ObjectMenu->Data[X].Id;
        // Compare current menu id
        switch(App.Game.ObjectMenu->Id)
        {
          // Main menu control
          case MNU_MAIN:
          {
            // Which id was clicked?
            switch(X)
            {
              // Player wants movement menu
              case 15: SetControlMenu(MNU_MOVE); break;
              // Player wants digging menu
              case 11: SetControlMenu(MNU_DIG); break;
              // Player wants to pickup the item underneith it
              case  5: if(!App.Game.Object.Active->PICKUPITEM(FALSE)) App.Audio.Sfx.Sound[SFX_ERROR]->PLAY(0); break;
              // Player wants to drop an item it is holding
              case  6: DropObject = DropObjectFirst = App.Game.Object.Active->Items; SetControlMenu(MNU_DROP); break;
              // Player wants to enter the trade menu
              case 12:
              {
                // If object is in the starting position
                if(App.Game.Object.Active->Position.X == App.Game.Object.Active->Owner->Start.X && App.Game.Object.Active->Position.Y == App.Game.Object.Active->Owner->Start.Y)
                  // Set trade menu
                  SetControlMenu(MNU_TRADE);
                // Not at starting position so walk to it
                else App.Game.Object.Active->SETACTION(ACT_WALK, JOB_HOME, DIR_HOME); break;
              }
              // Player wants to stop and reset its job
              case 10: App.Game.Object.Active->SETACTION(ACT_STOP, JOB_NONE, DIR_NONE); break;
              // Player wants to search for treasure
              case  9: App.Game.Object.Active->SETACTION(ACT_WALK, JOB_SEARCH, DIR_R); break;
              // Player wants to teleport back to the trading centre
              case 22: App.Game.Object.Active->SETACTION(ACT_PHASE, JOB_PHASE, DIR_U); break;
            }
            break;
          }
          // Movement control
          case MNU_MOVE:
          {
            // Which id was clicked?
            switch(X)
            {
              // Player wants to walk left
              case  0: App.Game.Object.Active->SETACTION(ACT_WALK, JOB_KEEP, DIR_L); break;
              // Player wants to jump
              case 13: App.Game.Object.Active->SETACTION(ACT_KEEP, JOB_JUMP, DIR_KEEP); break;
              // Player wants to walk right
              case  1: App.Game.Object.Active->SETACTION(ACT_WALK, JOB_KEEP, DIR_R); break;
              // Player wants to stop and reset job
              case 10: App.Game.Object.Active->SETACTION(ACT_STOP, JOB_NONE, DIR_NONE); break;
              // Player wants to run left
              case  3: App.Game.Object.Active->SETACTION(ACT_RUN, JOB_KEEP, DIR_L); break;
              // Player wants to stop (keeping current job)
              case  2: App.Game.Object.Active->SETACTION(ACT_STOP, JOB_KEEP, DIR_KEEP); break;
              // Player wants to run right
              case  4: App.Game.Object.Active->SETACTION(ACT_RUN, JOB_KEEP, DIR_R); break;
              // Player wants the main menu
              case 21: SetControlMenu(MNU_MAIN); break;
            }
            break;
          }
          // Digging control
          case MNU_DIG:
          {
            // Which id was clicked?
            switch(X)
            {
              // Player wants to dig up-left
              case 23: App.Game.Object.Active->SETACTION(ACT_WALK, JOB_DIG, DIR_UL); break;
              // Player wants to dig left
              case  0: App.Game.Object.Active->SETACTION(ACT_WALK, JOB_DIG, DIR_L); break;
              // Player wants to dig right
              case  1: App.Game.Object.Active->SETACTION(ACT_WALK, JOB_DIG, DIR_R); break;
              // Player wants to dig up-right
              case 25: App.Game.Object.Active->SETACTION(ACT_WALK, JOB_DIG, DIR_UR); break;
              // Player wants to dig down-left
              case 24: App.Game.Object.Active->SETACTION(ACT_WALK, JOB_DIG, DIR_DL); break;
              // Player wants to dig down
              case 14: App.Game.Object.Active->SETACTION(ACT_WALK, JOB_DIGDOWN, DIR_TCTR); break;
              // Player wants the main menu
              case 21: SetControlMenu(MNU_MAIN); break;
              // Player wants to dig down-right
              case 26: App.Game.Object.Active->SETACTION(ACT_WALK, JOB_DIG, DIR_DR); break;
            }
            break;
          }
          // Train drop item control
          case MNU_TRDROP:
          {
            // Player wants the main menu
            if(X == 28)
            {
              // Set train main menu
              SetControlMenu(MNU_TRAIN);
              // Done
              break;
            }
            // Fall through
          }
          // Drop item control
          case MNU_DROP:
          {
            // Which id was clicked?
            switch(X)
            {
              // Player wants the next item in the inventory
              case 27:
              {
                // There is an object to drop?
                if(DropObject)
                {
                  // If there is another object in inventory
                  if(DropObject->Next)
                    // Cycle to next object
                    DropObject = DropObject->Next;
                  else
                    // Else back to first object
                    DropObject = DropObjectFirst;
                }
                else
                  // Play error sound and ignore the command
                  App.Audio.Sfx.Sound[SFX_ERROR]->PLAY(0);
                break;
              }
              // Player wants to drop the visible item
              case 29:
              case 30:
              case 31:
              case 32:
              {
                // There is an object to drop?
                if(DropObject)
                {
                  // Record the dropped object because the DropObject var needs to
                  // be updated before the DROPINVENTORY function is called.
                  register POBJECT Object = DropObject;
                  // If there is another object in inventory
                  if(DropObject->Next)
                    // Cycle to next object
                    DropObject = DropObject->Next;
                  // If there is previous object in inventory
                  else if(DropObject->Prev)
                    // Cycle to previous object
                    DropObject = DropObject->Prev;
                  else
                    // Else no objects left in inventory
                    DropObject = DropObjectFirst = NULL;
                  // Drop the object
                  App.Game.Object.Active->DROPINVENTORY(Object);
                }
                else
                  // Play error sound and ignore command
                  App.Audio.Sfx.Sound[SFX_ERROR]->PLAY(0);
                // Done
                break;
              }
              // Player wants the main menu
              case 28: SetControlMenu(MNU_MAIN); break;
            }
            // Done;
            break;
          }
          // Trade menu control
          case MNU_TRADE:
          {
            // Which id was clicked?
            switch(X)
            {
              // Buy items
              case 34: SetControlMenu(MNU_BUY); break;
              // Sell items
              case 35:
              {
                // Set first item in active objects inventory
                DropObject = DropObjectFirst = App.Game.Object.Active->Items;
                // Set sell control menu
                SetControlMenu(MNU_SELL);
                // Calculate interest (1 zog for every 5 minutes)
                App.Game.Interest = App.Game.Time / 18000;
                break;
              }
              // Game status
              case  7: break;
              // Rest
              case 10: App.Audio.Sfx.Sound[App.Game.Object.Active->REST() ? SFX_TRADE : SFX_ERROR]->PLAY(0); break;
              // Previous menu
              case 21: SetControlMenu(MNU_MAIN); break;
              // Ressurect
              case 16: App.Audio.Sfx.Sound[App.Game.Player.This->RESSURECTDIGGER(TRUE) ? SFX_TRADE : SFX_ERROR]->PLAY(0); break;
            }
            // Done;
            break;
          }
          // Buy item control
          case MNU_BUY:
          {
            // Which id was clicked?
            switch(X)
            {
              // Player wants the next item in the inventory
              case 27:
              {
                // Create pointer to selected object
                register struct OBJDATA *SelectedObject = NULL;
                // Keep going until we have an object selected
                while(!SelectedObject)
                {
                  // Find first purchasable object
                  for(++BuyObject; BuyObject < TYP_MAX; BuyObject++)
                  {
                    // Alias selected object
                    SelectedObject = &ObjectData[BuyObject];
                    // If selected object is purchasable
                    if(SelectedObject->Flags & OFL_PURCHASABLE)
                      // Done
                      break;
                    // Keep searching for a purchasable object
                  }
                  // End of loop?
                  if(BuyObject == TYP_MAX)
                  {
                    // Try again from start
                    BuyObject = 0;
                    // Nullify selected object
                    SelectedObject = NULL;
                  }
                }
                break;
              }
              // Player wants to buy the visible item
              case 29:
              case 30:
              case 31:
              case 32:
              {
               // Sell item and play appropriate sound
               App.Audio.Sfx.Sound[App.Game.Object.Active->PURCHASEITEM((OBJTYP)BuyObject) ? SFX_TRADE : SFX_ERROR]->PLAY(0);
               // Done
               break;
              }
              // Player wants the trade menu
              case 28: SetControlMenu(MNU_TRADE); break;
            }
            // Done;
            break;
          }
          // Sell item control
          case MNU_SELL:
          {
            // Which id was clicked?
            switch(X)
            {
              // Player wants the next item in the inventory
              case 27:
              {
                // There is an object to drop?
                if(DropObject)
                {
                  // If there is another object in inventory
                  if(DropObject->Next)
                    // Cycle to next object
                    DropObject = DropObject->Next;
                  else
                    // Else back to first object
                    DropObject = DropObjectFirst;
                  // Done
                }
                else
                  // Play error sound and ignore the command
                  App.Audio.Sfx.Sound[SFX_ERROR]->PLAY(0);
                break;
              }
              // Player wants to sell the visible item
              case 29:
              case 30:
              case 31:
              case 32:
              {
                // There is an object to sell?
                if(DropObject)
                {
                  // Record the dropped object because the DropObject var needs to
                  // be updated before the DROPINVENTORY function is called.
                  register POBJECT Object = DropObject;
                  // If there is another object in inventory
                  if(DropObject->Next)
                    // Cycle to next object
                    DropObject = DropObject->Next;
                  // If there is previous object in inventory
                  else if(DropObject->Prev)
                    // Cycle to previous object
                    DropObject = DropObject->Prev;
                  else
                    // Else no objects left in inventory
                    DropObject = DropObjectFirst = NULL;
                  // Sell the object
                  App.Game.Object.Active->SELLITEM(Object);
                  // Play trade sound
                  App.Audio.Sfx.Sound[SFX_TRADE]->PLAY(0);
                }
                else
                  // Play error sound and ignore command
                  App.Audio.Sfx.Sound[SFX_ERROR]->PLAY(0);
                // Done
                break;
              }
              // Player wants the trade menu
              case 28: SetControlMenu(MNU_TRADE); break;
            }
            // Done;
            break;
          }
          // Tunneler menu control
          case MNU_TUNNEL:
          {
            // Which id was clicked?
            switch(X)
            {
              // Player wants to dig left
              case  0: App.Game.Object.Active->SETACTION(ACT_WALK, JOB_DIG, DIR_L); break;
              // Player wants to dig right
              case  1: App.Game.Object.Active->SETACTION(ACT_WALK, JOB_DIG, DIR_R); break;
              // Player wants to stop
              case  2: App.Game.Object.Active->SETACTION(ACT_STOP, JOB_NONE, DIR_NONE); break;
            }
            // Done
            break;
          }
          // Vertical tunneler menu control
          case MNU_CORK:
          {
            // Which id was clicked?
            switch(X)
            {
              // Player wants to dig left
              case  0: App.Game.Object.Active->SETACTION(ACT_CREEP, JOB_NONE, DIR_L); break;
              // Player wants to dig right
              case  1: App.Game.Object.Active->SETACTION(ACT_CREEP, JOB_NONE, DIR_R); break;
              // Player wants to dig down
              case 14: App.Game.Object.Active->SETACTION(ACT_CREEP, JOB_DIGDOWN, DIR_TCTR); break;
              // Player wants to stop
              case  2: App.Game.Object.Active->SETACTION(ACT_STOP, JOB_NONE, DIR_NONE); break;
            }
            // Done
            break;
          }
          // Explosive menu control
          case MNU_TNT:
          {
            // Only one button in it, so no bother checking
            App.Game.Object.Active->SETACTION(ACT_PHASE, JOB_EXPLODE, DIR_NONE);
            // Change ownership to neutral
            App.Game.Object.Active->Owner = NULL;
            // Close menu
            SetControlMenu(MNU_NONE);
            // Done
            break;
          }
          // Train control
          case MNU_TRAIN:
          {
            // Which id was clicked?
            switch(X)
            {
              // Player wants to pickup the item underneith it
              case  5: if(!App.Game.Object.Active->PICKUPITEM(TRUE)) App.Audio.Sfx.Sound[SFX_ERROR]->PLAY(0); break;
              // Player wants to drop an item it is holding
              case  6: DropObject = DropObjectFirst = App.Game.Object.Active->Items; SetControlMenu(MNU_TRDROP); break;
              // Player wants to search for treasure
              case  9: App.Game.Object.Active->SETACTION(ACT_WALK, JOB_SEARCH, DIR_R); break;
              // Player wants to walk left
              case  0: App.Game.Object.Active->SETACTION(ACT_WALK, JOB_KEEP, DIR_L); break;
              // Player wants to walk right
              case  1: App.Game.Object.Active->SETACTION(ACT_WALK, JOB_KEEP, DIR_R); break;
              // Player wants to stop and reset job
              case  2: App.Game.Object.Active->SETACTION(ACT_STOP, JOB_NONE, DIR_NONE); break;
            }
            // Done
            break;
          }
          // Deploy menu
          case MNU_DEPLOY:
          {
            // If object cant be deployed?
            if(!App.Game.Object.Active->DEPLOY())
              // Play error sound
              App.Audio.Sfx.Sound[SFX_ERROR]->PLAY(0);
            // Done
            break;
          }
          // Flood gate
          case MNU_GATE:
          {
            // Which id was clicked?
            switch(X)
            {
              // Close gate?
              case 17:
              {
                // close gate
                App.Game.Object.Active->GATECONTROL(FALSE);
                // Done
                break;
              }
              // Open gate?
              case 18:
              {
                // Open gate
                App.Game.Object.Active->GATECONTROL(TRUE);
                // Done
                break;
              }
            }
            // Done
            break;
          }
          // Flotation device
          case MNU_FLOAT:
          {
            // Which id was clicked?
            switch(X)
            {
              // Move left
              case 0: App.Game.Object.Active->SETACTION(ACT_CREEP, JOB_NONE, DIR_L); break;
              // Stop
              case 2: App.Game.Object.Active->SETACTION(ACT_STOP, JOB_NONE, DIR_NONE); break;
              // Move right
              case 1: App.Game.Object.Active->SETACTION(ACT_CREEP, JOB_NONE, DIR_R); break;
            }
            // Done
            break;
          }
          // Elevator
          case MNU_LIFT:
          {
            // Which id was clicked?
            switch(X)
            {
              // Move up
              case 13: App.Game.Object.Active->SETACTION(ACT_CREEP, JOB_NONE, DIR_U); break;
              // Stop
              case 2: App.Game.Object.Active->SETACTION(ACT_STOP, JOB_NONE, DIR_D); break;
              // Move down
              case 14: App.Game.Object.Active->SETACTION(ACT_CREEP, JOB_NONE, DIR_D); break;
            }
            // Done
            break;
          }
        }
      }
    }
    // Nothing in the interface was selected, so check if we clicked an object
    else
    {
      // Create pointer to last object in list
      register POBJECT Object;
      // Walk objects in descending order
      for(Object = App.Game.Object.Last; Object; Object = Object->Prev)
      {
        // Is mouse cursor not in the bounds of the object?
        if(!Object->GETMCOLLISION())
          // Try next object
          continue;
        // Play select sound
        App.Audio.Sfx.Sound[SFX_SELECT]->PLAY(0);
        // Set current object to this object
        Object->SELECT();
        // No need to check anymore objects
        break;
      }
      // No object was in mouse cursors co-ordinates
      if(!Object)
      {
        // Disable menu
        SetControlMenu(MNU_NONE);
        // De-select object
        App.Game.Object.Active = NULL;
      }
    }
  }
  // Right mouse button clicked?
  else if(App.Input.Mouse.State[MID_RMB])
  {
    // No control menu open, object selected and right mouse button pressed once?
    if(!App.Game.ObjectMenu && App.Game.Object.Active && App.Input.Mouse.State[MID_RMB] == 1)
    {
      // Is the client owner of the object and the object isnt dying and
      // there is a menu set for the object?
      if(App.Game.Object.Active->Owner == App.Game.Player.This && App.Game.Object.Active->ObjData->Menu != MNU_NONE && App.Game.Object.Active->Action != ACT_DEATH && App.Game.Object.Active->Action != ACT_EATEN)
      {
        // Open control menu
        SetControlMenu(App.Game.Object.Active->ObjData->Menu);
        // Play click sound
        App.Audio.Sfx.Sound[SFX_CLICK]->PLAY(0);
      }
      // Client player is not the owner of this object so play error sound
      else App.Audio.Sfx.Sound[SFX_ERROR]->PLAY(0);
    }
    // Control menu is open
    if(App.Game.ObjectMenu)
    {
      // Set new X menu position with bounds checking
      App.Game.ObjectMenuPos.X = App.Input.Mouse.Pos.X + (App.Game.ObjectMenu->Width * TILE_SIZE) > ASPECT_WIDTH ? ASPECT_WIDTH - (App.Game.ObjectMenu->Width * TILE_SIZE) : App.Input.Mouse.Pos.X;
      // Set new Y menu position with bounds checking
      App.Game.ObjectMenuPos.Y = App.Input.Mouse.Pos.Y + (App.Game.ObjectMenu->Height * TILE_SIZE) > ASPECT_HEIGHT ? ASPECT_HEIGHT - (App.Game.ObjectMenu->Height * TILE_SIZE) : App.Input.Mouse.Pos.Y;
    }
  }

  // Viewport needs scrolling right?
  if(App.Game.VPointAdj.X < 0)
  {
    // Increase X adjust
    ++App.Game.VPointAdj.X;
    // Scroll viewport right
    SetViewport(App.Game.ViewPoint.X - 1, App.Game.ViewPoint.Y);
  }
  // Viewport needs scrolling left?
  else if(App.Game.VPointAdj.X > 0)
  {
    // Decrease X adjust
    --App.Game.VPointAdj.X;
    // Scroll viewport left
    SetViewport(App.Game.ViewPoint.X + 1, App.Game.ViewPoint.Y);
  }
  // Viewport needs scrolling down?
  if(App.Game.VPointAdj.Y < 0)
  {
    // Increase Y adjust
    ++App.Game.VPointAdj.Y;
    // Scroll viewport down
    SetViewport(App.Game.ViewPoint.X, App.Game.ViewPoint.Y - 1);
  }
  // Viewport needs scrolling up?
  else if(App.Game.VPointAdj.Y > 0)
  {
    // Decrease Y adjust
    --App.Game.VPointAdj.Y;
    // Scroll viewport up
    SetViewport(App.Game.ViewPoint.X, App.Game.ViewPoint.Y + 1);
  }

  // Draw terrain
  DrawTerrain(TRUE);

  // App.Game.Object.First exist?
  if(App.Game.Object.First)
  {
    // Pointer to the first object in the list
    register POBJECT Object = App.Game.Object.First;
    // For each object, process object gameplay and catch next object
    do Object = Object->MAIN();
      // While there are still objects to process
      while(Object);
  }

  // Create counter
  register INT Index;
  // Draw digger status graphics
  for(Index = 0; Index < 7; ++Index)
    App.Video.Tex.Interface->BLTFAST(  8 + Index * TILE_SIZE, 216, 71 + Index);
  // Animate money value
  if(App.Game.Player.ThisMoney != App.Game.Player.This->Money)
    for(Index = 10; Index >= 1; Index /= 10)
      if(App.Game.Player.ThisMoney + Index <= App.Game.Player.This->Money)
      {
        App.Game.Player.ThisMoney += Index;
        break;
      }
      else if(App.Game.Player.ThisMoney - Index >= App.Game.Player.This->Money)
      {
        App.Game.Player.ThisMoney -= Index;
        break;
      }

  // Print player money
  App.Video.Tex.FontSmall->PRINTF(15, 220, FALSE, "%04u", App.Game.Player.ThisMoney);

  // This is storage for which object status block to draw on the HUD
  register USHORT Id = 83;
  // If object is selected
  if(App.Game.Object.Active)
  {
    // If selected object is a digger
    if(App.Game.Object.Active->ObjData->Flags & OFL_DIGGER)
      // Draw heartbeat if this is a digger
      App.Video.Tex.Interface->BLTFAST(47, 216, (App.Game.Time >> 3) % 4 + 47);
    // Object belongs to me?
    if(App.Game.Object.Active->Owner == App.Game.Player.This)
    {
      // Draw miniture inventory display above health bar
      if(App.Game.Object.Active->Items)
      {
        // Var for X coordinate of treasure icon
        register UCHAR X = 61;
        // Create pointer to items
        register POBJECT Item = App.Game.Object.Active->Items;
        // Walk inventory items
        while(Item)
        {
          // Is treasure?
          if(Item->ObjData->Flags & OFL_TREASURE)
          {
            // Draw icon
            App.Video.Tex.Interface->BLTFAST(X, 217, 92 + (Item->Type - TYP_JENNITE));
            // Increase X coordinate
            X += 8;
          }
          // Set next item
          Item = Item->Next;
        }
      }

      // If health is over 0
      if(App.Game.Object.Active->Health >> 1 > 0)
        // Draw health bar
        App.Video.Tex.Aux->BLTFASTPARTIAL(61, 227, App.Game.Object.Active->Health >> 1, 0);

      // Compare action
      switch(App.Game.Object.Active->Action)
      {
        // Stopped? Draw stopped icon
        case ACT_STOP: Id = 84; break;
        // Running? Fall through
        case ACT_RUN:
        // Walking? Draw moving icon
        case ACT_WALK: Id = 85; break;
        // Phasing? Draw phase icon
        case ACT_PHASE: Id = 91; break;
        // Digging? Draw digging icon
        case ACT_DIG: Id = 87; break;
        // Fighting? Draw fighting icon
        case ACT_FIGHT: Id = 90; break;
      }
      // Compare job
      switch(App.Game.Object.Active->Job)
      {
        // Going home? Draw home icon
        case JOB_HOME: Id = 88; break;
        // Searching for treasure? Draw search icon
        case JOB_SEARCH: Id = 89; break;
      }
      // Jumping?
      if(App.Game.Object.Active->Flags & OFL_JUMPRISE || App.Game.Object.Active->Flags & OFL_JUMPFALL)
        // Draw jumping icon
        Id = 86;
    }
    // Draw status indicator
    App.Game.Object.Active->DRAWINDICATOR();
  }
  // Draw the current action of the digger on the hud
  App.Video.Tex.Interface->BLTFAST(120, 216, Id);

  // Walk through each digger for this client player
  for(register INT X = 0, Z = 144; X < NUM_DIGGERS; ++X, Z += TILE_SIZE)
  {
    // Create pointer to digger
    register POBJECT Object = App.Game.Player.This->Diggers[X];
    // Digger exists?
    if(!Object)
    {
      // Just draw the button
      App.Video.Tex.Interface->BLTFAST(Z, 216, 54 + X);
      // Try next digger
      continue;
    }
    // Draw numbered button and highlight it, if this digger is selected
    App.Video.Tex.Interface->BLTFAST(Z, 216, 54 + (App.Game.Object.Active == Object ? X + NUM_DIGGERS : X));
    // Object isnt in danger?
    if(Object->Job != JOB_INDANGER)
    {
      // Draw appropriate status indicator
      App.Video.Tex.Interface->BLTFAST(Z, 204, Object->Flags & OFL_BUSY ? 80 : Object->Action != ACT_STOP ? 79 : 78);
      // Try next digger
      continue;
    }
    // Paint alternate danger icon
    App.Video.Tex.Interface->BLTFAST(Z, 204, App.Game.Time / 60 % 2 ? 81 : 82);
    // Each second...
    if(!(App.Game.Time % 60))
      // Play error sound
      App.Audio.Sfx.Sound[SFX_ERROR]->PLAY(0);
  }

  // Draw machinery select button
  App.Video.Tex.Interface->BLTFAST(232, 216, 64);
  // Draw player digger inventories button
  App.Video.Tex.Interface->BLTFAST(248, 216, InGameDisplay & GS_INVENTORY ? 66 : 65);
  // Draw player digger locations button
  App.Video.Tex.Interface->BLTFAST(264, 216, InGameDisplay & GS_LOCATIONS ? 68 : 67);
  // Draw status button
  App.Video.Tex.Interface->BLTFAST(280, 216, InGameDisplay & GS_GAMESTATUS ? 53 : 52);
  // Draw menu button
  App.Video.Tex.Interface->BLTFAST(296, 216, InGameDisplay & GS_HELPANDINFO ? 70 : 69);

  // Draw control menu
  if(App.Game.ObjectMenu)
  {
    // Create co-ordinate variables
    register UCHAR X, Y;
    // Create index location variable
    register USHORT Z;
    // Draw menu
    for(Y = 0; Y < App.Game.ObjectMenu->Height; ++Y)
    {
      for(X = 0; X < App.Game.ObjectMenu->Width; ++X)
      {
        // Calculate index location of tile
        Z = (Y * 8) + X;
        // Menu item is not valid?
        if(App.Game.ObjectMenu->Data[Z].Id == -1)
          // Skip
          continue;

        // Draw the control menu item
        App.Video.Tex.Interface->BLTFAST(App.Game.ObjectMenuPos.X + (X * TILE_SIZE), App.Game.ObjectMenuPos.Y + (Y * TILE_SIZE), App.Game.ObjectMenu->Data[Z].Id);
        // If selected object is busy and button should be disabled
        if(App.Game.Object.Active->Flags & OFL_BUSY && App.Game.ObjectMenu->Flags & 1 << Z)
          // Grey out the button
          App.Video.Tex.Interface->BLTFAST(App.Game.ObjectMenuPos.X + (X * TILE_SIZE), App.Game.ObjectMenuPos.Y + (Y * TILE_SIZE), 51);

        // Mouse cursor is not over this button?
        if(App.Input.Mouse.Pos.X < App.Game.ObjectMenuPos.X + (X * TILE_SIZE) ||
           App.Input.Mouse.Pos.Y < App.Game.ObjectMenuPos.Y + (Y * TILE_SIZE) ||
           App.Input.Mouse.Pos.X >= App.Game.ObjectMenuPos.X + ((X + 1) * TILE_SIZE) ||
           App.Input.Mouse.Pos.Y >= App.Game.ObjectMenuPos.Y + ((Y + 1) * TILE_SIZE))
          // Ignore
          continue;
        // Display tip
        ShowTip(App.Game.ObjectMenu->Data[Z].Tip, App.Game.Object.Active->ObjData->Name);
      }
    }
    // Perform additional induvidual tasks
    switch(App.Game.ObjectMenu->Id)
    {
      // Drop menu is currently being shown?
      case MNU_BUY:
      {
        // Alias selected object
        register struct OBJDATA *SelectedObject = &ObjectData[BuyObject];
        // Draw object
        App.Video.Tex.Sprite->BLT(App.Game.ObjectMenuPos.X + TILE_SIZE + 7, App.Game.ObjectMenuPos.Y + 4, SelectedObject->ActData[ACT_STOP].DirData[DIR_NONE].AnimStart);
        // Draw text
        App.Video.Tex.FontTiny->PRINTF(App.Game.ObjectMenuPos.X + 17, App.Game.ObjectMenuPos.Y + 23, FALSE, "%03uz %02u", SelectedObject->Value, SelectedObject->Weight);
        // Done
        break;
      }
      case MNU_SELL:
      {
        // Drop object not selected?
        if(!DropObject)
        {
          // Draw text
          App.Video.Tex.FontTiny->PRINT(App.Game.ObjectMenuPos.X + 17, App.Game.ObjectMenuPos.Y + 23, FALSE, "NO ITEM");
          // Dont do anything else
          break;
        }
        // Else show info
        else App.Video.Tex.FontTiny->PRINTF(App.Game.ObjectMenuPos.X + 17, App.Game.ObjectMenuPos.Y + 23, FALSE, "%03uz %02u", (DropObject->ObjData->Value >> 1) + App.Game.Interest, DropObject->ObjData->Weight);
        // Draw selected item to drop
        App.Video.Tex.Sprite->BLT(App.Game.ObjectMenuPos.X + TILE_SIZE + 7, App.Game.ObjectMenuPos.Y + 4, DropObject->DirData->AnimStart);
        // Done
        break;
      }
      case MNU_TRDROP:
      case MNU_DROP:
      {
        // Drop object not selected?
        if(!DropObject)
        {
          // Draw text
          App.Video.Tex.FontTiny->PRINT(App.Game.ObjectMenuPos.X + 17, App.Game.ObjectMenuPos.Y + 23, FALSE, "NO ITEM");
          // Dont do anything else
          break;
        }
        // Else show info
        else App.Video.Tex.FontTiny->PRINTF(App.Game.ObjectMenuPos.X + 17, App.Game.ObjectMenuPos.Y + 23, FALSE, "%03u%% %02u", DropObject->Health, DropObject->ObjData->Weight);
        // Draw selected item to drop
        App.Video.Tex.Sprite->BLT(App.Game.ObjectMenuPos.X + TILE_SIZE + 7, App.Game.ObjectMenuPos.Y + 4, DropObject->DirData->AnimStart);
        // Done
        break;
      }
    }
  }

  // Special display?
  if(InGameDisplay)
  {
    // Draw transparency
    App.Video.Tex.Trans->BLTFASTCUSTOM(8, 32, 0, 0, 304, 176, 0);
    // Now which display to show?
    switch(InGameDisplay)
    {
      // Status
      case GS_GAMESTATUS:
      {
        register INT   ScoreThis = 0;
        register INT   ScoreOppo = 0;
        register PCHAR Text = NULL;

        ShowTip("GAME STATUS", NULL);

        App.Video.Tex.FontLarge->PRINTF( 16,  40, FALSE, "%s", App.Game.LevelCurrent->Name);
        App.Video.Tex.FontSmall->PRINTF( 16,  56, FALSE, "%s TERRAIN", SceneNames[App.Game.LevelScene]);

        App.Video.Tex.FontLarge->PRINTF(208,  40, FALSE, "%02u:%02u:%02u", (App.Game.Time / 216000) % 24, (App.Game.Time / 3600) % 60, (App.Game.Time / 60) % 60);
        App.Video.Tex.FontSmall->PRINTF(214,  56, FALSE, "OPERATIONS TIME");

        App.Video.Tex.FontLarge->PRINTF(160,  72,  TRUE, "%s VS %s", ObjectData[App.Game.Player.This->Race].Name, ObjectData[App.Game.Player.This->Opponent->Race].Name);

        App.Video.Tex.FontSmall->PRINTF(160,  88,  TRUE, "YOU HAVE %u OF %u DIGGERS REMAINING", App.Game.Player.This->DiggerNum, NUM_DIGGERS);
        if(App.Game.Player.This->DiggerNum > App.Game.Player.This->Opponent->DiggerNum)
        {
          Text = "YOU HAVE MORE DIGGERS THEN YOUR OPPONENT";
          ++ScoreThis;
        }
        else if(App.Game.Player.This->DiggerNum < App.Game.Player.This->Opponent->DiggerNum)
        {
          Text = "YOUR OPPONENT HAS MORE DIGGERS";
          ++ScoreOppo;
        }
        else Text = "YOU AND YOUR OPPONENT HAVE EQUAL DIGGERS";
        App.Video.Tex.FontSmall->PRINT(160,  96,  TRUE, Text);

        App.Video.Tex.FontSmall->PRINTF(160,  112,  TRUE, "YOU MINED %u GEMS AND %uKG OF TERRAIN", App.Game.Player.This->GemsFound, App.Game.Player.This->DirtDug);
        if(App.Game.Player.This->DirtDug > App.Game.Player.This->Opponent->DirtDug)
          Text = "YOU HAVE MINED THE MOST TERRAIN";
        else if(App.Game.Player.This->DirtDug < App.Game.Player.This->Opponent->DirtDug)
          Text = "YOUR OPPONENT HAS MINED THE MOST TERRAIN";
        else
          Text = "YOU AND YOUR OPPONENT HAVE MINED EQUAL TERRAIN";
        App.Video.Tex.FontSmall->PRINTF(160,  120, TRUE, Text);

        if(App.Game.Player.This->GemsFound > App.Game.Player.This->Opponent->GemsFound)
          Text = "YOU HAVE FOUND THE MOST GEMS";
        else if(App.Game.Player.This->GemsFound < App.Game.Player.This->Opponent->GemsFound)
          Text = "YOUR OPPONENT HAS FOUND THE MOST GEMS";
        else
          Text = "YOU AND YOUR OPPONENT HAVE FOUND EQUAL GEMS";
        App.Video.Tex.FontSmall->PRINTF(160,  128, TRUE, Text);

        App.Video.Tex.FontSmall->PRINTF(160,  146,  TRUE, "YOU HAVE RAISED %u OF %u ZOGS", App.Game.Player.This->Money, App.Game.LevelCurrent->Required);
        if(App.Game.Player.This->Money > App.Game.Player.This->Opponent->Money)
        {
          Text = "YOU HAVE THE MOST CAPITAL";
          ++ScoreThis;
        }
        else if(App.Game.Player.This->Money < App.Game.Player.This->Opponent->Money)
        {
          Text = "YOUR OPPONENT HAS MORE CAPITAL";
          ++ScoreOppo;
        }
        else Text = "YOU AND YOUR OPPONENT HAVE EQUAL CAPITAL";
        App.Video.Tex.FontSmall->PRINT (160,  154,  TRUE, Text);
        App.Video.Tex.FontSmall->PRINTF(160,  162,  TRUE, "RAISE %u MORE ZOGS TO WIN", App.Game.LevelCurrent->Required - App.Game.Player.This->Money);

        App.Video.Tex.FontSmall->PRINT (160,  178,  TRUE, "THE TRADE CENTRE HAS PREDICTED");
        App.Video.Tex.FontLarge->PRINTF(160,  186,  TRUE, "%s IS WINNING", ScoreThis > ScoreOppo ? ObjectData[App.Game.Player.This->Race].Name : ScoreThis < ScoreOppo ? ObjectData[App.Game.Player.This->Opponent->Race].Name : "NOBODY");

        // Done
        break;
      }
      // Show locations and data of all the diggers
      case GS_LOCATIONS:
      {
        // Draw title
        ShowTip("DIGGERS LOCATIONS", NULL);
        // Draw map grid. For each Y position
        for(register INT Y = 0; Y < 11; ++Y)
          // For each X position
          for(register INT X = 0; X < 11; ++X)
            // Draw the square
            App.Video.Tex.Interface->BLTFAST(X * 15 + 141, Y * 15 + 38, 114);
        // For each digger
        for(register INT Z = 0; Z < NUM_DIGGERS; ++Z)
        {
          // Print id number of digger
          App.Video.Tex.FontLarge->PRINTF(16, Z * 32 + 40,  FALSE, "%u", Z + 1);
          // Draw colour key of digger
          App.Video.Tex.Interface->BLTFAST(31, Z * 32 + 43, 109 + Z);
          // Draw X and Y letters
          App.Video.Tex.FontLarge->PRINT(60, Z * 32 + 40,  FALSE, "X  Y");
          // Draw background of health bar
          App.Video.Tex.Aux->BLTFASTPARTIAL(24, Z * 32 + 62, 100, 1);
          // Get address of digger object
          register POBJECT Object = App.Game.Player.This->Diggers[Z];
          // Digger exists?
          if(!App.Game.Player.This->Diggers[Z])
          {
            // Draw grave icon
            App.Video.Tex.Sprite->BLTFAST(43, Z * 32 + 40, 460);
            // Draw dashes to show the data is not available
            App.Video.Tex.FontSmall->PRINTF(72, Z * 32 + 40,  FALSE, "----  ----\n\\---  \\---");
            // Skip drawing rest of the data
            continue;
          }
          // Draw digger on map
          App.Video.Tex.Interface->BLTFAST(Object->Position.X / 13 + 137, Object->Position.Y / 13 + 36, 109 + Z);
          // Draw digger position data
          App.Video.Tex.FontSmall->PRINTF(72, Z * 32 + 40,  FALSE, "%04u  %04u\n\\%03u  \\%03u", Object->Position.X, Object->Position.Y, Object->PositionAbs.X, Object->PositionAbs.Y);
          // Draw digger portrait
          App.Video.Tex.Sprite->BLTFAST(43, Z * 32 + 40, Object->AnimId);
          // Digger is alive? This is needed to prevent divide by zero error
          if(Object->Health > 0)
            // Draw amount of health the digger has
            App.Video.Tex.Aux->BLTFASTPARTIAL(24, Z * 32 + 62, Object->Health, 0);
        }
        // Done
        break;
      }
      // Display inventory of all diggers
      case GS_INVENTORY:
      {
        // Draw title
        ShowTip("DIGGERS INVENTORY", NULL);
        // For each digger
        for(register INT Z = 0; Z < NUM_DIGGERS; ++Z)
        {
          // Print id number of digger
          App.Video.Tex.FontLarge->PRINTF(16, Z * 32 + 40,  FALSE, "%u", Z + 1);
          // Draw background of health bar
          App.Video.Tex.Aux->BLTFASTPARTIAL(24, Z * 32 + 62, 100, 1);
          // Get address of digger object
          register POBJECT Object = App.Game.Player.This->Diggers[Z];
          // Digger exists?
          if(!App.Game.Player.This->Diggers[Z])
          {
            // Draw grave icon
            App.Video.Tex.Sprite->BLTFAST(31, Z * 32 + 40, 460);
            // Draw dashes for unavailable digger item data
            App.Video.Tex.FontSmall->PRINTF(48, Z * 32 + 40,  FALSE, "---%%\n----");
            // Skip drawing rest of the data
            continue;
          }
          // Draw digger item data
          App.Video.Tex.FontSmall->PRINTF(48, Z * 32 + 40,  FALSE, "%03u%%\n%04u", (INT)(FLOAT)((FLOAT)Object->ItemWeight / (FLOAT)Object->ObjData->Strength * 100.0F), Object->ItemWeight);
          // Draw digger portrait
          App.Video.Tex.Sprite->BLTFAST(31, Z * 32 + 40, Object->AnimId);
          // Digger is alive? This is needed to prevent divide by zero error
          if(Object->Health > 0)
            // Draw amount of health the digger has
            App.Video.Tex.Aux->BLTFASTPARTIAL(24, Z * 32 + 62, Object->Health, 0);
          // Draw health percentage, dirt dug and gems found value
          App.Video.Tex.FontSmall->PRINTF(132, Z * 32 + 59, FALSE, "%03u%%    %05u    %03u     %03u%%", Object->Health, Object->DirtDug, Object->GemsFound, (INT)(FLOAT)(((FLOAT)Object->LastDigTime/(FLOAT)App.Game.Time)*100));
          App.Video.Tex.FontTiny->PRINTF(162, Z * 32 + 60, FALSE, "DUG          GEMS       EFFI%%");
          // Digger has items?
          if(Object->Items)
          {
            // Variable for X position
            register INT X;
            // For each item in digger inventory
            for(Object = Object->Items, X = 78; Object; Object = Object->Next, X += 16)
              // Draw item
              App.Video.Tex.Sprite->BLTFAST(X, Z * 32 + 40, Object->AnimId);
          }
          // No inventory. Print no inventory message
          else App.Video.Tex.FontTiny->PRINTF(78, Z * 32 + 45, FALSE, "THIS %s IS NOT CARRYING ANYTHING", Object->ObjData->Name);
        }
        // Done
        break;
      }
      // Help and information
      case GS_HELPANDINFO:
      {
        // Show title
        ShowTip("HELP AND INFORMATION", NULL);
        // Check if mouse is in the area of the objects
        if(App.Input.Mouse.Pos.X >= 16 && App.Input.Mouse.Pos.Y >= 56 && App.Input.Mouse.Pos.X < 304 && App.Input.Mouse.Pos.Y < 102)
        {
          // Calculate X and Y
          register INT X = App.Input.Mouse.Pos.X / 16 * 16;
          register INT Y = (App.Input.Mouse.Pos.Y - 8) / 16 * 16 + 8;
          register INT I = ((App.Input.Mouse.Pos.X - 16) / 16) + (((App.Input.Mouse.Pos.Y - 56) / 16) * 18);
          register struct OBJDATA *D = &ObjectData[I];
          if(I < TYP_MAX && D->Name)
          {
            App.Video.Tex.Interface->BLTFAST(X, Y, 83);
            App.Video.Tex.FontSmall->PRINT(160, 40, TRUE, D->Name);
          }
          // Print object text
          else App.Video.Tex.FontSmall->PRINT(160, 40, TRUE, "NOT A VALID ITEM");

        }
        else App.Video.Tex.FontSmall->PRINT(160, 40, TRUE, "MOVE MOUSE CURSOR OVER IMAGE FOR MORE INFO");

        // Counter for loop
        register INT Index;
        // For each object
        for(Index = 0; Index < TYP_MAX; ++Index)
        {
          App.Video.Tex.Sprite->BLTFAST(16 * (Index % 18) + 16, 16 * (Index / 18) + 56, ObjectData[Index].ActData[ACT_STOP].DirData[DIR_NONE].AnimStart);
        }
        // Done
        break;
      }
    }
  }

  // Mouse is hovering over the HUD?
  if(App.Input.Mouse.Pos.X >= 8 && App.Input.Mouse.Pos.Y >= 216 && App.Input.Mouse.Pos.X < 312 && App.Input.Mouse.Pos.Y < 232)
  {
    // Mouse is hovering over the money display?
    if(App.Input.Mouse.Pos.X < 51)
      ShowTip("%uz IN BANK. RAISE %uz MORE TO WIN", App.Game.Player.This->Money, LevelInfo[App.Game.Level].Required - App.Game.Player.This->Money);
    // Mouse is hovering over the status display?
    else if(App.Input.Mouse.Pos.X < 124)
      ShowTip("STATUS OF SELECTED TARGET", NULL);
    // Mouse is hovering over the current action button?
    else if(App.Input.Mouse.Pos.X < 140)
      ShowTip("JOB OF SELECTED TARGET", NULL);
    // Game level, ignore
    else if(App.Input.Mouse.Pos.X < 148);
    // First digger
    else if(App.Input.Mouse.Pos.X < 160)
      ShowTip("SELECT FIRST %s", ObjectData[App.Game.Player.ThisRace].Name);
    // Second digger
    else if(App.Input.Mouse.Pos.X < 176)
      ShowTip("SELECT SECOND %s", ObjectData[App.Game.Player.ThisRace].Name);
    // Third digger
    else if(App.Input.Mouse.Pos.X < 192)
      ShowTip("SELECT THIRD %s", ObjectData[App.Game.Player.ThisRace].Name);
    // Fourth digger
    else if(App.Input.Mouse.Pos.X < 208)
      ShowTip("SELECT FOURTH %s", ObjectData[App.Game.Player.ThisRace].Name);
    // Fifth digger
    else if(App.Input.Mouse.Pos.X < 224)
      ShowTip("SELECT FIFTH %s", ObjectData[App.Game.Player.ThisRace].Name);
    // Game level, ignore
    else if(App.Input.Mouse.Pos.X < 232);
    // Cycle active devices
    else if(App.Input.Mouse.Pos.X < 248)
      ShowTip("CYCLE AND SELECT ACTIVE DEVICES", NULL);
    // Display inventory of all diggers
    else if(App.Input.Mouse.Pos.X < 264)
      ShowTip("INVENTORY OF ALL DIGGERS", NULL);
    // Show position of all diggers
    else if(App.Input.Mouse.Pos.X < 280)
      ShowTip("POSITION OF ALL DIGGERS", NULL);
    // Display game status
    else if(App.Input.Mouse.Pos.X < 296)
      ShowTip("GAME STATUS", NULL);
    // Display help and information
    else if(App.Input.Mouse.Pos.X < 312)
      ShowTip("HELP AND INFORMATION", NULL);
  }
}
/* --------------------------------------------------------------------------------- */
/* Quit the game                                                                     */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK QuitGame(VOID)
{
  // Break main game loop
  PostQuitMessage(0);
}
/* --------------------------------------------------------------------------------- */
/* Load level data                                                                   */
/* --------------------------------------------------------------------------------- */
VOID LoadLevelData(BOOL ResetAnim)
{
  // Set new level
  App.Game.Level = App.Game.NewLevel;
  // Alias current level data
  App.Game.LevelCurrent = &LevelInfo[App.Game.Level];

  // Clear viewpoint
  ZeroMemory(&App.Game.ViewPoint, sizeof(App.Game.ViewPoint));

  // Clear viewpoint adjustment
  ZeroMemory(&App.Game.VPointAdj, sizeof(App.Game.ViewPoint));

  // Create storage for level size
  ULONG Size;
  // Create pointer to hold script data (reuse App.Game.LevelData)
  App.Game.LevelData = (PUSHORT)LoadFile(App.Game.Level, "LEVELODATA", &Size);
  // Make sure we read the correct number of bytes
  if(Size != LEVEL_SIZE)
    // Bail out
    Exception(__LINE__, 0, "Read only %u bytes and not %u bytes from %s zone", Size, LEVEL_SIZE, App.Game.LevelCurrent->Name);
  // Scan level object data
  for(register INT X = 0; X < LEVEL_SIZE_WORD; ++X)
  {
    // No object here
    if(!App.Game.LevelData[X])
      // Goto next object
      continue;

    // Get low byte
    register UCHAR LId = LOBYTE(App.Game.LevelData[X]);
    // Get high byte
    register UCHAR HId = HIBYTE(App.Game.LevelData[X]);

    // Both id's are in range?
    if(LId >= TYP_MAX || HId >= TYP_MAX)
      // Bail out
      Exception(__LINE__, 0, "Found invalid object (l:%u,h:%u,v:%u) at %u x %u in zone %s", LId, HId, App.Game.LevelData[X], X % LEVEL_WIDTH, X / LEVEL_WIDTH, App.Game.LevelCurrent->Name);

    // Low order byte set?
    if(LId)
    {
      // Create the object specified in the low order byte
      register POBJECT TempObj = new OBJECT(NULL, (OBJTYP)LId, X % LEVEL_WIDTH * TILE_SIZE, X / LEVEL_WIDTH * TILE_SIZE, TRUE);
      // Init animation if requested
      if(ResetAnim == TRUE) TempObj->SETACTION(ACT_STOP, JOB_NONE, DIR_R);
    }
    // High order byte set?
    if(HId)
    {
      // Create the object specified in the high order byte
      register POBJECT TempObj = new OBJECT(NULL, (OBJTYP)HId, X % LEVEL_WIDTH * TILE_SIZE, X / LEVEL_WIDTH * TILE_SIZE, TRUE);
      // Init animation if requested
      if(ResetAnim == TRUE) TempObj->SETACTION(ACT_STOP, JOB_NONE, DIR_R);
    }
  }

  // Load level data
  PUSHORT LevelDataPtr = (PUSHORT)LoadFile(App.Game.Level, "LEVELDATA", &Size);
  // Make sure we read the correct number of bytes
  if(Size != LEVEL_SIZE)
    // Bail out
    Exception(__LINE__, 0, "Read only %u of %u bytes from %s zone", Size, LEVEL_SIZE, App.Game.LevelCurrent->Name);
  // We must create a duplicate of this level data as we obviously do not want to
  // modify the original data :P
  App.Game.LevelData = new USHORT[Size];
  // Duplicate data
  CopyMemory(App.Game.LevelData, LevelDataPtr, Size);
  // Update scene type (defined at App.Game.LevelData[16383])
  App.Game.LevelScene = App.Game.LevelData[(LEVEL_SIZE >> 1) - 1];
  // Replace that last character with a solid dig tile
  App.Game.LevelData[(LEVEL_SIZE >> 1) - 1] = 3;
}
/* --------------------------------------------------------------------------------- */
/* Init level                                                                        */
/* --------------------------------------------------------------------------------- */
VOID InitLevel(INT InitType, PPLAYER ThisPlayer, OBJTYP ThisPlayerRace, OBJTYP OpposingPlayerRace)
{
  // Unload current level data if there is any
  DeInitLevel();

  // Load level data
  LoadLevelData(FALSE);

  // Use a random race for player one?
  if(ThisPlayerRace == TYP_DIGRANDOM)
    // Select a random digger
    ThisPlayerRace = (OBJTYP)(Random() % 4);

  // Use a random race for player two?
  if(OpposingPlayerRace == TYP_DIGRANDOM)
  {
    OBJTYP Races[NUM_RACES]; // Holds digger races for race select
    INT Id = 0;              // For walking the above array

    // Build array
    if(ThisPlayerRace != TYP_FTARG)
      Races[Id++] = TYP_FTARG;
    if(ThisPlayerRace != TYP_GRABLIN)
      Races[Id++] = TYP_GRABLIN;
    if(ThisPlayerRace != TYP_QUARRIOR)
      Races[Id++] = TYP_QUARRIOR;
    if(ThisPlayerRace != TYP_HABBISH)
      Races[Id++] = TYP_HABBISH;

    // Pick a random digger other then player one's race
    OpposingPlayerRace = Races[Random() % Id];
  }

  // Set client player
  App.Game.Player.This = ThisPlayer;
  // Set client player race
  App.Game.Player.ThisRace = ThisPlayerRace;

  // Scan level for starting positions
  for(register UCHAR Y = 0; Y < LEVEL_WIDTH; ++Y)
  {
    for(register UCHAR X = 0; X < LEVEL_HEIGHT; ++X)
    {
      // Update level mask surface
      if(!BitBlt(App.Mask.LevelSurface, X * TILE_SIZE, Y * TILE_SIZE, TILE_SIZE, TILE_SIZE, App.Mask.Tileset, X && Y && X < LEVEL_WIDTH - 1 && Y < LEVEL_WIDTH - 1 ? App.Game.LevelData[Y * LEVEL_WIDTH + X] * TILE_SIZE : 7584, 0, SRCCOPY))
        Exception(__LINE__, GetLastError(), "Blit failed at %u by %u on level mask surface", X, Y);

      // Get tile flags
      register USHORT TileFlags = TileData[App.Game.LevelData[Y * LEVEL_WIDTH + X]];

      // Player one starting position?
      if(TileFlags & TF_PO)
      {
        App.Game.Player.One.INIT(ThisPlayerRace, X, Y, App.Game.Player.This ? FALSE : TRUE);
        SetViewport(X - (SCR_TILES_WIDTH >> 1), (Y + 3) - (SCR_TILES_HEIGHT >> 1));
      }
      // Player two starting position?
      else if(TileFlags & TF_PT)
      {
        App.Game.Player.Two.INIT(OpposingPlayerRace, X, Y, TRUE);
      }
    }
  }

  // Load textures
  LoadTextures(InitType);
  // Upload all the sounds to the sound device
  UploadSounds(FALSE);

  // Reset control menu id
  SetControlMenu(MNU_NONE);

  // Reset game time
  App.Game.Time = 0;
}
/* --------------------------------------------------------------------------------- */
/* Draw parallax background and terrain                                              */
/* --------------------------------------------------------------------------------- */
VOID DrawTerrain(BOOL Animate)
{
  // Draw parallax background
  App.Video.Tex.Parallax->BLTFAST(0, 0, 0);
  // Draw level scenery
  for(register CHAR Y = 0; Y < SCR_TILES_HEIGHT; ++Y)
    for(register CHAR X = 0; X < SCR_TILES_WIDTH; ++X)
    {
      // Lookup tile id at position X, Y
      register INT Z = (App.Game.ViewPoint.Y + Y) * LEVEL_WIDTH + App.Game.ViewPoint.X + X;
      // If delay to animate has been reached?
      if(Animate && App.Game.Time % TIMER_ANIMTERRAIN == 0)
      {
        // Tile animates?
        if(TileData[App.Game.LevelData[Z]] & TF_AB)
          // Goto next tile id
          ++App.Game.LevelData[Z];
        // Tile animates and is at the end?
        else if(TileData[App.Game.LevelData[Z]] & TF_AE)
          // Reset tile id (all terrain anims are 4 tiles)
          App.Game.LevelData[Z] -= 3;
      }

      // Draw the tile
      App.Video.Tex.Terrain->BLTFAST(X * TILE_SIZE, Y * TILE_SIZE, App.Game.LevelData[Z]);
    }
}
/* --------------------------------------------------------------------------------- */
/* DeInitialise the current level                                                    */
/* --------------------------------------------------------------------------------- */
VOID DeInitLevel(VOID)
{
  // Reset diggers array on player one
  App.Game.Player.One.RESETDIGGERS();
  // Reset diggers array on player two
  App.Game.Player.Two.RESETDIGGERS();
  // For each object
  while(App.Game.Object.First)
    // Delete object
    delete App.Game.Object.First;
  // Did we not delete every object?
  if(App.Game.Object.Count > 0)
    // Bail out!
    Exception(__LINE__, 0, "%u objects are un-accounted for", App.Game.Object.Count);
  // Clear objects, last object, active object and current device object pointers
  App.Game.Object.First = App.Game.Object.Last = App.Game.Object.Active = App.Game.Object.Device = NULL;

  // Unload level data
  if(App.Game.LevelData) { delete []App.Game.LevelData; App.Game.LevelData = NULL; }

  // Unload textures
  UnloadTextures();

  // Reset view point
  App.Game.ViewPoint.X = App.Game.ViewPoint.Y = -1;
}
/* --------------------------------------------------------------------------------- */
/* Dump primary surface to a bitmap on disk                                          */
/* --------------------------------------------------------------------------------- */
VOID ScreenShot(VOID)
{
  // Create storage for primary surface context
  HDC Surface;
  // Unlock primary direct draw surface so we can use GDI functions on it
  if(App.Video.SecondarySurface->GetDC(&Surface))
    // Not fatal error so just return
    return;
  // Create a GDI memory surface compatible with the primary surface
  HDC OutputSurface = CreateCompatibleDC(Surface);
  // Context created?
  if(OutputSurface)
  {
    // Create a bitmap from the primary surface proportional to the screen size
    HBITMAP Bitmap = CreateCompatibleBitmap(Surface, App.Video.ActiveMode->Width, App.Video.ActiveMode->Height);
    // Bitmap created?
    if(Bitmap)
    {
      // Attach bitmap to off-screen surface
      HBITMAP OldBitmap = (HBITMAP)SelectObject(OutputSurface, Bitmap);
      // Copy primary surface bits to off-screen surface
      BitBlt(OutputSurface, 0, 0, App.Video.ActiveMode->Width, App.Video.ActiveMode->Height, Surface, 0, 0, SRCCOPY);
      // Put old bitmap back into off-screen surface
      SelectObject(OutputSurface, OldBitmap);
      // Calculate size of bitmapinfo structure
      INT BitmapInfoSize = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
      // Allocate this memory
      LPBITMAPINFO BitmapInfo = (LPBITMAPINFO)new CHAR[BitmapInfoSize];
      // Prepare bitmapinfo structure
      BitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      // Fill in bitmapinfo structure
      if(GetDIBits(OutputSurface, Bitmap, 0, 0, 0, BitmapInfo, DIB_PAL_COLORS))
      {
        // Allocate DiB bit buffer
        PVOID Buffer = new CHAR[BitmapInfo->bmiHeader.biSizeImage];
        // Get the DiB bits and store them in our new buffer
        GetDIBits(OutputSurface, Bitmap, 0, BitmapInfo->bmiHeader.biHeight, Buffer, BitmapInfo, DIB_RGB_COLORS);
        // Create bitmap header structure
        BITMAPFILEHEADER BitmapHeader;
        // Fill in the bitmap file header details
        ZeroMemory(&BitmapHeader, sizeof(BitmapHeader));
        BitmapHeader.bfSize = sizeof(BITMAPFILEHEADER) + BitmapInfoSize + BitmapInfo->bmiHeader.biSizeImage;
        BitmapHeader.bfType = 0x4D42;
        BitmapHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + BitmapInfoSize;
        // Create structure that holds current date and time
        SYSTEMTIME Time;
        // Get local time, this is used in the filename
        GetLocalTime(&Time);
        // Create buffer that holds filename of screenshot
        CHAR FileName[1024];
        // Generate filename          Y   M   D    H   M   S   MS
        wsprintf(FileName, "diggers-%04u%02u%02u-%02u%02u%02u%03u.bmp", Time.wYear, Time.wMonth, Time.wDay, Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);
        // Try to create the file
        HANDLE Output = CreateFile(FileName, GENERIC_WRITE, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
        // Success?
        if(Output != INVALID_HANDLE_VALUE)
        {
          // Create storage for bytes written
          ULONG Bytes;
          // Write the bitmap file header
          WriteFile(Output, &BitmapHeader, sizeof(BITMAPFILEHEADER), &Bytes, 0);
          // Write the bitmap info header including palette
          WriteFile(Output, BitmapInfo, BitmapInfoSize, &Bytes, 0);
          // Write the DiB bits
          WriteFile(Output, Buffer, BitmapInfo->bmiHeader.biSizeImage, &Bytes, 0);
          // Success! Close the file
          CloseHandle(Output);
        }
        // Free DiB bit buffer
        delete []Buffer;
      }
      // Free bitmap info header buffer
      delete []BitmapInfo;
      // Free off-screen bitmap
      DeleteObject(Bitmap);
    }
    // Free off-screen surface
    DeleteDC(OutputSurface);
  }
  // Lock the primary surface back into video memory
  App.Video.SecondarySurface->ReleaseDC(Surface);
}
/* --------------------------------------------------------------------------------- */
/* Set current viewport on level performing checks to prevent read access violations */
/* --------------------------------------------------------------------------------- */
VOID SetViewport(INT X, INT Y)
{
  // Make sure viewport doesnt go off the left of the screen
  if(X < 0) X = 0;
  // or make sure viewport doesnt go off the right of the screen
  else if(X + SCR_TILES_WIDTH >= LEVEL_WIDTH) X = LEVEL_WIDTH - SCR_TILES_WIDTH;
  // Make sure viewport doesnt go off the top of the screen
  if(Y < 0) Y = 0;
  // or make sure viewport doesnt go off the bottom of the screen
  else if(Y + SCR_TILES_HEIGHT >= LEVEL_HEIGHT) Y = LEVEL_HEIGHT - SCR_TILES_HEIGHT;
  // Update globals
  App.Game.ViewPoint.X = X;
  App.Game.ViewPoint.Y = Y;
}
/* --------------------------------------------------------------------------------- */
/* Update the level data and mask surface                                            */
/* --------------------------------------------------------------------------------- */
VOID UpdateLevelData(INT Loc, USHORT Id)
{
  // Update level data for top part of lift
  App.Game.LevelData[Loc] = Id;
  // Update level mask surface for top part of lift
  BitBlt(App.Mask.LevelSurface, Loc % LEVEL_WIDTH * TILE_SIZE, Loc / LEVEL_WIDTH * TILE_SIZE, TILE_SIZE, TILE_SIZE, App.Mask.Tileset, Id * TILE_SIZE, 0, SRCCOPY);
}
/* --------------------------------------------------------------------------------- */
/* Set control menu                                                                  */
/* --------------------------------------------------------------------------------- */
VOID SetControlMenu(MENUID Id)
{
  // No menu was requested?
  if(Id == MNU_NONE)
  {
    // Close menu
    App.Game.ObjectMenu = NULL;
    // Done
    return;
  }
  // Set menu to requested menu
  App.Game.ObjectMenu = &MenuData[Id];
  // New menu's width would put the menu off the X axis of the screen?
  if(App.Input.Mouse.Pos.X + App.Game.ObjectMenu->Width * TILE_SIZE > ASPECT_WIDTH)
    // Update menu X position
    App.Game.ObjectMenuPos.X = ASPECT_WIDTH - App.Game.ObjectMenu->Width * TILE_SIZE;
  // New menu's height would put the menu off the Y axis of the screen?
  if(App.Input.Mouse.Pos.Y + App.Game.ObjectMenu->Height * TILE_SIZE > ASPECT_HEIGHT)
    // Update menu Y position
    App.Game.ObjectMenuPos.Y = ASPECT_HEIGHT - App.Game.ObjectMenu->Height * TILE_SIZE;
}
/* --------------------------------------------------------------------------------- */
/* Init editor proc                                                                  */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK InitEditorProc(VOID)
{
  // Load loading screen
  LoadTextures(INITTYP_LOADING);
  // Blit loading screen
  App.Video.Tex.Aux->BLTFAST(0, 0, 0);
  // Flip buffers
  App.Video.Function();

  // Unload current level data if there is any
  DeInitLevel();

  // Just load level data and reset all actions
  LoadLevelData(TRUE);

  // Load textures
  LoadTextures(INITTYP_BASE | INITTYP_FONTS | INITTYP_INGAME);
  // Upload all the sounds to the sound device
  UploadSounds(FALSE);

  // Reset control menu id
  SetControlMenu(MNU_NONE);

  // Transition in
  CreateTransition(FALSE, (GAMEFUNC)EditorProc);
}
/* --------------------------------------------------------------------------------- */
/* Editor proc                                                                       */
/* --------------------------------------------------------------------------------- */
VOID CALLBACK EditorProc(VOID)
{
  // Draw terrain
  DrawTerrain(FALSE);

  // Create pointer to objects list
  register POBJECT Object = App.Game.Object.First;
  // Create pointer to the mouse over object
  register POBJECT OverObject = NULL;

  // Objects exist
  if(Object)
  {
    // Walk objects list. Render object
    do
    {
      // Mouse is over this object?
      if(OverObject == NULL && Object->GETMCOLLISION())
      {
        // Set over object
        OverObject = Object;
        // Show tip
        ShowTip("OBJECT %s", Object->ObjData->Name);
      }
      // Render object and grab the next object
      Object = Object->RENDER();
    }
    // Goto next object
    while(Object);
  }

  // Browser activated?
  if(App.Game.Editor.Flags & EFL_BROWSER)
  {
    // Show title
    ShowTip("TILE AND OBJECT BROWSER");
    // Draw transparency
    App.Video.Tex.Trans->BLTFASTCUSTOM(8, 32, 0, 0, 304, 200, 0);
    // If escape pressed
    if(App.Input.Keyb.State[DIK_ESCAPE] & 1 || App.Input.Mouse.State[MID_MMB] & 1)
      // Close browser
      App.Game.Editor.Flags ^= EFL_BROWSER;
    // Scroll?
//    else if(App.Input.

    // Counters for tiles
    register INT X, Y;

    for(Y = 0; Y < 11; ++Y)
      for(X = 0; X < 18; ++X)
        App.Video.Tex.Terrain->BLT(16 + X * TILE_SIZE, 40 + Y * TILE_SIZE, Y * 18 + X + App.Game.Editor.BrowserStart);
  }
  // Browser not activated
  else
  {
    // If escape pressed
    if(App.Input.Keyb.State[DIK_ESCAPE] == 1)
      // Quit game
      CreateTransition(TRUE, (GAMEFUNC)QuitGame);
    // If user pressed F1?
    else if(App.Input.Keyb.State[DIK_F1] & 1 && App.Game.Level > 0)
    {
      // Previous level
      App.Game.NewLevel = App.Game.Level - 1;
      // Load previous level
      CreateTransition(TRUE, (GAMEFUNC)InitEditorProc);
    }
    // If user pressed F2?
    else if(App.Input.Keyb.State[DIK_F2] & 1 && App.Game.Level < LEVEL_NUM)
    {
      // Previous level
      App.Game.NewLevel = App.Game.Level + 1;
      // Load previous level
      CreateTransition(TRUE, (GAMEFUNC)InitEditorProc);
    }
    // Mouse is at left of screen
    else if(!App.Input.Mouse.Pos.X)
    {
      // Set left scroll arrow
      SetMouseCursor(CID_LEFT);
      // If button pressed?
      if(App.Input.Mouse.State[MID_LMB])
        // Scroll left
        SetViewport(App.Game.ViewPoint.X - 1, App.Game.ViewPoint.Y);
    }
    // Mouse is at the top of screen
    else if(!App.Input.Mouse.Pos.Y)
    {
      // Set top scroll arrow
      SetMouseCursor(CID_TOP);
      // If button pressed?
      if(App.Input.Mouse.State[MID_LMB])
        // Scroll left
        SetViewport(App.Game.ViewPoint.X, App.Game.ViewPoint.Y - 1);
    }
    // Mouse is at the right of screen
    else if(App.Input.Mouse.Pos.X == ASPECT_WIDTH - 1)
    {
      // Set right scroll arrow
      SetMouseCursor(CID_RIGHT);
      // If button pressed?
      if(App.Input.Mouse.State[MID_LMB])
        // Scroll left
        SetViewport(App.Game.ViewPoint.X + 1, App.Game.ViewPoint.Y);
    }
    // Mouse is at the bottom of screen
    else if(App.Input.Mouse.Pos.Y == ASPECT_HEIGHT - 1)
    {
      // Set right scroll arrow
      SetMouseCursor(CID_BOTTOM);
      // If button pressed?
      if(App.Input.Mouse.State[MID_LMB])
        // Scroll left
        SetViewport(App.Game.ViewPoint.X, App.Game.ViewPoint.Y + 1);
    }
    // Page up pressed or home held
    else if(App.Input.Keyb.State[DIK_HOME] || App.Input.Keyb.State[DIK_PRIOR] & 1)
      // Set previous tile
      App.Game.Editor.BrushTile = App.Game.Editor.BrushTile - 1 < 0 ? TILES_NUM - 1 : App.Game.Editor.BrushTile - 1;
    // Page down pressed or end held
    else if(App.Input.Keyb.State[DIK_END] || App.Input.Keyb.State[DIK_NEXT] & 1)
      // Set previous tile
      App.Game.Editor.BrushTile = App.Game.Editor.BrushTile + 1 >= TILES_NUM ? 0 : App.Game.Editor.BrushTile + 1;
    // Insert pressed
    else if(App.Input.Keyb.State[DIK_INSERT] & 1)
      // Set previous object
      App.Game.Editor.BrushObject = (OBJTYP)(App.Game.Editor.BrushObject - 1 < 0 ? TYP_MAX - 1 : App.Game.Editor.BrushObject - 1);
    // Delete pressed
    else if(App.Input.Keyb.State[DIK_DELETE] & 1)
      // Set next object
      App.Game.Editor.BrushObject = (OBJTYP)(App.Game.Editor.BrushObject + 1 >= TYP_MAX ? 0 : App.Game.Editor.BrushObject + 1);
    // Left or right bracket pressed
    else if(App.Input.Keyb.State[DIK_LBRACKET] & 1 || App.Input.Keyb.State[DIK_RBRACKET] & 1)
    {
      // Left bracket pressed?
      if(App.Input.Keyb.State[DIK_LBRACKET] & 1 && --App.Game.LevelScene >= SCENE_MAX) App.Game.LevelScene = SCENE_MAX - 1;
      // Right bracket pressed?
      else if(App.Input.Keyb.State[DIK_RBRACKET] & 1 && ++App.Game.LevelScene >= SCENE_MAX) App.Game.LevelScene = 0;
      // Free parallax and terrain textures
      delete App.Video.Tex.Terrain;
      delete App.Video.Tex.Parallax;
      // Reload parallax and terrain textures
      App.Video.Tex.Terrain = new TEXTURE((BMPID)(BMP_TSDESERT + App.Game.LevelScene), TILE_SIZE, TRUE);
      App.Video.Tex.Parallax = new TEXTURE((BMPID)(BMP_PLXDESERT + App.Game.LevelScene), 0, FALSE);
      // Init palette
      MergePalette(App.Video.Tex.Terrain, App.Video.Tex.Parallax);
      // Upload parallax and terrain textures
      App.Video.Tex.Terrain->UPLOAD(FALSE);
      App.Video.Tex.Parallax->UPLOAD(FALSE);
    }
    // Backspace pressed
    else if(App.Input.Keyb.State[DIK_BACK] & 1 && App.Game.Object.Active != NULL)
      // Delete the selected object
      delete App.Game.Object.Active;
    // Else
    else
    {
      // Mouse button pressed?
      if(App.Input.Mouse.State[MID_LMB])
      {
        // Mouse over an object
        if(OverObject != NULL)
        {
          // Object already selected and mouse button clicked?
          if(App.Game.Object.Active == OverObject && App.Input.Mouse.State[MID_LMB] & 1)
            // Make object the new brush
            App.Game.Editor.BrushObject = OverObject->Type;
          // Object not selected
          else
            // Select it
            OverObject->SELECT();
        }
        // Mouse not over an object
        else
        {
          // Get new tile position info
          register INT X = App.Game.ViewPoint.X + (App.Input.Mouse.Pos.X / TILE_SIZE);
          register INT Y = App.Game.ViewPoint.Y + (App.Input.Mouse.Pos.Y / TILE_SIZE);
          register INT P = (Y * LEVEL_WIDTH) + X;
          // Position changed?
          if(P != App.Game.Editor.BrushAPos)
            // Set new position
            App.Game.Editor.BrushPos.X = X,
            App.Game.Editor.BrushPos.Y = Y,
            App.Game.Editor.BrushAPos = P;
          // Else if user clicked mouse?
          else if(App.Input.Mouse.State[MID_LMB] & 1)
            // Select new brush tile
            App.Game.Editor.BrushTile = App.Game.LevelData[P];
        }
      }
      // Middle mouse button pressed?
      if(App.Input.Mouse.State[MID_MMB] & 1)
        // Toggle browser
        App.Game.Editor.Flags ^= EFL_BROWSER;
      // Right mouse button pressed
      if(App.Input.Mouse.State[MID_RMB])
        // Replace tile
        App.Game.LevelData[App.Game.Editor.BrushAPos] = App.Game.Editor.BrushTile;
      // Set arrow cursor
      SetMouseCursor(CID_ARROW);
    }

    // Print selected tile
    App.Video.Tex.Interface->BLT((App.Game.Editor.BrushPos.X - App.Game.ViewPoint.X) * TILE_SIZE, (App.Game.Editor.BrushPos.Y - App.Game.ViewPoint.Y) * TILE_SIZE, 116 + (App.Game.Time / 5 % 2));
    // Tab is not being held
    if(App.Input.Keyb.State[DIK_TAB] == 0)
    {
      App.Video.Tex.Trans->BLTFASTCUSTOM(216, 152, 32, 32, 128, 112, 0);

      // Print data
      App.Video.Tex.FontTiny->PRINT(220, 156, FALSE, "BRUSH TILE");
      App.Video.Tex.FontSmall->PRINTF(220, 164, FALSE, "%u/%03u", App.Game.LevelScene, App.Game.Editor.BrushTile);
      App.Video.Tex.Terrain->BLT(292, 156, App.Game.Editor.BrushTile);

      App.Video.Tex.FontTiny->PRINT(220, 176, FALSE, "BRUSH OBJECT");
      App.Video.Tex.FontSmall->PRINTF(220, 184, FALSE, "%02u %s", App.Game.Editor.BrushObject, ObjectData[App.Game.Editor.BrushObject].Name);
      App.Video.Tex.Sprite->BLT(292, 176, ObjectData[App.Game.Editor.BrushObject].ActData[ACT_STOP].DirData[DIR_R].AnimStart);

      App.Video.Tex.FontTiny->PRINT(220, 196, FALSE, "OBJECT SELECTED");
      if(App.Game.Object.Active != NULL)
      {
        // Print selected tile border
        App.Video.Tex.Interface->BLT((App.Game.Object.Active->PositionAbs.X - App.Game.ViewPoint.X) * TILE_SIZE, (App.Game.Object.Active->PositionAbs.Y - App.Game.ViewPoint.Y) * TILE_SIZE, 118 + (App.Game.Time / 5 % 2));
        App.Video.Tex.FontSmall->PRINTF(220, 204, FALSE, "X:%03u Y:%03u", App.Game.Object.Active->PositionAbs.X, App.Game.Object.Active->PositionAbs.Y);
        App.Video.Tex.Sprite->BLT(292, 196, App.Game.Object.Active->DirData->AnimStart);
      }
      else
        App.Video.Tex.FontSmall->PRINT(220, 204, FALSE, "NONE");

      App.Video.Tex.FontTiny->PRINTF(220, 216, FALSE, "V:%03ux%03u; M:%03ux%03u\nT:%03ux%03u (%05u) #%03u", App.Game.ViewPoint.X, App.Game.ViewPoint.Y, App.Input.Mouse.Pos.X, App.Input.Mouse.Pos.Y, App.Game.Editor.BrushPos.X, App.Game.Editor.BrushPos.Y, App.Game.Editor.BrushAPos, App.Game.LevelData[App.Game.Editor.BrushAPos]);
    }
  }
}
/* ================================================================================= */
/* CLASS FUNCTIONS - FLOODDATA::CONSTRUCTOR - Do flooding at specified position      */
/* --------------------------------------------------------------------------------- */
FLOODDATA::FLOODDATA(INT NewPos, USHORT IgnFlags)
{
  // Increase reference coutner
  ++App.Game.FloodCount;
  // Flood list already exists?
  if(App.Game.LastFlood)
  {
    // Set previous object
    Prev = App.Game.LastFlood;
    // Else set next obj of last obj to this obj
    App.Game.LastFlood->Next = this;
  }
  else
  {
    // Else set previous object to null
    Prev = NULL;
    // Set first object to this object
    App.Game.FirstFlood = this;
  }
  // Set next object to null
  Next = NULL;
  // Set last obj to this obj
  App.Game.LastFlood = this;
  // Set position
  Pos = NewPos;
  // Position out of range?
  if(Pos < 0 || Pos >= LEVEL_SIZE)
  {
    // Delete this class
    delete this;
    // Do not process this tile
    return;
  }
  // Get tile id
  Id = App.Game.LevelData[Pos];
  // Get tile flags
  Flags = TileData[Id];
  // Tile is already water or tile is solid
  if(Flags & TF_W || (~Flags & TF_EL && ~Flags & TF_ET && ~Flags & TF_ER && ~Flags & TF_EB))
  {
    // Delete this class
    delete this;
    // Do not flood
    return;
  }
  // Set flooded tile
  App.Game.LevelData[Pos] += 240;
  // Update level data
  UpdateLevelData(Pos, App.Game.LevelData[Pos]);
  // Set ignore flags
  IgnoreFlags = IgnFlags;
}
/* --------------------------------------------------------------------------------- */
/* FLOODDATA::DECONSTRUCTOR                                                          */
/* --------------------------------------------------------------------------------- */
FLOODDATA::~FLOODDATA(VOID)
{
  // Decrease reference counter
  --App.Game.FloodCount;
  // If there is a previous and a next object
  if(Prev && Next)
  {
    // Set next object of previous object to my next object
    Prev->Next = Next;
    // Set previous object of next object to my previous object
    Next->Prev = Prev;
  }
  // Else if there is a previous but not a next object (last in list)
  else if(Prev && !Next)
  {
    // Set next object of previous object to NULL
    Prev->Next = NULL;
    // Set the last object to my previous object
    App.Game.LastFlood = Prev;
  }
  // Else if there is no previous object but a next object (first in list)
  else if(!Prev && Next)
  {
    // Set the previous object of the next object to NULL
    Next->Prev = NULL;
    // Set the first object
    App.Game.FirstFlood = Next;
  }
  // Else if there is no previous and next object (Sole object)
  else
    // Set both objects and object last to NULL
    App.Game.FirstFlood = App.Game.LastFlood = NULL;
}
/* --------------------------------------------------------------------------------- */
/* FLOODDATA::SEED NEW FLOODS                                                        */
/* --------------------------------------------------------------------------------- */
VOID FLOODDATA::SEED(VOID)
{
  // Left edge is exposed? Create a new flood at left tile
  if(~IgnoreFlags & TF_EL && Flags & TF_EL) new FLOODDATA(Pos - 1, TF_ER);
  // Right edge is exposed? Create a new flood at right tile
  if(~IgnoreFlags & TF_ER && Flags & TF_ER) new FLOODDATA(Pos + 1, TF_EL);
  // Bottom edge is exposed? Create a new flood at bottom tile
  if(Flags & TF_EB) new FLOODDATA(Pos + LEVEL_WIDTH, TF_NONE);
  // This now dies
  delete this;
}
/* --------------------------------------------------------------------------------- */
/* PLAYER::INITIALISE VARIABLES                                                      */
/* --------------------------------------------------------------------------------- */
VOID PLAYER::INIT(OBJTYP nRace, INT nStartX, INT nStartY, BOOL IsAI)
{
  // Set player race (TYP_*)
  Race = nRace;
  // Set starting X position
  Start.X = nStartX * TILE_SIZE;
  // Set starting Y position
  Start.Y = (nStartY + 2) * TILE_SIZE;

  // Computer playeR?
  if(IsAI)
    // Set flag
    Flags |= PFL_AI;

  // Index for loop
  register INT Index;
  // Create NUM_DIGGERS for player
  for(Index = 0; Index < NUM_DIGGERS; ++Index)
    // Create and initialise digger for player
    CREATEDIGGER(Index);

  // Set opponent
  Opponent = this == &App.Game.Player.One ? &App.Game.Player.Two : &App.Game.Player.One;

  // Set initial money
  Money = 100;
  // Set target money to win
  Target = App.Game.LevelCurrent->Required;
}
/* --------------------------------------------------------------------------------- */
/* PLAYER::CREATE A DIGGER                                                           */
/* --------------------------------------------------------------------------------- */
POBJECT PLAYER::CREATEDIGGER(INT Index)
{
  // Digger already exists?
  if(Diggers[Index])
    // Failed
    return NULL;
  // Create digger
  Diggers[Index] = new OBJECT(this, Race, Start.X - TILE_SIZE + (Index * 5), Start.Y, Flags & PFL_AI);
  // Increment number of diggers
  ++DiggerNum;
  // Return object that was created
  return Diggers[Index];
}
/* --------------------------------------------------------------------------------- */
/* PLAYER::DIGGER HAS DIED                                                           */
/* --------------------------------------------------------------------------------- */
VOID PLAYER::DIGGERDIED(POBJECT Object)
{
  // Check which digger died
  for(register INT Index = 0; Index < NUM_DIGGERS; ++Index)
  {
    // If this object does not equal to one of the players diggers
    if(Object != Diggers[Index])
      // Skip
      continue;
    // Nullify the digger
    Diggers[Index] = NULL;
    // If reduced number of diggers isnt zero yet
    if(--DiggerNum)
      // Game isnt over yet
      break;
    // Not demo?
    if(App.Game.Function != (GAMEFUNC)InGameProc)
      // Dont do anything else
      return;
    // This player is the winner
    App.Game.Player.Remain = this;
    // Calculate financial records
    CALCINCEXP(TRUE);
    // Calculate opponents records
    Opponent->CALCINCEXP(FALSE);
    // Play music according to if this client player won or not
    MusicControl(MUSICCMD_PLAY, App.Game.Player.This == this ? MUSIC_SELECT : MUSIC_LOSE);
    // Set game end procedure
    App.Game.Function = (GAMEFUNC)GameOverProc;
    // Done
    return;
  }
}
/* --------------------------------------------------------------------------------- */
/* PLAYER::CHECK IF PLAYER HAS RAISED THE CAPTIAL                                    */
/* --------------------------------------------------------------------------------- */
VOID PLAYER::CHECKCAPITALRAISED(VOID)
{
  // Not demo? or player not raised the cash?
  if(App.Game.Function != (GAMEFUNC)InGameProc || Money < Target)
    // No
    return;
  // Set game function
  App.Game.Function = (GAMEFUNC)GameOverProc;
  // Player remaining is NULL so GameOverProc knows someone raised the money
  App.Game.Player.Remain = NULL;
  // Calculate reward for winning player
  CALCINCEXP(TRUE);
  // Calculate reward for losing player
  Opponent->CALCINCEXP(FALSE);
  // Play appropriate music according to if this client player won or not
  MusicControl(MUSICCMD_PLAY, App.Game.Player.This == this ? MUSIC_SELECT : MUSIC_LOSE);
}
/* --------------------------------------------------------------------------------- */
/* PLAYER::CALCULATE INCOME AND EXPENDATURES                                         */
/* --------------------------------------------------------------------------------- */
VOID PLAYER::CALCINCEXP(BOOL Winner)
{
  // Calculate reward
  Reward = Money / (Winner ? 2 : 10);
  // Create pointer to objects list and pointer to inventory of object
  POBJECT Object, Item;
  // Calculate assets (refund for devices carried)
  for(Object = App.Game.Object.First; Object; Object = Object->Next)
  {
    // I dont own this object or object doesnt have an inventory?
    if(Object->Owner != this || !Object->Items)
      // Ignore it
      continue;
    // For each item carried
    for(Item = Object->Items; Item; Item = Item->Next)
      // Increase assets and take a bigger cut then usual
      Assets = Assets + (Item->ObjData->Value >> 2);
  }
  // Calculate salary (5 zogs per 5 minutes for each digger)
  Salary = -(DiggerNum * ((App.Game.Time / 18000) * 5));
  // Calculate death duty (100 zogs per death)
  DeathDuty = -((NUM_DIGGERS - DiggerNum) * 100);
  // Calculate money deposited or withdrawn from the bank
  Deposit = Money + Assets + Reward + Salary + DeathDuty + Loan;
  // Adjust balance
  Balance += Deposit;
  // If the winner was this client
  if(Winner && App.Game.Player.This == this)
  {
    // This level completed
    LevelInfo[App.Game.Level].Flags &= ~LFL_UNFINISHED;
    // Unlock next level
    LevelInfo[App.Game.Level + 1].Flags &= ~LFL_LOCKED;
  }
}
/* --------------------------------------------------------------------------------- */
/* PLAYER::RESSURECT A FALLEN DIGGER                                                 */
/* --------------------------------------------------------------------------------- */
POBJECT PLAYER::RESSURECTDIGGER(BOOL NeedPayment)
{
  // Id of digger to ressurect
  register INT Id;
  // Walk diggers of player
  for(Id = 0; Id < NUM_DIGGERS; ++Id)
    // Digger is dead?
    if(!Diggers[Id])
      // Break
      break;
  // Operation failed if all diggers are alive
  if(Id == NUM_DIGGERS) return NULL;
  // Payment is required?
  if(NeedPayment)
  {
    // No funds are available?
    if(Money < ObjectData[Race].Value)
      // Ressurect failed
      return NULL;
    // Subtract funds
    Money -= ObjectData[Race].Value;
  }
  // Ressurect digger
  return CREATEDIGGER(Id);
}
/* --------------------------------------------------------------------------------- */
/* PLAYER::RESET DIGGERS                                                             */
/* --------------------------------------------------------------------------------- */
VOID PLAYER::RESETDIGGERS(VOID)
{
  // Id of digger to ressurect
  register INT Id;
  // Walk diggers of player
  for(Id = 0; Id < NUM_DIGGERS; ++Id)
  {
    // Digger is dead?
    if(!Diggers[Id])
      // Ignore
      continue;
    // Delete digger
    delete Diggers[Id];
    // Nullify address
    Diggers[Id] = NULL;
  }
  // Reset diggers counter
  DiggerNum = 0;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::CONSTRUCTOR                                                               */
/* --------------------------------------------------------------------------------- */
OBJECT::OBJECT(PPLAYER nOwner, OBJTYP nType, INT nX, INT nY, BOOL IsAI)
{
  // Increment reference counter
  ++App.Game.Object.Count;
  // Set the object type (TYP_*)
  Type = nType;
  // Set object data structure
  ObjData = &ObjectData[nType];
  // Set object animation structure (Prevent NULL reference on SETACTION)
  ActData = ObjData->ActData;
  // Set the object's owner id
  Owner = nOwner;
  // Set initial position
  SETPOS(nX, nY);
  // Compare AI type if AI is needed
  if(IsAI) switch(ObjData->AIType)
  {
    // Object is a digger
    case AI_DIGGER: AIFunc = &OBJECT::AIDIGGER; break;
    // Object does nothing but go left and right
    case AI_CRITTER: AIFunc = &OBJECT::AICRITTER; break;
    // Object slowly homes in on a digger
    case AI_FINDSLOW: AIFunc = &OBJECT::AIFIND; break;
    // Object quickly homes in on a digger
    case AI_FINDFAST: AIFunc = &OBJECT::AIFIND; break;
    // Object moves in 4 directions finding a digger
    case AI_RANDOM: AIFunc = &OBJECT::AIRANDOM; break;
    // Object moves around like a digger but steals items
    case AI_BIGFOOT: AIFunc = &OBJECT::AIBIGFOOT; break;
  }
  // Set initial behaviour
  SETACTION(ObjData->InitialAction, ObjData->InitialJob, ObjData->InitialDirection);
  // Reset fall speed
  FallSpeed = 1;
  // Initialise last dug time
  LastDigTime = App.Game.Time;
  // Initialise failed dig
  FailedDig = DIR_NONE;
  // Set object health to 100%
  ADJUSTHEALTH(100);
  // Add object to main objects list
  REGISTER();
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::DECONSTRUCTOR                                                             */
/* --------------------------------------------------------------------------------- */
OBJECT::~OBJECT(VOID)
{
  // Remove all inventory if not already done so
  DROPALLINVENTORY();
  // Unregister object from global object list
  UNREGISTER();
  // Decrement reference counter
  --App.Game.Object.Count;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::ADD THIS OBJECT TO MAIN OBJECTS LIST                                      */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::REGISTER()
{
  // App.Game.Object.First list already exists?
  if(App.Game.Object.Last)
  {
    // Set a new unique identifier
    Id = App.Game.Object.Last->Id + 1;
    // Set previous object
    Prev = App.Game.Object.Last;
    // Else set next obj of last obj to this obj
    App.Game.Object.Last->Next = this;
  }
  else
  {
    // Else set previous object to null
    Prev = 0;
    // Set unique id to 0
    Id = 0;
    // Set first object to this object
    App.Game.Object.First = this;
  }
  // Set next object to null
  Next = 0;
  // Set last obj to this obj
  App.Game.Object.Last = this;
  // Set registered flag
  Flags |= OFL_REGISTERED;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT:REMOVE THIS OBJECT FROM MAIN OBJECTS LIST                                  */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::UNREGISTER(VOID)
{
  // Object is not registered?
  if(~Flags & OFL_REGISTERED)
    // Ignore
    return;
  // This object is selected by the client?
  if(App.Game.Object.Active == this)
  {
    // Deselect
    App.Game.Object.Active = NULL;
    // Menu was open?
    if(App.Game.ObjectMenu)
      // Close it
      SetControlMenu(MNU_NONE);
  }
  // This object was selected using the cycle device button?
  if(App.Game.Object.Device == this)
    // Device cycling will start from the beginning next time the button is clicked
    App.Game.Object.Device = NULL;
  // We have to now walk all objects that are targeting this and NULL the target
  // pointer or the game is suceptable to crashing due to the object accessing
  // target properties when the target (this) has been removed.
  if(App.Game.Object.First)
  {
    // Create pointer to objects list
    register POBJECT Object = App.Game.Object.First;
    // Walk objects list and null their targets
    do
    {
      // Object is targeting this object?
      if(Object->Target == this)
        // Nullify it's target so it will find a new target
        Object->Target = NULL;
      // Get next object
      Object = Object->Next;
    }
    while(Object);
  }

  // If there is a previous and a next object
  if(Prev && Next)
  {
    // Set next object of previous object to my next object
    Prev->Next = Next;
    // Set previous object of next object to my previous object
    Next->Prev = Prev;
  }
  // Else if there is a previous but not a next object (last in list)
  else if(Prev && !Next)
  {
    // Set next object of previous object to NULL
    Prev->Next = NULL;
    // Set the last object to my previous object
    App.Game.Object.Last = Prev;
  }
  // Else if there is no previous object but a next object (first in list)
  else if(!Prev && Next)
  {
    // Set the previous object of the next object to NULL
    Next->Prev = NULL;
    // Set the first object
    App.Game.Object.First = Next;
  }
  // Else if there is no previous and next object (Sole object)
  else
    // Set both objects and object last to NULL
    App.Game.Object.First = App.Game.Object.Last = NULL;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::SET OJECT HEALTH AND PERFORM CHECKS                                       */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::ADJUSTHEALTH(INT Value)
{
  // Health would go over 100?
  if(Health + Value > 100)
    // Clamp at 100
    Health = 100;
  // Else adjust health
  else Health += Value;

  // Health is over ten percent?
  if(Health > 10)
    // Done
    return;

  // Health is over zero percent?
  if(Health > 0)
  {
    // Object is a digger and is fighting?
    if(Flags & OFL_DIGGER && Action == ACT_FIGHT)
      // Run in opposite direction to avoid death
      SETACTION(ACT_RUN, JOB_INDANGER, DIR_OPPOSITE);
    // Done
    return;
  }

  // Clamp health at zero
  Health = 0;

  // This object is selected?
  if(this == App.Game.Object.Active)
    // Remove menu
    SetControlMenu(MNU_NONE);

  // Set death animation
  SETACTION(ACT_DEATH, JOB_INDANGER, DIR_NONE);

  // Object explodes?
  if(ObjData->Flags & OFL_EXPLODE)
  {
    // Destroy centre tile
    EXPLODE( 0,  0);
    // Destroy left tile
    EXPLODE(-1,  0);
    // Destroy above tile
    EXPLODE( 0, -1);
    // Destroy right tile
    EXPLODE( 1,  0);
    // Destroy below tile
    EXPLODE( 0,  1);

    // Until all flood data items are removed
    while(App.Game.FirstFlood)
      // Seed new floods
      App.Game.FirstFlood->SEED();
  }

  // Drop all inventory
  DROPALLINVENTORY();
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::ADD SPECIFIED OBJECT TO OBJECTS INVENTORY                                 */
/* --------------------------------------------------------------------------------- */
BOOL OBJECT::ADDINVENTORY(POBJECT Object)
{
  // If we are able to carry any more weight
  if(ItemWeight + Object->ObjData->Weight > ObjData->Strength)
    // Can't pick up as its too heavy
    return FALSE;
  // Remove object from main list but do not delete it
  Object->UNREGISTER();
  // Reset objects actions
  Object->SETACTION(ACT_STOP, JOB_NONE, DIR_NONE);
  // Iventory items already exist?
  if(ItemsLast)
  {
    // Set objects previous object to last item in inventory
    // Don't worry, its safe to reuse this var as the item has been removed from
    // the global object list.
    Object->Prev = ItemsLast;
    // Set next object of last item to object that has been picked up
    ItemsLast->Next = Object;
    // Set a new id relative to items in inventory. Again safe to reuse this var
    Object->Id = Object->Prev->Id + 1;
  }
  // First inventory item that is being added
  else
  {
    // No previous object. Safe to reuse (see above)
    Object->Prev = NULL;
    // Set first item to the object that has been picked up
    Items = Object;
    // Set new id. Safe to reuse
    Object->Id = 0;
  }
  // Set the carrier of the object to this object
  Object->Carrier = this;
  // Set object's next item to NULL. Safe to reuse
  Object->Next = NULL;
  // Update last item in inventory to the object to be picked up
  ItemsLast = Object;
  // Increase weight counter
  ItemWeight += Object->ObjData->Weight;
  // Success
  return TRUE;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::DEPLOY OBJECT RETURNS TRUE IF SUCCESSFULL                                 */
/* --------------------------------------------------------------------------------- */
BOOL OBJECT::DEPLOY(VOID)
{
  BOOL DeploySuccess = FALSE; // Object failed to deploy by default

  // Compare object type
  switch(Type)
  {
    // Train track?
    case TYP_TRACK:
    {
      register BOOL Continue; // Continue or not?
      register INT Loc;       // Tile location
      register USHORT Id;     // Tile id

      // Check 5 tiles at object position and lay track
      for(register UCHAR Index = 0; Index < 5; ++Index)
      {
        Continue = TRUE;

        // Calculate absolute location of object
        Loc = ((Position.Y + 15) / TILE_SIZE) * LEVEL_WIDTH + ((Position.X + 8) / TILE_SIZE + Index);
        // Get terrain tile id the object is on
        Id = App.Game.LevelData[Loc];

        // Compare id
        switch(Id)
        {
          case 171:                  // Forward horizontal support beam
          case 172:                  // Vertical support beam
          case   7: Id = 210; break; // Clear underground
          case 150: Id = 149; break; // Lantern
          case 170: Id = 169; break; // Backward horizontal support beam
          default : Continue = FALSE; break;
        }

        // Continue?
        if(!Continue)
          // No
          break;

        // Get terrain tile id blow this tile
        register USHORT BId = App.Game.LevelData[Loc + LEVEL_WIDTH];

        // Can deploy on this
        if(TileData[BId] & TF_F)
        {
          // Update level data
          UpdateLevelData(Loc, Id);
          // Deploy successful
          DeploySuccess = TRUE;
        }
      }
      // Done
      break;
    }
    // Flood gate
    case TYP_GATE:
    {
      // Calculate absolute location of object
      register INT Loc = ((Position.Y + 15) / TILE_SIZE) * LEVEL_WIDTH + ((Position.X + 8) / TILE_SIZE);
      // Get terrain tile id below what the object is on
      register USHORT BId = App.Game.LevelData[Loc + LEVEL_WIDTH];

      // Can build on tile below?
      if(TileData[BId] & TF_F)
      {
        // Update level data
        UpdateLevelData(Loc, 438);
        // Create invisible flood gate object
        new OBJECT(Owner, TYP_GATEB, (Position.X + 8) / TILE_SIZE * TILE_SIZE, (Position.Y + 15) / TILE_SIZE * TILE_SIZE, FALSE);
        // Deploy successful
        DeploySuccess = TRUE;
      }
      // Done
      break;
    }
    // Elevator
    case TYP_LIFT:
    {
      // Calculate absolute location of object
      register INT Loc = ((Position.Y + 15) / TILE_SIZE) * LEVEL_WIDTH + ((Position.X + 8) / TILE_SIZE);
      // Create variable to hold bottom location of lift
      register INT Bottom;
      // Search for a buildable ground surface
      for(Bottom = Loc; Bottom < LEVEL_SIZE - LEVEL_WIDTH; Bottom += LEVEL_WIDTH)
      {
        // Tile has been dug
        if(TileData[App.Game.LevelData[Bottom]] & TF_AD)
          // Search next tile
          continue;
        // Tile is not firm buildable ground?
        if(~TileData[App.Game.LevelData[Bottom]] & TF_F)
          // Deploy failed
          return FALSE;
        // Done
        break;
      }
      // Could not find a valid ground surface for lift?
      if(Bottom == LEVEL_SIZE - LEVEL_WIDTH)
        // Deploy failed
        return FALSE;

      // Create variable to hold top location of lift
      register INT Top = Loc;
      // Search for a buildable ground surface
      for(; Top >= LEVEL_WIDTH; Top -= LEVEL_WIDTH)
      {
        // Tile has been dug
        if(TileData[App.Game.LevelData[Top]] & TF_AD)
          // Search next tile
          continue;
        // Tile is not firm buildable ground?
        if(~TileData[App.Game.LevelData[Top]] & TF_F)
          // Deploy failed
          return FALSE;
        // Done
        break;
      }
      // Could not find a valid ceiling surface for lift or Bottom and top
      // location is under 3 blocks?

      if(Top < LEVEL_WIDTH || Bottom - Top < 3 * LEVEL_WIDTH)
        // Deploy failed
        return FALSE;
      // We are deploying now so set success
      DeploySuccess = TRUE;

      // Update level data for top part of lift
      UpdateLevelData(Top, 62);
      // Update level data for bottom part of lift
      UpdateLevelData(Bottom, 190);

      // Draw cable
      for(Top += LEVEL_WIDTH; Top < Bottom; Top += LEVEL_WIDTH)
        // Update level data for bottom part of lift
        UpdateLevelData(Top, 189);

      // Create lift
      new OBJECT(Owner, TYP_LIFTB, (Position.X + 8) / TILE_SIZE * TILE_SIZE, (Position.Y + 15) / TILE_SIZE * TILE_SIZE, FALSE);

      // Done
      break;
    }
  }

  // If item was deployed succesfully?
  if(DeploySuccess)
    // Delete object from game
    delete this;

  // Return outcome
  return DeploySuccess;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::OPEN OR CLOSE FLOOD GATE                                                  */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::GATECONTROL(BOOL Open)
{
  // Calculate absolute location of object
  register INT Loc = PositionAbs.Y * LEVEL_WIDTH + PositionAbs.X;
  // Get tile id
  register USHORT Id = App.Game.LevelData[Loc];
  // Sound to play
  register SFXID Sound = SFX_NONE;

  // Gate is to be opened
  if(Open)
  {
    switch(Id)
    {
      // Gate closed (no water any side)
      case 434:
      {
        // Set open gate
        Id = 438;
        // Open sound
        Sound = SFX_GOPEN;
        // Done
        break;
      }
      // Gate closed (water on left side)
      case 435:
      // Gate closed (water on right side)
      case 436:
      // Gate closed (water on both sides)
      case 437:
      {
        // Set flooded open gate
        Id = 439;
        // Open sound
        Sound = SFX_GOPEN;
        // Done
        break;
      }
      // Can't open gate?
      default:
      {
        // Make no terrain change
        Id = 0;
        // Error sound
        Sound = SFX_ERROR;
      }
    }
  }
  // Gate is to be closed
  else
  {
    switch(Id)
    {
      // Gate open (water on neither side)
      case 438:
      {
        // Set non-flooded closed gate
        Id = 434;
        // Closed sound
        Sound = SFX_GCLOSE;
        // Done
        break;
      }
      // Gate open (water on both sides)
      case 439:
      {
        // Set flooded closed gate
        Id = 437;
        // Closed sound
        Sound = SFX_GOPEN;
        // Done
        break;
      }
      // Can't close gate?
      default:
      {
        // Make no terrain change
        Id = 0;
        // Error sound
        Sound = SFX_ERROR;
      }
    }
  }

  // Play appropriate sound
  App.Audio.Sfx.Sound[Sound]->PLAY(0);
  // Change the terrain?
  if(Id)
    // Update level data
    UpdateLevelData(Loc, Id);
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::DROP ALL INVENTORY                                                        */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::DROPALLINVENTORY(VOID)
{
  // Object is not carrying inventory?
  if(!Items)
    // Dont bother
    return;
  // Create pointer to inventory
  POBJECT Item = Items;
  // Walk inventory items and drop them all
  do Item = DROPINVENTORY(Item);
    // Process the next object
    while(Item);
  // Clear inventory object pointers
  Items = ItemsLast = NULL;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::REMOVE SPECIFIED OBJECT FROM OBJECTS INVENTORY AND PLACE AT FEET          */
/* --------------------------------------------------------------------------------- */
POBJECT OBJECT::DROPINVENTORY(POBJECT Object)
{
  // Record next object in list
  POBJECT ObjNext = Object->Next;
  // Set new position of object
  Object->SETPOS(Position.X, Position.Y);
  // If there is a previous and a next inventory object
  if(Object->Prev && Object->Next)
  {
    // Set next inventory object of previous inventory object to my next inventory object
    Object->Prev->Next = Object->Next;
    // Set previous inventory object of next inventory object to my previous inventory object
    Object->Next->Prev = Object->Prev;
  }
  // Else if there is a previous but not a next inventory object (last in list)
  else if(Object->Prev && !Object->Next)
  {
    // Set next inventory object of previous inventory object to NULL
    Object->Prev->Next = NULL;
    // Set the last inventory object to my previous inventory object
    ItemsLast = Object->Prev;
  }
  // Else if there is no previous inventory object but a next inventory object (first in list)
  else if(!Object->Prev && Object->Next)
  {
    // Set the previous inventory object of the next inventory object to NULL
    Object->Next->Prev = NULL;
    // Set the first inventory object
    Items = Object->Next;
  }
  // Else if there is no previous and next inventory object (Sole inventory object)
  else
    // Set both inventory objects and inventory object last to NULL
    Items = ItemsLast = NULL;
  // Reduce carry weight
  ItemWeight -= Object->ObjData->Weight;
  // Re-add to global list
  Object->REGISTER();
  // Object no longer being carried
  Object->Carrier = NULL;
  // Return next object in the objects list
  return ObjNext;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::SELL HELD ITEM TO VENDOR                                                  */
/* --------------------------------------------------------------------------------- */
POBJECT OBJECT::SELLITEM(POBJECT Object)
{
  // Record next item in inventory
  POBJECT ObjNext = Object->Next;
  // Deposit owners balance with objects value
  Owner->Money += Object->ObjData->Value / 2 + App.Game.Interest;
  // Drop item to remove it from the inventory
  DROPINVENTORY(Object);
  // Delete item completely
  delete Object;
  // Check if game is won yet for this player
  Owner->CHECKCAPITALRAISED();
  // Return next item in list
  return ObjNext;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::PURCHASE ITEM INTO INVENTORY                                              */
/* --------------------------------------------------------------------------------- */
BOOL OBJECT::PURCHASEITEM(OBJTYP Type)
{
  // Not enough funds?
  if(ObjectData[Type].Value > Owner->Money)
    // Failed
    return FALSE;
  // Create new object
  POBJECT Object = new OBJECT(Owner, Type, Position.X, Position.Y, FALSE);
  // Add object to this objects inventory
  if(ADDINVENTORY(Object))
  {
    // Deposit owners funds
    Owner->Money -= Object->ObjData->Value;
    // Success
    return TRUE;
  }
  // Remove object as it could not be added to inventory
  delete Object;
  // Failed
  return FALSE;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::IF OBJECT IS COLLIDING WITH ITEM THEN PICK IT UP                          */
/* --------------------------------------------------------------------------------- */
BOOL OBJECT::PICKUPITEM(BOOL OnlyTreasure)
{
  // App.Game.Object.First list exists?
  if(App.Game.Object.First)
  {
    // Set pointer to objects list
    POBJECT Object = App.Game.Object.First;
    // Walk the objects list
    do
    {
      // The object can be picked up and the object is colliding with this object
      if(Object != this && Object->Flags & OFL_PICKUP && !Object->Items && (!OnlyTreasure || OnlyTreasure && Object->ObjData->Flags & OFL_TREASURE) && GETCOLLISION(Object))
      {
        // Item is explosives and is lit
        if(Object->Flags & OFL_VOLATILE)
        {
          // Blow it up ;-)
          Object->ADJUSTHEALTH(-100);
          // Pretend it was picked up
          return TRUE;
        }
        // Pick object up and place it in this objects inventory
        return ADDINVENTORY(Object);
      }
      // Get next object
      Object = Object->Next;
    }
    while(Object);
  }
  // No objects colloding with this object
  return FALSE;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::SET NOT BUSY                                                              */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::SETNOTBUSY(VOID)
{
  // Unset busy flag
  Flags &= ~OFL_BUSY;
  // Ignore AI callback
  AIActiveFunc = AIFunc;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::SET BUSY                                                                  */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::SETBUSY(VOID)
{
  // Set busy flag
  Flags |= OFL_BUSY;
  // Ignore AI callback
  AIActiveFunc = NULL;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::SET OBJECT BEHAVIOUR                                                      */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::SETACTION(OBJACT nAction, OBJJOB nJob, OBJDIR nDirection)
{
  // Reset action timer
  ActionTimer = 0;

  switch(nDirection)
  {
    // Want to go left or right
    case DIR_LR:
    {
      // Randomly select left or right
      nDirection = Random() % 2 ? DIR_L : DIR_R;
      // Done
      break;
    }
    // Want object to goto the centre of the tile
    case DIR_TCTR:
    {
      // Set object walking
      nAction = ACT_WALK;
      // Set direction so it heads to the centre of the tile
      nDirection = (Position.X % TILE_SIZE) - 8 < 0 ? DIR_L : DIR_R;
      // Done
      break;
    }
    // Want to go in direction of home?
    case DIR_HOME:
    {
      // Preserve action but action stopped?
      if(nAction == ACT_KEEP && Action == ACT_STOP)
        // Set object walking
        nAction = ACT_WALK;
      // Set direction of home
      nDirection = Position.X < Owner->Start.X ? DIR_R : DIR_L;
      // Done
      break;
    }
    // Want to go in opposite direction?
    case DIR_OPPOSITE:
    {
      // Compare direction
      switch(Direction)
      {
        // Up-left -> Up-right
        case DIR_UL: nDirection = DIR_UR; break;
        // No direction set (fall through)
        case DIR_NONE:
        // Left -> Right
        case DIR_L: nDirection = DIR_R; break;
        // Down-left -> Down-right
        case DIR_DL: nDirection = DIR_DR; break;
        // Up-right -> Up-left
        case DIR_UR: nDirection = DIR_UL; break;
        // Right -> Left
        case DIR_R: nDirection = DIR_L; break;
        // Down-right -> Down-left
        case DIR_DR: nDirection = DIR_DL; break;
      }
      // Done
      break;
    }
    // Keep existing job?
    case DIR_KEEP:
    {
      // Keep existing job!
      nDirection = Direction;
      // Done
      break;
    }
  }

  // Compare job
  switch(nJob)
  {
    // Jump requested?
    case JOB_JUMP:
    {
      // Object is already jumping or falling
      if(Flags & OFL_JUMPRISE || Flags & OFL_JUMPFALL || FallDamage > 0)
        // Block action change
        return;
      // Set flags but dont modify job
      Flags |= OFL_JUMPRISE;
      // Keep existing job!
      nJob = Job;
      // Done
      break;
    }
    // In-danger?
    case JOB_INDANGER:
    {
      // Keep busy unset
      SETNOTBUSY();
      // Done
      break;
    }
    // Keep existing job?
    case JOB_KEEP:
    {
      // Keep existing job!
      nJob = Job;
      // Fall through to unset jump
    }
    // Any other job
    default:
    {
      // Remove jump flags
      Flags &= ~(OFL_JUMPRISE | OFL_JUMPFALL);
      // Done
      break;
    }
  }

  // Compare action
  switch(nAction)
  {
    // Stop requested?
    case ACT_STOP:
    {
      // Keep busy unset
      SETNOTBUSY();
      // Done
      break;
    }
    // Keep existing job?
    case ACT_KEEP:
    {
      // Keep existing action!
      nAction = Action;
      // Done
      break;
    }
  }

  // Action, direction and job change not required?
  if(Action == nAction && Direction == nDirection && Job == nJob)
    // No changes needed
    return;

  // Set new action
  Action = nAction;
  // Set new direction
  Direction = nDirection;
  // Set new job
  Job = nJob;

  // Remove all flags that are related to object actions
  Flags &= ~ActData->Flags;
  // Set action data according to lookup table
  ActData = &ObjData->ActData[Action];
  // Set directional data according to lookup table
  DirData = &ActData->DirData[Direction];
  // Does a new animation id needs to be set?
  if(AnimId < DirData->AnimStart || AnimId >= DirData->AnimEnd)
    // Set first animation frame and reset animation timer
    AnimId = DirData->AnimStart, AnimTimer = 0;
  // Re-add flags according to lookup table
  Flags |= ActData->Flags;

  // If object is now busy?
  if(Flags & OFL_BUSY)
    // Set busy
    SETBUSY();
  // Object not busy
  else
    // Unset busy
    SETNOTBUSY();

  // If sound is to be played?
  if(ActData->SoundId != SFX_NONE)
    // Play the sound
    SOUND(ActData->SoundId);

  // Object does not have attachment?
  if(ObjData->Attachment == TYP_NULL)
    // No changes needed
    return;
  // Set directional data according to lookup table for attachment
  DirDataAtt = &ObjectData[ObjData->Attachment].ActData[Action].DirData[Direction];
  // Set first animation frame of attachment
  AnimIdAtt = DirDataAtt->AnimStart;
  // Compare direction
  switch(Direction)
  {
    // Attach object at left side
    case DIR_UL: case DIR_L: case DIR_DL: AnimIdAttOff.X = -TILE_SIZE; break;
    // Attach object at upper side
    case DIR_D: case DIR_U: AnimIdAttOff.Y = -TILE_SIZE; break;
    // Attach object at right side
    case DIR_UR: case DIR_R: case DIR_DR: case DIR_NONE: AnimIdAttOff.X = TILE_SIZE; break;
  }
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::SET VIEWPOINT ON OBJECT                                                   */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::FOCUS(VOID)
{
  // This object is not selected?
  if(App.Game.Object.Active != this)
    // Ignore
    return;
  // Object is totally out of viewport?
  if(PositionAbs.X < App.Game.ViewPoint.X || PositionAbs.X >= App.Game.ViewPoint.X + SCR_TILES_WIDTH || PositionAbs.Y < App.Game.ViewPoint.Y || PositionAbs.Y >= App.Game.ViewPoint.Y + SCR_TILES_HEIGHT)
  {
    // Centre viewport on object
    SetViewport(PositionAbs.X - (SCR_TILES_WIDTH >> 1), PositionAbs.Y - (SCR_TILES_HEIGHT >> 1));
    // Done
    return;
  }
  // Object is getting close to the end of the viewport horizontally?
  if(PositionAbs.X - 4 < App.Game.ViewPoint.X || PositionAbs.X + 4 >= App.Game.ViewPoint.X + SCR_TILES_WIDTH)
    // Adjust X coordinate
    App.Game.VPointAdj.X = PositionAbs.X - (SCR_TILES_WIDTH >> 1) - App.Game.ViewPoint.X;
  // Object is getting close to the end of the viewport vertically?
  if(PositionAbs.Y - 3 < App.Game.ViewPoint.Y || PositionAbs.Y + 3 >= App.Game.ViewPoint.Y + SCR_TILES_HEIGHT)
    // Adjust Y coordinate
    App.Game.VPointAdj.Y = PositionAbs.Y - (SCR_TILES_HEIGHT >> 1) - App.Game.ViewPoint.Y;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::SET OBJECT POSITION                                                       */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::SETPOS(INT nX, INT nY)
{
  // Set pixel position
  Position.X = nX;
  Position.Y = nY;
  // Set absolute position
  PositionAbs.X = Position.X / TILE_SIZE;
  PositionAbs.Y = Position.Y / TILE_SIZE;
  // Update viewport over object
  FOCUS();
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::SET OBJECT POSITION                                                       */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::ADJUSTPOS(INT nX, INT nY)
{
  // Adjust pixel position
  Position.X += nX;
  Position.Y += nY;
  // Set absolute position
  PositionAbs.X = Position.X / TILE_SIZE;
  PositionAbs.Y = Position.Y / TILE_SIZE;
  // If i'm a platform
  if(Flags & OFL_BLOCK)
  {
    // Create pointer to object
    register POBJECT Object;
    // For each object
    for(Object = App.Game.Object.First; Object; Object = Object->Next)
      // Object inst me, object doesnt block object is
      // colliding with me and object can move to that position
      // and object isnt dying or phasing.
      if(Object != this && ~Object->Flags & OFL_BLOCK && Object->Action != ACT_PHASE && Object->Action != ACT_DEATH && GETCOLLISION(Object))
      {
        // Move that object too
        Object->MOVEX(nX);
        Object->MOVEY(nY);
      }
  }
  // Update viewport over object
  FOCUS();
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::SELECT OBJECT                                                             */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::SELECT(VOID)
{
  // Set active object
  App.Game.Object.Active = this;
  // Hide menu
  SetControlMenu(MNU_NONE);
  // Update viewport over object
  FOCUS();
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::EXPLODE TILE AT SPECIFIED OFFSET POSITION                               */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::EXPLODE(CHAR X, CHAR Y)
{
  // Check for victims :)
  register POBJECT Object;
  // Walk objects list
  for(Object = App.Game.Object.First; Object; Object = Object->Next)
    // Ppositions match?
    if(Object != this && Object->Action != ACT_DEATH && Object->PositionAbs.X == PositionAbs.X + X && Object->PositionAbs.Y == PositionAbs.Y + Y)
      // DIE! :)
      Object->ADJUSTHEALTH(-100);

  // Calculate locate of tile
  register INT Loc = (((Position.Y + 8) / TILE_SIZE) + Y) * LEVEL_WIDTH + ((Position.X + 8) / TILE_SIZE) + X;
  // Calculate tile id
  register USHORT Id = App.Game.LevelData[Loc];
  // Get tile flags
  register USHORT TFlags = TileData[Id];
  // Tile is not destructable?
  if(~TFlags & TF_D)
    // Ignore it
    return;
  // Tile has not been artificially dug yet?
  if(~TFlags & TF_AD)
  {
    // Quietly set position to that of the exploded tile so the treasure spawns
    // in the right position
    Position.X += X;
    Position.Y += Y;
    // Roll the dice and spawn treasure
    SPAWNTREASURE();
    // Quietly reset position back to original
    Position.X -= X;
    Position.Y -= Y;
  }

  // Clear block at specified location
  UpdateLevelData(Loc, 7);

  // Reuse Id;
  Id = 0;
  // Tile blown up was water or there is exposed water around the explosion?
  if(TFlags & TF_W) Id = 1;
  TFlags = TileData[App.Game.LevelData[Loc - LEVEL_WIDTH]];
  if(TFlags & TF_W && TFlags & TF_EB) Id = 1;
  TFlags = TileData[App.Game.LevelData[Loc + LEVEL_WIDTH]];
  if(TFlags & TF_W && TFlags & TF_ET) Id = 1;
  TFlags = TileData[App.Game.LevelData[Loc - 1]];
  if(TFlags & TF_W && TFlags & TF_ER) Id = 1;
  TFlags = TileData[App.Game.LevelData[Loc + 1]];
  if(TFlags & TF_W && TFlags & TF_EL) Id = 1;
  // Begin flooding
  if(Id) new FLOODDATA(Loc, TF_NONE);
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::RETURNS TRUE IF SPECIFIED OBJECT COLLIDES WITH THE MOUSE CURSOR           */
/* --------------------------------------------------------------------------------- */
BOOL OBJECT::GETMCOLLISION(VOID)
{
  // Check see if bounds are colliding first to save cpu cycles
  if(App.Game.ViewPoint.X * TILE_SIZE + App.Input.Mouse.Pos.X - Position.X >= TILE_SIZE ||
     App.Game.ViewPoint.X * TILE_SIZE + App.Input.Mouse.Pos.X - Position.X < 0 ||
     App.Game.ViewPoint.Y * TILE_SIZE + App.Input.Mouse.Pos.Y - Position.Y >= TILE_SIZE ||
     App.Game.ViewPoint.Y * TILE_SIZE + App.Input.Mouse.Pos.Y - Position.Y < 0)
    return FALSE;

  // Blit blank tile onto tile mask using first tile of terrain part
  BitBlt(App.Mask.Tile, 0, 0, TILE_SIZE, TILE_SIZE, App.Mask.Tileset, 0, 0, SRCCOPY);
  // Blit blank tile onto tile mask using first tile of terrain part
  BitBlt(App.Mask.Tile, Position.X - (App.Game.ViewPoint.X * TILE_SIZE + App.Input.Mouse.Pos.X),
                   Position.Y - (App.Game.ViewPoint.Y * TILE_SIZE + App.Input.Mouse.Pos.Y),
                   TILE_SIZE, TILE_SIZE, App.Mask.Tileset, AnimId * TILE_SIZE, TILE_SIZE, SRCCOPY);
  // Blit test objects mask onto the tile mask using SRCCOPY
  BitBlt(App.Mask.Tile, 0, 0, TILE_SIZE, TILE_SIZE, App.Mask.Tileset, 7664, TILE_SIZE, NOTSRCERASE);
  // Create buffer for bitmap bits
  register ULONG Buffer[16];
  // Copy the tile mask bits into the buffer so we can check its contents
  GetDIBits(App.Mask.Tile, App.Mask.TileBmp, 0, TILE_SIZE, Buffer, App.Mask.TileBmpInfo, DIB_RGB_COLORS);

  // Create index for scanning surface
  register INT Index;
  // Walk through the buffer. Note the buffer is formatted into 32-bit integers so
  // We can go through it is much faster then comparing 16-bit or 8-bit integers.
  // Note: It would be worth using 64-bit integers if using a 64-bit compiler.
  for(Index = 0; Index < 16; ++Index)
    // Are any bits in the array (0-31) set to 1?
    if(Buffer[Index])
      // Collision detected
      return TRUE;

  // No collision
  return FALSE;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::RETURNS TRUE IF SPECIFIED OBJECT COLLIDES WITH THIS OBJECT                */
/* --------------------------------------------------------------------------------- */
BOOL OBJECT::GETCOLLISION(POBJECT Object)
{
  // Check see if bounds are colliding first to save cpu cycles
  if(Object->Position.X - Position.X >= TILE_SIZE ||
     Object->Position.X - Position.X < -TILE_SIZE ||
     Object->Position.Y - Position.Y >= TILE_SIZE ||
     Object->Position.Y - Position.Y < -TILE_SIZE)
    return FALSE;

  // Blit blank tile onto tile mask using first tile of terrain part
  BitBlt(App.Mask.Tile, 0, 0, TILE_SIZE, TILE_SIZE, App.Mask.Tileset, 0, 0, SRCCOPY);
  // Blit test objects mask onto the tile mask using SRCCOPY
  BitBlt(App.Mask.Tile, Position.X - Object->Position.X, Position.Y - Object->Position.Y, TILE_SIZE, TILE_SIZE, App.Mask.Tileset, 7632, TILE_SIZE, SRCCOPY);
  // Blit this objects mask onto the tile mask using NOTSRCERASE
  BitBlt(App.Mask.Tile, 0, 0, TILE_SIZE, TILE_SIZE, App.Mask.Tileset, 7632, TILE_SIZE, NOTSRCERASE);
  // Create buffer for bitmap bits
  register ULONG Buffer[16];
  // Copy the tile mask bits into the buffer so we can check its contents
  GetDIBits(App.Mask.Tile, App.Mask.TileBmp, 0, TILE_SIZE, Buffer, App.Mask.TileBmpInfo, DIB_RGB_COLORS);

  // Create index for scanning surface
  register INT Index;
  // Walk through the buffer. Note the buffer is formatted into 32-bit integers so
  // We can go through it is much faster then comparing 16-bit or 8-bit integers.
  // Note: It would be worth using 64-bit integers if using a 64-bit compiler.
  for(Index = 0; Index < 16; ++Index)
    // Are any bits in the array (0-31) set to 1?
    if(Buffer[Index])
      // Collision detected
      return TRUE;

  // No collision
  return FALSE;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::RETURNS TRUE IF THIS OBJECT IS COLLIDING WITH THE BACKGROUND              */
/* --------------------------------------------------------------------------------- */
BOOL OBJECT::ISCOLLIDE(INT AdjX, INT AdjY)
{
  // Blit the appropriate part of the level mask into the tile mask
  BitBlt(App.Mask.Tile, 0, 0, TILE_SIZE, TILE_SIZE, App.Mask.LevelSurface, Position.X + AdjX, Position.Y + AdjY, SRCCOPY);

  // Blit the appropriate sprite onto the tile mask using NOTSRCERASE. This makes it
  // so only the parts of the level mask that have been overlapped by the sprite mask
  // are only drawn which makes it much easier to detect.
  BitBlt(App.Mask.Tile, 0, 0, TILE_SIZE, TILE_SIZE, App.Mask.Tileset, 7648, TILE_SIZE, NOTSRCERASE);

   // Create buffer for bitmap bits
  register ULONG Buffer[16];
  // Copy the tile mask bits into the buffer so we can check its contents
  GetDIBits(App.Mask.Tile, App.Mask.TileBmp, 0, TILE_SIZE, Buffer, App.Mask.TileBmpInfo, DIB_RGB_COLORS);

  // Create index for scanning surface
  register INT Index;
  // Walk through the buffer. Note the buffer is formatted into 32-bit integers so
  // We can go through it is much faster then comparing 16-bit or 8-bit integers.
  // Note: It would be worth using 64-bit integers if using a 64-bit compiler.
  for(Index = 0; Index < 16; ++Index)
    // Are any bits in the array (0-31) set to 1?
    if(Buffer[Index])
      // Collision detected
      return TRUE;

  // Pointer to an object
  register POBJECT Object;
  // For each object
  for(Object = App.Game.Object.First; Object; Object = Object->Next)
  {
    // Object doesnt block or object is this object or object not in bounds
    if(~Object->Flags & OFL_BLOCK || Object == this ||
      Object->Position.X - Position.X >= TILE_SIZE ||
      Object->Position.X - Position.X < -TILE_SIZE ||
      Object->Position.Y - Position.Y >= TILE_SIZE ||
      Object->Position.Y - Position.Y < -TILE_SIZE)
      // Ignore
      continue;
    // Blit blank tile onto tile mask using first tile of terrain part
    BitBlt(App.Mask.Tile, 0, 0, TILE_SIZE, TILE_SIZE, App.Mask.Tileset, 0, 0, SRCCOPY);
    // Blit test objects mask onto the tile mask using SRCCOPY
    BitBlt(App.Mask.Tile, Position.X - Object->Position.X + AdjX, Position.Y - Object->Position.Y + AdjY, TILE_SIZE, TILE_SIZE, App.Mask.Tileset, 7648, TILE_SIZE, SRCCOPY);
    // Blit this objects mask onto the tile mask using NOTSRCERASE
    BitBlt(App.Mask.Tile, 0, 0, TILE_SIZE, TILE_SIZE, App.Mask.Tileset, 7584, TILE_SIZE, NOTSRCERASE);
    // Create buffer for bitmap bits
    register ULONG Buffer[16];
    // Copy the tile mask bits into the buffer so we can check its contents
    GetDIBits(App.Mask.Tile, App.Mask.TileBmp, 0, TILE_SIZE, Buffer, App.Mask.TileBmpInfo, DIB_RGB_COLORS);

    // Create index for scanning surface
    register INT Index;
    // Walk through the buffer. Note the buffer is formatted into 32-bit integers so
    // We can go through it is much faster then comparing 16-bit or 8-bit integers.
    // Note: It would be worth using 64-bit integers if using a 64-bit compiler.
    for(Index = 0; Index < 16; ++Index)
      // Are any bits in the array (0-31) set to 1?
      if(Buffer[Index])
        // Collision detected
        return TRUE;
  }

  // No collision
  return FALSE;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::DRAW STATUS INDICATOR ABOVE THE OBJECT                                    */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::DRAWINDICATOR(VOID)
{
  // Which indicator to draw ('v' is default, free to control)?
  register USHORT Id = 475;
  // Object does not have an owner?
  if(!Owner)
    // Draw 'O' indicator above object to indicate only the program can control this
    Id = 467;
  // If object has an owner but this client is not that owner?
  else if(Owner != App.Game.Player.This)
    // Draw 'X' indicator above object to indicate only the owner can control this
    Id = 463;
  // If object is busy?
  else if(Flags & OFL_BUSY)
    // Draw 'Zz' indicator above object to indicate control is temporarily disabled
    Id = 471;
  // Draw the indicator
  App.Video.Tex.Interface->BLT(Position.X - App.Game.ViewPoint.X * TILE_SIZE, Position.Y - TILE_SIZE - App.Game.ViewPoint.Y * TILE_SIZE, Id + ((App.Game.Time / 5) % 4));
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::RENDER OBJECT                                                             */
/* --------------------------------------------------------------------------------- */
POBJECT OBJECT::RENDER(VOID)
{
  // Draw the object (PutTile will handle if out of bounds)
  App.Video.Tex.Sprite->BLT(Position.X - App.Game.ViewPoint.X * TILE_SIZE + ActData->OffsetX, Position.Y - App.Game.ViewPoint.Y * TILE_SIZE + ActData->OffsetY, AnimId);
  // If the object has an attachment, Draw the attached object too
  if(AnimIdAtt) App.Video.Tex.Sprite->BLT(Position.X - App.Game.ViewPoint.X * TILE_SIZE + ActData->OffsetX + AnimIdAttOff.X, Position.Y - App.Game.ViewPoint.Y * TILE_SIZE + ActData->OffsetY + AnimIdAttOff.Y, AnimIdAtt);
  // Return the next object in the list
  return Next;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::MOVE OBJECT VIRTICALLY                                                  */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::MOVEY(INT AdjY)
{
  // Object can move up or down?
  if(!ISCOLLIDE(0, AdjY))
  {
    // Move it
    ADJUSTPOS(0, AdjY);
    // Done
    return;
  }
  // Blocked
  SETACTION(ACT_STOP, JOB_NONE, DIR_D);
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::MOVE OBJECT HORIZONTALLY                                                  */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::MOVEX(INT AdjX)
{
  // Object is jumping or floating?
  if(Flags & OFL_JUMPRISE || Flags & OFL_JUMPFALL || Flags & OFL_FLOATING)
  {
    // Object is jumping and move to next horizontal pixel?
    if(!ISCOLLIDE(AdjX, 0))
      // Move horizontally
      ADJUSTPOS(AdjX, 0);
    // Done
    return;
  }

  // Search for slopes
  register INT Index;
  // Try walking up to walking down a steep slope
  for(Index = 2; Index >= -2; --Index)
    // Can move to this pixel?
    if(!ISCOLLIDE(AdjX, Index))
    {
      // Walk up the steep slope
      ADJUSTPOS(AdjX, Index);
      // Done
      return;
    }

  // Object is being blocked so perform job
  switch(Job)
  {
    // Object is to dig? Start digging
    case JOB_DIG: SETACTION(ACT_DIG, JOB_DIG, DIR_KEEP); return;
    // Looting? Move in opposite direction
    case JOB_SEARCH: SETACTION(ACT_KEEP, JOB_SEARCH, DIR_OPPOSITE); return;
    // Object is in danger? Walk in opposite direction
    case JOB_INDANGER: SETACTION(ACT_WALK, JOB_BOUNCE, DIR_OPPOSITE); return;
    // Bounce? Move in opposite direction
    case JOB_BOUNCE: SETACTION(ACT_KEEP, JOB_BOUNCE, DIR_OPPOSITE); return;
    // Any other job? Stop the object completely
    default: SETACTION(ACT_STOP, JOB_NONE, DIR_NONE); return;
  }
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::PLAY SOUND AT OBJECT                                                      */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::SOUND(SFXID Id)
{
  // Object is in clients viewing area and in game?
  if(PositionAbs.X >= App.Game.ViewPoint.X && PositionAbs.Y >= App.Game.ViewPoint.Y && PositionAbs.X < App.Game.ViewPoint.X + SCR_TILES_WIDTH && PositionAbs.Y < App.Game.ViewPoint.Y + SCR_TILES_HEIGHT)
    // Play the sound
    App.Audio.Sfx.Sound[Id]->PLAY((Position.X - (App.Game.ViewPoint.X + 10) * TILE_SIZE) * 5);
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::DIG TERRAIN AT OBJECT                                                     */
/* --------------------------------------------------------------------------------- */
BOOL OBJECT::DIG(VOID)
{
  // Location of tile and location of tile (in centre)
  INT Loc, CLoc;
  // Initialise values
  Loc = CLoc = 0;
  // What direction is the object going in
  switch(Direction)
  {
    // Object wants to dig up-left?
    case DIR_UL:
    {
      // Get the tile id that the object is on now
      CLoc = (((Position.Y + 15) / TILE_SIZE) * LEVEL_WIDTH) + ((Position.X + 8) / TILE_SIZE);
      // Fall through
    }
    // Object wants to dig left?
    case DIR_L:
    {
      // Get the tile which is to the left of the object
      Loc = (((Position.Y + 15) / TILE_SIZE) * LEVEL_WIDTH) + ((Position.X + 5) / TILE_SIZE);
      break;
    }
    // Object wants to dig down-left?
    case DIR_DL:
    {
      // Get the tile the object is on now
      CLoc = (((Position.Y + 1) / TILE_SIZE) * LEVEL_WIDTH) + ((Position.X + 8) / TILE_SIZE);
      // Get the tile that is to the left of the object
      Loc = (((Position.Y + 1) / TILE_SIZE) * LEVEL_WIDTH) + ((Position.X + 5) / TILE_SIZE);
      break;
    }
    // Object wants to dig up-right
    case DIR_UR:
    {
      // Get the tile the object is on now
      CLoc = (((Position.Y + 15) / TILE_SIZE) * LEVEL_WIDTH) + ((Position.X + 8) / TILE_SIZE);
      // Fall through
    }
    // Object wants to dig right
    case DIR_R:
    {
      // Get the tile that is to the right of the object
      Loc = (((Position.Y + 15) / TILE_SIZE) * LEVEL_WIDTH) + ((Position.X + 10) / TILE_SIZE);
      break;
    }
    // Object wants to dig down right
    case DIR_DR:
    {
      // Get the tile the object is on now
      CLoc = (((Position.Y + 1) / TILE_SIZE) * LEVEL_WIDTH) + ((Position.X + 8) / TILE_SIZE);
      // Get the tile that is to the right of the object
      Loc = (((Position.Y + 1) / TILE_SIZE) * LEVEL_WIDTH) + ((Position.X + 10) / TILE_SIZE);
      break;
    }
    case DIR_D:
    {
      // Get the tile that is below the object
      Loc = (((Position.Y + 16) / TILE_SIZE) * LEVEL_WIDTH) + ((Position.X + 8) / TILE_SIZE);
      break;
    }
  }

  // Get the tile id that the object is on now
  register USHORT CId = App.Game.LevelData[CLoc];
  // Get the tile above and adjacent the object
  register USHORT TId = Loc >= LEVEL_WIDTH ? App.Game.LevelData[Loc - LEVEL_WIDTH] : 0;
  // Get the tile below and adjacent to the object
  register USHORT BId = Loc < LEVEL_SIZE - LEVEL_WIDTH ? App.Game.LevelData[Loc + LEVEL_WIDTH] : 0;
  // Get the tile adjacent to the object
  register USHORT AId = App.Game.LevelData[Loc];
  // Get the tile left of the object
  register USHORT LId = Loc - 1 >= 0 ? App.Game.LevelData[Loc - 1] : 0;
  // Get the tile right of the object
  register USHORT RId = Loc + 1 < LEVEL_SIZE ? App.Game.LevelData[Loc + 1] : 0;

  switch(Direction)  // What direction is the object going in
  {
    case DIR_UL:
    case DIR_L:
    {
      register USHORT TDLId = TileData[LId];
      register USHORT TDTId = TileData[TId];
      if(TDTId & TF_P || (TDLId & TF_W && TDLId & TF_ER) || (TDTId & TF_W && TDTId & TF_EB))
      {
        // Set failed dig direction for computer controlled objects
        FailedDig = Direction;
        return FALSE;
      }
      break;
    }
    case DIR_DL:
    {
      register USHORT TDBId = TileData[BId];
      register USHORT TDLId = TileData[LId];
      register USHORT TDTId = TileData[TId];
      if(TDTId & TF_P || (TDBId & TF_W && TDBId & TF_ET) || (TDLId & TF_W && TDLId & TF_ER) || (TDTId & TF_W && TDTId & TF_EB))
      {
        // Set failed dig direction for computer controlled objects
        FailedDig = Direction;
        return FALSE;
      }
      break;
    }
    case DIR_UR:
    case DIR_R:
    {
      register USHORT TDTId = TileData[TId];
      register USHORT TDRId = TileData[RId];
      if(TDTId & TF_P || (TDRId & TF_W && TDRId & TF_EL) || (TDTId & TF_W && TDTId & TF_EB))
      {
        // Set failed dig direction for computer controlled objects
        FailedDig = Direction;
        return FALSE;
      }
      break;
    }
    case DIR_DR:
    {
      register USHORT TDBId = TileData[BId];
      register USHORT TDRId = TileData[RId];
      register USHORT TDTId = TileData[TId];
      if(TDTId & TF_P || (TDBId & TF_W && TDBId & TF_ET) || (TDRId & TF_W && TDRId & TF_EL) || (TDTId & TF_W && TDTId & TF_EB))
      {
        // Set failed dig direction for computer controlled objects
        FailedDig = Direction;
        return FALSE;
      }
      break;
    }
    case DIR_D:
    {
      register USHORT TDLId = TileData[LId];
      register USHORT TDTId = TileData[TId];
      register USHORT TDRId = TileData[RId];
      register USHORT TDBId = TileData[BId];
      if(TDTId & TF_P || (TDBId & TF_W && TDBId & TF_ET) || (TDLId & TF_W && TDLId & TF_ER) || (TDRId & TF_W && TDRId & TF_EL))
      {
        // Set failed dig direction for computer controlled objects
        FailedDig = Direction;
        return FALSE;
      }
      break;
    }
  }

  // Create pointer to a digdata item
  register struct DIGDATAITEM *DD;
  // Walk through all the digdata structures to find info about current tile
  for(DD = &DigData[Direction].Data[0]; DD->Flags; *++DD)
  {
    // Perform the checks
    if((~DD->Flags & DFM_O || (DD->Flags & DFM_O && AId == DD->FromOver)) &&
       (~DD->Flags & DFM_A || (DD->Flags & DFM_A && TId == DD->FromAbove)) &&
       (~DD->Flags & DFM_B || (DD->Flags & DFM_B && BId == DD->FromBelow)) &&
       (~DD->Flags & DFM_C || (DD->Flags & DFM_C && CId == DD->FromCentre)))
    {
      // Digging is allowed. Add to dirt dug
      ++DirtDug;
      // ...and if object has owner?
      if(Owner)
        // At to owners global total
        ++Owner->DirtDug;
      // Terrain should change?
      if(DD->Flags & DFS_O)
      {
        // Check for special tile
        switch(DD->ToOver)
        {
          case 0xFFFE:
          {
            USHORT Swap[3] = { 150, 172, 172 };

            AId = Swap[Random() % 3];
            break;
          }
          case 0xFFFF:
          {
            USHORT Swap[3] = { 150, 170, 171 };

            AId = Swap[Random() % 3];
            break;
          }
          default:
          {
            AId = DD->ToOver;
            break;
          }
        }
        // Successful dig should search for treasure
        if(DD->Flags & DFO_TREASURE)
          // Roll the dice
          SPAWNTREASURE();
      }
      if(DD->Flags & DFS_A)
        TId = DD->ToAbove;
      if(DD->Flags & DFS_B)
        BId = DD->ToBelow;

      if(DD->Flags & DFO_BUSY)
        // Set busy
        SETBUSY();
      if(DD->Flags & DFO_NOTBUSY)
        // Unset busy
        SETNOTBUSY();

      // If tile location is not at the top of the level
      if(Loc >= LEVEL_WIDTH)
        // Update above tile
        UpdateLevelData(Loc - LEVEL_WIDTH, TId);
      // Update over tile
      UpdateLevelData(Loc, AId);
      // If tile location is not at the bottom of the level
      if(Loc < LEVEL_SIZE - LEVEL_WIDTH)
        // Update below tile
        UpdateLevelData(Loc + LEVEL_WIDTH, BId);

      // Update dig time for AI
      LastDigTime = App.Game.Time;

      // Dig was successful
      FailedDig = DIR_NONE;

      // Dig was successful
      return TRUE;
    }
  }

  // Set failed dig direction for computer controlled objects
  FailedDig = Direction;

  // Dig failed
  return FALSE;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::SPAWN TREASURE                                                            */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::SPAWNTREASURE(VOID)
{
  // Create variable for random number
  register INT Index, Number = Random() % 100;
  // Walk through all the object types (Reuse CLoc)
  for(Index = 0; Index < TYP_MAX; ++Index)
  {
    // Object type is treasure and drop chance has won
    if(ObjectData[Index].Flags & OFL_TREASURE && Number < ObjectData[Index].DigDelay)
    {
      // Add to number of gems found
      ++GemsFound;
      // ...and if object has owner
      if(Owner)
        // Add to owners global total
        ++Owner->GemsFound;
      // Spawn the object! SPARKLE! SPARKLE!
      new OBJECT(NULL, (OBJTYP)Index, Position.X, Position.Y, FALSE);
      // Done test any more objects. One drop is enough!
      break;
    }
  }
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::REST                                                                      */
/* --------------------------------------------------------------------------------- */
BOOL OBJECT::REST(VOID)
{
  // Owner doesnt have enough money or already is full health
  if(!Owner->Money || Health == 100)
    // Return failed
    return FALSE;
  // Rest
  SETACTION(ACT_PHASE, JOB_PHASE, DIR_R);
  // Return success
  return TRUE;
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::AI DIGGER LIKE MOVEMENT                                                   */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::AIBIGFOOT(VOID)
{
  // Random event occurs?
  if(Random() % AIData[Action][Job][Direction] == 0)
    // Set a random action
    switch(Random() % 5)
    {
      // Stop it
      case 0: SETACTION(ACT_STOP, JOB_NONE, DIR_NONE); return;
      // Drop something
      case 1: if(Items) { DROPINVENTORY(Random() % 2 ? Items : ItemsLast); return; }
      // Rest it if needed else fall through
      case 2: if(Health < 100) { SETACTION(ACT_PHASE, JOB_PHASE, DIR_R); return; }
      // Telport somewhere
      case 3: if(Health == 100 && Random() % 2) { SETACTION(ACT_PHASE, JOB_PHASE, DIR_DR); return; }
      // Walk it
      case 4: SETACTION(ACT_WALK, JOB_BOUNCE, DIR_LR); return;
    }
  // If digger is not walking?
  if(Action != ACT_WALK)
    // Dont continue
    return;
  // Every two seconds and digger isnt searching
  if(App.Game.Time % 60 == 0)
  {
    // Try to steal anything
    PICKUPITEM(FALSE);
    // Done
    return;
  }
  // Which direction?
  switch(Direction)
  {
    // Left
    case DIR_L: case DIR_UL: case DIR_DL:
      // Value between 0 and 1 is 1 and going left would block but could jump it
      if(Random() % 2 && ISCOLLIDE(-1, -2) && !ISCOLLIDE(-1, -TILE_SIZE))
        // Jump left
        SETACTION(ACT_KEEP, JOB_JUMP, DIR_KEEP);
      // Done
      return;
    // Right
    case DIR_R: case DIR_UR: case DIR_DR:
      // Value between 0 and 1 is 1 and going right would block but could jump it
      if(Random() % 2 && ISCOLLIDE(1, -2) && !ISCOLLIDE(1, -TILE_SIZE))
        // Jump right
        SETACTION(ACT_KEEP, JOB_JUMP, DIR_KEEP);
      // Done
      return;
  }
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::DIGGER AI PROCEDURE                                                       */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::AIDIGGER(VOID)
{
  // Object is falling, falling damage would kill digger? then give 25% chance to teleport
  // or if health is <= 10% and a 1 in 100 chance
  // or if digger is in danger
  if((Flags & OFL_FALL && FallDamage >= Health && Random() % 2) ||
     ((Health <= 10 || Job == JOB_INDANGER) && Random() % 100 == 0))
  {
    // Teleport home to safely regain health
    SETACTION(ACT_PHASE, JOB_PHASE, DIR_U);
    // Done
    return;
  }
  // Else if health is < 100% or digger has items and digger Y axis is same as starting Y
  // And digger X axis is same as starting X
  else if(Position.Y == Owner->Start.Y && Position.X == Owner->Start.X && (Flags & OFL_CPUSELL || Health < 75))
  {
    // Digger needs to sell items?
    if(Flags & OFL_CPUSELL)
    {
      // Create pointer to digger inventory
      register POBJECT Item = Items;
      // For each item in digger inventory
      do
      {
        // If item does not belong to this digger
        if(Item->Owner != Owner)
        {
          // Sell it and catch next item in inventory
          Item = SELLITEM(Item);
          // Test next item
          continue;
        }
        // Record next item
        Item = Item->Next;
      }
      while(Item);
      // Unset sell flag
      Flags &= ~OFL_CPUSELL;
    }
    // Enter rest state to make digger look like it went inside the trade centre
    // to rest and sell items.
    REST();
    // Done
    return;
  }
  // If a number between 0-999 is 0 and digger has items
  if(Random() % 1000 == 0 && Items && Action == ACT_STOP)
  {
    // Create pointer to digger's inventory
    register POBJECT Item;
    // For each item in digger's inventory
    for(Item = Items; Item; Item = Item->Next)
    {
      // If item is not sellable or owner owns the object
      if(~Item->ObjData->Flags & OFL_SELLABLE || Item->Owner == Owner)
        // Ignore object
        continue;
      // Item is sellable so teleport home so digger can sell it :)
      SETACTION(ACT_PHASE, JOB_PHASE, DIR_U);
      // Set flag so the iventory is only scanned once
      Flags |= OFL_CPUSELL;
      // Done
      return;
    }
    // Done
    return;
  }
  // If the time we last had a successful dig was a long time. The digger
  // could be stuck, so teleport home.
  if(App.Game.Time - LastDigTime >= 7200 && Action == ACT_STOP)
  {
    // Reset last dig time
    LastDigTime = App.Game.Time;
    // Teleport home in hope the digger will find a new direction
    SETACTION(ACT_PHASE, JOB_PHASE, DIR_U);
    // Done
    return;
  }
  if(Random() % AIData[Action][Job][Direction] == 0)
  {
    // Select a random direction
    switch(Random() % 9)
    {
      case 0:
      {
        if(FailedDig != DIR_L)
          SETACTION(ACT_WALK, JOB_DIG, DIR_L);
        else
          SETACTION(ACT_WALK, JOB_DIGDOWN, DIR_TCTR);
        // Done
        return;
      }
      case 1:
      {
        if(FailedDig != DIR_R)
          SETACTION(ACT_WALK, JOB_DIG, DIR_R);
        else
          SETACTION(ACT_WALK, JOB_DIGDOWN, DIR_TCTR);
        // Done
        return;
      }
      case 2:
      {
        if(FailedDig != DIR_UL)
          SETACTION(ACT_WALK, JOB_DIG, DIR_UL);
        else
          SETACTION(ACT_WALK, JOB_DIG, DIR_L);
        // Done
        return;
      }
      case 3:
      {
        if(FailedDig != DIR_UR)
          SETACTION(ACT_WALK, JOB_DIG, DIR_UR);
        else
          SETACTION(ACT_WALK, JOB_DIG, DIR_R);
        // Done
        return;
      }
      case 4:
      {
        if(FailedDig != DIR_DL)
          SETACTION(ACT_WALK, JOB_DIG, DIR_DL);
        else
          SETACTION(ACT_WALK, JOB_DIG, DIR_L);
        // Done
        return;
      }
      case 5:
      {
        if(FailedDig != DIR_DR)
          SETACTION(ACT_WALK, JOB_DIG, DIR_DR);
        else
          SETACTION(ACT_WALK, JOB_DIG, DIR_R);
        // Done
        return;
      }
      case 6:
      {
        if(FailedDig != DIR_L)
          SETACTION(ACT_WALK, JOB_SEARCH, DIR_L);
        else
          SETACTION(ACT_WALK, JOB_DIGDOWN, DIR_TCTR);
        // Done
        return;
      }
      case 7:
      {
        if(FailedDig != DIR_R)
          SETACTION(ACT_WALK, JOB_SEARCH, DIR_R);
        else
          SETACTION(ACT_WALK, JOB_DIGDOWN, DIR_TCTR);
        // Done
        return;
      }
      case 8:
      {
        SETACTION(ACT_STOP, JOB_NONE, DIR_NONE);
        // Done
        return;
      }
    }
    // Done
    return;
  }
  // If digger is walking and digger isnt digging
  if(Action == ACT_WALK)
  {
    // Every two seconds and digger isnt searching
    if(App.Game.Time % 60 == 0 && Job != JOB_SEARCH)
    {
      // Try to pick up any treasure
      PICKUPITEM(TRUE);
      // Done
      return;
    }
    // Digger is not digging?
    if(Job != JOB_DIG)
    {
      // Which direction?
      switch(Direction)
      {
        // Left
        case DIR_L: case DIR_UL: case DIR_DL:
          // Value between 0 and 1 is 1 and going left would block but could jump it
          if(Random() % 2 && ISCOLLIDE(-1, -2) && !ISCOLLIDE(-1, -TILE_SIZE))
            // Jump left
            SETACTION(ACT_KEEP, JOB_JUMP, DIR_KEEP);
          // Done
          return;
        // Right
        case DIR_R: case DIR_UR: case DIR_DR:
          // Value between 0 and 1 is 1 and going right would block but could jump it
          if(Random() % 2 && ISCOLLIDE(1, -2) && !ISCOLLIDE(1, -TILE_SIZE))
            // Jump right
            SETACTION(ACT_KEEP, JOB_JUMP, DIR_KEEP);
          // Done
          return;
      }
    }
    // Done
    return;
  }
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::AI CRITTER PROCEDURE                                                      */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::AICRITTER(VOID)
{
  // Direction to go in
  register INT AdjX = Direction == DIR_L ? -1 : 1;
  // Ignore even numbers on timer to slow it down
  if((AnimTimer % 2)) return;
  // If object can move in its current direction still?
  if(!ISCOLLIDE(AdjX, 0) && App.Game.Time % TIMER_TARGETTIME)
  {
    // Move it
    ADJUSTPOS(AdjX, 0);
    // Done
    return;
  }
  // Blocked so go in opposite direction
  SETACTION(ACT_KEEP, JOB_KEEP, DIR_OPPOSITE);
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::AI RANDOM AXIS MOVEMENT                                                   */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::AIRANDOM(VOID)
{
  // If object was blocked
  register BOOL Blocked = FALSE;

  // Compare direction
  if(Direction == DIR_NONE)
  {
    // Set a random direction. 50% chance of going right
    if(Random() % 2) SETACTION(ACT_KEEP, JOB_KEEP, DIR_R);
    // Set a random direction. 50% chance of going left
    if(Random() % 2) SETACTION(ACT_KEEP, JOB_KEEP, DIR_L);
    // Set a random direction. 50% chance of going down
    if(Random() % 2) SETACTION(ACT_KEEP, JOB_KEEP, DIR_D);
    // Set a random direction. 50% chance of going up
    if(Random() % 2 || Direction == DIR_NONE) SETACTION(ACT_KEEP, JOB_KEEP, DIR_U);
  }
  // Move
  switch(Direction)
  {
    // Going up?
    case DIR_U: if(!ISCOLLIDE(0, -2)) ADJUSTPOS(0, -2); else Blocked = TRUE; break;
    // Going down?
    case DIR_D: if(!ISCOLLIDE(0, 2)) ADJUSTPOS(0, 2); else Blocked = TRUE; break;
    // Going left?
    case DIR_L: if(!ISCOLLIDE(-2, 0)) ADJUSTPOS(-2, 0); else Blocked = TRUE; break;
    // Going right?
    case DIR_R: if(!ISCOLLIDE(2, 0)) ADJUSTPOS(2, 0); else Blocked = TRUE; break;
  }
  // If we were blocked then we move in a diff direction
  if(Blocked)
  {
    // Now we have to find a new direction
    register OBJDIR Directions[4]; // Hold possible directions
    register INT    Id = 0;        // Id number of array

    // Comprea direction
    switch(Direction)
    {
      // Going UP?
      case DIR_U:
        // If object can go LEFT
        if(!ISCOLLIDE(-2, 0))
          // Can go LEFT
          Directions[Id++] = DIR_L;
        // If object can go RIGHT
        if(!ISCOLLIDE(2, 0))
          // Can go RIGHT
          Directions[Id++] = DIR_R;
        // Can't go in either direction so try to go DOWN
        if(Id == 0 && !ISCOLLIDE(0, -2))
          // Can go up
          Directions[Id++] = DIR_D;
        // Done
        break;
      // Going DOWN?
      case DIR_D:
        // If object can go LEFT
        if(!ISCOLLIDE(-2, 0))
          // Can go LEFT
          Directions[Id++] = DIR_L;
        // If object can go RIGHT
        if(!ISCOLLIDE(2, 0))
          // Can go RIGHT
          Directions[Id++] = DIR_R;
        // Can't go in either direction so try to go UP
        if(Id == 0 && !ISCOLLIDE(0, -2))
          // Can go up
          Directions[Id++] = DIR_U;
        // Done
        break;
      // Going LEFT?
      case DIR_L:
        // If object can go UP
        if(!ISCOLLIDE(0, -2))
          // Can go UP
          Directions[Id++] = DIR_U;
        // If object can go DOWN
        if(!ISCOLLIDE(0, 2))
          // Can go DOWN
          Directions[Id++] = DIR_D;
        // Can't go in either direction so try to go RIGHT
        if(Id == 0 && !ISCOLLIDE(2, 0))
          // Can go up
          Directions[Id++] = DIR_R;
        // Done
        break;
      // Going RIGHT?
      case DIR_R:
        // If object can go UP
        if(!ISCOLLIDE(0, -2))
          // Can go UP
          Directions[Id++] = DIR_U;
        // If object can go DOWN
        if(!ISCOLLIDE(0, 2))
          // Can go DOWN
          Directions[Id++] = DIR_D;
        // Can't go in either direction so try to go LEFT
        if(Id == 0 && !ISCOLLIDE(2, 0))
          // Can go up
          Directions[Id++] = DIR_L;
        // Done
        break;
    }

    // Pick a new direction from eligable directions
    Direction = Id ? Directions[Random() % Id] : DIR_NONE;
  }
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::AI FIND AND KILL ANY DIGGER                                               */
/* --------------------------------------------------------------------------------- */
VOID OBJECT::AIFIND(VOID)
{
  // Object aquires a target, target's time expired or target not initialised?
  if(ActionTimer % TIMER_TARGETTIME == 0 || Target == NULL)
  {
    // Holds entitled digger targets
    register POBJECT DiggerList[NUM_DIGGERS * NUM_PLAYERS];
    // Pointer to current object
    register POBJECT Object;
    // Id in DiggerList array
    register INT Id = 0;

    // For each object
    for(Object = App.Game.Object.First; Object; Object = Object->Next)
    {
      // Object is not a digger?
      if(~Object->ObjData->Flags & OFL_DIGGER)
        // Ignore object
        continue;
      // Record object
      DiggerList[Id++] = Object;
    }

    // Pick a random target form eligable objects
    Target = Id ? DiggerList[Random() % Id] : this;

    // Go in direction of target
    SETACTION(ACT_KEEP, JOB_KEEP, Target->Position.X < Position.X ? DIR_L : DIR_R);
  }

  // Object is slow and action timer is odd
  if(ObjData->AIType == AI_FINDSLOW && ActionTimer % 2 == 1)
    // Done
    return;

  // Variables for adjusted coordinates
  register INT XA = 0, YA = 0;

  // This X is left of target X?
  if(Position.X < Target->Position.X)
    // Move right one pixel
    ++XA;
  // X is right of target X
  else if(Position.X > Target->Position.X)
    // Move left one pixel
    --XA;
  // If we can move to requested horizontal position
  if(!ISCOLLIDE(XA, 0))
    // Move to requested horizontal position
    ADJUSTPOS(XA, 0);
  // Y is below target Y?
  if(Position.Y < Target->Position.Y)
    // Move up one pixel
    ++YA;
  // Y is above target Y
  else if(Position.Y > Target->Position.Y)
    // Move down one pixel
    --YA;
  // If we can move to requested vertical position
  if(!ISCOLLIDE(0, YA))
    // Move to requested vertical position
    ADJUSTPOS(0, YA);

  // Direction changed?
  if(Direction != DirTable[++YA][++XA])
    // Change direction
    SETACTION(ACT_KEEP, JOB_KEEP, DirTable[YA][XA]);
}
/* --------------------------------------------------------------------------------- */
/* OBJECT::MAIN PROCEDURE                                                            */
/* --------------------------------------------------------------------------------- */
POBJECT OBJECT::MAIN(VOID)
{
  // If AI function set, not dying and not phasing
  if(AIActiveFunc && Action != ACT_DEATH && Action != ACT_PHASE)
    // Call AI function
    (this->*AIActiveFunc)();

  // If object is on the ground or 'Flags' does not have the OFL_FALL flag
  if(ISCOLLIDE(0, 1) || ~Flags & OFL_FALL)
  {
    // If fall damage is set then object fell and now we must reduce its health
    if(FallDamage)
    {
      // Object still has fall flag set and object fell >= 16 pixels (1->3->6->10->15)
      if(Flags & OFL_FALL && FallDamage >= 5)
      {
        // Adjust health
        ADJUSTHEALTH(-FallDamage / (ObjData->Flags & OFL_DELICATE ? 1 : 2));
        // Damage would reduce health < 10 %
        if(Health && Health < 10)
          // Stop object moving
          SETACTION(ACT_STOP, JOB_INDANGER, DIR_NONE);
      }
      // Reset fall damage;
      FallDamage = 0;
    }
    // Reset fall speed;
    FallSpeed = 1;
    // Object is jumping?
    if(Flags & OFL_JUMPRISE)
    {
      // On first frame?
      if(!ActionTimer)
      {
        // if object is on solid ground
        // Play jump sound
        SOUND(SFX_JUMP);
        // Remove 'falling' flag, we control it while jumping
        Flags &= ~OFL_FALL;
        // Set busy
        SETBUSY();
      }
      // Object can move up and the rise limit hasnt been reached yet
      if(!ISCOLLIDE(0, -1) && ActionTimer <= JD_SIZE)
      {
        // We can actually move up?
        if(JumpDataRising[ActionTimer])
          // Move up
          ADJUSTPOS(0, -1);
      }
      // Object cannot move up or action timer is the last frame
      else
      {
        // Remove rising flag
        Flags &= ~OFL_JUMPRISE;
        // Set falling flag
        Flags |= OFL_JUMPFALL;
        // Reset action timer
        ActionTimer = 0;
      }
    }
    // Object is falling (during the jump)?
    else if(Flags & OFL_JUMPFALL)
    {
      // Object can fall down and the rise limit hasnt been reached yet
      if(!ISCOLLIDE(0, 1) && ActionTimer <= JD_SIZE)
      {
        // And we can drop down?
        if(JumpDataFalling[ActionTimer])
          // Drop down
          ADJUSTPOS(0, 1);
      }
      // Object cannot move down or action timer is the last frame
      else
      {
        // Let object fall normally now
        Flags |= OFL_FALL;
        // Remove falling and busy flags
        Flags &= ~(OFL_JUMPFALL | OFL_BUSY);
        // Reset action timer
        ActionTimer = 0;
      }
    }
    // Compare object action
    switch(Action)
    {
      // Object is hidden?
      case ACT_HIDE:
      {
        // Object is not resting?
        if(Job != JOB_PHASE)
          // Do nothing
          break;
        // Health at full or player run out of money?
        if(Health == 100 || (Owner && !Owner->Money))
          // Reappear
          SETACTION(ACT_PHASE, JOB_NONE, DIR_NONE);
        // Else for every five seconds
        else if(ActionTimer % 300 == 1 && Owner)
          // Take 1 zog as a cost for resting
          --Owner->Money;
        // Health not full
        else if(ActionTimer % 10 == 0)
          // Regenerate health faster
          ADJUSTHEALTH(1);
        // Done
        break;
      }
      // Object has been eaten?
      case ACT_EATEN:
      {
        // Mutate animation time not reached yet?
        if(ActionTimer < TIMER_MUTATEWAIT)
          // Break
          break;
        // Kill digger
        ADJUSTHEALTH(-100);
        // Spawn an alien
        new OBJECT(NULL, TYP_ALIEN, Position.X, Position.Y, TRUE);
        // Done
        break;
      }
      // Object is dead?
      case ACT_DEATH:
      {
        // Death animation time not reached yet?
        if(ActionTimer < TIMER_DEADWAIT)
          // Break
          break;
        // Object has an owner and it was a digger?
        if(Owner && ObjData->Flags & OFL_DIGGER)
          // Tell player the digger died
          Owner->DIGGERDIED(this);
        // Get next object because we are deleting this object and
        // will no longer be able to reference any variables in this class
        POBJECT ObjNext = Next;
        // Delete object from global list
        delete this;
        // And return the next object
        return ObjNext;
      }
      // Object is stopped?
      case ACT_STOP:
      {
        // Object stamina delay timer reached
        if(ObjData->Stamina >= 0 && ActionTimer % ObjData->Stamina >= ObjData->Stamina - 1)
          // Object slowly regenerates health when idle
          ADJUSTHEALTH(1);
        // Done
        break;
      }
      // Object is phasing?
      case ACT_PHASE:
      {
        // Phase delay not reached?
        if(ActionTimer < ObjData->PhaseDelay)
          // Don't do anything else
          break;
        // Compare job (phase type)
        switch(Job)
        {
          // Object is phasing home
          case JOB_PHASE:
          {
            // If this object is selected
            if(App.Game.Object.Active == this && (!Owner || Owner && Owner != App.Game.Player.This))
              // Deselect it. Opponents are not allowed to see where they went!
              App.Game.Object.Active = NULL;
            // Compare direction. This tells where to teleport to
            switch(Direction)
            {
              // Reset
              case DIR_R:
              {
                // Hide object
                SETACTION(ACT_HIDE, JOB_PHASE, DIR_R);
                // Done
                break;
              }
              // Phase home
              case DIR_U:
              {
                // Select first object to search
                Target = Target ? Target->Next : App.Game.Object.First;
                // Walk target until end of object list
                while(Target)
                {
                  // If object is owned by this objects owner and object is a telepole
                  // Habbish can intercept any telepole as it is a racial ability
                  if(Target->Type == TYP_TELEPOLE && (Target->Owner == Owner || Owner->Race == TYP_HABBISH))
                  {
                    // Teleport to this device
                    SETPOS(Target->Position.X, Target->Position.Y);
                    // Break the loop
                    break;
                  }
                  // Get next target
                  Target = Target->Next;
                }
                // Did not find a telepole?
                if(!Target)
                  // Set position of object to player's home
                  SETPOS(Owner->Start.X, Owner->Start.Y);
                // Re-phase back into stance
                SETACTION(ACT_PHASE, JOB_NONE, DIR_NONE);
                // Done
                break;
              }
              // Phase random but no diggers. Set flag and fall through
              case DIR_DR:
              // Phase random
              case DIR_D:
              {
                // Create pointer to objects list
                register POBJECT  Object = App.Game.Object.First;
                // Initialise object id
                register INT      ObjectId = 0;
                // Create object list array
                register POBJECT *ObjectList = (POBJECT*)new CHAR[App.Game.Object.Count * App.Core.AddressLength];
                // Walk objects list
                do
                {
                  // If object isnt this object and object is a valid phase target
                  if(Object != this && Object->ObjData->Flags & OFL_PHASETARGET &&
                    (Direction == DIR_D || (Direction == DIR_DR && ~Object->ObjData->Flags & OFL_DIGGER)))
                    // Add object to object list
                    ObjectList[ObjectId++] = Object;
                  // Set next object in list
                  Object = Object->Next;
                }
                // Process next object
                while(Object);
                // List has items?
                if(ObjectId)
                {
                  // Pick random object from array
                  Object = ObjectList[Random() % ObjectId];
                  // Set this objects position to the object
                  SETPOS(Object->Position.X, Object->Position.Y);
                  // Re-phase back into stance
                  SETACTION(ACT_PHASE, JOB_NONE, DIR_NONE);
                }
                // Else if object has owner, teleport home
                else if(Owner)
                {
                  // Set position of object to player's home
                  SETPOS(Owner->Start.X, Owner->Start.Y);
                  // Re-phase back into stance
                  SETACTION(ACT_PHASE, JOB_NONE, DIR_NONE);
                }
                // No owner = no home, just de-phase
                else SETACTION(ACT_STOP, JOB_NONE, DIR_NONE);
                // Free allocated data
                delete []ObjectList;
                // Done
                break;
              }
            }
            // Done
            break;
          }
          // Object explodes?
          case JOB_EXPLODE:
          {
            // Kill it
            ADJUSTHEALTH(-100);
            // Done
            break;
          }
          // Object has finished phasing
          default:
          {
            // Set normal stance mode
            SETACTION(ACT_STOP, JOB_NONE, DIR_NONE);
            // Done
            break;
          }
        }
        // Phase control done
        break;
      }
      // Object is walking?
      case ACT_CREEP:
      {
        // Actiontimer is an odd number (to control speed)
        if(ActionTimer % 4)
          // Do not continue processing for now
          break;
        // Fall through
      }
      // Object is walking?
      case ACT_WALK:
      {
        // Actiontimer is an odd number (to control speed)
        if(ActionTimer % 2)
          // Do not continue processing for now
          break;
        // Fall through
      }
      // or Object is running?
      case ACT_RUN:
      {
        // Object wants to dig down and object X position is in the middle of the tile
        if(Job == JOB_DIGDOWN && !(Position.X % TILE_SIZE))
        {
          // Make object dig down
          SETACTION(ACT_DIG, JOB_DIGDOWN, DIR_D);
          // Done
          break;
        }
        // Object wants to enter the trading centre
        if(Job == JOB_HOME && Position.X == Owner->Start.X && Position.Y == Owner->Start.Y)
        {
          // Stop object
          SETACTION(ACT_STOP, JOB_NONE, DIR_NONE);
          // Done
          break;
        }
        // Object is for rails only and train is not on track
        if(ObjData->Flags & OFL_TRACK && ~TileData[App.Game.LevelData[(PositionAbs.Y * LEVEL_WIDTH) + ((Position.X + (Direction == DIR_L ? 7 : 9)) / TILE_SIZE)]] & TF_T)
        {
          // At end of track so stop
          SETACTION(ACT_STOP, JOB_NONE, DIR_NONE);
          // Done
          break;
        }
        // If timer goes over 1 second and object is busy
        if(ActionTimer >= 60 && Flags & OFL_BUSY)
          // Unset busy flag as abnormal digging can make it stick
          SETNOTBUSY();
        // Compare direction
        switch(Direction)
        {
          // Object is moving up?
          case DIR_U:
          {
            // Move object up
            MOVEY(-1);
            // Done
            break;
          }
          // Object is moving down?
          case DIR_D:
          {
            // Move object down
            MOVEY(1);
            // Done
            break;
          }
          // Object is moving up-left, left or down-left?
          case DIR_UL: case DIR_L: case DIR_DL:
          {
            // Move object left
            MOVEX(-1);
            // Done
            break;
          }
          // Object is moving up-right, right or down-right?
          case DIR_UR: case DIR_R: case DIR_DR:
          {
            // Move object right
            MOVEX(1);
            // Done
            break;
          }
        }
        // Done processing object movement
        break;
      }
      // Object is digging?
      case ACT_DIG:
      {
        // Digging delay not reached?
        if(ActionTimer < ObjData->DigDelay)
          // Don't do anything else
          break;
        // Dig the terrain
        if(DIG())
          // Terrain dig was successful, continue to walk in the direction
          SETACTION(ACT_WALK, JOB_KEEP, DIR_KEEP);
        else
          // Terrain dig was not successful, stop the object's actions completly
          SETACTION(ACT_STOP, JOB_NONE, DIR_NONE);
        // Done processing object digging
        break;
      }
    }
    // Post action job process
    switch(Job)
    {
      // Object search for other objects to pickup?
      case JOB_SEARCH:
      {
        // Use actiontimer to limit call frequency to 1/6 of a second
        // This is because if there are tons of objects in the level,
        // calling this on every tick really could bog the processor severly
        // due to the sprite mask collision checking.
        if(ActionTimer % TIMER_PICKUPDELAY == 0)
          // Get touching pickup object and add it to inventory
          PICKUPITEM(TRUE);
        // Done
        break;
      }
      // Object is in danger?
      case JOB_INDANGER:
      {
        // danger timeout is reached?
        if(ActionTimer >= TIMER_DANGERTIMEOUT)
          // Remove the objects job
          SETACTION(ACT_KEEP, JOB_NONE, DIR_KEEP);
        // Done
        break;
      }
    }
  }
  // Else object should fall
  else
  {
    // Start from 'fallspeed' pixels and count down to 1
    for(register INT Index = FallSpeed; Index >= 1; --Index)
    {
      // No collision found with terrain?
      if(!ISCOLLIDE(0, Index))
      {
        // Move Y position down
        ADJUSTPOS(0, Index);
        // Increase fall speed
        if(FallSpeed < 5) ++FallSpeed;
        // Increase fall damage
        ++FallDamage;
        // break loop
        break;
      }
      // Collision detected, but we must find how many pixels exactly to fall by, so
      // Reduce the pixel count and try to find the exact value.
    }
  }

  // Check for fights with other objects only if object isnt busy
  if(Action != ACT_DEATH)
  {
    // Create pointer to objects list
    register POBJECT Object;
    // Walk objects list
    for(Object = App.Game.Object.First; Object; Object = Object->Next)
      // Object is not me and object isnt a friend and object is a digger and object is colliding
      if(this != Object && Owner != Object->Owner && Object->Flags & OFL_DIGGER && GETCOLLISION(Object))
      {
        // If object can hatch
        if(Flags & OFL_CONSUME)
        {
          // Kill egg
          ADJUSTHEALTH(-100);
          // Eat digger
          Object->SETACTION(ACT_EATEN, JOB_NONE, DIR_NONE);
          // This digger is selected by the client
          if(App.Game.Object.Active == Object)
            // Unset control menu
            SetControlMenu(MNU_NONE);
        }
        // If object can phase the digger
        if(Flags & OFL_PHASEDIGGER)
          // Make object phase to some other object
          Object->SETACTION(ACT_PHASE, JOB_PHASE, DIR_D);
        // If object can hurt the digger
        if(Flags & OFL_HURTDIGGER)
        {
          // This object is stationary
          if(ObjData->Flags & OFL_STATIONARY)
            // Make me fight and face the enemy
            SETACTION(ACT_FIGHT, JOB_NONE, Position.X > Object->Position.X ? DIR_L : DIR_R);
          // Reduce health
          Object->ADJUSTHEALTH(-1);
        }
        // If object is a digger and is not busy
        if(Flags & OFL_DIGGER && ~Flags & OFL_BUSY)
        {
          // If I am not fighting
          if(Action != ACT_FIGHT)
            // Make me fight and face the enemy
            SETACTION(ACT_FIGHT, JOB_INDANGER, Position.X > Object->Position.X ? DIR_L : DIR_R);
          // Increase no of objects I am fighting with
          // I can only fight with one person so...
          if(Random() % (ObjData->Strength >> 2) == 0)
          {
            // Deal damage equal to my strength
            Object->ADJUSTHEALTH(-(ObjData->Strength >> 3));
            // If a random value from 0-19 is 0
            if(Random() % 20 == 0)
              // Play punch sound
              SOUND(SFX_PUNCH);
            // Else value is 1
            else if(Random() % 20 == 0)
              // Play kick sound
              SOUND(SFX_KICK);
          }
          // Check no more objects
          break;
        }
        // Object is dangerous, object is not running and object isnt dead? and object isnt busy
        if(Flags & OFL_DANGEROUS && Object->Action != ACT_RUN && Object->Action != ACT_DEATH && ~Object->Flags & OFL_BUSY)
        {
          // Object is moving?
          if(Object->Direction != DIR_NONE)
            // Make the object run in the direction it is going in
            Object->SETACTION(ACT_RUN, JOB_INDANGER, DIR_KEEP);
          // Object is not moving?
          else
            // Run in opposite direction
            Object->SETACTION(ACT_RUN, JOB_INDANGER, Position.X > Object->Position.X ? DIR_L : DIR_R);
        }
      }
    // if I am fighting and there are no more enemies?
    if(Action == ACT_FIGHT && !Object)
    {
      // Object is a digger
      if(Flags & OFL_DIGGER)
        // Stop me from fighting with air :)
        SETACTION(ACT_STOP, JOB_INDANGER, DIR_KEEP);
      // Not a digger
      else if(ActionTimer >= 360)
        // Stop me from fighting with air :)
        SETACTION(ACT_STOP, JOB_NONE, DIR_KEEP);
    }
    // If object isnt phasing?
    if(Action != ACT_PHASE)
    {
      // If object floats and object is on top of water
      if(Flags & OFL_FLOAT && TileData[App.Game.LevelData[((Position.Y + 10) / TILE_SIZE) * LEVEL_WIDTH + ((Position.X + 8) / TILE_SIZE)]] & TF_W)
      {
        // It's now floting
        Flags |= OFL_FLOATING;
        // Don't let it fall
        Flags &= ~OFL_FALL;
        // Rise
        MOVEY(-1);
      }
      // Else if object is in water and object cant breath water
      else if(~ObjData->Flags & OFL_AQUALUNG && TileData[App.Game.LevelData[((Position.Y + 1) / TILE_SIZE) * LEVEL_WIDTH + ((Position.X + 8) / TILE_SIZE)]] & TF_W)
      {
        // If object is a digger and it isnt in danger
        if(Flags & OFL_DIGGER && Job != JOB_INDANGER)
          // Run!
          SETACTION(ACT_RUN, JOB_INDANGER, DIR_KEEP);
        // Only reduce health once per two game ticks
        if(App.Game.Time % 2) ADJUSTHEALTH(-1);
      }
    }
    // Object is carring inventory?
    if(Items)
    {
      // Create pointer to objects inventory
      register POBJECT Item = Items;
      // Walk inventory
      do
      {
        // Item is first aid and holder has an eligable amount of health
        if(Item->Type == TYP_FIRSTAID && Health > 0 && Health < 100)
        {
          // Increase holder's health
          ADJUSTHEALTH(1);
          // Decrease first aid health
          Item->ADJUSTHEALTH(-1);
        }
        // Item is dead?
        if(Item->Health <= 0)
          // Drop the inventory so it can die and grab next item in inventory
          Item = DROPINVENTORY(Item);
        // Else get next item
        else Item = Item->Next;
      }
      while(Item);
    }
  }

  // Render the object
  RENDER();

  // Animation timer expired?
  if(AnimTimer == ObjData->AnimTimer)
  {
    // Has the incremented sprite id not reached the ending sprite id?
    if(AnimId < DirData->AnimEnd)
      // Next frame
      ++AnimId;
    else if(~Flags & OFL_NOANIMLOOP)
    {
      // Reset sprite tile id
      AnimId = DirData->AnimStart;
      // If sound loops?
      if(Flags & OFL_SOUNDLOOP)
        // Play the sound
        SOUND(ActData->SoundId);
    }
    // Object has attachment?
    if(AnimIdAtt)
    {
      // Has the attachment incremented sprite id not reached the ending sprite id?
      if(AnimIdAtt < DirDataAtt->AnimEnd)
        // Next frame for attachment
        ++AnimIdAtt;
      // End of animation. Sequence can loop?
      else if(~Flags & OFL_NOANIMLOOP)
        // Reset sprite tile id
        AnimIdAtt = DirDataAtt->AnimStart;
    }
    // Reset animation timer
    AnimTimer = 0;
  }
  else ++AnimTimer;

  // Increase action timer
  ++ActionTimer;

  // Return next object
  return Next;
}
/* ================================================================================= */
/* END-OF-FILE                                                           END-OF-FILE */
/* ================================================================================= */
