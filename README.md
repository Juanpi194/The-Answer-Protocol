*This project has been created as part of the 42 curriculum by jvizcain, rohena, cdonaire.*

# TAP — The Answer Protocol

A shared-world retro text adventure: a TCP server and two interchangeable clients (CLI and GUI) speaking a line-based, RFC-style protocol (**RFC 42TAP**, see [`protocol-rfc.html`](./protocol-rfc.html)).

## Description

TAP is a small multiplayer MUD (Multi-User Dungeon). Multiple players connect over TCP to a shared server, explore a persistent world of interconnected rooms, fight enemies, talk to NPCs, take on quests, trade items, and chat with each other — all through a simple, line-oriented text protocol.

The project has three parts, built as a single C++ codebase:

- **`tap`** — the server. Owns the world state, accepts connections, and is the only authority on what actually happens in the game.
- **`tap_tui_client`** — a terminal client. Connects to the server and lets you play by typing raw protocol commands.
- **`tap_gui_client`** — a graphical client (Dear ImGui + SDL2). Connects to the same server and gives you buttons, panels, and live-updating views instead of typed commands, while still speaking the exact same protocol underneath.

Both clients are protocol-compatible with any other group's server built against RFC 42TAP — neither client hardcodes anything about *this* group's server implementation beyond the protocol itself.

## Instructions

