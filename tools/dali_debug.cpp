// dali_debug.cpp
//
// Interactive low-level DALI test tool for the DALI HAT on the Pi's serial port.
// Uses the same text protocol as fhx_manager's Dali class:
//   "hXXXX\n"  send one 16-bit forward frame        -> reply "N" (no answer) or "Jxx"
//   "tXXXX\n"  send the frame twice (config cmds)    -> two replies
//
// Stop fhx_manager before using this - only one program can own the serial port.
//
// Build (add to CMakeLists.txt):
//   add_executable(dali_debug tools/dali_debug.cpp)
//   target_link_libraries(dali_debug wiringPi)
//
// Usage:
//   ./dali_debug                   interactive prompt, type "help"
//   ./dali_debug scan              run a single command and exit
//   ./dali_debug -d /dev/ttyUSB0 -b 19200    other port / baud rate
 
#include <wiringSerial.h>
#include <unistd.h>
 
#include <algorithm>
#include <cctype>
#include <cerrno>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
 
using Clock = std::chrono::steady_clock;
 
static int  g_fd = -1;
static bool g_verbose = true;   // show raw serial traffic
 
// ------------------------------------------------------------------ helpers
 
static std::string hex2(int v)
{
    std::ostringstream s;
    s << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (v & 0xFF);
    return s.str();
}
 
static std::string lower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}
 
// Accepts decimal ("12") or hex with 0x prefix ("0x0C").
static bool parseInt(const std::string& s, int lo, int hi, int& out)
{
    if (s.empty()) return false;
    int base = (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) ? 16 : 10;
    try {
        size_t used = 0;
        int v = std::stoi(s, &used, base);
        if (used != s.size() || v < lo || v > hi) return false;
        out = v;
        return true;
    } catch (...) {
        return false;
    }
}
 
static void sleepMs(int ms) { usleep(ms * 1000); }
 
// ------------------------------------------------------------------ serial
 
static void drainInput()
{
    while (serialDataAvail(g_fd) > 0) serialGetchar(g_fd);
}
 
// Read one reply line. Returns false if nothing arrived within timeoutMs.
static bool readLine(std::string& out, int timeoutMs)
{
    out.clear();
    auto deadline = Clock::now() + std::chrono::milliseconds(timeoutMs);
    while (Clock::now() < deadline) {
        if (serialDataAvail(g_fd) > 0) {
            int c = serialGetchar(g_fd);
            if (c < 0) break;
            if (c == '\n') return true;
            if (c != '\r') out += static_cast<char>(c);
        } else {
            usleep(500);
        }
    }
    return !out.empty();
}
 
struct Reply {
    std::vector<std::string> lines;   // raw reply lines from the HAT
    bool answered = false;            // got a "Jxx" backward frame
    int  value = -1;                  // the xx
};
 
static Reply sendText(const std::string& text, int expectedLines)
{
    drainInput();
    sleepMs(10);
    serialPuts(g_fd, (text + "\n").c_str());
 
    Reply r;
    std::string line;
    for (int i = 0; i < expectedLines; i++) {
        if (!readLine(line, 500)) break;
        r.lines.push_back(line);
    }
    while (readLine(line, 30)) r.lines.push_back(line);   // anything unexpected
 
    for (const auto& l : r.lines) {
        if (l.size() >= 3 && l[0] == 'J') {
            try {
                r.value = std::stoi(l.substr(1, 2), nullptr, 16);
                r.answered = true;
            } catch (...) {
            }
        }
    }
 
    if (g_verbose) {
        std::cout << "  >> " << text << "   <<";
        if (r.lines.empty()) std::cout << " (nothing - HAT did not reply)";
        for (const auto& l : r.lines) std::cout << " " << l;
        std::cout << "\n";
    }
    return r;
}
 
static Reply sendFrame(int addrByte, int data, bool twice = false)
{
    return sendText(std::string(twice ? "t" : "h") + hex2(addrByte) + hex2(data), twice ? 2 : 1);
}
 
