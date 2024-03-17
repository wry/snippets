#!/usr/bin/env python3
#
#  Copyright (c) 2024 wry
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in all
#  copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.
#
# collection of commands to extend lldb functionality
#
# lldb py ref doc : https://lldb.llvm.org/use/python-reference.html
#
# use : 
#   echo "command script import <path>" >> ~/.lldbinit

import platform

# Paint affected views red when breaking on UIKitCore`UIViewAlertForUnsatisfiableConstraints
def ios_paint_uc_views(debugger, command, exe_ctx, result, internal_dict):
    """Paint affected views when breaking on UIKitCore`UIViewAlertForUnsatisfiableConstraints"""

    # SBFrame
    frame = exe_ctx.frame
    # string
    sym = frame.symbol.name
    # string
    reg0 = None
    for regs in frame.registers:
        if 'general purpose registers' in regs.name.lower():
            reg0 = regs.children[0].name
            break
    if reg0 == None:
        return
    
    arg0 = "rdi" if reg0 == "rax" else "x0"
    frame.EvaluateExpression(f"(void)[[(id)${arg0} firstItem] setBackgroundColor:UIColor.redColor]")
    frame.EvaluateExpression(f"(void)[[(id)${arg0} secondItem] setBackgroundColor:UIColor.redColor]")

# Stop executing CoreGraphics`is_image_alpha_opaque in Rosetta 2
def ios_ros2_ret_is_image_alpha_opaque(debugger, command, exe_ctx, result, internal_dict):
    """Stop executing CoreGraphics`is_image_alpha_opaque if in Rosetta 2"""

    if debugger.GetSelectedPlatform().GetName() != "ios-simulator":
        return 
    if debugger.GetSelectedTarget().GetABIName() != "sysv-x86_64":
        return
    if platform.processor() != "arm":
        return
    
    debugger.HandleCommand(f"mem wr $pc 0xc3")

# Add all commands
def __lldb_init_module(debugger, dict):
    for k, v in list(globals().items()):
        if callable(v):
            debugger.HandleCommand(f"command script add -f {__name__}.{k} {k}")
