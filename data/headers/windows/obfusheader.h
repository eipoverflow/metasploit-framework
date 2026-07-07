/*
         ██████╗ ██████╗ ███████╗██╗   ██╗███████╗     ██╗
        ██╔═══██╗██╔══██╗██╔════╝██║   ██║██╔════╝     ██║
        ██║   ██║██████╔╝█████╗  ██║   ██║███████╗     ███████║
        ██║   ██║██╔══██╗██╔══╝  ██║   ██║╚════██║     ██╔══██║
        ╚██████╔╝██████╔╝██║     ╚██████╔╝███████║ ██╗ ██║  ██║
         ╚═════╝ ╚═════╝ ╚═╝      ╚═════╝ ╚══════╝ ╚═╝ ╚═╝  ╚═╝
                  Very reliable armor for your C programs!
                        Coded by (C) DosX, 2025

 [Additional options]
 ~ CFLOW_V2       = more powerful Control Flow obfuscation (slowly!)
 ~ ANTIDEBUG_V2   = use better dynamic anti-debugging protection
 ~ FAKE_SIGNS     = adds fake signatures of various protectors or packers

 [Advanced code protection]
 ~ VIRT           = allows you to use the functions of a math VM

 [Disabling default features]
 ~ NO_CFLOW       = disable control flow obfuscation
 ~ NO_ANTIDEBUG   = disable antidebug protection

 ~ NO_OBF         = disable obfuscation

 GitHub:
 -> https://github.com/DosX-dev/obfus.h

 (Full documentation and examples are available on the GitHub page)

 ---------------------------------------------------------------------
 NOTE: inline-asm blocks that contain register mnemonics are written as
 *extended* asm using GCC's `{att | intel}` dialect-alternative syntax,
 so each block assembles correctly whether the file is built in the
 default AT&T mode or with `-masm=intel`. GCC only expands `{ | }` (and
 collapses `%%`) in extended asm, which is why those blocks carry a
 `:::` operand/clobber section. Dialect-neutral blocks (`nop`, `cpuid`,
 `ret`, `.byte`, labels) need no alternatives and stay as basic asm.
 ---------------------------------------------------------------------
*/

#ifndef OBFH
#define OBFH

#if !__TINYC__ && !__GNUC__ && !__MINGW32__
#define __attribute__(...)
#endif

// if virtualization disabled
#if NO_OBF == 1 || VIRT != 1
#define VM_ADD(num1, num2) num1 + num2
#define VM_SUB(num1, num2) num1 - num2
#define VM_MUL(num1, num2) num1 *num2
#define VM_DIV(num1, num2) num1 / num2
#define VM_MOD(num1, num2) num1 % num2
#define VM_EQU(num1, num2) num1 == num2
#define VM_NEQ(num1, num2) num1 != num2
#define VM_LSS(num1, num2) num1 < num2
#define VM_GTR(num1, num2) num1 > num2
#define VM_LEQ(num1, num2) num1 <= num2
#define VM_GEQ(num1, num2) num1 >= num2
#define VM_ADD_DBL(num1, num2) num1 + num2
#define VM_SUB_DBL(num1, num2) num1 - num2
#define VM_MUL_DBL(num1, num2) num1 *num2
#define VM_DIV_DBL(num1, num2) num1 / num2
#define VM_LSS_DBL(num1, num2) num1 < num2
#define VM_GTR_DBL(num1, num2) num1 > num2
#define VM_IF(condition) if (condition)
#define VM_ELSE_IF(condition) else if (condition)
#define VM_ELSE else
#define VM_OBF_INT(num) num
#define VM_OBF_DBL(num) num
#endif

#if NO_OBF == 1
#define HIDE_STRING(str) str
#define ANTI_DEBUG 0
#endif

#if !NO_OBF

#include <conio.h>
#include <direct.h>  // FIX #6: needed for _getcwd on MinGW
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#if defined _MSC_VER
#warning obfus.h doesn't support Visual C/C++. You can use [obfusheader.h] by ac3ss0r to obfuscate this app (https://github.com/ac3ss0r/obfusheader.h)
#endif

#if !defined __COUNTER__
#error You are using too old a compiler version!
#endif

#ifndef __asm__
#define __asm__(...)
#endif

#define SECTION_ATTRIBUTE(NAME) __attribute__((section(NAME)))

#define DATA_SECTION_ATTRIBUTE SECTION_ATTRIBUTE(".data")  // Data section

// FIX: a single named section can't hold both code and data (modern binutils
// rejects this as a "section type conflict"). The decoy letter/digit variables
// must live in a data-only section, separate from the function sections.
#define OBFH_VAR_SECTION_ATTRIBUTE DATA_SECTION_ATTRIBUTE  // decoy vars -> data

// Fake signatures ;)
#if FAKE_SIGNS == 1

static const char FAKE_ENIGMAVM_1[] SECTION_ATTRIBUTE(".enigma1") = {0};
static const char FAKE_ENIGMAVM_2[] SECTION_ATTRIBUTE(".enigma2") = {0};
static const char FAKE_VMPROTECT_1[] SECTION_ATTRIBUTE(".vmp0") = {0};  // (now is open-source)
static const char FAKE_VMPROTECT_2[] SECTION_ATTRIBUTE(".vmp1") = {0};
static const char FAKE_VMPROTECT_3[] SECTION_ATTRIBUTE(".vmp2") = {0};

#define OBFH_SECTION_ATTRIBUTE SECTION_ATTRIBUTE("UPX0")  // OBFH section (code)
// FIX: keep this decoy in a data section; UPX0 now holds OBFH functions (code)
// and can't also hold data without a section type conflict.
static const char FAKE_UPX[] DATA_SECTION_ATTRIBUTE = {0};

static const char FAKE_THEMIDA[] SECTION_ATTRIBUTE(".winlice") = {0};
static const char FAKE_PETITE[] SECTION_ATTRIBUTE(".petite") = {0};
static const char FAKE_RLP[] SECTION_ATTRIBUTE(".rlp") = {0};
static const char FAKE_SECUROM[] SECTION_ATTRIBUTE(".dsstext") = {0};
static const char FAKE_SQUISHY[] SECTION_ATTRIBUTE("logicoma") = {0};
static const char FAKE_THEARK_1[] SECTION_ATTRIBUTE("adr") = {0};
static const char FAKE_THEARK_2[] SECTION_ATTRIBUTE("have") = {0};
static const char FAKE_THEARK_3[] SECTION_ATTRIBUTE("30cm") = {0};
static const char FAKE_PETETRIS[] SECTION_ATTRIBUTE("PETETRIS") = {0};

static const char FAKE_ENIGMA[] SECTION_ATTRIBUTE(".data") = {0x45, 0x6e, 0x69, 0x67,
                                                              0x6d, 0x61, 0x20, 0x70,
                                                              0x72, 0x6f, 0x74, 0x65,
                                                              0x63, 0x74, 0x6f, 0x72,
                                                              0x20, 0x76, 0x01};

static const char FAKE_ALINYZE[] SECTION_ATTRIBUTE(".alien") = {0};
static const char FAKE_PWDPROTECT[] SECTION_ATTRIBUTE(".pwdprot") = {0};

static const char FAKE_DENUVO[] SECTION_ATTRIBUTE(".arch") = {0x64, 0x65, 0x6E, 0x75,
                                                              0x76, 0x6F, 0x5F, 0x61,
                                                              0x74, 0x64, 0x00, 0x00,
                                                              0x00, 0x00, 0x00, 0x00};

static const char FAKE_NUITKA[] SECTION_ATTRIBUTE(".rdata") = {0x4e, 0x55, 0x49, 0x54,
                                                               0x4b, 0x41, 0x5f, 0x4f,
                                                               0x4e, 0x45, 0x46, 0x49,
                                                               0x4c, 0x45, 0x5f, 0x50,
                                                               0x41, 0x52, 0x45, 0x4e,
                                                               0x54};

static const char FAKE_THEARK_4[] SECTION_ATTRIBUTE(".tw") = {0};
static const char FAKE_THEARK_5[] SECTION_ATTRIBUTE("logicoma") = {0};
static const char FAKE_OREANSVM[] SECTION_ATTRIBUTE(".vlizer") = {0};

static const char FAKE_SCREEN2EXE[] SECTION_ATTRIBUTE(".text") = {0x56, 0x69, 0x64, 0x65,
                                                                  0x6f, 0x20, 0x63, 0x72,
                                                                  0x65, 0x61, 0x74, 0x65,
                                                                  0x64, 0x20, 0x62, 0x79,
                                                                  0x20, 0x53, 0x43, 0x52,
                                                                  0x45, 0x45, 0x4e, 0x32,
                                                                  0x45, 0x58, 0x45, 0x2f,
                                                                  0x53, 0x43, 0x52, 0x45,
                                                                  0x45, 0x4e, 0x32, 0x53,
                                                                  0x57, 0x46};