static Reply setDtr(int v) { return sendFrame(0xA3, v); }   // DTR0 special command
 
// ------------------------------------------------------------------ decoding
 
static std::string decodeStatus(int v)
{
    static const char* bits[] = {"gear failure", "lamp failure", "lamp on", "limit error",
                                 "fade running", "reset state", "NO SHORT ADDRESS", "power failure"};
    std::string s;
    for (int i = 0; i < 8; i++)
        if (v & (1 << i)) s += std::string(s.empty() ? "" : ", ") + bits[i];
    return s.empty() ? "ok, lamp off" : s;
}
 
static std::string decodeDevType(int v)
{
    switch (v) {
        case 0:    return "fluorescent";
        case 1:    return "emergency";
        case 6:    return "LED (DT6)";
        case 8:    return "colour control (DT8)";
        case 0xFF: return "multiple device types";
        default:   return "device type " + std::to_string(v);
    }
}
 
static std::string decodeGroups(int v, int first)
{
    std::string s;
    for (int i = 0; i < 8; i++)
        if (v & (1 << i)) s += std::string(s.empty() ? "" : " ") + "g" + std::to_string(first + i);
    return s.empty() ? "none" : s;
}
 
// ------------------------------------------------------------------ command tables
 
struct Cmd {
    const char* name;
    int  opcode;
    bool query;
    bool twice;
};
 
static const Cmd CMDS[] = {
    {"off",      0x00, false, false},
    {"up",       0x01, false, false},
    {"down",     0x02, false, false},
    {"stepup",   0x03, false, false},
    {"stepdown", 0x04, false, false},
    {"max",      0x05, false, false},
    {"min",      0x06, false, false},
    {"reset",    0x20, false, true },
    {"status",   0x90, true,  false},
    {"present",  0x91, true,  false},
    {"lampfail", 0x92, true,  false},
    {"lampon",   0x93, true,  false},
    {"version",  0x97, true,  false},
    {"dtr",      0x98, true,  false},
    {"devtype",  0x99, true,  false},
    {"physmin",  0x9A, true,  false},
    {"actual",   0xA0, true,  false},
    {"maxlevel", 0xA1, true,  false},
    {"minlevel", 0xA2, true,  false},
    {"poweron",  0xA3, true,  false},
    {"sysfail",  0xA4, true,  false},
    {"fade",     0xA5, true,  false},
    {"randh",    0xC2, true,  false},
    {"randm",    0xC3, true,  false},
    {"randl",    0xC4, true,  false},
};
 
// "set..." commands: DTR0 = value, then the config command sent twice
struct SetCmd { const char* name; int opcode; int lo; int hi; };
static const SetCmd SETCMDS[] = {
    {"setmax",     0x2A, 0, 254},
    {"setmin",     0x2B, 0, 254},
    {"setsysfail", 0x2C, 0, 255},
    {"setpoweron", 0x2D, 0, 255},
    {"setfade",    0x2E, 0, 15 },
};
 
static void printAnswer(const char* name, int opcode, const Reply& r)
{
    std::cout << "  " << std::left << std::setw(10) << name << std::right;
    if (!r.answered) {
        std::cout << "no answer\n";
        return;
    }
    int v = r.value;
    std::cout << "0x" << hex2(v) << " (" << std::dec << v << ")";
    if (opcode == 0x90) std::cout << "  " << decodeStatus(v);
    if (opcode == 0x99) std::cout << "  " << decodeDevType(v);
    if (opcode == 0xA5) std::cout << "  fade time " << (v >> 4) << ", fade rate " << (v & 0x0F);
    std::cout << "\n";
}
 
// ------------------------------------------------------------------ targets
 
struct Target {
    int base;            // address byte with bit 0 cleared (bit 0 = 1 for commands)
    std::string name;
    bool single;         // a single short address
};
 
