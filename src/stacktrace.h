
// stacktrace.h (c) 2008, Timo Bingmann from http://idlebox.net/
// published under the WTFPL v2.0

/*

C++ Code Snippet - Print Stack Backtrace Programmatically with Demangled Function Names
https://panthema.net/2008/0901-stacktrace-demangled/

Posted on 2008-09-01 22:30 by Timo Bingmann at Permlink with 34 Comments. Tags: c++ code-snippet
coding tricks frontpage

Yesterday I was tasked to analyzed an inner function of a reasonably complex software package. The
inner function was called thousands of times from many different parts of the program, a simple
counter print-out showed that. However I was interested in which execution paths reach this inner
function and how often the different parts access the function.

My straight-forward idea was to dump a stack backtrace each time the inner function is called,
similar to the one printed by a debugger. However I needed some code snippet to dump the stack
backtrace programmatically, without using gdb to halt the program each time.

Stack backtraces can be saved with backtrace(3), resolved into symbolic names using
backtrace_symbols(3) and printed using backtrace_symbols_fd(3). These functions are well documented
and fairly easy to use.

However I was debugging a C++ program, which made heavy use of templates and classes. C++ symbols
names (including namespace, class and parameters) are mangled by the compiler into plain text
symbols: e.g. the function N::A<int>::B::func(int) becomes the symbol _ZN1N1AIiE1B4funcEi. This
makes the standard backtrace output very unreadable for C++ programs.

To demangle these strings the GNU libstdc++ library (integrated into the GNU Compiler Collection)
provides a function called __cxa_demangle(). Combined with backtrace(3) a pretty stack backtrace can
be outputted. The demangling function only works for programs compiled with g++.

The following header file contains a function print_stacktrace(), which uses backtrace(3),
backtrace_symbols(3) and __cxa_demangle() to print a readable C++ stack backtrace.

You can freely use it for whatever purpose: download stacktrace.h. I hope you find this utility
function useful.

*/

#ifndef _C_DEMANGLED_STACKTRACE_WITH_FUNCTION_NAMES_H_
#define _C_DEMANGLED_STACKTRACE_WITH_FUNCTION_NAMES_H_

#ifdef __linux__

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <cxxabi.h>

/**
 * Print a demangled stack backtrace of the caller function to FILE* out.
 *
 * @param out        [description]
 * @param max_frames [description]
 */
static inline void print_stacktrace(FILE *out = stderr, unsigned int max_frames = 63)
{
    fprintf(out, "stack trace:\n");

    // storage array for stack trace address data
    // void** addrlist = new (void *)[max_frames+1];
    void* addrlist[63+1];

    // retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    if (addrlen == 0) {
        fprintf(out, "  <empty, possibly corrupt>\n");
        return;
    }

    // resolve addresses into strings containing "filename(function+address)",
    // this array must be free()-ed
    char** symbollist = backtrace_symbols(addrlist, addrlen);

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    char* funcname = (char*)malloc(funcnamesize);

    // iterate over the returned symbol lines. skip the first, it is the
    // address of this function.
    for (int i = 1; i < addrlen; i++)
    {
        char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

        // find parentheses and +address offset surrounding the mangled name:
        // ./module(function+0x15c) [0x8048a6d]
        for (char *p = symbollist[i]; *p; ++p)
        {
            if (*p == '(')
                begin_name = p;
            else if (*p == '+')
                begin_offset = p;
            else if (*p == ')' && begin_offset) {
                end_offset = p;
                break;
            }
        }

        if (begin_name && begin_offset && end_offset
            && begin_name < begin_offset)
        {
            *begin_name++ = '\0';
            *begin_offset++ = '\0';
            *end_offset = '\0';

            // mangled name is now in [begin_name, begin_offset) and caller
            // offset in [begin_offset, end_offset). now apply
            // __cxa_demangle():

            int status;
            char* ret = abi::__cxa_demangle(begin_name, funcname, &funcnamesize, &status);

            if (status == 0) {
                funcname = ret; // use possibly realloc()-ed string
                fprintf(out, "  %s : %s+%s\n", symbollist[i], funcname, begin_offset);
            }
            else {
                // demangling failed. Output function name as a C function with
                // no arguments.
                fprintf(out, "  %s : %s()+%s\n", symbollist[i], begin_name, begin_offset);
            }
        }
        else
        {
            // couldn't parse the line? print the whole line.
            fprintf(out, "  %s\n", symbollist[i]);
        }
    }

    free(funcname);
    free(symbollist);
}

#else

static inline void print_stacktrace(FILE *out = stderr, unsigned int max_frames = 63)
{
    fprintf(out, "stack trace:\n  The `print_stacktrace()` is currently available only under Linux platform.\n");
    fflush(out);
}

#endif // __linux__

#endif // _C_DEMANGLED_STACKTRACE_WITH_FUNCTION_NAMES_H_