static const char FAKE_ASPACK_1[] SECTION_ATTRIBUTE(".aspack") = {0};
static const char FAKE_ASPACK_2[] SECTION_ATTRIBUTE(".adata") = {0};
static const char FAKE_WIBUCODEMETER_1[] SECTION_ATTRIBUTE("__wibu00") = {0};
static const char FAKE_WIBUCODEMETER_2[] SECTION_ATTRIBUTE("__wibu01") = {0};
static const char *FAKE_DONGLE[] = {"skeydrv.dll", "HASPDOSDRV",
                                    "MARXDEV1.SYS", "MxLPT_Sem",
                                    "nethasp.ini", "sense4.dll",
                                    "SNTNLUSB", "RNBOspro",
                                    "SSIVDDP.DLL", "WIBUKEY",
                                    "\\\\.\\WIZZKEYRL",
                                    "\\\\.\\NVKEY"};

#else

#define OBFH_SECTION_ATTRIBUTE SECTION_ATTRIBUTE(".obfh")  // OBFH section

#endif

// Thanks to @horsicq && @ac3ss0r
#define RND(min, max) \
    (min + (((__COUNTER__ + (__LINE__ * __LINE__)) * 2654435761u) % (max - min + 1)))

#define STACK_STRING(str) ((char[]){str})

// FIX #2: cast the (dead) false branch to char * so both ternary branches
// share a compatible type under -Wint-conversion (now an error in GCC 14+).
// FIX #9: avoid string-literal concatenation; pass str directly as char *.
#define HIDE_STRING(str) \
    (_0 < RND(1, 255) ? obfh_process_hidden_string((char *)(uintptr_t)(str), (float)__s_rdtsc(RND(0, 255)) != 0.1) : (char *)(uintptr_t)((float)__s_rdtsc(RND(0, 255)) == RND(0, 255)))

#define RCAST(T, v) ((T)(uintptr_t)(v))

#define CALL_EXPORT(lib, mtd, def, ...) RCAST(def, GetProcAddress(GetModuleHandleA(HIDE_STRING(lib)), HIDE_STRING(mtd)))(__VA_ARGS__)

typedef enum {
    SALT_SHIFT = RND(0xBAD, 0xBEEF)
} VAR_ADDR_SHIFT;

// FIX #1: derive the pointer type from `value` with __typeof__ so the macro
// works for int/double/float proxies instead of hard-coding `int *`.
#define RET_BY_VAR(value)                                                \
    {                                                                    \
        int _obfh_ret_val_shift = SALT_SHIFT;                            \
        __typeof__(value) *_obfh_ret_val_addr = (&(value) + SALT_SHIFT); \
        return *(_obfh_ret_val_addr - _obfh_ret_val_shift);              \
    }

/* FIX: letters moved from the OBFH *code* section to a data-only section
   (OBFH_VAR_SECTION_ATTRIBUTE) to avoid the code/data section type conflict. */
volatile static char _s_a[] OBFH_VAR_SECTION_ATTRIBUTE = "a", _s_b[] OBFH_VAR_SECTION_ATTRIBUTE = "b", _s_c[] OBFH_VAR_SECTION_ATTRIBUTE = "c", _s_d[] OBFH_VAR_SECTION_ATTRIBUTE = "d",
                            _s_e[] OBFH_VAR_SECTION_ATTRIBUTE = "e", _s_f[] OBFH_VAR_SECTION_ATTRIBUTE = "f", _s_g[] OBFH_VAR_SECTION_ATTRIBUTE = "g", _s_h[] OBFH_VAR_SECTION_ATTRIBUTE = "h",
                            _s_i[] OBFH_VAR_SECTION_ATTRIBUTE = "i", _s_j[] OBFH_VAR_SECTION_ATTRIBUTE = "j", _s_k[] OBFH_VAR_SECTION_ATTRIBUTE = "k", _s_l[] OBFH_VAR_SECTION_ATTRIBUTE = "l",
                            _s_m[] OBFH_VAR_SECTION_ATTRIBUTE = "m", _s_n[] OBFH_VAR_SECTION_ATTRIBUTE = "n", _s_o[] OBFH_VAR_SECTION_ATTRIBUTE = "o", _s_p[] OBFH_VAR_SECTION_ATTRIBUTE = "p",
                            _s_q[] OBFH_VAR_SECTION_ATTRIBUTE = "q", _s_r[] OBFH_VAR_SECTION_ATTRIBUTE = "r", _s_s[] OBFH_VAR_SECTION_ATTRIBUTE = "s", _s_t[] OBFH_VAR_SECTION_ATTRIBUTE = "t",
                            _s_u[] OBFH_VAR_SECTION_ATTRIBUTE = "u", _s_v[] OBFH_VAR_SECTION_ATTRIBUTE = "v", _s_w[] OBFH_VAR_SECTION_ATTRIBUTE = "w", _s_x[] OBFH_VAR_SECTION_ATTRIBUTE = "x",
                            _s_y[] OBFH_VAR_SECTION_ATTRIBUTE = "y", _s_z[] = "z",
                            _a OBFH_VAR_SECTION_ATTRIBUTE = 'a', _b OBFH_VAR_SECTION_ATTRIBUTE = 'b', _c OBFH_VAR_SECTION_ATTRIBUTE = 'c', _d OBFH_VAR_SECTION_ATTRIBUTE = 'd',
                            _e OBFH_VAR_SECTION_ATTRIBUTE = 'e', _f OBFH_VAR_SECTION_ATTRIBUTE = 'f', _g OBFH_VAR_SECTION_ATTRIBUTE = 'g', _h OBFH_VAR_SECTION_ATTRIBUTE = 'h',
                            _i OBFH_VAR_SECTION_ATTRIBUTE = 'i', _j OBFH_VAR_SECTION_ATTRIBUTE = 'j', _k OBFH_VAR_SECTION_ATTRIBUTE = 'k', _l OBFH_VAR_SECTION_ATTRIBUTE = 'l',
                            _m OBFH_VAR_SECTION_ATTRIBUTE = 'm', _n OBFH_VAR_SECTION_ATTRIBUTE = 'n', _o OBFH_VAR_SECTION_ATTRIBUTE = 'o', _p OBFH_VAR_SECTION_ATTRIBUTE = 'p',
                            _q OBFH_VAR_SECTION_ATTRIBUTE = 'q', _r OBFH_VAR_SECTION_ATTRIBUTE = 'r', _s OBFH_VAR_SECTION_ATTRIBUTE = 's', _t OBFH_VAR_SECTION_ATTRIBUTE = 't',
                            _u OBFH_VAR_SECTION_ATTRIBUTE = 'u', _v OBFH_VAR_SECTION_ATTRIBUTE = 'v', _w OBFH_VAR_SECTION_ATTRIBUTE = 'w', _x OBFH_VAR_SECTION_ATTRIBUTE = 'x',
                            _y OBFH_VAR_SECTION_ATTRIBUTE = 'y', _z OBFH_VAR_SECTION_ATTRIBUTE = 'z',
                            _S OBFH_VAR_SECTION_ATTRIBUTE = 'S', _L OBFH_VAR_SECTION_ATTRIBUTE = 'L', _A OBFH_VAR_SECTION_ATTRIBUTE = 'A', _I OBFH_VAR_SECTION_ATTRIBUTE = 'I',
                            _D OBFH_VAR_SECTION_ATTRIBUTE = 'D', _P OBFH_VAR_SECTION_ATTRIBUTE = 'P',
                            _0 DATA_SECTION_ATTRIBUTE = 0, _1 DATA_SECTION_ATTRIBUTE = 1, _2 DATA_SECTION_ATTRIBUTE = 2, _3 DATA_SECTION_ATTRIBUTE = 3, _4 DATA_SECTION_ATTRIBUTE = 4,
                            _5 DATA_SECTION_ATTRIBUTE = 5, _6 DATA_SECTION_ATTRIBUTE = 6, _7 DATA_SECTION_ATTRIBUTE = 7, _8 DATA_SECTION_ATTRIBUTE = 8, _9 DATA_SECTION_ATTRIBUTE = 9;

/* No syntax-switching wrapper: dialect-neutral blocks pass straight through,
   and blocks with register mnemonics use `{att | intel}` alternatives + a
   `:::` clobber list so GCC expands them for whichever -masm mode is active. */
#define __obfh_asm__(...) __asm__ __volatile__(__VA_ARGS__)

#define BREAK_STACK_1                      \
    __obfh_asm__(                           \
        "{xorl %%eax,%%eax|xor eax, eax};" \
        "jz 1f;"                            \
        ".byte 0xE8;"                       \
        "1:"                                \
        "cpuid;" ::: "eax", "ebx", "ecx", "edx", "cc")

#define BREAK_STACK_2 \
    if (_0) __obfh_asm__(".byte 0x00;")

#define BREAK_STACK_3                          \
    switch (_0) {                              \
        case RND(1, 1000):                     \
            __obfh_asm__(".byte 0x00, 0x00;"); \
    }