static bool parseTarget(const std::string& s, Target& t)
{
    int n;
    if (s == "bc" || s == "all") {
        t = {0xFE, "broadcast", false};
        return true;
    }
    if (s.size() > 1 && s[0] == 'g' && parseInt(s.substr(1), 0, 15, n)) {
        t = {0x80 | (n << 1), "group " + std::to_string(n), false};
        return true;
    }
    if (parseInt(s, 0, 63, n)) {
        t = {n << 1, "A" + std::to_string(n), true};
        return true;
    }
    return false;
}
 
// ------------------------------------------------------------------ composite commands
 
static void info(const Target& t)
{
    bool v = g_verbose;
    g_verbose = false;
    int a = t.base | 1;
 
    std::cout << t.name << ":\n";
    for (int op : {0x91, 0x90, 0x99, 0x97, 0xA0, 0xA1, 0xA2, 0x9A, 0xA3, 0xA4, 0xA5}) {
        for (const auto& c : CMDS) {
            if (c.query && c.opcode == op) {
                printAnswer(c.name, op, sendFrame(a, op));
                break;
            }
        }
    }
 
    Reply g0 = sendFrame(a, 0xC0), g1 = sendFrame(a, 0xC1);
    std::cout << "  groups    "
              << (g0.answered ? decodeGroups(g0.value, 0) : "?") << " | "
              << (g1.answered ? decodeGroups(g1.value, 8) : "?") << "\n";
 
    Reply rh = sendFrame(a, 0xC2), rm = sendFrame(a, 0xC3), rl = sendFrame(a, 0xC4);
    std::cout << "  random    ";
    if (rh.answered && rm.answered && rl.answered)
        std::cout << hex2(rh.value) << hex2(rm.value) << hex2(rl.value) << "\n";
    else
        std::cout << "no answer\n";
 
    g_verbose = v;
}
 
static void scan()
{
    bool v = g_verbose;
    g_verbose = false;
 
    std::cout << "Scanning short addresses 0-63...\n";
    int found = 0, silent = 0;
 
    for (int a = 0; a < 64; a++) {
        int ab = (a << 1) | 1;
        Reply r = sendFrame(ab, 0x91);   // QUERY CONTROL GEAR PRESENT
 
        if (r.lines.empty()) silent++;
        bool odd = false;
        for (const auto& l : r.lines)
            if (!(l == "N" || (l.size() >= 3 && l[0] == 'J'))) odd = true;
        if (!r.answered && !odd) continue;
 
        found++;
        std::cout << "  A" << std::left << std::setw(3) << a << std::right;
        if (r.answered && r.value != 0xFF)
            std::cout << " answer 0x" << hex2(r.value) << " (expected FF - two devices on this address?)";
        if (odd) {
            std::cout << " raw:";
            for (const auto& l : r.lines) std::cout << " " << l;
        }
 
        Reply dt = sendFrame(ab, 0x99), lv = sendFrame(ab, 0xA0), st = sendFrame(ab, 0x90);
        if (dt.answered) std::cout << " " << decodeDevType(dt.value);
        if (lv.answered) std::cout << ", level " << lv.value;
        if (st.answered) std::cout << ", " << decodeStatus(st.value);
        std::cout << "\n";
    }
 
    std::cout << found << " address(es) answered.\n";
    if (silent == 64)
        std::cout << "WARNING: the HAT never replied - wrong port/baud, or is fhx_manager still running?\n";
 
    // QUERY MISSING SHORT ADDRESS, broadcast: anything but "N" means someone has no address
    Reply m = sendFrame(0xFF, 0x96);
    bool unaddressed = false;
    for (const auto& l : m.lines)
        if (l != "N") unaddressed = true;
    std::cout << (unaddressed ? "At least one device on the bus has NO short address.\n"
                              : "No unaddressed devices answered.\n");
 
    g_verbose = v;
}
 
// ------------------------------------------------------------------ help
 