Quick start (see [Building and Running](#building-and-running) below for the full picture):

```bash
make install       # fetch json.hpp and Dear ImGui (once, on a fresh checkout)
make all            # build server + both clients
./tap                # in one terminal: the server
./tap_tui_client      # in another: the CLI client
./tap_gui_client      # or: the GUI client
```

## Resources

**References used while building this:**
- [RFC 42TAP](./protocol-rfc.html) — the protocol specification this project implements.
- [nlohmann/json](https://github.com/nlohmann/json) — single-header JSON library used both server-side (world files, protocol payloads) and client-side (parsing responses in the GUI).
- [Dear ImGui](https://github.com/ocornut/imgui) — immediate-mode GUI toolkit used for the graphical client, with the SDL2 + OpenGL3 backends.
- [SDL2](https://www.libsdl.org/) — windowing/input backend for the GUI client.
- Standard BSD sockets API (`man 2 socket`, `man 2 connect`, `man 2 accept`) for the TCP layer on both server and clients.

**How AI was used:**
AI (Claude) was used as a pair-programming and debugging assistant across the project, primarily on the client side (CLI and GUI implementations, the shared networking class, and the `Makefile`) and for reviewing/fixing specific bugs pointed out during manual testing. Concretely, it was used for:
- Iterating on the GUI client (Dear ImGui layout, the `WorldCache` that reconstructs room/stats/inventory/quest state from parsed server responses, and the combat/quest/stats panels).
- Diagnosing real build and runtime issues by actually compiling and testing candidate fixes rather than guessing — for example, tracing a GUI freeze to `ThreadSanitizer` overhead on the graphics driver (not a deadlock in our own code), and tracing a "map not loading" report down to a specific missing `,` in a hand-built JSON string.
- Writing and iterating on the `Makefile` (build targets, the ImGui/json.hpp fetch step, the sanitizer-enabled build).
- Drafting this README from the subject, the RFC, and the server's own `HELP` command text.
All AI-assisted code was reviewed, compiled, and tested by the team before being kept; sections describing game-design decisions (combat formulas, quest mechanics, world layout) reflect the team's own choices, not AI suggestions.

## Architecture

<!-- TODO(team): confirm/expand — this section describes the client-side architecture in detail (built collaboratively with AI assistance) and the server-side shape as currently understood; the server owners should verify and fill in specifics (concurrency model details, dispatcher internals) that live in code not covered by this pass. -->

**Server.** Commands are parsed into a `Command` struct (`CommandParser::parse`) and routed through a single dispatcher (`CommandHandler::handle`), which switches on a `CommandType` enum to a dedicated handler function per command (`cmd_look`, `cmd_move`, `cmd_take`, `cmd_fight`, ...). This is a classic **dispatcher/router** design rather than inline per-connection handling, which keeps each command's logic in one place and makes it straightforward to add new commands.

**Concurrency model.** The server runs several long-lived threads:
- `main` — the game loop, reading and dispatching commands from connected clients.
- `accept` — continuously accepts new incoming TCP connections.
- one `client` thread per connected player.
- `owner` — reads the server operator's console (`INIT`, `SHUTDOWN`, `BAN`, ...), separate from player traffic.

**Clients.** Both clients share a single class, `CLI` (`inc/ui/CLI.hpp` / `src/ui/CLI.cpp`), that owns the TCP socket, a background receive thread, and a thread-safe inbound message queue. `CLI` is intentionally "dumb": it knows nothing about rooms, items, or JSON — it only opens a connection, sends raw lines, and hands back whatever lines arrive. This is what keeps the two clients interchangeable with any other group's server: all protocol-specific interpretation happens above `CLI`, per client.
- The **TUI** (`ui/tui/main.cpp`) sends typed lines straight through and prints whatever comes back — a thin, direct RFC pass-through (see [Protocol Implementation](#protocol-implementation) for the one exception, the connection bootstrap).
- The **GUI** (`ui/gui/main.cpp`) adds a `WorldCache` layer that parses the JSON payloads `CLI` hands it (room state from `LOOK`, stats from `STATUS`, inventory from `INVENTORY`, quests from `QUESTS`, and combat state from `ATTACK`/`DEFEND`/`FLEE`/`CONSUME`) and keeps the last-known state of each, which the panels render every frame. Connecting is done on a background thread so the render loop never blocks on the network.

## Protocol Implementation

The server and both clients implement the full RFC 42TAP command set (`CONNECT`, `LOOK`, `MOVE`, `CHAT`, `TAKE`, `DROP`, `INVENTORY`, `TALK`, `ATTACK`, `STATUS`, `QUEST`, `QUESTS`, `WHO`, `GROUP`, `QUIT`), plus protocol extensions the RFC explicitly leaves open for implementers (§6.1): `FIGHT` (initiate combat with the enemy in the room), `DEFEND`, `FLEE`, `CONSUME` (a consumable item, during combat), `OPEN` (a chest in the room), `BUY`, `EQUIP`, `ENCHANT`, and `HELP`. The exact syntax for every command, as reported by the server itself, is:

| Command | Syntax |
|---|---|
| `CONNECT` | One argument, the client name. |
| `LOOK` | No arguments. Returns the player's room as JSON. |
| `MOVE` | One argument, a direction. |
| `CHAT` | Two arguments: scope (`GLOBAL`, `ROOM`, `GROUP`) and the message. |
| `TAKE` | One argument, the item to take. |
| `DROP` | One argument, the item to drop in the current room. |
| `INVENTORY` | No arguments. Returns the player's inventory. |
| `TALK` | One argument, the NPC to talk to. |
| `OPEN` | No arguments. Opens the chest in the current room, if any. |
| `BUY` | One argument, the item to buy (spends the player's gold). |
| `EQUIP` | One argument, the gear to equip (shields, armor, weapons only). |
| `ENCHANT` | Two arguments: the enchantment and the gear to apply it to. |
| `FIGHT` | No arguments. Starts a fight with the enemy in the room, if any. |
| `ATTACK` | No arguments. Attacks the enemy in the current fight. |
| `DEFEND` | No arguments. Defends against the enemy's next attack. |
| `FLEE` | No arguments. Leaves the current fight (at a gold cost). |
| `CONSUME` | One argument, the consumable to use during the current fight. |
| `STATUS` | No arguments. Returns the player's status. |
| `QUEST` | One argument, the Quest Giver's name. Requests a quest. |
| `QUESTS` | No arguments. Returns the player's quest list. |
| `WHO` | No arguments. Returns the number of connected players. |
| `GROUP` | At least one argument (scope: `CREATE`, `INVITE`, `JOIN`, `KICK`, `LEAVE`), some scopes need a second. |
| `QUIT` | No arguments. Disconnects the client. |
| `HELP` | No arguments. Shows this command list. |

**Deviation — connection bootstrap.** The RFC (§3.1–3.3) describes the server pushing an initial `OK hello proto=1` greeting immediately after the TCP handshake, before the client sends `CONNECT`. Our clients do not wait for or parse that greeting: opening the socket and sending `CONNECT` are two independent, explicit steps from the player's point of view. Concretely, both clients start from a small prompt that accepts a line like:

```
nc 127.0.0.1 8080
```

(also accepting `nc <host>`, `nc <host>:<port>`, or bare `nc` for `127.0.0.1:8080`, the server's default port) — deliberately mirroring the everyday experience of opening a raw connection with `nc(1)` before typing anything at the protocol level. Only once that socket is open does the player type `CONNECT <name>` themselves, exactly as they would any other command, and it is sent and answered like any other command. We chose this because it keeps `CLI` a pure, protocol-agnostic transport (see [Architecture](#architecture)) — it never has to special-case a "first message after connecting", which also means it never has to be updated if the RFC's greeting text changes. Everything from `CONNECT` onward follows the RFC exactly.

<!-- TODO(team): document any other deviations from RFC 42TAP here (event delivery format, exact error-code coverage, anything else decided during implementation) that this pass didn't have visibility into. -->

## Combat System

<!-- TODO(team): fill in with the actual mechanics implemented server-side — this pass only had visibility into the client-facing shape of combat, not the final damage formulas/initiative rules. -->

The server exposes turn-based combat through `FIGHT` (start), `ATTACK`, `DEFEND`, `FLEE`, and `CONSUME` (act during a fight), and reports live combat state as JSON after every turn — both fighters' current/max HP, strength, defense, speed, and status. Please document here:
- The damage formula (how strength/defense/speed factor into damage dealt and received).
- Initiative/turn order (who acts first, how simultaneity or speed ties are resolved).
- What `DEFEND` and `FLEE` actually do mechanically (damage reduction amount, gold cost of fleeing, cooldowns).
- How a fight starts, ends (victory/defeat), and what happens to the player at 0 HP (respawn location, HP penalty).

## Quest System

<!-- TODO(team): fill in with the actual mechanics implemented server-side. -->

Quests are requested from Quest Giver NPCs via `QUEST <name>` and listed via `QUESTS`; each quest carries a description, a gold reward, an optional item reward, and a status (in progress / completed). Please document here:
- How quest objectives are tracked and validated (what actually triggers completion — killing an NPC, reaching a room, delivering an item, etc.).
- Whether completion is automatic (detected by the server as soon as the objective is met) or requires an explicit player action.
- How rewards are distributed once a quest completes.
- Whether any quests have prerequisites or form a chain.

## World Design

<!-- TODO(team): fill in with the specifics of the submitted world file — this pass worked with a small test map, not necessarily the final submitted world. -->

The world is defined in a JSON file (see `map/default.json`) and loaded by the server at startup: a `world` object (name and spawn room), a `rooms` array (each with an id, name, description, exits, and an optional single occupant — an enemy or a pacific NPC), and an `items` object. The subject requires at least 8 interconnected rooms forming one or more loops plus a branch, at least 3 distinct NPC roles, at least 4 distinct items (2+ obtainable in-world), and at least 2 quests. Please document here:
- The final room count and layout (a short map/diagram is ideal).
- Which NPCs occupy which rooms and in what role (dialogue / quest-giver / enemy).
- Which items exist and where they're found.
- The 2+ quests and where they're offered.

## Server Logging

<!-- TODO(team): fill in with the actual logging implementation — this pass focused on the client side and didn't have visibility into the server's logging setup. -->

Please document here:
- Log format (the subject recommends structured/JSON logging) and log levels used (INFO/WARN/ERROR).
- What gets logged: connections/disconnections (with timestamp and IP), every command received (with player and parameters), every response/error sent, world state changes (item movement, NPC interaction, combat results), and quest progress/completion.
- Where logs are written (stdout, a file, both) and how to monitor them live.
- How abuse patterns (command flooding, rapid reconnects) are detected and logged.

## Group Contributions

<!-- TODO(team): fill in with each member's actual responsibilities. -->

| Member | Responsibilities |
|---|---|
| `<login1>` | |
| `<login2>` | |
| `<login3>` | |

## Building and Running

The build system is a `Makefile` at the repository root.

**One-time setup on a fresh checkout:**
```bash
make install
```
Fetches `inc/libs/json.hpp` (nlohmann::json, single header) and Dear ImGui (`inc/libs/imgui`, both fetched with `curl`, file by file — no `git clone` involved) via HTTPS. `make gui` on a fresh checkout requires this to have been run first — see the note on the `gui` target below.

**Compiling:**
```bash
make all      # server + both clients
make server   # only the server (produces ./tap)
make tui      # only the CLI client (produces ./tap_tui_client)
make gui      # only the GUI client (produces ./tap_gui_client) -- run `make install` first
```

**Running:**
```bash
make run-server   # compiles (if needed) and launches ./tap
make run-tui       # compiles (if needed) and launches ./tap_tui_client
make run-gui       # compiles (if needed) and launches ./tap_gui_client
```
Or run the built binaries directly: `./tap`, `./tap_tui_client`, `./tap_gui_client`.

The server also has a standalone **debug mode** (`make debug-mode`, or `./tap --debug`) that runs the game engine locally, reading commands straight from stdin with no socket involved — useful for testing world/command logic in isolation from networking.

**Sanitized build.** `make all-sanitize` compiles the server and CLI client with `-fsanitize=thread` (`tap_sanitize`, `tap_tui_client_sanitize`), in a separate object tree so it never conflicts with a normal `make all` — both can be built back to back without a `clean` in between. The GUI client is deliberately excluded from this: ThreadSanitizer instruments every thread in the process, including the graphics driver's own internal threads (shader compiler, GL worker), which makes the GUI appear to hang under the OS's "not responding" watchdog even though nothing is actually deadlocked — confirmed by instrumenting the render loop and watching it keep iterating normally the whole time. The CLI client shares the exact same networking class (`CLI`) as the GUI, so it's a faithful stand-in for testing that code path under TSan without the driver noise.

**Other targets:**
```bash
make valgrind-run   # runs the server under valgrind, output in result.txt
make clean          # removes compiled object files
make fclean          # clean + removes binaries + removes installed dependencies
make re               # fclean, then install, then all (as three separate `make` invocations --
                       # see note below)
```

> **Note on `re`:** `re` runs `fclean`, `install`, and `all` as three separate `make` sub-invocations rather than as prerequisites of one target. GNU Make resolves whether a pattern rule applies before running any recipe; `install`'s freshly-fetched ImGui files aren't "visible" for that resolution within the same `make` process even after `install`'s recipe has actually completed, which broke `make gui` when this was a single combined target. Separate `make` invocations avoid the issue entirely, since each one re-scans the filesystem from scratch.

## Testing

<!-- TODO(team): expand with the specific scenarios you actually verified before submission. -->

**Multiplayer:** start one server (`make run-server`) and connect with two or more clients (any mix of `tap_tui_client` / `tap_gui_client`) to the same `host:port`. Confirm that actions from one player (moving rooms, chatting, taking items) are visible to the others in the same room, and that `WHO` reflects the correct count.

**Combat:** approach a room with a hostile NPC, `FIGHT` to start, then exercise `ATTACK`/`DEFEND`/`FLEE`/`CONSUME` and confirm `STATUS` and the combat responses reflect HP changes correctly on both sides, including defeat/respawn.

**Quests:** `QUEST <name>` a Quest Giver, confirm it appears via `QUESTS`, complete the underlying objective, and confirm the quest's status and reward are applied correctly.

**Sanitizer:** `make all-sanitize` and run `./tap_sanitize` / `./tap_tui_client_sanitize` through the same multiplayer/combat/quest scenarios above, watching for `ThreadSanitizer` reports (see [Building and Running](#building-and-running) for why the GUI is excluded from this).