#define BREAK_STACK_4                      \
    __obfh_asm__(                           \
        "{xorl %%ebx,%%ebx|xor ebx, ebx};" \
        "{xorl %%edx,%%edx|xor edx, edx};" \
        "{xorl %%ebx,%%edx|xor edx, ebx};" \
        "jz 1f;"                            \
        "{movl $4,%%eax|mov eax, 4};"      \
        ".byte 0x00;"                       \
        "1:"                                \
        "cpuid;" ::: "eax", "ebx", "ecx", "edx", "cc")

#define BREAK_STACK_5                      \
    __obfh_asm__(                           \
        "{xorl %%ebx,%%ebx|xor ebx, ebx};" \
        "{xorl %%eax,%%eax|xor eax, eax};" \
        "{movl %%eax,%%ebx|mov ebx, eax};" \
        "{movl %%edx,%%ebx|mov ebx, edx};" \
        "{xorl %%eax,%%edx|xor edx, eax};" \
        "jz 1f;"                            \
        ".byte 0x20;"                       \
        "1:"                                \
        "cpuid;" ::: "eax", "ebx", "ecx", "edx", "cc")

#define BREAK_STACK_6                      \
    __obfh_asm__(                           \
        "{xorl %%edx,%%edx|xor edx, edx};" \
        "{xorl %%eax,%%eax|xor eax, eax};" \
        "{movl %%eax,%%edx|mov edx, eax};" \
        "jz 1f;"                            \
        ".byte 0xE8;"                       \
        "1:"                                \
        "cpuid;" ::: "eax", "ebx", "ecx", "edx", "cc")

#define BREAK_STACK_7                      \
    __obfh_asm__(                           \
        "{xorl %%edx,%%edx|xor edx, edx};" \
        "jz 1f;"                            \
        ".byte 0xE8;"                       \
        "1:"                                \
        "cpuid;" ::: "eax", "ebx", "ecx", "edx", "cc")

#define BREAK_STACK_8                      \
    __obfh_asm__(                           \
        "{xorl %%eax,%%eax|xor eax, eax};" \
        "jz 1f;"                            \
        ".byte 0x50;"                       \
        "1:"                                \
        "cpuid;" ::: "eax", "ebx", "ecx", "edx", "cc")

#define BREAK_STACK_9                      \
    __obfh_asm__(                           \
        "{xorl %%edx,%%edx|xor edx, edx};" \
        "jz 1f;"                            \
        ".byte 0x00, 0x00;"                 \
        "1:"                                \
        "cpuid;" ::: "eax", "ebx", "ecx", "edx", "cc")

#if defined(__x86_64__)
#define BAD_JMP __obfh_asm__("cpuid; {movl %%eax,%%eax|mov eax, eax}; {movl %%ebx,%%edx|mov edx, ebx}; .byte 0xFF, 0x25, 0xF1, 0xF2, 0xF3, 0xF4;" ::: "eax", "ebx", "ecx", "edx", "cc")
#else
#define BAD_JMP __obfh_asm__(".byte 0xEB, 0xE1;")
#endif

#define BAD_CALL __obfh_asm__(".byte 0xB8;")

/* FIX: attribute moved before return type (GCC 16 requires pre-declarator placement) */
OBFH_SECTION_ATTRIBUTE void obfh_junk_func_args(int z, ...) {
    BREAK_STACK_1;
    __obfh_asm__("nop;");
    return;
}

/* FIX: this is a function, so it must go in the OBFH *code* section, not
   .data (which holds the decoy variables) - otherwise section type conflict. */
OBFH_SECTION_ATTRIBUTE void obfh_junk_func() {
    BREAK_STACK_5;
    __obfh_asm__("nop;");
    return;
}

#define __CRASH       \
    __obfh_asm__(                \
        ".byte 0xED;"            \
        "{int $3|int 3};" :::);  \
    exit(1);

#define TRUE ((((_9 + _7 + (RND(0, 1000) * _0))) / _8) - _1)
#define FALSE (((_3 + _6 + (RND(0, 1000) * _0)) - _9) * RND(0, 255))

#define FAKE_CPUID __obfh_asm__( \
    "nop;"                       \
    "cpuid;"                     \
    "nop;")

#define NOP_FLOOD                                  \
    (RND(0, 1000)) + obfh_int_proxy(RND(0, 1000)); \
    if (obfh_junk_func_args) {                     \
        __obfh_asm__("nop;");                      \
    }                                              \
    do {                                           \
        __obfh_asm__(                              \
            "nop;"                                 \
            "nop;");                               \
    } while (RND(0, 200) * _0)

/* FIX: parameter type size_t (was int*), return type void* (was int) */
void *malloc_proxy(size_t size) {
    BREAK_STACK_1;
    return malloc(size);
}
#define malloc(...) malloc_proxy(__VA_ARGS__)

static float rndValueToProxy = RND(0, 10);

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE int obfh_int_proxy(int value) {
    RET_BY_VAR(value);
}

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE double obfh_double_proxy(double value) {
    RET_BY_VAR(value);
}

float obfh_condition_true();

/* FIX: attribute moved before return type */
// ... only for junk data
OBFH_SECTION_ATTRIBUTE char *obfh_process_hidden_string(char *string, ...) {
    BREAK_STACK_1;

    if (!obfh_condition_true() || _0) {
        BAD_JMP;
    }

    // FIX #11: return the string directly. The local-copy approach always
    // returned a dangling stack pointer (string_to_return lifetime ends on
    // return). FIX #9 ensures `string` is already a valid pointer (a string
    // literal in .rdata or a caller-owned buffer), so no copy is needed.
    return string;
}

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE float obfh_condition_true() {
    BREAK_STACK_1;
    return _1 && TRUE;
}

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE int obfh_condition_proxy(float junk, float condition, ...) {
    RET_BY_VAR(condition);
}

/* FIX: 'unsigned long double' is not a valid type; changed to 'unsigned long long'.
        Attribute moved before return type. */
OBFH_SECTION_ATTRIBUTE unsigned long long __s_rdtsc(float junk, ...) {
    {
        unsigned int lo, hi;
        /* byte-only read: `.byte` is syntax-independent, so this is a raw
           __asm__ with no Intel/AT&T wrapper (the operand list can't follow
           a trailing template string). */
        __asm__ __volatile__(".byte 0x0f, 0x31;"  // rdtsc
                             : "=a"(lo), "=d"(hi));

        unsigned long long rdtsc_result = ((unsigned long long)hi << 32) | lo;

        if (rdtsc_result == obfh_int_proxy(0)) __obfh_asm__(".byte 0xE8;");
    }

    unsigned long long time;
    unsigned int low, high;
    __asm__ __volatile__(".byte 0x0f, 0xc7, 0xf8;"  // rdtscp
                         : "=a"(low), "=d"(high));
    time = ((unsigned long long)high << 32) | low;
    return time;
}

// =============================================================
// Control Flow (global)
#if NO_CFLOW != 1

// if
#define if(cond)                                                                         \
    if ((float)__s_rdtsc(RND(0, 255)) == (float)((RND(1, 255) * -1)) * (float)1.0) {     \
        BAD_CALL;                                                                        \
    } else if (&__s_rdtsc && (cond ? ((float)__s_rdtsc(RND(0, 255), RND(0, 255)) != 0.1) \
                                   : (float)__s_rdtsc(RND(0, 255)) == 0.1))

// else
#define else      \
    else if (0) { \
        BAD_CALL; \
    }             \
    else

#if CFLOW_V2
#define OBFUS_CONDITION_BLOCK(...) (obfh_condition_proxy(RND(0, 255), (__VA_ARGS__) ? !!obfh_int_proxy(!!obfh_condition_true()) : !!!obfh_condition_true(), RND(0, 255)) ? !!obfh_condition_true() : obfh_int_proxy(!obfh_condition_true()))
#else
#define OBFUS_CONDITION_BLOCK(...) (obfh_condition_proxy(RND(0, 255), (__VA_ARGS__), RND(0, 255)) ? !!obfh_condition_true() : !obfh_condition_true())
#endif

// break
#define break                                                  \
    {                                                          \
        if (OBFUS_CONDITION_BLOCK(RND(1, 255))) BREAK_STACK_1; \
        break;                                                 \
    }

// switch
#define switch(...)                         \
    if (OBFUS_CONDITION_BLOCK(RND(1, 255))) \
        switch (__VA_ARGS__)

// while
#define while(...) while ((float)__s_rdtsc(RND(0, 255)) != 0.1 && (&__s_rdtsc != !&__s_rdtsc) && (__VA_ARGS__))

// for
#define for(...)                            \
    if (OBFUS_CONDITION_BLOCK(RND(1, 255))) \
        for (__VA_ARGS__)

#endif
// =============================================================

