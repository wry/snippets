//File tempFile = File.createTempFile("prefix-", "-suffix");
//tempFile.deleteOnExit();

import java.io.InputStream;
import java.io.FileOutputStream;
import java.io.File;

public class Bla implements IBla
{
	private static File tempFile;

	static
	{
		loadLibFromJAR("Bla");
	}

	@Override
	public native void bla();

	public static void main(String[] args)
	{
		new Bla().bla();
	}

	private static void loadLibFromJAR(final String name)
	{
		try (InputStream in = Bla.class.getResourceAsStream("lib" + name + ".dylib"))
		{
			Bla.tempFile = File.createTempFile("lib"+name, ".so", new File("/tmp"));
			Bla.tempFile.deleteOnExit();

			byte[] buffer = new byte[1024];
			int read = -1;

			try (FileOutputStream fos = new FileOutputStream(Bla.tempFile))
			{
				while((read = in.read(buffer)) != -1)
				{
					fos.write(buffer, 0, read);
				}
			}

			System.load(Bla.tempFile.getAbsolutePath());
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
}
