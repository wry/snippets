package com.example.instrlib;

import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;

public class CallClassVisitor extends ClassVisitor implements Opcodes {
    public CallClassVisitor(ClassVisitor cv) {
        super(ASM4, cv);
    }

    @Override
    public void visit(int version, int access, String name, String signature, String superName, String[] interfaces) {
        cv.visit(version, access, name, signature, superName, interfaces);
    }

    @Override
    public MethodVisitor visitMethod(int access, String name, String desc, String signature, String[] exceptions) {
        final MethodVisitor mv = cv.visitMethod(access, name, desc, signature, exceptions);
        if (name.equals("newCall")) {
            System.out.println("newCall method found, patching..");
            return new NewCallMethodVisitor(mv);
        }
        return mv;
    }

    public static class NewCallMethodVisitor extends MethodVisitor {
        public NewCallMethodVisitor(MethodVisitor mv) {
            super(ASM4, mv);
        }

        @Override
        public void visitCode() {
            // use ASMifier from ASM to write your code, or see the java-asm example
        }
    }
}
