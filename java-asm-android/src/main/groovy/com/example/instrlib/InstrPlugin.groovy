package com.example.instrlib

import com.android.build.api.transform.Context
import com.android.build.api.transform.Format
import com.android.build.api.transform.JarInput
import com.android.build.api.transform.QualifiedContent
import com.android.build.api.transform.Status
import com.android.build.api.transform.Transform
import com.android.build.api.transform.TransformException
import com.android.build.api.transform.TransformInput
import com.android.build.api.transform.TransformOutputProvider

import java.util.jar.JarEntry
import java.util.jar.JarFile
import java.util.jar.JarOutputStream

import org.apache.commons.io.FileUtils
import org.apache.commons.io.IOUtils

import org.gradle.api.Plugin
import org.gradle.api.Project
import org.objectweb.asm.ClassReader
import org.objectweb.asm.ClassWriter

@SuppressWarnings("UnstableApiUsage")
class InstrPlugin implements Plugin<Project> {

    @Override
    void apply(Project project) {
        project.android.registerTransform(new InstrPluginTransform())
    }

    static class InstrPluginTransform extends Transform {

        @Override
        String getName() {
            return "InstrPluginTransform"
        }

        @Override
        Set<QualifiedContent.ContentType> getInputTypes() {
            return Collections.singleton(QualifiedContent.DefaultContentType.CLASSES)
        }

        @Override
        Set<QualifiedContent.Scope> getScopes() {
            return EnumSet.of(
                    QualifiedContent.Scope.PROJECT,
                    QualifiedContent.Scope.PROJECT_LOCAL_DEPS,
                    QualifiedContent.Scope.SUB_PROJECTS,
                    QualifiedContent.Scope.SUB_PROJECTS_LOCAL_DEPS,
                    QualifiedContent.Scope.EXTERNAL_LIBRARIES
            )
        }

        @Override
        boolean isIncremental() {
            return false
        }

        @Override
        void transform(Context context, Collection<TransformInput> inputs, Collection<TransformInput> referencedInputs, TransformOutputProvider outputProvider, boolean isIncremental) throws IOException, TransformException, InterruptedException {
            inputs.each { TransformInput input ->
                input.directoryInputs.each {
                    def dest = outputProvider.getContentLocation(it.name, it.contentTypes, it.scopes, Format.DIRECTORY)
                    FileUtils.copyDirectory(it.file, dest)
                }
                input.jarInputs.each { JarInput jarInput ->
                    def jarName = jarInput.name
                    def src = jarInput.getFile()
                    def dest = outputProvider.getContentLocation(jarName, jarInput.contentTypes, jarInput.scopes, Format.JAR)
                    def status = jarInput.getStatus()

                    if (status == Status.REMOVED) {
                        FileUtils.delete(dest)
                    }
                    else if (!isIncremental || status != Status.NOTCHANGED) {
                        if ("${src}" ==~ ".*com.squareup.okhttp3.*.jar") {
                            def srcjar = new JarFile(src)
                            def dstjar = new JarOutputStream(new FileOutputStream(dest))

                            srcjar.entries().each {
                                if (it.getName() == "okhttp3/OkHttpClient.class") {
                                    println("OkHttpClient.class found, patching..")
                                    def is = srcjar.getInputStream(it)
                                    def bytes = IOUtils.toByteArray(is)
                                    is.close()

                                    def cr = new ClassReader(bytes)
                                    def cw = new ClassWriter(cr, ClassWriter.COMPUTE_MAXS)
                                    def cv = new CallClassVisitor(cw)
                                    cr.accept(cv, 0)
                                    def newBytes = cw.toByteArray()

                                    dstjar.putNextEntry(new JarEntry(it.getName()))
                                    IOUtils.copy(new ByteArrayInputStream(newBytes), dstjar)
                                }
                                else {
                                    def s = srcjar.getInputStream(it)
                                    dstjar.putNextEntry(new JarEntry(it.getName()))
                                    IOUtils.copy(s, dstjar)
                                    s.close()
                                }
                            }
                            dstjar.close()
                        }
                        else {
                            FileUtils.copyFile(src, dest)
                        }
                    }
                }
            }
        }
    }
}