static void printHelp()
{
    std::cout <<
        "Targets: 0-63 = short address, g0-g15 = group, bc = broadcast\n"
        "  <t> max | min | off | up | down | stepup | stepdown\n"
        "  <t> level N          direct arc power, N = 0-254\n"
        "  <t> scene N          go to scene N (0-15)\n"
        "  <t> blink            max/off three times - shows which light it is\n"
        "  <t> info             query everything (use a short address)\n"
        "  <t> status | present | actual | devtype | version | maxlevel | minlevel\n"
        "      physmin | poweron | sysfail | fade | lampfail | lampon | dtr\n"
        "      randh | randm | randl\n"
        "  <t> groups           which groups it belongs to\n"
        "  <t> setmax N | setmin N | setpoweron N | setsysfail N | setfade N\n"
        "  <t> addgroup N | removegroup N\n"
        "  <t> setaddr N        give it short address N (single address only)\n"
        "  <t> deladdr          remove its short address\n"
        "  <t> reset            factory-reset its settings (keeps short address)\n"
        "General:\n"
        "  scan                 list all short addresses that answer\n"
        "  raw XXXX             send a 16-bit frame once,  e.g. raw 0105 = A0 max\n"
        "  twice XXXX           send a frame twice (config commands)\n"
        "  hat TEXT             send TEXT to the HAT as-is, e.g. hat v\n"
        "  verbose              toggle showing raw serial traffic\n"
        "  quit\n"
        "Note: queries to a group or broadcast get garbled if several devices answer.\n";
}
 
// ------------------------------------------------------------------ dispatcher
 