// =============================================================
// Virtualization (global)
#if VIRT == 1
typedef enum {
    OP__ADD = RND(0, 900) * __COUNTER__ * 5,
    OP__SUB = RND(1000, 1900) * __COUNTER__ * 5,
    OP__MUL = RND(2000, 2900) * __COUNTER__ * 5,
    OP__DIV = RND(3000, 3900) * __COUNTER__ * 5,
    OP__MOD = RND(4000, 4900) * __COUNTER__ * 5,
    OP__EQU = RND(5000, 5900) * __COUNTER__ * 5,
    OP__NEQ = RND(6000, 6900) * __COUNTER__ * 5,
    OP__GTR = RND(7000, 7900) * __COUNTER__ * 5,
    OP__LSS = RND(8000, 8900) * __COUNTER__ * 5,
    OP__LEQ = RND(9000, 9900) * __COUNTER__ * 5,
    OP__GEQ = RND(10000, 10900) * __COUNTER__ * 5,
    OP__NOP = RND(11000, 11900) * __COUNTER__ * 5
} CMD;

typedef enum {
    SALT_CMD = RND(100, 900),
    SALT_NUM1 = RND(16, 48),
    SALT_NUM2 = RND(16, 48)
} VM_SALT;

static int _salt = SALT_CMD;

#define _VM_DEMUTATOR_KEY (__COUNTER__) / 5
#define _VM_MUTATOR_KEY (__COUNTER__ - 1) / 5

#define _VM_ENCRYPT_INT(value) ((value - _VM_MUTATOR_KEY) * ~SALT_CMD)
#define _ENC_OP__ADD _VM_ENCRYPT_INT(OP__ADD)
#define _ENC_OP__SUB _VM_ENCRYPT_INT(OP__SUB)
#define _ENC_OP__MUL _VM_ENCRYPT_INT(OP__MUL)
#define _ENC_OP__DIV _VM_ENCRYPT_INT(OP__DIV)
#define _ENC_OP__MOD _VM_ENCRYPT_INT(OP__MOD)
#define _ENC_OP__EQU _VM_ENCRYPT_INT(OP__EQU)
#define _ENC_OP__NEQ _VM_ENCRYPT_INT(OP__NEQ)
#define _ENC_OP__GTR _VM_ENCRYPT_INT(OP__GTR)
#define _ENC_OP__LSS _VM_ENCRYPT_INT(OP__LSS)
#define _ENC_OP__LEQ _VM_ENCRYPT_INT(OP__LEQ)
#define _ENC_OP__GEQ _VM_ENCRYPT_INT(OP__GEQ)
#define _ENC_OP__NOP _VM_ENCRYPT_INT(OP__NOP)

#define VM_ADD(num1, num2) (long)Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__ADD, (num1) * -1 + SALT_NUM1, RND(1, 500), (num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_SUB(num1, num2) (long)Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__SUB, (num1) * -1 + SALT_NUM1, RND(1, 500), (num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_MUL(num1, num2) (long)Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__MUL, (num1) * -1 + SALT_NUM1, RND(1, 500), (num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_DIV(num1, num2) (long)Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__DIV, (num1) * -1 + SALT_NUM1, RND(1, 500), (num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_MOD(num1, num2) (long)Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__MOD, (num1) * -1 + SALT_NUM1, RND(1, 500), (num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_EQU(num1, num2) (long)Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__EQU, (num1) * -1 + SALT_NUM1, RND(1, 500), (num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_NEQ(num1, num2) (long)Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__NEQ, (num1) * -1 + SALT_NUM1, RND(1, 500), (num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_LSS(num1, num2) (long)Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__LSS, (num1) * -1 + SALT_NUM1, RND(1, 500), (num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_GTR(num1, num2) (long)Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__GTR, (num1) * -1 + SALT_NUM1, RND(1, 500), (num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_LEQ(num1, num2) (long)Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__LEQ, (num1) * -1 + SALT_NUM1, RND(1, 500), (num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_GEQ(num1, num2) (long)Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__GEQ, (num1) * -1 + SALT_NUM1, RND(1, 500), (num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_OBF_INT(num1) (VM_MUL(RND(1, 999), 0) ? RND(1, 9999) : (long)Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__NOP, (num1) * -1 + SALT_NUM1, RND(1, 500), RND(1, 99999999) * -1 + SALT_NUM2, RND(1, 500)))

#define VM_ADD_DBL(num1, num2) Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__ADD, (double)(num1) * -1 + SALT_NUM1, RND(1, 500), (double)(num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_SUB_DBL(num1, num2) Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__SUB, (double)(num1) * -1 + SALT_NUM1, RND(1, 500), (double)(num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_MUL_DBL(num1, num2) Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__MUL, (double)(num1) * -1 + SALT_NUM1, RND(1, 500), (double)(num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_DIV_DBL(num1, num2) Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__DIV, (double)(num1) * -1 + SALT_NUM1, RND(1, 500), (double)(num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_LSS_DBL(num1, num2) (long)Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__LSS, (double)(num1) * -1 + SALT_NUM1, RND(1, 500), (double)(num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_GTR_DBL(num1, num2) (long)Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__GTR, (double)(num1) * -1 + SALT_NUM1, RND(1, 500), (double)(num2) * -1 + SALT_NUM2, RND(1, 500))
#define VM_OBF_DBL(num1) (VM_MUL(RND(1, 999), 0) ? RND(1, 9999) : Obfh_VirtualMachine(_VM_DEMUTATOR_KEY, _ENC_OP__NOP, (num1) * -1 + SALT_NUM1, RND(1, 500), RND(1, 99999999) * -1 + SALT_NUM2, RND(1, 500)))

#define VM_IF(condition) if (!VM_EQU((int)(condition), VM_MUL(RND(111111, 999999), 0)))
#define VM_ELSE_IF(condition) else if (!VM_EQU((int)(condition), VM_MUL(0, RND(111111, 999999))))
#define VM_ELSE else if (VM_EQU(1, _1))

long double obfhVmResult = 0;
/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE long double Obfh_VirtualMachine(long double uni_key, int command, long double num1, long double junk_2, long double num2, long double junk_3) {
    BREAK_STACK_1;
    goto firstFakePoint;

    // Restore values
restoreCommand:
    BREAK_STACK_1;
    command /= ~_salt;
    command += uni_key;
    goto restoreNum2;

restoreNum1:
    BREAK_STACK_1;
    num1 -= SALT_NUM1;
    num1 *= (-1 * _1);
    goto letsExecute;

restoreNum2:
    BREAK_STACK_1;
    num2 -= SALT_NUM2;
    num2 *= (-1 * _1);
    goto restoreNum1;

firstFakePoint:
    BREAK_STACK_2;
    goto secondFakePoint;

letsExecute:

    switch (command) {
        case -1 * __LINE__:
            goto restoreCommand;
        case -2 * __LINE__:
            goto firstFakePoint;
        case -3 * __LINE__:
            return _0 * ~_1 + junk_2;
        case -4 * __LINE__:
            goto restoreNum2;
        case -5 * __LINE__:
            goto restoreNum1;
        case -6 * __LINE__:
            __obfh_asm__(".byte 0xFF, 0x25;");  // fake JMP
        case -7 * __LINE__:

#if defined(__x86_64__) || defined(_M_X64)  // fake code, just for decompiler break
            __asm__ __volatile__(
                "{movq %%rax,%%rbx|mov rbx, rax};"
                "{xorq %%rcx,%%rax|xor rax, rcx};"
                "{shrq $8,%%rdx|shr rdx, 8};"
                "{shlq $4,%%rax|shl rax, 4};"
                "{pushq %%rbx|push rbx};"
                "{popq %%rbx|pop rbx};"
                "{incq %%rax|inc rax};"
                "{decq %%rdx|dec rdx};"
                :
                :
                : "rax",
                  "rbx", "rcx", "rdx", "cc");
#elif defined(__i386__) || defined(_M_IX86)
            __asm__ __volatile__(
                "{movl %%ebx,%%eax|mov eax, ebx};"
                "{addl %%ecx,%%eax|add eax, ecx};"
                "{subl %%edx,%%ebx|sub ebx, edx};"
                "{shll %%cl,%%ecx|shl ecx, cl};"
                "{pushl %%ebx|push ebx};"
                "{popl %%ebx|pop ebx};"
                "{sarl %%cl,%%ecx|sar ecx, cl};"
                "{orl %%edx,%%eax|or eax, edx};"
                "{decl %%edx|dec edx};"
                :
                :
                : "eax",
                  "ebx", "ecx", "edx", "cc");
#else
#endif
        case -8 * __LINE__:
            BAD_JMP;

        case OP__ADD:  // plus
            obfhVmResult = (num1 + num2) + VM_MUL(junk_3, _0);
            goto afterCalc;
        case OP__SUB:  // minus
            obfhVmResult = (num1 - num2) + VM_MUL(junk_3, _0);
            goto afterCalc;
        case OP__MUL:  // multiply
            if (num1 == _0 || num2 == _0)
                obfhVmResult = _0;
            else
                return num1 * num2;

            goto afterCalc;
        case OP__DIV:  // divide
            if (num2 != _0)
                obfhVmResult = num1 / num2;
            else
                obfhVmResult = VM_ADD(_0, _0);
            goto afterCalc;
        case OP__MOD:  // modulo
            if (num2 != 0)
                obfhVmResult = (int)num1 % (int)num2;
            else
                obfhVmResult = _0;
            goto afterCalc;
        case OP__EQU:            // equal
            if (num1 == num2) {  // 1 + 0 = 1
                obfhVmResult = VM_ADD(_1, _0);
            } else {
                obfhVmResult = _0;
            }
            goto afterCalc;
        case OP__NEQ:            // not equal
            if (num1 != num2) {  // 1 + 0 = 1
                obfhVmResult = VM_ADD(_0, _1) + VM_MUL(junk_2, _0);
            } else {
                obfhVmResult = VM_MUL(junk_3, _0);
            }
            goto afterCalc;
        case OP__LSS:
            obfhVmResult = num1 != num2 && !(num1 + VM_MUL(junk_2, _0) > num2);
            goto afterCalc;
        case OP__GTR:
            obfhVmResult = num1 != num2 && !(num1 + VM_MUL(junk_2, _0) < num2);
            goto afterCalc;
        case OP__LEQ:
            obfhVmResult = !(num1 + VM_MUL(junk_2, _0) > num2);
            goto afterCalc;
        case OP__GEQ:
            obfhVmResult = (num1 + VM_MUL(junk_2, _0) > num2) || (num1 == num2);
            // result = num1 >= num2;
            goto afterCalc;
        case OP__NOP:
            obfhVmResult = num1;
            goto afterCalc;
        default:
            // printf("ADD: %d, CMD: %d\n", OP__ADD, command);
            obfhVmResult = _0 * (uni_key * _3);
            BAD_JMP;
    }
    BREAK_STACK_8;

    long double result = uni_key;
afterCalc:

    goto saveValueToLocal;
resetResult:
    obfhVmResult = 0;
    goto returnValue;
saveValueToLocal:
    result = obfhVmResult;
    goto resetResult;

returnValue:
    return result;

    __obfh_asm__(".byte 0xFF, 0xE0;");  // fake JMP EAX

secondFakePoint:
    BREAK_STACK_7;
    goto restoreCommand;
}
#endif
// =============================================================

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE char *getCharMask(int count) {
    BREAK_STACK_1;
    static char mask[16];
    if (count <= _0 || count >= sizeof(mask)) {
        BAD_JMP;
    }
    int i = (((_1 * _5) - _4) + _1) - _2;
    BREAK_STACK_1;
    char *ptr = mask;
    for (i = _0; i < count; i++) {
        *ptr++ = '%';
        *ptr++ = _c;
    }
    *ptr = '\0';

    BREAK_STACK_8;

    FAKE_CPUID;

    return mask;
}

