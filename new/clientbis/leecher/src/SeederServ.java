import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class SeederServ extends Thread
{
	
	ServerSocket sockserv = null;

	public SeederServ()
	{
		super();
		System.out.println("HERE IN THE SeederServ COSNRT");
		try
		{
			sockserv = new ServerSocket(60001);
		}
		catch (IOException ex) { }
	}


	public void run()
	{
		try
		{
			while (true)
			{
				Socket sockcli = sockserv.accept();
				Seeder s = new Seeder(sockcli);
				s.start();
			}
		}
		catch (IOException ex) { }
	}

}
