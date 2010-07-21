//
// Copyright (c) 2002-2010 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
/****************************************************************************\
Copyright (c) 2002, NVIDIA Corporation.

NVIDIA Corporation("NVIDIA") supplies this software to you in
consideration of your agreement to the following terms, and your use,
installation, modification or redistribution of this NVIDIA software
constitutes acceptance of these terms.  If you do not agree with these
terms, please do not use, install, modify or redistribute this NVIDIA
software.

In consideration of your agreement to abide by the following terms, and
subject to these terms, NVIDIA grants you a personal, non-exclusive
license, under NVIDIA's copyrights in this original NVIDIA software (the
"NVIDIA Software"), to use, reproduce, modify and redistribute the
NVIDIA Software, with or without modifications, in source and/or binary
forms; provided that if you redistribute the NVIDIA Software, you must
retain the copyright notice of NVIDIA, this notice and the following
text and disclaimers in all such redistributions of the NVIDIA Software.
Neither the name, trademarks, service marks nor logos of NVIDIA
Corporation may be used to endorse or promote products derived from the
NVIDIA Software without specific prior written permission from NVIDIA.
Except as expressly stated in this notice, no other rights or licenses
express or implied, are granted by NVIDIA herein, including but not
limited to any patent rights that may be infringed by your derivative
works or by other works in which the NVIDIA Software may be
incorporated. No hardware is licensed hereunder. 

THE NVIDIA SOFTWARE IS BEING PROVIDED ON AN "AS IS" BASIS, WITHOUT
WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
INCLUDING WITHOUT LIMITATION, WARRANTIES OR CONDITIONS OF TITLE,
NON-INFRINGEMENT, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
ITS USE AND OPERATION EITHER ALONE OR IN COMBINATION WITH OTHER
PRODUCTS.

IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT,
INCIDENTAL, EXEMPLARY, CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, LOST PROFITS; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) OR ARISING IN ANY WAY
OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE
NVIDIA SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT,
TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\****************************************************************************/
//
// cpp.h
//

#if !defined(__CPP_H)
#define __CPP_H 1

#include "compiler/preprocessor/parser.h"
#include "compiler/preprocessor/tokens.h"

int InitCPP(void);
int FinalCPP(void);
int  readCPPline(yystypepp * yylvalpp);
int MacroExpand(int atom, yystypepp * yylvalpp);
int ChkCorrectElseNesting(void);

typedef struct MacroSymbol {
    int argc;
    int *args;
    TokenStream *body;
    unsigned busy:1;
    unsigned undef:1;
} MacroSymbol;

void FreeMacro(MacroSymbol *);
void PredefineIntMacro(const char *name, int value);

void  CPPDebugLogMsg(const char *msg);      // Prints information into debug log
void  CPPShInfoLogMsg(const char*);         // Store cpp Err Msg into Sh.Info.Log
void  CPPWarningToInfoLog(const char *msg); // Prints warning messages into info log
void  HandlePragma(const char**, int numTokens);  // #pragma directive container.
void  ResetTString(void);                   // #error Message as TString.
void  CPPErrorToInfoLog(char*);             // Stick all cpp errors into Sh.Info.log   .
void  StoreStr(char*);                      // Store the TString in Parse Context.
void  SetLineNumber(int);                   // Set line number.  
void  SetStringNumber(int);                 // Set string number.    
int   GetLineNumber(void);                  // Get the current String Number. 
int   GetStringNumber(void);                // Get the current String Number. 
const char* GetStrfromTStr(void);           // Convert TString to String.  
void  updateExtensionBehavior(const char* extName, const char* behavior);
int   FreeCPP(void);

#endif // !(defined(__CPP_H)