// WriteConsoleA
/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE BOOL WriteConsoleA_proxy(HANDLE hConsoleOutput, const void *lpBuffer, DWORD nNumberOfCharsToWrite, LPDWORD lpNumberOfCharsWritten, LPVOID lpReserved) {
    BREAK_STACK_1;
    FAKE_CPUID;
    return WriteConsoleA(hConsoleOutput, lpBuffer, nNumberOfCharsToWrite, lpNumberOfCharsWritten, lpReserved);
}
#define WriteConsoleA(...) WriteConsoleA_proxy(__VA_ARGS__)

// GetStdHandle
/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE HANDLE GetStdHandle_proxy(DWORD nStdHandle) {
    BREAK_STACK_1;
    FAKE_CPUID;
    return GetStdHandle(obfh_int_proxy(nStdHandle));
}
#define GetStdHandle(...) GetStdHandle_proxy(__VA_ARGS__)

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE HMODULE GetModuleHandleA_proxy(LPCSTR lpModuleName) {
    BREAK_STACK_9;
    FAKE_CPUID;
    return GetModuleHandleA(lpModuleName);
}
#define GetModuleHandleA(...) GetModuleHandleA_proxy(__VA_ARGS__)

// strcmp
/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE int strcmp_custom(const char *str1, const char *str2) {
    BREAK_STACK_1;
    while (*str1 != '\0' || *str2 != '\0') {
        NOP_FLOOD;
        if ((obfh_int_proxy(*str1) < obfh_int_proxy(*str2)) && obfh_int_proxy(_1)) {
            return (obfh_int_proxy(_2) / _2) * -1;  // -1
        } else if (obfh_int_proxy(*str1) > obfh_int_proxy(*str2)) {
            return obfh_int_proxy(_0 + _1);  // 1
        }
        str1 += obfh_int_proxy(_1);
        str2 += obfh_int_proxy(_2 - _1);
    }
    FAKE_CPUID;
    return _0;
}
#define strcmp(...) strcmp_custom(__VA_ARGS__)

// strlen
/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE size_t strlen_custom(const char *str) {
    BREAK_STACK_1;
    size_t length = _0;
    while (*str != '\0') {
        length += obfh_int_proxy(_1);
        str += obfh_int_proxy(_2 - _1);
    }
    FAKE_CPUID;
    return obfh_int_proxy(length + (RND(0, 1000) * _0));
}
#define strlen(...) strlen_custom(__VA_ARGS__)

