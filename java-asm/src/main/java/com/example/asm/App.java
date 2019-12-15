package com.example.asm;

import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.lang.instrument.Instrumentation;
import java.security.ProtectionDomain;

import org.objectweb.asm.ClassReader;
import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;

// 1) run the 'package' target of mvn
// 2) java -javaagent:"./target/asmtest-0.0.1-SNAPSHOT.jar" -cp target/classes:$HOME/.m2/repository/org/ow2/asm/asm/5.0.3/asm-5.0.3.jar com.example.asm.App
// 3) you should see the ASM code run as well as the method you called on HJClass
public class App extends ClassLoader implements Opcodes 
{
	/**
	 * you know what this is
	 * @param args
	 */
    public static void main(String[] args)
    {
    	final HJClass hj = new HJClass();
    	hj.execute();
    }

    /**
     * method called when class packed into jar and provided to the `java` command with -javaagent <jarfile>
     * @param agentArgs
     * @param inst
     */
    public static void premain(String agentArgs, Instrumentation inst) {
        inst.addTransformer(new ClassTransformer());
    }
    
    /**
     *  the class to hijack; note: default classes will be loaded by the time premain is called
     */
    public static class HJClass {
    	public void execute() {
    		System.out.println("soup has been granted");
    	}
    }
    
    /**
     * class transformer to rewrite the class bytes
     */
    public static class ClassTransformer implements ClassFileTransformer {
        public byte[] transform(
        		ClassLoader      loader,
                String           className,
                Class<?>         classBeingRedefined,
                ProtectionDomain protectionDomain,
                byte[]           b)
                        throws IllegalClassFormatException {
            try
            {
            	if (className.equals("com/example/asm/App$HJClass"))
            	{
            		ClassReader cr = new ClassReader(b);
                    ClassWriter cw = new ClassWriter(cr, ClassWriter.COMPUTE_MAXS);
                    CallClassVisitor cv = new CallClassVisitor(cw);
                    cr.accept(cv, 0);
                    return cw.toByteArray();
                }
            }
            catch(Exception e)
            {
                System.err.println(e);
            }
            return b;
        }
    }
    
    /**
     * class visitor to locate the method
     */
    public static class CallClassVisitor extends ClassVisitor {
        public CallClassVisitor(ClassVisitor cv) {
            super(ASM4, cv);
        }

        @Override
        public void visit(int version, int access, String name, String signature, String superName, String[] interfaces) {
            cv.visit(version, access, name, signature, superName, interfaces);
        }
        
        @Override
        public MethodVisitor visitMethod(int access, String name, String desc, String signature, String[] exceptions) {
          MethodVisitor mv = cv.visitMethod(access, name, desc, signature, exceptions);
          if (name.equals("execute")) {
        	  mv = new ExecuteMethodVisitor(mv);
          }
          return mv;
        }
    }
    
    /**
     * method visitor to rewrite the method
     */
    public static class ExecuteMethodVisitor extends MethodVisitor {

		public ExecuteMethodVisitor(MethodVisitor mv) {
			super(ASM4, mv);
		}
		
		@Override
        public void visitCode() {
			// asm byte generation; the code below was generated using ASMifier (asm-util) from :
			//   System.out.println("execute method is run");
			mv.visitCode();
			mv.visitFieldInsn(GETSTATIC, "java/lang/System", "out", "Ljava/io/PrintStream;");
			mv.visitLdcInsn("execute method is run");
			mv.visitMethodInsn(INVOKEVIRTUAL, "java/io/PrintStream", "println", "(Ljava/lang/String;)V", false);
        } 	
    }
}