static void runCommand(const std::string& line)
{
    std::istringstream in(line);
    std::vector<std::string> w;
    for (std::string x; in >> x;) w.push_back(lower(x));
    if (w.empty()) return;
 
    const std::string c0 = w[0];
    int n;
 
    if (c0 == "help" || c0 == "?") { printHelp(); return; }
    if (c0 == "scan") { scan(); return; }
    if (c0 == "verbose") {
        g_verbose = !g_verbose;
        std::cout << "verbose " << (g_verbose ? "on" : "off") << "\n";
        return;
    }
 
    if (c0 == "hat") {
        std::string rest = line.substr(lower(line).find("hat") + 3);
        rest.erase(0, rest.find_first_not_of(" \t"));
        if (rest.empty()) { std::cout << "usage: hat TEXT   e.g. hat v\n"; return; }
        bool v = g_verbose;
        g_verbose = true;
        sendText(rest, 1);
        g_verbose = v;
        return;
    }
 
    if (c0 == "raw" || c0 == "twice") {
        bool ok = w.size() == 2 && w[1].size() == 4 &&
                  std::all_of(w[1].begin(), w[1].end(), [](unsigned char c) { return std::isxdigit(c); });
        if (!ok) { std::cout << "usage: " << c0 << " XXXX   (4 hex digits, e.g. raw 0105)\n"; return; }
        std::string hex = w[1];
        std::transform(hex.begin(), hex.end(), hex.begin(),
                       [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
        bool v = g_verbose;
        g_verbose = true;
        Reply r = sendText((c0 == "raw" ? "h" : "t") + hex, c0 == "raw" ? 1 : 2);
        g_verbose = v;
        if (r.answered) std::cout << "  answer 0x" << hex2(r.value) << " (" << r.value << ")\n";
        return;
    }
 
    Target t;
    if (!parseTarget(c0, t)) { std::cout << "Unknown command '" << c0 << "'. Type 'help'.\n"; return; }
    if (w.size() < 2) { std::cout << "What should " << t.name << " do? e.g. '" << c0 << " max'\n"; return; }
 
    const std::string cmd = w[1];
    const std::string arg = w.size() > 2 ? w[2] : "";
    const int a = t.base | 1;   // address byte for commands (bit 0 = 1)
 
    if (cmd == "level") {
        if (!parseInt(arg, 0, 254, n)) { std::cout << "usage: <t> level 0-254\n"; return; }
        sendFrame(t.base, n);   // direct arc power: bit 0 = 0
        return;
    }
    if (cmd == "scene") {
        if (!parseInt(arg, 0, 15, n)) { std::cout << "usage: <t> scene 0-15\n"; return; }
        sendFrame(a, 0x10 + n);
        return;
    }
    if (cmd == "blink") {
        for (int i = 0; i < 3; i++) {
            sendFrame(a, 0x05);
            sleepMs(700);
            sendFrame(a, 0x00);
            sleepMs(700);
        }
        return;
    }
    if (cmd == "info") { info(t); return; }
    if (cmd == "groups") {
        Reply g0 = sendFrame(a, 0xC0), g1 = sendFrame(a, 0xC1);
        std::cout << "  groups: " << (g0.answered ? decodeGroups(g0.value, 0) : "?") << " | "
                  << (g1.answered ? decodeGroups(g1.value, 8) : "?") << "\n";
        return;
    }
    if (cmd == "addgroup" || cmd == "removegroup") {
        if (!parseInt(arg, 0, 15, n)) { std::cout << "usage: <t> " << cmd << " 0-15\n"; return; }
        sendFrame(a, (cmd == "addgroup" ? 0x60 : 0x70) + n, true);
        return;
    }
    if (cmd == "setaddr" || cmd == "deladdr") {
        if (!t.single) { std::cout << cmd << " only works on a single short address.\n"; return; }
        if (cmd == "setaddr") {
            if (!parseInt(arg, 0, 63, n)) { std::cout << "usage: <addr> setaddr 0-63\n"; return; }
            setDtr((n << 1) | 1);
        } else {
            setDtr(0xFF);
        }
        sendFrame(a, 0x80, true);   // STORE DTR AS SHORT ADDRESS
        std::cout << "  Done. Verify with 'scan'.\n";
        return;
    }
    for (const auto& s : SETCMDS) {
        if (cmd == s.name) {
            if (!parseInt(arg, s.lo, s.hi, n)) {
                std::cout << "usage: <t> " << s.name << " " << s.lo << "-" << s.hi << "\n";
                return;
            }
            setDtr(n);
            sendFrame(a, s.opcode, true);
            return;
        }
    }
    for (const auto& c : CMDS) {
        if (cmd == c.name) {
            Reply r = sendFrame(a, c.opcode, c.twice);
            if (c.query) printAnswer(c.name, c.opcode, r);
            return;
        }
    }
    std::cout << "Unknown command '" << cmd << "'. Type 'help'.\n";
}
 
// ------------------------------------------------------------------ main
 
int main(int argc, char** argv)
{
    std::string dev = "/dev/serial0";
    int baud = 19200;
    std::string oneShot;
 
    for (int i = 1; i < argc; i++) {
        std::string s = argv[i];
        if (s == "-d" && i + 1 < argc)      dev = argv[++i];
        else if (s == "-b" && i + 1 < argc) baud = std::atoi(argv[++i]);
        else oneShot += (oneShot.empty() ? "" : " ") + s;
    }
 
    g_fd = serialOpen(dev.c_str(), baud);
    if (g_fd < 0) {
        std::cerr << "Unable to open " << dev << ": " << std::strerror(errno) << "\n";
        return 1;
    }
    sleepMs(100);
    drainInput();
 
    if (!oneShot.empty()) {
        runCommand(oneShot);
        serialClose(g_fd);
        return 0;
    }
 
    std::cout << "DALI debug on " << dev << " @ " << baud << " baud. Type 'help'.\n";
    std::string line;
    while (std::cout << "dali> " << std::flush, std::getline(std::cin, line)) {
        std::string l = lower(line);
        if (l == "quit" || l == "exit" || l == "q") break;
        runCommand(line);
    }
 
    serialClose(g_fd);
    return 0;
}
 