// GetProcAddress
/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE FARPROC GetProcAddress_custom(HMODULE hModule, LPCSTR lpProcName) {
    BREAK_STACK_2;
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((BYTE *)hModule + dosHeader->e_lfanew);
    BREAK_STACK_1;
    PIMAGE_EXPORT_DIRECTORY exportDirectory = (PIMAGE_EXPORT_DIRECTORY)((BYTE *)hModule +
                                                                        ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    obfh_junk_func_args(RND(0, 885));
    BREAK_STACK_1;
    DWORD *addressOfFunctions = (DWORD *)((BYTE *)hModule + exportDirectory->AddressOfFunctions);
    WORD *addressOfNameOrdinals = (WORD *)((BYTE *)hModule + exportDirectory->AddressOfNameOrdinals);
    BREAK_STACK_1;
    DWORD *addressOfNames = (DWORD *)((BYTE *)hModule + exportDirectory->AddressOfNames);

    for (DWORD i = 0; i < exportDirectory->NumberOfNames; ++i) {
        if (strcmp(lpProcName, (const char *)hModule + addressOfNames[i]) == 0) {
            BREAK_STACK_2;
            return (FARPROC)((BYTE *)hModule + addressOfFunctions[addressOfNameOrdinals[i]]);
        }
    }
    BREAK_STACK_1;
    return NULL;
}
#define GetProcAddress(...) GetProcAddress_custom(__VA_ARGS__)

static char loadStr[5];
static HMODULE hKernel32 = NULL;

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE HMODULE LoadLibraryA_0(LPCSTR lpLibFileName) {
    switch (_0) {
        case 1:
            __obfh_asm__(".byte 0x74;");  // fake JE

            break;
        case 0:
            BREAK_STACK_3;
            // return LoadLibraryA(lpLibFileName);

            typedef HMODULE(WINAPI * LoadLibraryAFunc)(LPCSTR);
            static LoadLibraryAFunc loadLibraryA = NULL;
            if (loadLibraryA == NULL) {
                char libName[32];

                // kernel32
                sprintf(libName, strcat(getCharMask(_6), "%d"), _k, _e, _r, _n, _e, _l, (_4 * _8));

                hKernel32 = GetModuleHandleA(libName);

                if (hKernel32 != NULL) {
                    FAKE_CPUID;
                    char _L_char = _L;
                    obfh_junk_func_args(_0 + RND(1, 5));

                    if (loadStr[_3] != obfh_int_proxy(_d)) {  // restore "Load"
                        loadStr[_4] = obfh_int_proxy(_0);
                        loadStr[_3] = obfh_int_proxy(_d);
                        loadStr[_2] = obfh_int_proxy(_a);
                        BREAK_STACK_2;
                        loadStr[_1] = obfh_int_proxy(_o);
                        loadStr[_0] = obfh_int_proxy(_L);
                    }

                    char *funcName = malloc(32);

                    FAKE_CPUID;

                    // FIX #12: "Library" is in .rdata (read-only); strcat into it
                    // is UB and faults on any modern PE loader. Use a mutable
                    // local array so strcat has a writable destination.
                    char _libFmt[] = {'L','i','b','r','a','r','y','\0'};
                    sprintf(funcName, strcat(_libFmt, "%c"), _A);  // _A = LoadLibrary{'A'}
                    loadLibraryA = (LoadLibraryAFunc)GetProcAddress(hKernel32, strcat(loadStr, funcName));
                    free(funcName);

#if MEM_CLEANER__JUST_FOR_FUN
                    int *value = _1 * -1;
                    SetProcessWorkingSetSize(GetCurrentProcess(), value, value);
#endif
                }
            }
            if (loadLibraryA != NULL) {
                BREAK_STACK_1;
                return loadLibraryA(lpLibFileName);
            }
            return NULL;
    }
}

/* FIX #3: return type HMODULE (was char *) so it matches LoadLibraryA_0 */
OBFH_SECTION_ATTRIBUTE HMODULE LoadLibraryA_1(LPCSTR lpLibFileName) {
    BREAK_STACK_6;
    return LoadLibraryA_0((LPCSTR)lpLibFileName);
}

/* FIX #3: return type HMODULE (was char *) */
HMODULE LoadLibraryA_2(LPCSTR lpLibFileName) {
    BREAK_STACK_5;
    return LoadLibraryA_1((LPCSTR)lpLibFileName);
}

/* FIX #3: return type HMODULE (was char *) */
OBFH_SECTION_ATTRIBUTE HMODULE LoadLibraryA_3(LPCSTR lpLibFileName) {
    BREAK_STACK_4;
    return LoadLibraryA_2((LPCSTR)lpLibFileName);
}

/* FIX #3: return type HMODULE (was char *) */
HMODULE LoadLibraryA_4(LPCSTR lpLibFileName) {
    BREAK_STACK_3;
    return LoadLibraryA_3((LPCSTR)lpLibFileName);
}

/* FIX #3: return type HMODULE (was char *) */
OBFH_SECTION_ATTRIBUTE HMODULE LoadLibraryA_5(LPCSTR lpLibFileName) {
    BREAK_STACK_2;
    return LoadLibraryA_4((LPCSTR)lpLibFileName);
}

/* FIX #3: return type HMODULE (was char *) */
HMODULE LoadLibraryA_proxy(LPCSTR lpLibFileName) {
    BREAK_STACK_1;
    return LoadLibraryA_5((LPCSTR)lpLibFileName);
}
#define LoadLibraryA(...) LoadLibraryA_proxy(__VA_ARGS__)

// =============================================================
// Anti-Debug (global)
#if NO_ANTIDEBUG != 1

#if ANTIDEBUG_V2 == 1  // for ANTIDEBUG_V2
void ad_ZeroDRs(PCONTEXT pCtx) {
    BREAK_STACK_1;
    pCtx->Dr0 = _0;
    pCtx->Dr1 = _0;
    pCtx->Dr2 = _0;
    pCtx->Dr3 = _0;
    pCtx->Dr6 = _0;
    pCtx->Dr7 = _0;
}

int ad_CompareDRs(PCONTEXT pCtx) {
    BREAK_STACK_1;
    if (pCtx->Dr7 != _0) {
        ad_ZeroDRs(pCtx);
        return _1;
    } else {
        // ensure DR0 - DR3 contain zeros even if they are disabled.
        // Skip DR6.  It seems to change erratically, but it's output-only.
        if (_0 == (pCtx->Dr0 | pCtx->Dr1 | pCtx->Dr2 | pCtx->Dr3)) {
            ad_ZeroDRs(pCtx);
        }
        // zero any active debug registers to erase breakpoints.
        // the caller is responsible for ensuring the DR values set are
        // actually applied.
        ad_ZeroDRs(pCtx);
    }
    return _0;
}

// FIX #7: added DWORD return type (WINAPI alone defaults to int, incompatible
// with LPTHREAD_START_ROUTINE which is DWORD (WINAPI *)(void *)).
DWORD WINAPI ThreadCompareDRs(void *p) {
    BREAK_STACK_1;
    DWORD dwRet = _0;
    HANDLE hMainThread = (HANDLE)p;
    if (-1 != SuspendThread(hMainThread)) {
        BREAK_STACK_2;
        CONTEXT context;
        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        if (GetThreadContext(hMainThread, &context)) {
            if (ad_CompareDRs(&context))
                dwRet = _1;
        }
        ResumeThread(hMainThread);
    }
    CloseHandle(hMainThread);
    return dwRet;
}
#endif

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE int IsDebuggerPresent_proxy() {
    BREAK_STACK_1;
    NOP_FLOOD;
    BREAK_STACK_2;
#if ANTIDEBUG_V2 == 1

    // Registers validation
    HANDLE hMainThread;
    DWORD dwDummy, exitCode;

    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(),
                    &hMainThread, _0, FALSE, DUPLICATE_SAME_ACCESS);

    HANDLE hThread = CreateThread(NULL, _0, ThreadCompareDRs, hMainThread, _0, &dwDummy);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, &exitCode);
        CloseHandle(hThread);
    }

    if (exitCode) return exitCode;

    // Dynamic antidebugger
    char result[32];
    sprintf(result, strcat(getCharMask(_6), "%d"), _k, _e, _r, _n, _e, _l, (_6 * _6 - _4));

    char funcName[18];
    funcName[_9 + _8] = _0;

    funcName[_9 + _7 * _1] = _t;
    funcName[_2 + _5 * _1] = _g;
    funcName[_0 * _8 * _1] = _I;
    funcName[_1 + _0 * _1] = _s;
    funcName[_7 * _2 * _1] = _e;
    funcName[_3 * _3 * _1] = _r;
    funcName[_9 + _4 * _1] = _s;
    funcName[_5 * _3 * _1] = _n;
    BREAK_STACK_3;
    funcName[_1 + _1 * _1] = _D;
    funcName[_1 + _2 * _1] = _e;
    funcName[_5 * _2 * _1] = _P;
    funcName[_2 + _2 * _1] = _b;
    funcName[_3 + _2 * _1] = _u;
    funcName[_4 * _2 * _1] = _e;
    funcName[_2 + _9 * _1] = _r;
    funcName[_3 * _2 * _1] = _g;
    funcName[_6 * _2 * _1] = _e;

    return ((BOOL(*)())GetProcAddress(LoadLibraryA(result), funcName))();
#else

    // Standard antidebugger
    NOP_FLOOD;
    return IsDebuggerPresent();

#endif
}
// =============================================================

/*
void antiDebugMessage() {
typedef int(WINAPI * MessageBoxAType)(HWND, LPCSTR, LPCSTR, UINT);
MessageBoxAType MessageBoxA = (MessageBoxAType)GetProcAddress(LoadLibraryA("user32.dll"), "MessageBoxA");
if (MessageBoxA != NULL) MessageBoxA(NULL, "Debugging prevented.", "", 0x10);
}
*/

void crash() {
    BREAK_STACK_1;
    __obfh_asm__(
        "{int $3|int 3};"
        ".byte 0xED, 0x00;" :::);
}

void loop() {
    while (1) {
    }
}

#define ANTI_DEBUG                                                                                      \
    if (IsDebuggerPresent() || obfh_int_proxy(_0 / !IsDebuggerPresent_proxy() * (_1 + _0 + _1) / _2)) { \
        obfh_double_proxy(RND(1, 999));                                                                 \
        /* antiDebugMessage(); */                                                                       \
        loop();                                                                                         \
        while (1) {                                                                                     \
        };                                                                                              \
        __obfh_asm__(".byte 0xED;");                                                                    \
        BREAK_STACK_1;                                                                                  \
        __obfh_asm__(".byte 0x66, 0xC1, 0xE8, 0x05;");                                                  \
        __obfh_asm__(".byte 0x00;");                                                                    \
        __obfh_asm__("ret;");                                                                           \
        crash();                                                                                        \
    } else {                                                                                            \
        0.0 / !IsDebuggerPresent();                                                                     \
    };

#else
#define ANTI_DEBUG 0
#endif

char *getStdLibName_proxy() {
    BREAK_STACK_7;
    return HIDE_STRING("msvcrt");
}

// printf
void printf_custom(int junk, const char *format, ...) {
    BREAK_STACK_1;
    char buffer[1024];
    va_list args;
    NOP_FLOOD;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // FIX #5: don't route a HANDLE (pointer) through obfh_int_proxy(int) -
    // that both breaks -Wint-conversion and truncates the handle to 32 bits
    // on x64. Keep the inner proxy on the DWORD constant only.
    HANDLE hConsole = GetStdHandle(obfh_int_proxy(STD_OUTPUT_HANDLE));
    obfh_junk_func_args(RND(0, 1000) * (int)(uintptr_t)hConsole + junk);
    WriteConsoleA(hConsole, buffer, strlen(buffer), NULL, NULL);
}

// printf as void
#define printf(...)                                   \
    do {                                              \
        BREAK_STACK_1;                                \
        obfh_junk_func_args((RND(0, 1000) * 3) < _0); \
        printf_custom(RND(0, 1000), __VA_ARGS__);     \
    } while (_0 > (RND(0, 100000000000) * _2) + 82)

// scanf
char *getScanfName_proxy() {
    BREAK_STACK_1;
    FAKE_CPUID;
    return "scanf";
}
#define scanf(...) ((int (*)(const char *, ...))GetProcAddress(LoadLibraryA_proxy(getStdLibName_proxy()), getScanfName_proxy()))(__VA_ARGS__)

// sprintf
char *getSprintfName_proxy() {
    BREAK_STACK_1;
    FAKE_CPUID;
    return "sprintf";
}
#define sprintf(...) ((int (*)(char *, const char *, ...))GetProcAddress(LoadLibraryA_proxy(getStdLibName_proxy()), getSprintfName_proxy()))(__VA_ARGS__)

// fclose
char *getFcloseName_proxy() {
    BREAK_STACK_1;
    FAKE_CPUID;
    return "fclose";
}
#define fclose(...) ((int (*)(FILE *))GetProcAddress(LoadLibraryA_proxy(getStdLibName_proxy()), getFcloseName_proxy()))(__VA_ARGS__)

// fopen
char *getFopenName_proxy() {
    BREAK_STACK_1;
    FAKE_CPUID;
    return "fopen";
}
#define fopen(...) ((FILE * (*)(const char *, const char *)) GetProcAddress(LoadLibraryA_proxy(getStdLibName_proxy()), getFopenName_proxy()))(__VA_ARGS__)

// fread
char *getFreadName_proxy() {
    BREAK_STACK_1;
    FAKE_CPUID;
    return "fread";
}
#define fread(...) ((size_t(*)(void *, size_t, size_t, FILE *))GetProcAddress(LoadLibraryA_proxy(getStdLibName_proxy()), getFreadName_proxy()))(__VA_ARGS__)

// fwrite
char *getFwriteName_proxy() {
    BREAK_STACK_1;
    FAKE_CPUID;
    return "fwrite";
}
#define fwrite(...) ((size_t(*)(const void *, size_t, size_t, FILE *))GetProcAddress(LoadLibraryA_proxy(getStdLibName_proxy()), getFwriteName_proxy()))(__VA_ARGS__)

// exit
char *getExitName_proxy() {
    BREAK_STACK_1;
    FAKE_CPUID;
    return "exit";
}
#define exit(...) ((void (*)(int))GetProcAddress(LoadLibraryA_proxy(getStdLibName_proxy()), getExitName_proxy()))(__VA_ARGS__)

// strcpy
char *getStrcpyName_proxy() {
    BREAK_STACK_1;
    FAKE_CPUID;
    return "strcpy";
}
#define strcpy(...) ((char *(*)(char *, const char *))GetProcAddress(LoadLibraryA_proxy(getStdLibName_proxy()), getStrcpyName_proxy()))(__VA_ARGS__)

// strtok
char *getStrtokName_proxy() {
    BREAK_STACK_1;
    FAKE_CPUID;
    return "strtok";
}
#define strtok(...) ((char *(*)(char *, const char *))GetProcAddress(LoadLibraryA_proxy(getStdLibName_proxy()), getStrtokName_proxy()))(__VA_ARGS__)

// memset
void *memset_proxy(void *ptr, int value, size_t num) {
    BREAK_STACK_1;
    return memset(ptr, value * _1, num);
}
#define memset(...) memset_proxy(__VA_ARGS__)

// memcpy
char *getMemcpyName_proxy() {
    BREAK_STACK_1;
    FAKE_CPUID;
    return "memcpy";
}
#define memcpy(...) ((void *(*)(void *, const void *, size_t))GetProcAddress(LoadLibraryA_proxy(getStdLibName_proxy()), getMemcpyName_proxy()))(__VA_ARGS__)

// strchr
char *getStrchrName_proxy() {
    BREAK_STACK_1;
    FAKE_CPUID;
    return "strchr";
}
#define strchr(...) ((char *(*)(const char *, int))GetProcAddress(LoadLibraryA_proxy(getStdLibName_proxy()), getStrchrName_proxy()))(__VA_ARGS__)

// strrchr
char *getStrrchrName_proxy() {
    BREAK_STACK_1;
    FAKE_CPUID;
    return "strrchr";
}
#define strrchr(...) ((char *(*)(const char *, int))GetProcAddress(LoadLibraryA_proxy(getStdLibName_proxy()), getStrrchrName_proxy()))(__VA_ARGS__)

// rand
char *getRandName_proxy() {
    BREAK_STACK_1;
    FAKE_CPUID;
    return "rand";
}
#define rand(...) ((int (*)(void))GetProcAddress(LoadLibraryA_proxy(getStdLibName_proxy()), getRandName_proxy()))(__VA_ARGS__)

// realloc
/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE char *getReallocName_proxy() {
    BREAK_STACK_1;
    FAKE_CPUID;
    return "realloc";
}
/* FIX #4: give the resolved pointer a real prototype so the call with two
   args is valid under C23 (where () means (void)). */
#define realloc(...) ((void *(*)(void *, size_t))GetProcAddress(LoadLibraryA_proxy(getStdLibName_proxy()), getReallocName_proxy()))(__VA_ARGS__)

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE void *calloc_proxy(size_t nmemb, size_t size) {
    BREAK_STACK_1;
    return calloc(nmemb, size);
}
#define calloc(nmemb, size) calloc_proxy(nmemb, size)

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE void *realloc_proxy(void *ptr, size_t size) {
    BREAK_STACK_1;
    return realloc(ptr, size);
}
#define realloc(ptr, size) realloc_proxy(ptr, size)

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE char *gets_proxy(char *s) {
    BREAK_STACK_1;
    return gets(s);
}
#define gets(s) gets_proxy(s)

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE int snprintf_proxy(char *str, size_t size, const char *format, ...) {
    BREAK_STACK_1;
    va_list args;
    va_start(args, format);
    int result = vsnprintf(str, size, format, args);
    va_end(args);
    return result;
}
#define snprintf(str, size, format, ...) snprintf_proxy(str, size, format, __VA_ARGS__)

/*
#define printf(...) (([](...) -> int {                                                                        \
static void (*printf_proxy)(const char *, ...) = NULL;                                                    \
if (printf_proxy == NULL) {                                                                               \
printf_proxy = (void (*)(const char *, ...))GetProcAddress(GetModuleHandleA("msvcrt.dll"), "printf"); \
}                                                                                                         \
return printf_proxy(__VA_ARGS__);                                                                         \
})(__VA_ARGS__))
*/

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE int vsprintf_proxy(char *str, const char *format, va_list args) {
    BREAK_STACK_1;
    return vsprintf(str, format, args);
}
#define vsprintf(str, format, args) vsprintf_proxy(str, format, args)

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE int vsnprintf_proxy(char *str, size_t size, const char *format, va_list args) {
    BREAK_STACK_1;
    return vsnprintf(str, size, format, args);
}
#define vsnprintf(str, size, format, args) vsnprintf_proxy(str, size, format, args)

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE char *getenv_proxy(const char *name) {
    BREAK_STACK_1;
    return getenv(name);
}
#define getenv(name) getenv_proxy(name)

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE int system_proxy(const char *command) {
    BREAK_STACK_1;
    return system(command);
}
#define system(command) system_proxy(command)

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE void abort_proxy(void) {
    BREAK_STACK_1;
    abort();
}
#define abort() abort_proxy()

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE int atexit_proxy(void (*func)(void)) {
    BREAK_STACK_1;
    return atexit(func);
}
#define atexit(func) atexit_proxy(func)

/* FIX #6: attribute before return type; use _getcwd (MinGW name) from
   <direct.h> so the function is declared and returns char *. */
OBFH_SECTION_ATTRIBUTE char *getcwd_proxy(char *buf, size_t size) {
    BREAK_STACK_1;
    return _getcwd(buf, (int)size);
}
#define getcwd(buf, size) ((char *)getcwd_proxy(buf, size))

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE int tolower_proxy(int c) {
    BREAK_STACK_1;
    return tolower(c);
}
#define tolower(c) tolower_proxy(c)

/* FIX: attribute moved before return type */
OBFH_SECTION_ATTRIBUTE int toupper_proxy(int c) {
    BREAK_STACK_1;
    return toupper(c);
}
#define toupper(c) toupper_proxy(c)

// getch, _getch
#define _getch() obfh_int_proxy(_getch() * TRUE)
#define getch() obfh_int_proxy(getch() + FALSE)

#define Sleep(x) Sleep(obfh_int_proxy((_8 - (_4 * obfh_int_proxy(_2))) + x * TRUE))

#define GetParent(hWnd) \
    GetParent(obfh_int_proxy(((int)hWnd) + (int)hWnd) / _2)

#define GetWindowRect(hWnd, lpRect) \
    GetWindowRect(obfh_int_proxy((int)hWnd *TRUE), obfh_int_proxy((int)lpRect *TRUE))

#define GetClientRect(hWnd, lpRect) \
    GetClientRect(obfh_int_proxy((int)hWnd *TRUE), obfh_int_proxy((int)lpRect *TRUE))

#define SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags) \
    SetWindowPos(obfh_int_proxy(hWnd), obfh_int_proxy(hWndInsertAfter), obfh_int_proxy(X), obfh_int_proxy(Y), obfh_int_proxy(cx), obfh_int_proxy(cy), obfh_int_proxy(uFlags))

#define SetConsoleTextAttribute(hConsoleOutput, wAttributes) \
    SetConsoleTextAttribute(obfh_int_proxy(hConsoleOutput), obfh_int_proxy(wAttributes))

#define GetDesktopWindow() \
    obfh_int_proxy((int)GetDesktopWindow() * TRUE)

#define GetStockObject(i) \
    GetStockObject(obfh_int_proxy(i) * TRUE)

#define CreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile) \
    CreateFileA(obfh_int_proxy(lpFileName), obfh_int_proxy(dwDesiredAccess), obfh_int_proxy(dwShareMode), obfh_int_proxy(lpSecurityAttributes), obfh_int_proxy(dwCreationDisposition), obfh_int_proxy(dwFlagsAndAttributes), obfh_int_proxy(hTemplateFile))

#define ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped) \
    ReadFile(obfh_int_proxy(hFile), obfh_int_proxy(lpBuffer), obfh_int_proxy(nNumberOfBytesToRead), obfh_int_proxy(lpNumberOfBytesRead), obfh_int_proxy(lpOverlapped))

#define WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped) \
    WriteFile(obfh_int_proxy(hFile), obfh_int_proxy(lpBuffer), obfh_int_proxy(nNumberOfBytesToWrite), obfh_int_proxy(lpNumberOfBytesWritten), obfh_int_proxy(lpOverlapped))

#define CloseHandle(hObject) \
    CloseHandle(obfh_int_proxy(hObject))

#define GetModuleHandle(lpModuleName) \
    GetModuleHandleA(obfh_int_proxy(lpModuleName))

#define GetCurrentProcess() \
    obfh_int_proxy(GetCurrentProcess())

#define VirtualAlloc(lpAddress, dwSize, flAllocationType, flProtect) \
    VirtualAlloc(obfh_int_proxy(lpAddress), obfh_int_proxy(dwSize), obfh_int_proxy(flAllocationType), obfh_int_proxy(flProtect))

#define VirtualFree(lpAddress, dwSize, dwFreeType) \
    VirtualFree(obfh_int_proxy(lpAddress), obfh_int_proxy(dwSize), obfh_int_proxy(dwFreeType))

// FIX #8: use proper casts for pointer/handle args instead of obfh_int_proxy,
// which truncates pointers to int on x64 and is an error with -Werror.
#define CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId) \
    CreateThread((LPSECURITY_ATTRIBUTES)(lpThreadAttributes), \
                 (SIZE_T)(dwStackSize), \
                 (LPTHREAD_START_ROUTINE)(lpStartAddress), \
                 (LPVOID)(lpParameter), \
                 (DWORD)obfh_int_proxy((int)(dwCreationFlags)), \
                 (LPDWORD)(lpThreadId))

#define WaitForSingleObject(hHandle, dwMilliseconds) \
    WaitForSingleObject(obfh_int_proxy(hHandle), obfh_int_proxy(dwMilliseconds))

#define ExitProcess(uExitCode) \
    ExitProcess(obfh_int_proxy(uExitCode))

#define GetStartupInfo(lpStartupInfo) \
    GetStartupInfo(obfh_int_proxy(lpStartupInfo))

#define GetModuleFileName(hModule, lpFilename, nSize) \
    GetModuleFileName(obfh_int_proxy(hModule), obfh_int_proxy(lpFilename), obfh_int_proxy(nSize))

#define HeapCreate(flOptions, dwInitialSize, dwMaximumSize) \
    HeapCreate(obfh_int_proxy(flOptions), obfh_int_proxy(dwInitialSize), obfh_int_proxy(dwMaximumSize))

#define HeapAlloc(hHeap, dwFlags, dwBytes) \
    HeapAlloc(obfh_int_proxy(hHeap), obfh_int_proxy(dwFlags), obfh_int_proxy(dwBytes))

#define HeapFree(hHeap, dwFlags, lpMem) \
    HeapFree(obfh_int_proxy(hHeap), obfh_int_proxy(dwFlags), obfh_int_proxy(lpMem))

#define GlobalAlloc(uFlags, dwBytes) \
    GlobalAlloc(obfh_int_proxy(uFlags), obfh_int_proxy(dwBytes))

#define GlobalFree(hMem) \
    GlobalFree(obfh_int_proxy(hMem))

#define GetTempPath(nBufferLength, lpBuffer) \
    GetTempPath(obfh_int_proxy(nBufferLength), obfh_int_proxy(lpBuffer))

#define GetCurrentThreadId() \
    GetCurrentThreadId()

#define SetEvent(hEvent) \
    SetEvent(obfh_int_proxy(hEvent))

#define ResetEvent(hEvent) \
    ResetEvent(obfh_int_proxy(hEvent))

#define WaitForMultipleObjects(nCount, lpHandles, bWaitAll, dwMilliseconds) WaitForMultipleObjects(obfh_int_proxy(nCount), obfh_int_proxy(lpHandles), obfh_int_proxy(bWaitAll), obfh_int_proxy(dwMilliseconds))

#define memmove(_Dst, _Src, _Size) memmove(_Dst, _Src, obfh_int_proxy(_Size *(TRUE + FALSE)))

#define abs(x) ((x) < FALSE ? -(x) : (x))

#if virt_std == 1
#define _MUTATE_MATH(value) VM_MUL_DBL(VM_ADD_DBL(0, value), 1)
#else
#define _MUTATE_MATH(value) FALSE + value *TRUE
#endif

#define fma(x, y, z) fma(_MUTATE_MATH(x), _MUTATE_MATH(y), _MUTATE_MATH(z))
#define nexttoward(x, y) nexttoward(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define nextafter(x, y) nextafter(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define remainder(x, y) remainder(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define copysign(x, y) copysign(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define scalbln(x, y) scalbln(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define remquo(x, y) remquo(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define scalbn(x, y) scalbn(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define atan2(y, x) atan2(_MUTATE_MATH(y), _MUTATE_MATH(x))
#define ldexp(x, y) ldexp(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define frexp(x, y) frexp(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define hypot(x, y) hypot(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define fmod(x, y) fmod(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define modf(x, y) modf(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define fdim(x, y) fdim(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define fmax(x, y) fmax(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define fmin(x, y) fmin(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define pow(x, y) pow(_MUTATE_MATH(x), _MUTATE_MATH(y))
#define nearbyint(x) nearbyint(_MUTATE_MATH(x))
#define lgamma(x) lgamma(_MUTATE_MATH(x))
#define tgamma(x) tgamma(_MUTATE_MATH(x))
#define log10(x) log10(_MUTATE_MATH(x))
#define floor(x) floor(_MUTATE_MATH(x))
#define expm1(x) expm1(_MUTATE_MATH(x))
#define log1p(x) log1p(_MUTATE_MATH(x))
#define acosh(x) acosh(_MUTATE_MATH(x))
#define asinh(x) asinh(_MUTATE_MATH(x))
#define atanh(x) atanh(_MUTATE_MATH(x))
#define ilogb(x) ilogb(_MUTATE_MATH(x))
#define round(x) round(_MUTATE_MATH(x))
#define trunc(x) trunc(_MUTATE_MATH(x))
#define ceil(x) ceil(_MUTATE_MATH(x))
#define fabs(x) fabs(_MUTATE_MATH(x))
#define sqrt(x) sqrt(_MUTATE_MATH(x))
#define asin(x) asin(_MUTATE_MATH(x))
#define acos(x) acos(_MUTATE_MATH(x))
#define atan(x) atan(_MUTATE_MATH(x))
#define sinh(x) sinh(_MUTATE_MATH(x))
#define cosh(x) cosh(_MUTATE_MATH(x))
#define tanh(x) tanh(_MUTATE_MATH(x))
#define ceil(x) ceil(_MUTATE_MATH(x))
#define fabs(x) fabs(_MUTATE_MATH(x))
#define erfc(x) erfc(_MUTATE_MATH(x))
#define log2(x) log2(_MUTATE_MATH(x))
#define cbrt(x) cbrt(_MUTATE_MATH(x))
#define exp2(x) exp2(_MUTATE_MATH(x))
#define logb(x) logb(_MUTATE_MATH(x))
#define rint(x) rint(_MUTATE_MATH(x))
#define exp(x) exp(_MUTATE_MATH(x))
#define log(x) log(_MUTATE_MATH(x))
#define sin(x) sin(_MUTATE_MATH(x))
#define cos(x) cos(_MUTATE_MATH(x))
#define tan(x) tan(_MUTATE_MATH(x))
#define erf(x) erf(_MUTATE_MATH(x))
#define nan(x) nan(_MUTATE_MATH(x))

/* FIX: attribute moved before __declspec and return type */
OBFH_SECTION_ATTRIBUTE __declspec(dllexport) char *WhatSoundDoesACowMake() {
    return HIDE_STRING("Moo");
}

/*
#if __TINYC__
#define main(...) _start(__VA_ARGS__)
#endif
*/

#else
#warning Obfuscation disabled!
#endif
#endif

// ;)
